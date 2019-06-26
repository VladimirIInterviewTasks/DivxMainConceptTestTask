#include "algorithm.h"

#include <algorithm>
#include <vector>
#include <thread>

#include <QtDebug>

std::vector<point> getMove(
        const unsigned char * dataBase,
        const unsigned char * dataFuture,
        unsigned int height, unsigned int width)
{
    std::vector <point> p_vector;

    p_vector.resize(height*width/(8*8),{0,0});

    static constexpr unsigned int threadCount = 4;
    std::vector <std::thread> threads;

    for (unsigned int threadIndex = 0 ; threadIndex < threadCount; threadIndex++)
    {

        threads.emplace_back( (std::thread([&,threadIndex](){
            for (unsigned int i = ((threadIndex)*height)/threadCount ; i < ((1+threadIndex)*height)/threadCount; i = i + 8)
                for (unsigned int k = 0 ; k < width; k = k + 8)
                {

                    struct point p =  bestFitingPoint (dataBase,dataFuture,
                                                       k, i,
                                                       height, width,8,8);

                    unsigned int index = (i/8)*(width/8)+k/8;

                    p_vector.at(index) = p;

                }
        })));
    }

    for (unsigned int j = 0 ; j < threadCount; j++)
    {
        threads.at(j).join();
    }

    return p_vector;
}


point  bestFitingPoint (const unsigned char * data,const unsigned char * data2,
                        unsigned int x, unsigned int y,
                        unsigned int height, unsigned int width,unsigned int radius,unsigned int splitSize)
{
    unsigned int min=INT_MAX;
    struct point p;


    for ( unsigned int k = static_cast<unsigned int>(std::max(0,static_cast<int>(y)-static_cast<int>(radius)));
          k < std::min(height,y+radius+splitSize);
          k++)
        for (unsigned int i = static_cast<unsigned int>(std::max(static_cast<int>(x)-static_cast<int>(radius),static_cast<int>(0)));
             i <  std::min(width,x+radius+splitSize);
             i++)


        {
            unsigned int temp = deviationsSum8by8Matrix(data, data2,x,y,i,k,height,width);

            if (temp < min)
            {
                min = temp;
                p.x = i;
                p.y = k;
            }
        }

    return p;

}

unsigned int deviationsSum8by8Matrix(const unsigned char * data, const unsigned char * data2,  unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY, unsigned int height, unsigned int width)
{
    (void)height;
    unsigned int sum = 0;

    for ( unsigned int k = 0;
          k < 8;
          k++)
        for (unsigned int i =0;
             i < 8;
             i++)
        {
            sum += static_cast<unsigned int> (abs(static_cast<int>(data2[(y+k)*width+i+x])-data[offsetX+i+(offsetY+k)*width]));
        }
    return sum;
}
