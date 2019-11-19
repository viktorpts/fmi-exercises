#include <iostream>
#include <vector>
#include <string>
#include "simple.h"

using namespace std;
using namespace vecta;

vector<vec2d<>> input2poly(int argc, char *argv[])
{
    if ((argc - 1) % 2 != 0)
    {
        cout << "Arguments must be x y pairs. Number of arguments must be even." << endl;
        throw;
    }
    else
    {
        vector<vec2d<>> polygon;
        for (int i = 1; i < argc; i += 2)
        {
            vec2d<> p(stod(argv[i], nullptr), stod(argv[i + 1], nullptr));
            polygon.push_back(p);
        }

        return polygon;
    }
}

double orientedArea(vec2d<> A1, vec2d<> A2, vec2d<> A3)
{
    return ((A2 - A1) ^ (A3 - A1));
}

vec2d<> projectUtoV(vec2d<> u, vec2d<> v)
{
    auto scalar = (u * v) / (v.x * v.x + v.y * v.y);
    return v * scalar;
}

bool isInTriangle(vec2d<> point, vec2d<> node, vec2d<> next, vec2d<> prev)
{
    int sign = sgn(orientedArea(node, next, prev));
    int s1 = sgn(orientedArea(point, node, next));
    int s2 = sgn(orientedArea(point, next, prev));
    int s3 = sgn(orientedArea(point, prev, next));

    return sign == s1 && sign == s2 & sign == s3;
}

bool pointOnLine(vec2d<> point, vec2d<> u, vec2d<> v) {
    auto ux = point - u;
    auto vx = point - v;
    return (ux ^ vx) == 0;
}

bool pointInSegment(vec2d<> point, vec2d<> u, vec2d<> v)
{
    // It's assumed the colinearility has been determined elsewhere
    auto ux = point - u;
    auto vx = point - v;
    return ux * vx <= 0;
}

bool segmentIntersect(vec2d<> p, vec2d<> q, vec2d<> u, vec2d<> v)
{
    // Solution does not check for colinearity
    // Returns true if one node lies within the other segment
    return sgn(orientedArea(p, q, u)) != sgn(orientedArea(p, q, v)) && sgn(orientedArea(u, v, p)) != sgn(orientedArea(u, v, q));
}

bool isConvex(vector<vec2d<>> polygon)
{
    int size = polygon.size();
    if (size < 3)
    {
        return true;
    }

    int prevSign = sgn(orientedArea(polygon[0], polygon[1], polygon[2]));
    for (int i = 1; i < size; i++)
    {
        int i2 = i < size - 1 ? i + 1 : i + 1 - size;
        int i3 = i < size - 2 ? i + 2 : i + 2 - size;
        int currentSign = sgn(orientedArea(polygon[i], polygon[i2], polygon[i3]));
        // Note if two edges are colinear, the polygon is still convex, hence < 0, instead of <= 0
        if (prevSign * currentSign < 0)
        {
            return false;
        }
        prevSign = currentSign;
    }

    return true;
}

int main(int argc, char *argv[])
{
    cout << isConvex(input2poly(argc, argv)) << endl;
    return 0;
}