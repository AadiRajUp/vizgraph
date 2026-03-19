#include<SFML/Graphics.hpp>
#include"linegraph.hpp"
#include"piechart.hpp"
#include"donutchart.hpp"
#include<string>

int main(){
    sf::RenderWindow window(sf::VideoMode({800,600}),"Vizgraph");
    sf::Image canvas({800,600},sf::Color::White);
    sf::Texture texture({800,600});
    sf::Sprite sprite(texture);
    sf::RenderTexture tx({800,600});

    tx.clear(sf::Color::Transparent);

    // 0 = linegraph, 1 = piechart, 2 = donutchart
    int currentChart = 0;

    // --- linegraph data ---
    int x[]={0,1,2,3,4,5,6,7,8,9,10};
    int y[]={0,1,4,9,16,25,36,49,64,81,100};

    // --- pie / donut data ---
    int    values[] = {30, 50, 20, 45, 15};
    std::string labels[] = {"Alpha","Beta","Gamma","Delta","Epsilon"};
    int sampleSize = sizeof(values)/sizeof(int);

    // helper lambda: clears and redraws the selected chart
    auto redraw = [&](){
        canvas = sf::Image({800,600}, sf::Color::White);
        tx.clear(sf::Color::Transparent);

        if      (currentChart == 0) linegraph (canvas, tx, x, y, sizeof(x)/sizeof(int));
        else if (currentChart == 1) piechart  (canvas, tx, values, labels, sampleSize);
        else if (currentChart == 2) donutchart(canvas, tx, values, labels, sampleSize);

        texture.update(canvas);
    };

    redraw(); // draw initial chart

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Press 1/2/3 to switch charts
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()){
                if      (key->code == sf::Keyboard::Key::Num1) { currentChart = 0; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num2) { currentChart = 1; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num3) { currentChart = 2; redraw(); }
            }
        }

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.draw(sf::Sprite(tx.getTexture()));
        window.display();
    }
}