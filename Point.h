#ifndef POINT_H
#define POINT_H
#include "Constants.h"

struct Point {
    int id;
    double coords[DIM];

    double operator[](int i) const {
        return coords[i];
    }

    // Surcharge de l'opérateur == pour comparer deux points
    bool operator==(const Point& other) const {
        for (int i = 0; i < DIM; ++i) {
            if (coords[i] != other.coords[i]) {
                return false;
            }
        }
        return true;
    }
};

#endif
