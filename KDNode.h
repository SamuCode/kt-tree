#ifndef KDNODE_H
#define KDNODE_H

#include <array>

template <std::size_t K>
struct KDNode {
    std::array<double, K> point;
    KDNode* left;
    KDNode* right;

    KDNode(const std::array<double, K>& p) : point(p), left(nullptr), right(nullptr) {}
};

#endif
