#include <iostream>
#include <vector>
#include <string>
#include "vecta.h"

using namespace std;
using namespace vecta;

template <typename N>
ostream &operator<<(ostream &os, vec2d<N> p)
{
    return os << p.x << ' ' << p.y;
}

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

void renderPolygon(vector<vec2d<>> polygon)
{
    int size = polygon.size();

    // Outline
    cout << "width .3\ncolour purple\nlines\n";
    for (int i = 0; i < size; i++)
    {
        if (i != size - 1)
        {
            cout << polygon[i] << " " << polygon[i + 1] << endl;
        }
        else
        {
            cout << polygon[i] << " " << polygon[0] << endl;
        }
    }

    // Points
    cout << "colour red\npoints\n";
    for (int i = 0; i < size; i++)
    {
        cout << polygon[i] << endl;
    }
}

void renderSplit(vec2d<> p1, vec2d<> p2)
{
    cout << "colour green\nlinetype 0 1\nlines\n"
         << p1 << " " << p2 << endl;
}

double orientedArea(vec2d<> A1, vec2d<> A2, vec2d<> A3)
{
    return ((A2 - A1) ^ (A3 - A1));
}

int selectExtremeNode(vector<vec2d<>> polygon)
{
    auto nodeIndex = 0;

    int size = polygon.size();
    for (int i = 1; i < size; i++)
    {
        if (polygon[i].x > polygon[nodeIndex].x)
        {
            nodeIndex = i;
        }
    }

    return nodeIndex;
}

bool isInTriangle(vec2d<> point, vec2d<> node, vec2d<> next, vec2d<> prev)
{
    int sign = sgn(orientedArea(node, next, prev));
    int s1 = sgn(orientedArea(point, node, next));
    int s2 = sgn(orientedArea(point, next, prev));
    int s3 = sgn(orientedArea(point, prev, next));

    return sign == s1 && sign == s2 & sign == s3;
}

void triangulate(vector<vec2d<>> polygon)
{
    int size = polygon.size();

    // Select starting node, based on coordinate extremes
    int node = selectExtremeNode(polygon);

    // Construct triangle, taking into account cyclic selection
    int next = node == size - 1 ? 0 : node + 1;
    int prev = node == 0 ? size - 1 : node - 1;

    // TODO Check if triangle is internal to the polygon

    // Check reaining points for falling inside triangle [node, node+1, node-1]
    vector<vec2d<>> insidePoints;
    for (int i = 0; i < size; i++)
    {
        // Skip triangle nodes
        if (i == node || i == prev || i == next)
            continue;
        if (isInTriangle(polygon[i], node, next, prev))
        {
            insidePoints.push_back(i);
        }
    }

    // If no point is inside triangle -> cut segment (ear) and repeat for reduced polygon (remove node)
    if (insidePoints.size() == 0)
    {
        // Draw line to mark split
        renderSplit(polygon[next], polygon[prev]);

        // Generate reduced polygon, if we have more than 3 points left
        if (size > 4)
        {
            vector<vec2d<>> reducedPolygon;
            for (int i = 0; i < size; i++)
            {
                if (i == node)
                    continue;
                reducedPolygon.push_back(polygon[i]);
            }
            triangulate(reducedPolygon);
        }
    }

    // TODO Else, split polygon along the line [node, furthest node from [node+1, node-1]] and repeat for each half
}

int main(int argc, char *argv[])
{
    if ((argc - 1) % 2 != 0)
    {
        cout << "Arguments must be x y pairs. Number of arguments must be even." << endl;
        for (int i = 1; i < argc; i += 2)
        {
            cout << argv[i] << " ";
        }
        return 1;
    }

    vector<vec2d<>> polygon;
    for (int i = 1; i < argc; i += 2)
    {
        vec2d<> p(stod(argv[i], nullptr), stod(argv[i + 1], nullptr));
        polygon.push_back(p);
    }

    renderPolygon(polygon);
    triangulate(polygon);

    return 0;
}