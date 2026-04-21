#ifndef KDTREE_H
#define KDTREE_H

#include "KDNode.h"
#include "Constants.h"
#include <array>
#include <optional>
#include <cmath>
#include <limits>

class KDTree {
    public:
        KDTree() : root(nullptr) {}

        //Insertion
        void insert(const Point& point) {
            root = insertRec(root, point, 0);
        }

        //Recherche
        bool search(const Point& point) const {
            return searchRec(root, point, 0);
        }

        //Suppression
        void remove(const Point& point) {
            root = removeRec(root, point, 0);
        }

        // Plus proche voisin
        std::optional<Point> nearestNeighbor(const Point& target) const {
            if (!root) return std::nullopt;

            KDNode* best = nullptr;
            double bestDist = std::numeric_limits<double>::infinity();
            nearestRec(root, target, 0, best, bestDist);
            return best->point;
        }

    private:
        KDNode* root;

        //Insertion récursive
        KDNode* insertRec(KDNode* node, const Point& point, std::size_t depth) {
            if (!node) return new KDNode(point);

            std::size_t axis = depth % DIM;
            if (point[axis] < node->point[axis])
                node->left = insertRec(node->left, point, depth + 1);
            else
                node->right = insertRec(node->right, point, depth + 1);

            return node;
        }

        //Recherche récursive
        bool searchRec(KDNode* node, const Point& point, std::size_t depth) const {
            if (!node) return false;
            if (node->point == point) return true;

            std::size_t axis = depth % DIM;
            if (point[axis] < node->point[axis])
                return searchRec(node->left, point, depth + 1);
            else
                return searchRec(node->right, point, depth + 1);
        }

        // Trouver le nœud avec la valeur minimale sur un axe donné
        KDNode* findMin(KDNode* node, std::size_t axis, std::size_t depth) {
            if (!node) return nullptr;

            std::size_t currentAxis = depth % DIM;

            // Si l'axe actuel correspond à l'axe de recherche, on explore uniquement le sous-arbre gauche
            if (currentAxis == axis) {
                if (!node->left) return node;
                return findMin(node->left, axis, depth + 1);
            }

            KDNode* leftMin = findMin(node->left, axis, depth + 1);
            KDNode* rightMin = findMin(node->right, axis, depth + 1);

            KDNode* minNode = node;
            if (leftMin && leftMin->point[axis] < minNode->point[axis]) minNode = leftMin;
            if (rightMin && rightMin->point[axis] < minNode->point[axis]) minNode = rightMin;

            return minNode;
        }

        // Suppression récursive
        KDNode* removeRec(KDNode* node, const Point& point, std::size_t depth) {
            if (!node) return nullptr;

            std::size_t axis = depth % DIM;

            if (node->point == point) {
                if (node->right) {
                    KDNode* minNode = findMin(node->right, axis, depth + 1);
                    node->point = minNode->point;
                    node->right = removeRec(node->right, minNode->point, depth + 1);
                }
                else if (node->left) {
                    KDNode* minNode = findMin(node->left, axis, depth + 1);
                    node->point = minNode->point;
                    node->left = removeRec(node->left, minNode->point, depth + 1);
                } // sinon, le nœud est une feuille et peut être supprimé directement
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
        double dist2(const Point& a, const Point& b) const {
            double s = 0.0;
            for (std::size_t i = 0; i < 2; ++i) {
                double d = a[i] - b[i];
                s += d * d;
            }
            return s;
        }

        // Recherche du plus proche voisin récursive
        void nearestRec(KDNode* node,
                        const Point& target,
                        std::size_t depth,
                        KDNode*& best,
                        double& bestDist) const {
            if (!node) return;

            double d = dist2(node->point, target);
            if (d < bestDist) {
                bestDist = d;
                best = node;
            }

            std::size_t axis = depth % DIM;

            KDNode* first = (target[axis] < node->point[axis]) ? node->left : node->right;
            KDNode* second = (first == node->left) ? node->right : node->left;

            nearestRec(first, target, depth + 1, best, bestDist);

            double diff = target[axis] - node->point[axis];
            if (diff * diff < bestDist)
                nearestRec(second, target, depth + 1, best, bestDist);
        }
};

#endif
