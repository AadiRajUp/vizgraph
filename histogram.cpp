#include "histogram.hpp"
#include <cstdio>

static int max_value(int* values, int n){
    int m = values[0];
    for(int i = 1; i < n; i++){
        if(values[i] > m) m = values[i];
    }
    return m;
}

void histogram(sf::Image& canvas, sf::RenderTexture& rt, int* counts, std::string* binLabels, int sampleSize){
    int left   = 80;
    int right  = 760;
    int top    = 50;
    int bottom = 550;

    draw_line(canvas, left, top, left, bottom, sf::Color::Black);
    draw_line(canvas, left, bottom, right, bottom, sf::Color::Black);

    if(sampleSize <= 0) return;

    int largestY = max_value(counts, sampleSize);
    if(largestY <= 0) largestY = 1;

    // tens scaling like your linegraph
    int tensY = 1;
    float tempY = (float)largestY;
    while(tempY > 1){
        tempY /= 10;
        tensY *= 10;
    }

    // histogram bars usually touch: use full width per bin
    int plotW = right - left;
    int barW = plotW / sampleSize;
    if(barW < 3) barW = 3;

    sf::Text text(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(11);

    // y ticks + labels
    for(int i = 0; i <= 10; i++){
        int yv = (tensY/10) * i;
        int py = bottom - (int)((float)yv / (float)tensY * (bottom - top));

        draw_line(canvas, left-3, py, left+3, py, sf::Color::Black);

        text.setString(std::to_string(yv));
        text.setPosition({20.0f, (float)py - 6});
        rt.draw(text);
    }

    // bars + bin labels
    for(int i = 0; i < sampleSize; i++){
        float v = (float)counts[i];
        if(v < 0) v = 0;

        int barH = (int)(v / (float)tensY * (bottom - top));
        int x0 = left + i * barW;
        int y0 = bottom - barH;

        sf::Color col(120, 170, 240);

        draw_rect(canvas, x0, y0, barW, barH, col, true);
        draw_rect(canvas, x0, y0, barW, barH, sf::Color::Black, false);

        text.setString(binLabels[i]);
        text.setPosition({(float)x0 + 2, (float)bottom + 8});
        rt.draw(text);
    }

    rt.display();
}