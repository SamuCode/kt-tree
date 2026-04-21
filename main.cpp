#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include "KDTree.h"
using namespace std;

// Fonction pour valider et lire un entier
int lireEntier(const string& prompt) {
    int valeur;
    while (true) {
        cout << prompt;
        cin >> valeur;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrée invalide. Veuillez saisir un entier.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valeur;
        }
    }
}

// Fonction pour valider et lire un double
double lireDouble(const string& prompt) {
    double valeur;
    while (true) {
        cout << prompt;
        cin >> valeur;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrée invalide. Veuillez saisir un nombre réel.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valeur;
        }
    }
}

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
        choix = lireEntier("Votre choix : ");

        if (choix < 0 || choix > 4) {
            cout << "Choix invalide. Veuillez choisir entre 0 et 4.\n";
            continue;
        }

        if (choix == 1) {
            int id = lireEntier("ID : ");
            double x = lireDouble("X : ");
            double y = lireDouble("Y : ");

            // Vérifier si l'ID existe déjà
            bool idExists = false;
            for (const auto& pt : listePoints) {
                if (pt.id == id) {
                    idExists = true;
                    break;
                }
            }
            if (idExists) {
                cout << "Erreur : Un point avec cet ID existe déjà.\n";
                continue;
            }

            // Vérifier si le point (coordonnées) existe déjà
            if (tree.search(Point{-1, {x, y}})) {
                cout << "Erreur : Un point avec ces coordonnées existe déjà.\n";
                continue;
            }

            tree.insert(Point{id, {x, y}});
            listePoints.push_back(Point{id, {x, y}});
            std::cout << "Point ajouté.\n";
        }

        else if (choix == 2) {
            double x = lireDouble("X : ");
            double y = lireDouble("Y : ");

            bool found = tree.search(Point{-1, {x, y}});
            std::cout << (found ? "Point trouvé.\n" : "Point non trouvé.\n");
        }

        else if (choix == 3) {
            double x = lireDouble("X : ");
            double y = lireDouble("Y : ");

            tree.remove(Point{-1, {x, y}});
            std::cout << "Suppression effectuée.\n";
        }

        else if (choix == 4) {
            double x = lireDouble("X : ");
            double y = lireDouble("Y : ");

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
