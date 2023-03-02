#include <vector>
#include <math.h>
#include <iostream>
#include "mesh.cpp"

using namespace std;

struct PS
{
    vector<float> distance;
    vector<float> angle;
};

struct PS_relation_help_node
{
    Point2D point;
    float distance;
    float theta; // angle created by Oy and the line from source to point
};

PS calculatePSrelation(Point2D source, vector<Point2D> mesh)
{
    static const double TWOPI = 6.2831853071795865;
    PS PS; // PS = Parameter Set

    ///////////////////////////// calculate help list /////////////////////////////
    vector<PS_relation_help_node> PS_relation_help_list;
    for (int i = 0; i < mesh.size(); i++)
    {
        // We do not need to calculate for the source point
        if (mesh[i].x == source.x && mesh[i].y == source.y)
            continue;

        PS_relation_help_node help_node;
        help_node.point = mesh[i];
        help_node.distance = sqrt(pow(source.x - mesh[i].x, 2) + pow(source.y - mesh[i].y, 2));
        double theta = atan2(mesh[i].x - source.x, mesh[i].y - source.y);
        if (theta < 0.0)
        {
            theta += M_PI * 2.0;
        }
        help_node.theta = theta;

        // Insertion Sort idea
        bool haveInserted = false;
        for (int j = 0; j < PS_relation_help_list.size(); j++)
        {
            if (help_node.theta == PS_relation_help_list[j].theta)
            {
                if (help_node.distance < PS_relation_help_list[j].distance)
                {
                    PS_relation_help_list.insert(PS_relation_help_list.begin() + j, help_node);
                    haveInserted = true;
                    break;
                }
            }

            if (help_node.theta < PS_relation_help_list[j].theta)
            {
                PS_relation_help_list.insert(PS_relation_help_list.begin() + j, help_node);
                haveInserted = true;
                break;
            }
        }

        if(!haveInserted)
            PS_relation_help_list.push_back(help_node);
    }

    // for(auto x : PS_relation_help_list){
    //     cout << "Point: " << x.point.x << " " << x.point.y << " Distance: " << x.distance << " Theta: " << x.theta << endl;
    // }

    ///////////////////////////// calculate PS /////////////////////////////
    for (int i = 0; i < PS_relation_help_list.size(); i++)
    {
        PS.distance.push_back(PS_relation_help_list[i].distance);
        if (i == PS_relation_help_list.size() - 1)
        {
            PS.angle.push_back( TWOPI - PS_relation_help_list[i].theta - PS_relation_help_list[0].theta);
        }
        else
        {
            PS.angle.push_back(PS_relation_help_list[i + 1].theta - PS_relation_help_list[i].theta);
        }
    }
    // cout << "------------------------------------------------------------" <<endl;
    // float sumOfAngle = 0;
    // for (int i = 0; i < PS.angle.size(); i++)
    // {
    //     sumOfAngle += PS.angle[i];
    //     cout << "Distance: " << PS.distance[i] << " Angle: " << PS.angle[i] << endl;
    // }
    // cout  << "Sum of angle: " << sumOfAngle << endl;
    return PS;
};
