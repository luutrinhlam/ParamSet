#ifndef SOME_HEADER_FILE_H
#define SOME_HEADER_FILE_H

#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm> 


class Point2D {
public:
    Point2D() : x(0), y(0) {}
    Point2D(float x, float y) : x(x), y(y) {}
    float x, y;
};

struct PS
{
    std::vector<float> distance;
    std::vector<float> angle;
};

struct PS_help_node
{
    Point2D point;
    float distance;
    float theta; // angle created by Oy and the line from source to point
};

PS PS_Generator(Point2D source, std::vector<Point2D> mesh);
bool isSubset(std::vector<float> a, std::vector<float> b);

#endif