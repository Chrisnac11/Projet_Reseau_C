#include "graphe.h"

int main() {
    // Charger un graphe à partir d'un fichier
    graphe g = creerReseau("test_reseau.txt");

    // Afficher les informations du graphe
    afficher_reseau(&g);

    // Libérer la mémoire allouée pour le graphe
    deinit_reseau(&g);

    return 0;
}