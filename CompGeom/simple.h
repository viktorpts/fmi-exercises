#ifndef SIMPLE_H
#define SIMPLE_H
#endif

#include <iostream>
#include "vecta.h"

namespace std
{

template <typename N>
ostream &operator<<(ostream &os, vecta::vec2d<N> p)
{
    return os << p.x << ' ' << p.y;
}

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

} // namespace std