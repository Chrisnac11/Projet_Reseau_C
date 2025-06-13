#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"

int main(int argc, char* argv[]) {
    // Vérifier si un fichier réseau est passé en argument
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_fichier_reseau>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Charger un graphe à partir du fichier spécifié
    const char* nom_fichier = argv[1];
    graphe g = creerReseau(nom_fichier);

    // Afficher les informations du graphe
    afficher_reseau(&g);

    // Afficher les échanges de trames
    afficher_echanges(&g);

    // Libérer la mémoire allouée pour le graphe
    deinit_reseau(&g);

    return EXIT_SUCCESS;
}