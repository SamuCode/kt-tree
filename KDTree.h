#ifndef KDTREE_H
#define KDTREE_H

#include "KDNode.h"
#include <array>
#include <optional>
#include <cmath>
#include <limits>

template <std::size_t K>
class KDTree {
    public:
        KDTree() : root(nullptr) {}

        //Insertion
        void insert(const std::array<double, K>& point) {
            root = insertRec(root, point, 0);
        }

        //Recherche
        bool search(const std::array<double, K>& point) const {
            return searchRec(root, point, 0);
        }

        //Suppression
        void remove(const std::array<double, K>& point) {
            root = removeRec(root, point, 0);
        }

        // Plus proche voisin
        std::optional<std::array<double, K>> nearestNeighbor(const std::array<double, K>& target) const {
            if (!root) return std::nullopt;

            KDNode<K>* best = nullptr;
            double bestDist = std::numeric_limits<double>::infinity();
            nearestRec(root, target, 0, best, bestDist);
            return best->point;
        }

    private:
        KDNode<K>* root;

        //Insertion récursive
        KDNode<K>* insertRec(KDNode<K>* node, const std::array<double, K>& point, std::size_t depth) {
            if (!node) return new KDNode<K>(point);

            std::size_t axis = depth % K;
            if (point[axis] < node->point[axis])
                node->left = insertRec(node->left, point, depth + 1);
            else
                node->right = insertRec(node->right, point, depth + 1);

            return node;
        }

        //Recherche récursive
        bool searchRec(KDNode<K>* node, const std::array<double, K>& point, std::size_t depth) const {
            if (!node) return false;
            if (node->point == point) return true;

            std::size_t axis = depth % K;
            if (point[axis] < node->point[axis])
                return searchRec(node->left, point, depth + 1);
            else
                return searchRec(node->right, point, depth + 1);
        }

        // Trouver le nœud avec la valeur minimale sur un axe donné
        KDNode<K>* findMin(KDNode<K>* node, std::size_t axis, std::size_t depth) {
            if (!node) return nullptr;

            std::size_t currentAxis = depth % K;

            if (currentAxis == axis) {
                if (!node->left) return node;
                return findMin(node->left, axis, depth + 1);
            }

            KDNode<K>* leftMin = findMin(node->left, axis, depth + 1);
            KDNode<K>* rightMin = findMin(node->right, axis, depth + 1);

            KDNode<K>* minNode = node;
            if (leftMin && leftMin->point[axis] < minNode->point[axis]) minNode = leftMin;
            if (rightMin && rightMin->point[axis] < minNode->point[axis]) minNode = rightMin;

            return minNode;
        }

        // Suppression récursive
        KDNode<K>* removeRec(KDNode<K>* node, const std::array<double, K>& point, std::size_t depth) {
            if (!node) return nullptr;

            std::size_t axis = depth % K;

            if (node->point == point) {
                if (node->right) {
                    KDNode<K>* minNode = findMin(node->right, axis, depth + 1);
                    node->point = minNode->point;
                    node->right = removeRec(node->right, minNode->point, depth + 1);
                }
                else if (node->left) {
                    KDNode<K>* minNode = findMin(node->left, axis, depth + 1);
                    node->point = minNode->point;
                    node->right = removeRec(node->left, minNode->point, depth + 1);
                    node->left = nullptr;
                }
                else {
                    delete node;
                    return nullptr;
                }
                return node;
            }

            if (point[axis] < node->point[axis])
                node->left = removeRec(node->left, point, depth + 1);
            else
                node->right = removeRec(node->right, point, depth + 1);

            return node;
        }

        // Calcul de la distance au carré entre deux points
        double dist2(const std::array<double, K>& a, const std::array<double, K>& b) const {
            double s = 0.0;
            for (std::size_t i = 0; i < K; ++i) {
                double d = a[i] - b[i];
                s += d * d;
            }
            return s;
        }

        // Recherche du plus proche voisin récursive
        void nearestRec(KDNode<K>* node,
                        const std::array<double, K>& target,
                        std::size_t depth,
                        KDNode<K>*& best,
                        double& bestDist) const {
            if (!node) return;

            double d = dist2(node->point, target);
            if (d < bestDist) {
                bestDist = d;
                best = node;
            }

            std::size_t axis = depth % K;

            KDNode<K>* first = (target[axis] < node->point[axis]) ? node->left : node->right;
            KDNode<K>* second = (first == node->left) ? node->right : node->left;

            nearestRec(first, target, depth + 1, best, bestDist);

            double diff = target[axis] - node->point[axis];
            if (diff * diff < bestDist)
                nearestRec(second, target, depth + 1, best, bestDist);
        }
};

#endif
