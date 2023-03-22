#ifndef PS_H
#define PS_H

#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>

#define ANGLE_ERROR 0.00001

using namespace std;

class Point2D {
public:
    Point2D() : x(0), y(0) {}
    Point2D(float x, float y) : x(x), y(y) {}
    float x, y;
};

struct PS
{
    vector<float> distances;
    vector<float> angles;
};

struct PS_help_node
{
    Point2D point;
    float distance;
    float theta; // angle created by Oy and the line from source to point
};

bool approx(float x, float y);


PS PS_Generator(Point2D source, vector<Point2D> mesh);
bool isSubset(vector<float> a, vector<float> b);
vector<PS> offlinePhase (vector<Point2D> mesh);
vector<Point2D> onlinePhase (vector<PS> myMap, PS seeable_PS, vector<Point2D> mesh);

#endif