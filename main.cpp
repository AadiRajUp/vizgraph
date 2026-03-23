#include<SFML/Graphics.hpp>
#include"linegraph.hpp"
#include"piechart.hpp"
#include"donutchart.hpp"
#include "bargraph.hpp"
#include "histogram.hpp"
#include "scatterplot.hpp"
#include <cstdlib>
#include <ctime>
#include<string>
#include<cstdio>

int main(){
    sf::RenderWindow window(sf::VideoMode({800,600}),"Vizgraph");
    sf::Texture texture({800,600});
    sf::Sprite sprite(texture);
    sf::RenderTexture tx({800,600});

    // --- linegraph data ---

    int x[]={1,2,3,4,5,6,7,8,9};
    int y[]={1,4,9,16,25,36,49,64,81};

srand(time(NULL));
int Sx[60],Sy[60];
for (int i = 0; i < 60; i++) {
    Sx[i] = rand() % 100;
    Sy[i] = rand() % 100;
}
    /*
   int x[] = {
0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
90, 91, 92, 93, 94, 95, 96, 97, 98, 99
};

int y[] = {
0, 1, 4, 9, 16, 25, 36, 49, 64, 81,
100, 121, 144, 169, 196, 225, 256, 289, 324, 361,
400, 441, 484, 529, 576, 625, 676, 729, 784, 841,
900, 961, 1024, 1089, 1156, 1225, 1296, 1369, 1444, 1521,
1600, 1681, 1764, 1849, 1936, 2025, 2116, 2209, 2304, 2401,
2500, 2601, 2704, 2809, 2916, 3025, 3136, 3249, 3364, 3481,
3600, 3721, 3844, 3969, 4096, 4225, 4356, 4489, 4624, 4761,
4900, 5041, 5184, 5329, 5476, 5625, 5776, 5929, 6084, 6241,
6400, 6561, 6724, 6889, 7056, 7225, 7396, 7569, 7744, 7921,
8100, 8281, 8464, 8649, 8836, 9025, 9216, 9409, 9604, 9801
};

*/
    // --- pie / donut data ---
    int values[] = {30, 50, 20, 45, 15};
    std::string labels[] = {"Alpha","Beta","Gamma","Delta","Epsilon"};
    int sampleSize = sizeof(values)/sizeof(int);
     
    // --- histogram data ---
    int counts[] = {2, 6, 12, 9, 4};
    std::string binLabels[] = {"0-10","10-20","20-30","30-40","40-50"};

    int histSize = sizeof(counts)/sizeof(int);

    int currentChart = 0;

    // helper: full clear and redraw
    auto redraw = [&](){
        // fresh white canvas every time
        sf::Image canvas({800u,600u}, sf::Color::White);
        tx.clear(sf::Color::Transparent);

        printf("drawing chart %d\n", currentChart);

        if      (currentChart == 0) linegraph (canvas, tx, x, y, sizeof(x)/sizeof(int));
        else if (currentChart == 1) scatterplot (canvas, tx, Sx, Sy, sizeof(Sx)/sizeof(int));
        else if (currentChart == 2) piechart  (canvas, tx, values, labels, sampleSize);
        else if (currentChart == 3) donutchart(canvas, tx, values, labels, sampleSize);
        else if (currentChart == 4) bargraph (canvas, tx, values, labels, sampleSize);
        else if (currentChart == 5) histogram(canvas, tx, counts, binLabels, histSize);

        texture.update(canvas);
    };

    redraw();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()){
                printf("key pressed: %d\n", (int)key->code);
                if      (key->code == sf::Keyboard::Key::Num1){ currentChart = 0; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num2){ currentChart = 1; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num3){ currentChart = 2; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num4){ currentChart = 3; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num5){ currentChart = 4; redraw(); }
                else if (key->code == sf::Keyboard::Key::Num6){ currentChart = 5; redraw(); }
            
            }
        }

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.draw(sf::Sprite(tx.getTexture()));
        window.display();
    }
}