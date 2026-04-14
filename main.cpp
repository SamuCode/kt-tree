#include <iostream>
#include "KDTree.h"

int main() {
    KDTree<2> tree;

    //Insertion de points
    tree.insert({40, 45});
    tree.insert({15, 70});
    tree.insert({70, 10});
    tree.insert({69, 50});
    tree.insert({66, 85});
    tree.insert({85, 90});

    //Recherche
    std::cout << std::boolalpha;
    std::cout << "Recherche (70,50) : " << tree.search({70, 50}) << "\n";

    //Suppression
    tree.remove({71, 10});
    std::cout << "Recherche (70,10) après suppression : " << tree.search({70, 10}) << "\n";

    // Plus proche voisin
    auto nn = tree.nearestNeighbor({45, 45});
    if (nn) {
        std::cout << "Plus proche voisin de (45,45) : ("
                  << (*nn)[0] << ", " << (*nn)[1] << ")\n";
    } else {
        std::cout << "Aucun point dans l'arbre.\n";
    }

    return 0;
}
