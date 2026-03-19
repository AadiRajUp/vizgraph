#include "donutchart.hpp"
#include<cmath>
#include<cstdio>

#define M_PI 3.14159265358979323846f

// Palette used to color slices, cycles if sampleSize > 8
static sf::Color sliceColors[] = {
    sf::Color(220, 80,  80 ),   // red
    sf::Color(80,  150, 220),   // blue
    sf::Color(80,  200, 100),   // green
    sf::Color(240, 180, 50 ),   // yellow
    sf::Color(180, 80,  200),   // purple
    sf::Color(240, 130, 50 ),   // orange
    sf::Color(80,  210, 210),   // cyan
    sf::Color(200, 100, 140),   // pink
};
static const int NUM_COLORS = sizeof(sliceColors) / sizeof(sf::Color);


// Fills the ring band between innerR and outerR within the angular wedge
// [startAngle, endAngle] (radians) using a scanline over the bounding box.
void draw_donut_sector(sf::Image& canvas, int cx, int cy, int outerR, int innerR, float startAngle, float endAngle, sf::Color color){
    int imgW = (int)canvas.getSize().x;
    int imgH = (int)canvas.getSize().y;

    for (int py = cy - outerR; py <= cy + outerR; py++){
        for (int px = cx - outerR; px <= cx + outerR; px++){
            if (px < 0 || py < 0 || px >= imgW || py >= imgH) continue;

            float dx   = (float)(px - cx);
            float dy   = (float)(py - cy);
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist > (float)outerR) continue;
            if (dist < (float)innerR) continue;   // hollow center

            // atan2 returns [-pi, pi]; normalise to [0, 2*pi]
            float angle = atan2f(dy, dx);
            if (angle < 0) angle += 2.0f * M_PI;

            // Handle wedges that wrap around the 0/2pi boundary
            bool inside = false;
            if (startAngle <= endAngle){
                inside = (angle >= startAngle && angle <= endAngle);
            } else {
                inside = (angle >= startAngle || angle <= endAngle);
            }

            if (inside){
                canvas.setPixel({(unsigned int)px, (unsigned int)py}, color);
            }
        }
    }
}


void donutchart(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize){
    // Chart geometry
    int cx     = 400, cy = 300;
    int outerR = 220;
    int innerR = 110;   // hole = 50% of outer radius

    // Sum all values to compute proportions
    float total = 0;
    for (int i = 0; i < sampleSize; i++){
        total += (float)values[i];
    }

    printf("donutchart: total=%.2f  slices=%d\n", total, sampleSize);

    // Draw each ring wedge
    float currentAngle = 0.0f;
    for (int i = 0; i < sampleSize; i++){
        float sweep    = (values[i] / total) * 2.0f * M_PI;
        float endAngle = currentAngle + sweep;

        sf::Color col = sliceColors[i % NUM_COLORS];

        printf("  slice[%d] label=%s value=%d angle=%.3f->%.3f\n",
               i, labels[i].c_str(), values[i], currentAngle, endAngle);

        draw_donut_sector(canvas, cx, cy, outerR, innerR, currentAngle, endAngle, col);

        // White separator line from inner edge to outer edge at each boundary
        float sepX1 = cx + cosf(currentAngle) * innerR;
        float sepY1 = cy + sinf(currentAngle) * innerR;
        float sepX2 = cx + cosf(currentAngle) * outerR;
        float sepY2 = cy + sinf(currentAngle) * outerR;
        draw_line(canvas, (int)sepX1, (int)sepY1, (int)sepX2, (int)sepY2, sf::Color::White);

        currentAngle = endAngle;
    }

    // Outer and inner border circles for clean edges
    draw_circle(canvas, cx, cy, outerR, sf::Color::White, false);
    draw_circle(canvas, cx, cy, innerR, sf::Color::White, false);

    // Center label showing total
    sf::Text centerText(font);
    centerText.setFillColor(sf::Color::Black);
    centerText.setCharacterSize(16);
    centerText.setString("Total\n" + std::to_string((int)total));
    centerText.setPosition({(float)cx - 26, (float)cy - 16});
    rt.draw(centerText);

    // Legend + percentage labels
    sf::Text labelText(font);
    labelText.setFillColor(sf::Color::Black);
    labelText.setCharacterSize(13);

    int legendX = 30;
    int legendY = 30;

    currentAngle = 0.0f;
    for (int i = 0; i < sampleSize; i++){
        float sweep    = (values[i] / total) * 2.0f * M_PI;
        float midAngle = currentAngle + sweep * 0.5f;

        // Percentage label placed at ring mid-depth
        float labelR = (outerR + innerR) * 0.5f;
        int lx = cx + (int)(cosf(midAngle) * labelR);
        int ly = cy + (int)(sinf(midAngle) * labelR);
        float pct = (values[i] / total) * 100.0f;

        labelText.setFillColor(sf::Color::White);
        char buf[32];
        snprintf(buf, sizeof(buf), "%.1f%%", pct);
        labelText.setString(buf);
        labelText.setPosition({(float)lx - 12, (float)ly - 6});
        rt.draw(labelText);

        // Color swatch + name in legend on the left
        draw_rect(canvas, legendX, legendY + i * 22, 14, 14, sliceColors[i % NUM_COLORS], true);

        labelText.setFillColor(sf::Color::Black);
        labelText.setString(labels[i] + " (" + std::to_string(values[i]) + ")");
        labelText.setPosition({(float)legendX + 18, (float)legendY + i * 22});
        rt.draw(labelText);

        currentAngle += sweep;
    }

    rt.display();
}