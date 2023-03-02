#include <vector>
#include <math.h>
#include <iostream>
#include "mesh.cpp"

using namespace std;

struct node
{
    float distance;
    float angle;
    Point2D point;
};

vector<node> calculatePSsort(Point2D source, vector<Point2D> mesh)
{
    vector<node> PS; // PS = Parameter Set
    for (int i = 0; i < mesh.size(); i++)
    {
        node destinationNode;
        destinationNode.distance = sqrt(pow(source.x - mesh[i].x, 2) + pow(source.y - mesh[i].y, 2));
        destinationNode.angle = atan2(mesh[i].y - source.y, mesh[i].x - source.x);
        destinationNode.point = mesh[i];

        // Insertion Sort idea
        bool haveInserted = false;
        for (int j = 0; j < i; j++)
        {
            if (destinationNode.distance == PS[j].distance)
            {
                if (destinationNode.angle < PS[j].angle)
                {
                    PS.insert(PS.begin() + j, destinationNode);
                }
                else
                {
                    PS.insert(PS.begin() + j + 1, destinationNode);
                };
                haveInserted = true;
                break;
            }

            if (destinationNode.distance < PS[j].distance)
            {
                PS.insert(PS.begin() + j, destinationNode);
                haveInserted = true;
                break;
            }
        }

        if (!haveInserted)
            PS.push_back(destinationNode);
    }
    return PS;
};