#pragma once
#include<SFML/Graphics.hpp>
#include "utils.hpp"
#include<string>
void piechart(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize);
void draw_sector(sf::Image& canvas, int cx, int cy, int r, float startAngle, float endAngle, sf::Color color);