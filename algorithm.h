#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>

struct point{
    unsigned int x;
    unsigned int y;
};


std::vector<point> getMove(
         const unsigned char * dataBase,
         const unsigned char * dataFuture,
         unsigned int height, unsigned int width);

point  bestFitingPoint (const unsigned char * data, const unsigned char * data2,
           unsigned int x, unsigned int y,
           unsigned int height, unsigned int width, unsigned int radius, unsigned int splitSize);


unsigned int deviationsSum8by8Matrix (const unsigned char * data,const unsigned char * data2,  unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY, unsigned int height, unsigned int width);


#endif // ALGORITHM_H
