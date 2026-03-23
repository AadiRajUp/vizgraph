#include "scatterplot.hpp"
#include<cmath>
//#include<stdio.h>

#include<cstdio>
void scatterplot(sf::Image& canvas,sf::RenderTexture& rt, int* x, int* y,int sampleSize){
// Setting up the screen
draw_line(canvas,50,50,50,550,sf::Color::Black);//Yaxis line
draw_line(canvas,50,550,750,550,sf::Color::Black);//xaxis line


// various shenanigans to scale accordingly
float largestX = x[sampleSize-1];
for(int i =0;i<sampleSize;i++){
    if (x[i]>largestX){
        largestX=x[i];
    }
}
float largestY=y[sampleSize-1];
for(int i =0;i<sampleSize;i++){
    if (y[i]>largestY){
        largestY=y[i];
    }
}

int tensX=1,tensY=1;
float tempX=largestX,tempY=largestY;
while(tempX>1){
    tempX/=10;
    tensX*=10;
}
while(tempY>1){
    tempY/=10;
    tensY*=10;
}

// plotting the points
float px,py;
for ( int i =0;i<sampleSize;i++){
    px = ((float)x[i]/tensX) *700+50;
    py = 600 - ((float)y[i]/tensY) *500-50;
    printf("%d,%d,%f,%f\n",tensX,tensY,px,600-py);
    draw_circle(canvas, px, py, 3, sf::Color::Blue, true);
}
// making labels 
sf::Text labelText(font);
labelText.setFillColor(sf::Color::Black);
labelText.setCharacterSize(10);
for (int i =0;i<=10;i++){
    labelText.setString(std::to_string(tensY/10*i));
    labelText.setPosition({30,550-50*i});
    rt.draw(labelText);
}
for (int i =0;i<=10;i++){
    labelText.setString(std::to_string(tensX/10*i));
    labelText.setPosition({50+70*i,565});
    rt.draw(labelText);
}
rt.display();

}