// #include "PS_sort.cpp"
#include "PS_relation.cpp"
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <algorithm>

#define EYE_RANGE 2.2

using namespace std;

bool approx(float x, float y){
    return abs(x - y) < 0.0001;
}

int main()
{
    // Point2D source(1,1);
    // vector<Point2D> mesh;
    // mesh.push_back(Point2D(2,2));
    // mesh.push_back(Point2D(1,2));
    // mesh.push_back(Point2D(2,1));
    // mesh.push_back(Point2D(3,2));
    // mesh.push_back(Point2D(0,-1));

    // //////// online phase ////////
    vector<Point2D> mesh;
    string buffer;
    ifstream HCMUTCampus("HCMUT_campus.txt");

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
    std:: cout << "Map Size: " << mesh.size() << endl;

    vector<PS> myMap;

    for (int i = 0; i < mesh.size(); i++)
    {
        myMap.push_back(calculatePSrelation(mesh[i], mesh));
    }

    //////// offline phase ////////
    Point2D lostPoint(5, 3);
    Point2D closestPoint;
    vector<Point2D> resultPoints;
    float minDistance = 10000;
    for (auto x : mesh)
    {
        if (minDistance > sqrt(pow(lostPoint.x - x.x, 2) + pow(lostPoint.y - x.y, 2)))
        {
            minDistance = sqrt(pow(lostPoint.x - x.x, 2) + pow(lostPoint.y - x.y, 2));
            closestPoint = x;
        }
    }
    std:: cout << "Closest point: " << closestPoint.x << " " << closestPoint.y << endl;

    vector<Point2D> seeable_points;
    for (auto x : mesh)
    {
        if (sqrt(pow(lostPoint.x - x.x, 2) + pow(lostPoint.y - x.y, 2)) < EYE_RANGE)
        {
            seeable_points.push_back(x);
        }
    }

    PS seeable_PS;
    seeable_PS = calculatePSrelation(closestPoint, seeable_points);
    std:: cout << "Number of seeable points: " << seeable_points.size() << endl;

    // // print myMap
    // for (int i = 0; i < myMap.size(); i++){
    //     cout << "Index: " << i << endl;
    //     cout << "Distance: " << endl;
    //     for (auto x : myMap[i].distance){
    //         cout << x << " ";
    //     }
    //     cout << endl;
    //     cout << "Angle: " << endl;
    //     for (auto x : myMap[i].angle){
    //         cout << x << " ";
    //     }
    //     cout << endl;
    // }

    // cout <<endl;
    // // lost point PS
    // cout << "Lost point PS: " << endl;
    // cout << "Distance: " << endl;
    // for (auto x : seeable_PS.distance){
    //     cout << x << " ";
    // }
    // cout << endl;
    // cout << "Angle: " << endl;
    // for (auto x : seeable_PS.angle){
    //     cout << x << " ";
    // }
    // cout << endl;

    for (int i = 0; i < myMap.size(); i++)
    {
        // check if seeable_PS is a subset of myMap[i]
        bool isFind = true;
        
        for (int j = 0; j < seeable_PS.distance.size(); j++)
        {
            bool exist = false;
            for (int k = 0; k < myMap[i].distance.size(); k++)
            {
                if (seeable_PS.distance[j] == myMap[i].distance[k])
                {
                    exist = true;
                    break;
                }
            }
            if (!exist)
            {
                isFind = false;
                break;
            }
        }
        // cout << "isFind: " << isFind << endl;
        
        if (isFind) // seeable_PS is a subset of myMap[i]
        {
            // cout << "Seeable_PS angle: " <<endl;
            // for (auto abc : seeable_PS.angle){
            //     cout << abc << " ";
            // }
            // cout <<endl;

            // cout << "myMap[i] angle: " <<endl;
            // for (auto abc : myMap[i].angle){
            //     cout << abc << " ";
            // }
            // cout <<endl;

            float alpha = myMap[i].angle[0];
            int left_pointer = 0;
            int right_pointer = 0;
            float window_sum = 0;
            do
            {
                left_pointer %= seeable_PS.angle.size();
                right_pointer %= seeable_PS.angle.size();
                window_sum += seeable_PS.angle[right_pointer];
                if (window_sum > alpha)
                {
                    window_sum -= seeable_PS.angle[left_pointer];
                    window_sum -= seeable_PS.angle[right_pointer];
                    left_pointer++;
                }
                if (approx(window_sum, alpha))
                {
                    bool isMatch = true;
                    float accumulatedSum = 0;
                    for (int l = 1; l < myMap[i].angle.size(); l++)
                    {
                        for (int k = right_pointer; k != left_pointer; k = (k + 1) % seeable_PS.angle.size())
                        {
                            accumulatedSum += seeable_PS.angle[k];
                            if (accumulatedSum > myMap[i].angle[l])
                            {
                                isMatch = false;
                                break;
                            }
                            if (accumulatedSum == myMap[i].angle[l]) accumulatedSum = 0;
                            if (!isMatch) break;
                        }
                    }
                    if (isMatch){
                        std:: cout << "Found i: " << i + 1 << endl;
                        resultPoints.push_back(mesh[i]);
                        break;
                    };
                    window_sum -= seeable_PS.angle[left_pointer];
                    window_sum -= seeable_PS.angle[right_pointer];
                    left_pointer++;
                }
                if (window_sum < alpha)
                    right_pointer++;
            } while (left_pointer != 0);
        }
    }

    std:: cout << "----------------------------------------------" << endl;
    std:: cout << "Result:" << endl;
    for (auto result : resultPoints)
    {
        std:: cout << result.x << " " << result.y << endl;
    }

    return 0;
}