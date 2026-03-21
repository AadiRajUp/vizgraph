#pragma once
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include <string>

// Histogram (Option A): counts[i] for each bin label binLabels[i]
void histogram(sf::Image& canvas, sf::RenderTexture& rt, int* counts, std::string* binLabels, int sampleSize);