#include "piechart.hpp"
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


// Fills every pixel inside the angular wedge [startAngle, endAngle] (radians)
// and within radius r from (cx,cy) using a scanline over the bounding box.
void draw_sector(sf::Image& canvas, int cx, int cy, int r, float startAngle, float endAngle, sf::Color color){
    int imgW = (int)canvas.getSize().x;
    int imgH = (int)canvas.getSize().y;

    for (int py = cy - r; py <= cy + r; py++){
        for (int px = cx - r; px <= cx + r; px++){
            if (px < 0 || py < 0 || px >= imgW || py >= imgH) continue;

            float dx = (float)(px - cx);
            float dy = (float)(py - cy);
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist > (float)r) continue;

            // atan2 returns [-pi, pi]; normalise to [0, 2*pi]
            float angle = atan2f(dy, dx);
            if (angle < 0) angle += 2.0f * M_PI;

            // Handle wedges that wrap around 0/2pi boundary
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


void piechart(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize){
    // Chart geometry
    int cx = 400, cy = 300;   // center of chart
    int r  = 220;             // radius

    // Sum all values to compute proportions
    float total = 0;
    for (int i = 0; i < sampleSize; i++){
        total += (float)values[i];
    }

    printf("piechart: total=%.2f  slices=%d\n", total, sampleSize);

    // Draw each wedge
    float currentAngle = 0.0f;
    for (int i = 0; i < sampleSize; i++){
        float sweep    = (values[i] / total) * 2.0f * M_PI;
        float endAngle = currentAngle + sweep;

        sf::Color col = sliceColors[i % NUM_COLORS];

        printf("  slice[%d] label=%s value=%d angle=%.3f->%.3f\n",
               i, labels[i].c_str(), values[i], currentAngle, endAngle);

        draw_sector(canvas, cx, cy, r, currentAngle, endAngle, col);

        // White separator line at every slice boundary
        float sepX = cx + cosf(currentAngle) * r;
        float sepY = cy + sinf(currentAngle) * r;
        draw_line(canvas, cx, cy, (int)sepX, (int)sepY, sf::Color::White);

        currentAngle = endAngle;
    }

    // Outer border circle drawn over everything for a clean edge
    draw_circle(canvas, cx, cy, r, sf::Color::White, false);

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

        // Percentage label placed at mid-radius of each slice
        float labelR = r * 0.65f;
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