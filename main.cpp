#include <iostream>
#include <fstream>
#include <vector>
#include "KDTree.h"
using namespace std;

int main() {
    KDTree tree;

    vector<Point> listePoints; 

    // --- LECTURE DU FICHIER : ville.txt---
    ifstream fichier("ville.txt");
    if (!fichier.is_open()) {
        cerr << "Erreur: Impossible d'ouvrir ville.txt. Verifiez l'emplacement du fichier." << endl;
        return 1;
    }

    Point p;
    cout << "Chargement des points dans l'arbre..." << endl;

    // Lecture ligne par ligne au format : ID  COORDONNÉE_X  COORDONNÉE_Y
    while (fichier >> p.id >> p.coords[0] >> p.coords[1]) {
        tree.insert(p);
        listePoints.push_back(p); 
    }
    fichier.close();

    if (listePoints.empty()) {
        cout << "Aucune donnée chargee. Fin du programme." << endl;
        return 1;
    }

    cout << listePoints.size() << " points chargés avec succès." << endl;

    int choix;

    do {
        std::cout << "\n===== MENU KD-TREE =====\n";
        std::cout << "1. Ajouter un point\n";
        std::cout << "2. Rechercher un point\n";
        std::cout << "3. Supprimer un point\n";
        std::cout << "4. Rechercher le plus proche voisin\n";
        std::cout << "0. Quitter\n" << std::endl;
        std::cout << "Votre choix : ";
        std::cin >> choix;

        if (choix == 1) {
            int id;
            double x, y;
            std::cout << "ID : ";
            std::cin >> id;
            std::cout << "X : ";
            std::cin >> x;
            std::cout << "Y : ";
            std::cin >> y;

            tree.insert(Point{id, {x, y}});
            std::cout << "Point ajouté.\n";
        }

        else if (choix == 2) {
            double x, y;
            std::cout << "X : ";
            std::cin >> x;
            std::cout << "Y : ";
            std::cin >> y;

            bool found = tree.search(Point{-1, {x, y}});
            std::cout << (found ? "Point trouvé.\n" : "Point non trouvé.\n");
        }

        else if (choix == 3) {
            double x, y;
            std::cout << "X : ";
            std::cin >> x;
            std::cout << "Y : ";
            std::cin >> y;

            tree.remove(Point{-1, {x, y}});
            std::cout << "Suppression effectuée.\n";
        }

        else if (choix == 4) {
            double x, y;
            std::cout << "X : ";
            std::cin >> x;
            std::cout << "Y : ";
            std::cin >> y;

            auto nn = tree.nearestNeighbor(Point{-1, {x, y}});
            if (nn.has_value()) {
                std::cout << "Plus proche voisin : ID=" << nn->id
                          << " (" << nn->coords[0] << ", " << nn->coords[1] << ")\n";
            } else {
                std::cout << "Aucun point dans l'arbre.\n";
            }
        }

    } while (choix != 0);


    return 0;
}
