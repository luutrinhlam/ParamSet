// #include "PS_sort.cpp"
#include "PS.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>
#include "Timer.h"

#define EYE_RANGE 100
#define ANGLE_ERROR 0.00001

using namespace std;

bool approx(float x, float y)
{
    return abs(x - y) <= ANGLE_ERROR;
}

int main()
{
	float offline_t[3];
	float online_t[3];
	for(int t_index = 0; t_index < 3; t_index++){
    // //////// offline phase ////////
    vector<Point2D> mesh;
    string buffer;
    ifstream HCMUTCampus("HCMUT_campus_test2.txt");

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(HCMUTCampus, buffer))
    {
        // Output the text from the file
        string x;
        string y;
        bool xStart = false;
        bool yStart = false;
        for (char c : buffer)
        {
            if (c == '(')
            {
                xStart = true;
                continue;
            }
            if (c == ',')
            {
                xStart = false;
                yStart = true;
                continue;
            }
            if (xStart)
            {
                x += c;
            }
            if (yStart)
            {
                y += c;
            }
        }
        Point2D p(stof(x), stof(y));
        mesh.push_back(p);
    }
    std::cout << "Map Size: " << mesh.size() << endl;
    vector<PS> myMap;

    Timer t;
    for (int i = 0; i < mesh.size(); i++)
    {
        myMap.push_back(PS_Generator(mesh[i], mesh));
    }
	offline_t[t_index ] = t.elapsed();
    std::cout << "[t] Offline phase time taken: " << t.elapsed() << " seconds\n";
    //////// online phase ////////
    Point2D lostPoint(998 / 2, 586 / 2);
    Point2D closestPoint;
    vector<Point2D> resultPoints;
    float minDistance = std::numeric_limits<float>::max();
    ;
    for (auto x : mesh)
    {
        if (minDistance > sqrt(pow(lostPoint.x - x.x, 2) + pow(lostPoint.y - x.y, 2)))
        {
            minDistance = sqrt(pow(lostPoint.x - x.x, 2) + pow(lostPoint.y - x.y, 2));
            closestPoint = x;
        }
    }
    std::cout << "Closest point: " << closestPoint.x << " " << closestPoint.y << endl;

    vector<Point2D> seeable_points;
    for (auto x : mesh)
    {
        if (sqrt(pow(lostPoint.x - x.x, 2) + pow(lostPoint.y - x.y, 2)) < EYE_RANGE)
        {
            seeable_points.push_back(x);
        }
    }

    PS seeable_PS;
    seeable_PS = PS_Generator(closestPoint, seeable_points);
    std::cout << "Number of seeable points: " << seeable_points.size() - 1 << endl;

    t.reset();	
    for (int i = 0; i < myMap.size(); i++)
    {
        // check if seeable_PS.distances is a subset of myMap[i].distances
        bool isFind = isSubset(seeable_PS.distance, myMap[i].distance);

        // for (int j = 0; j < seeable_PS.distance.size(); j++)
        // {
        //     bool exist = false;
        //     for (int k = 0; k < myMap[i].distance.size(); k++)
        //     {
        //         if (seeable_PS.distance[j] == myMap[i].distance[k])
        //         {
        //             exist = true;
        //             break;
        //         }
        //     }
        //     if (!exist)
        //     {
        //         isFind = false;
        //         break;
        //     }
        // }

        if (isFind) // seeable_PS is a subset of myMap[i]
        {
            cout << "Find a point match distance" << endl;
            float alpha = seeable_PS.angle[0];
            int left_pointer = 0;
            int right_pointer = 0;
            float window_sum = 0;
            do
            {
                right_pointer %= myMap[i].angle.size();
                window_sum += myMap[i].angle[right_pointer];
                if (approx(window_sum, alpha))
                {
                    // cout << "Found a match in angle" << endl;
                    bool isMatch = true;
                    float accumulatedSum = 0;

                    int l = 1;
                    for (int k = (right_pointer + 1) % myMap[i].angle.size();
                         k != left_pointer;
                         k = (k + 1) % myMap[i].angle.size())
                    {
                        // if (l >= seeable_PS.angle.size())
                        // {
                        //     isMatch = false;
                        //     break;
                        // };
                        accumulatedSum += myMap[i].angle[k];
                        if (approx(accumulatedSum, seeable_PS.angle[l]))
                        {
                            accumulatedSum = 0;
                            l++;
                        }
                        else if (accumulatedSum > seeable_PS.angle[l])
                        {
                            isMatch = false;
                            break;
                        }
                    }
                    if (accumulatedSum != 0) isMatch = false;
                    if (isMatch)
                    {
                        std::cout << "Found i: " << i + 1 << endl;
                        resultPoints.push_back(mesh[i]);
                        break;
                    };
                    window_sum -= myMap[i].angle[left_pointer];
                    // window_sum -= myMap[i].angle[right_pointer];
                    right_pointer++;
                    left_pointer++;
                }
                else if (window_sum > alpha)
                {
                    window_sum -= myMap[i].angle[left_pointer];
                    window_sum -= myMap[i].angle[right_pointer];
                    left_pointer++;
                }
                else if (window_sum < alpha)
                    right_pointer++;
            } while (left_pointer != myMap[i].angle.size());
        }
    }
	online_t[t_index ] = t.elapsed();
    std::cout << "[t] Online phase time taken: " << t.elapsed() << " seconds\n";

    std::cout << "----------------------------------------------" << endl;
    std::cout << "Result:" << endl;
    for (auto result : resultPoints)
    {
        std::cout << result.x << " " << result.y << endl;
    }
	}
	cout << "+++++++++ Offline time: " << (offline_t[0] + offline_t[1] +offline_t[2])/3  <<endl;
	cout << "+++++++++ Online time: " << (online_t[0] + online_t[1] +online_t[2])/3  <<endl;
    return 0;
}