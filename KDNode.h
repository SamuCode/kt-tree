#ifndef KDNODE_H
#define KDNODE_H
#include "Point.h"
#include <array>

struct KDNode {
    Point point;
    KDNode* left;
    KDNode* right;

    KDNode(const Point& p) : point(p), left(nullptr), right(nullptr) {
        
    }
};

#endif
