#include "PS.h"

using namespace std;

PS PS_Generator(Point2D source, vector<Point2D> mesh)
{
    static const double TWOPI = 6.2831853071795865;
    PS PS; // PS = Parameter Set

    ///////////////////////////// calculate help list /////////////////////////////
    vector<PS_help_node> PS_help_list;
    for (int i = 0; i < mesh.size(); i++)
    {
        // We do not need to calculate for the source point
        if (mesh[i].x == source.x && mesh[i].y == source.y)
            continue;

        PS_help_node help_node;
        help_node.point = mesh[i];
        help_node.distance = sqrt(pow(source.x - mesh[i].x, 2) + pow(source.y - mesh[i].y, 2));
        double theta = atan2(mesh[i].y - source.y, mesh[i].x - source.x);
        // double theta = atan2(mesh[i].x - source.x, mesh[i].y - source.y);
        if (theta < 0.0)
        {
            theta += TWOPI;
        }
        help_node.theta = theta;

        // Insertion Sort idea
        bool haveInserted = false;
        for (int j = 0; j < PS_help_list.size(); j++)
        {
            if (help_node.theta == PS_help_list[j].theta)
            {
                if (help_node.distance < PS_help_list[j].distance)
                {
                    PS_help_list.insert(PS_help_list.begin() + j, help_node);
                    haveInserted = true;
                    break;
                }
            }

            if (help_node.theta < PS_help_list[j].theta)
            {
                PS_help_list.insert(PS_help_list.begin() + j, help_node);
                haveInserted = true;
                break;
            }
        }

        if(!haveInserted)
            PS_help_list.push_back(help_node);
    }

    ///////////////////////////// calculate PS /////////////////////////////
    for (int i = 0; i < PS_help_list.size(); i++)
    {
        PS.distance.push_back(PS_help_list[i].distance);
        if (i == PS_help_list.size() - 1)
        {
            PS.angle.push_back( TWOPI - (PS_help_list[i].theta - PS_help_list[0].theta));
        }
        else
        {
            PS.angle.push_back(PS_help_list[i + 1].theta - PS_help_list[i].theta);
        }
    }
    return PS;
};

bool isSubset(vector <float> a, vector <float> b)
{
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    int index = 0;
    for (auto Bnode : b){
        if (a[index] == Bnode) index++;
    }
    return (index == a.size());
}