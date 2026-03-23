#pragma once
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include <string>

// Categorical bar chart: values[i] with label labels[i]
void bargraph(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize);