#include "graphe.h"

#define INITIAL_CAPACITE 8

void init_graphe(graphe *g)
{
    // allocation d'un tableau d'arêtes de capacité initiale 8
    // le graphe ne contient initialement ni sommet ni arête
    g->nb_aretes=0;
    g->aretes_capacite=INITIAL_CAPACITE;
    g->ordre=0;
    g->aretes=malloc(g->aretes_capacite*sizeof(arete));
}

void ajouter_sommet(graphe *g)
{
    g->ordre++;
}

size_t index_sommet(graphe const *g, sommet s)
{
    // retourne l'index du sommet s dans le graphe g
    // la valeur UNKNOWN_INDEX si le sommet n'existe pas dans g)
    if (s >= g->ordre)
    {
        return UNKNOWN_INDEX;
    }
    return s;
}

// une fonction locale "static arete swap_sommets(arete a)" pourra être utile
// cette fonction retourne une nouvelle arête dont les sommets sont les même que l'arête reçue mais inversés

bool existe_arete(graphe const *g, arete a)
{
    // retourne true si l'arête a est contenue dans le graphe g, false sinon
    // /!\ l'arête (s1,s2) et l'arête (s2,s1) sont considérées équivalentes
    for (size_t i = 0; i < g->nb_aretes; i++)
    {
        if ((g->aretes[i].s1 == a.s1 && g->aretes[i].s2 == a.s2) || (g->aretes[i].s1 == a.s2 && g->aretes[i].s2 == a.s1))
        {
            return true;
        }
    }
    // si l'arête n'est pas trouvée dans le graphe, on retourne false
    return false;
}

bool ajouter_arete(graphe *g, arete a)
{
    // l'arête a n'est ajoutée que si les conditions suivantes sont remplies :
    //  - les sommets s1 et s2 de a existent dans g
    if (index_sommet(g, a.s1) == UNKNOWN_INDEX || index_sommet(g, a.s2) == UNKNOWN_INDEX)
    {
        return false;
    }
    //  - les sommets s1 et s2 de a sont distincts
    if (a.s1 == a.s2)
    {
        return false;
    }
    //  - l'arête a n'existe pas dans g
    if(existe_arete(g, a))
    {
        return false;
    }

    // /!\ si la capacité actuelle du tableau d'arêtes n'est pas suffisante,
    // /!\ il faut le réallouer.
    // /!\ on peut par exemple doubler la capacité du tableau actuel.

    if (g->nb_aretes == g->aretes_capacite)
    {
        g->aretes_capacite = g->aretes_capacite * 2;
        g->aretes = realloc(g->aretes, g->aretes_capacite * sizeof(arete));
        if (g->aretes == NULL)
        {
            fprintf(stderr, "Erreur d'allocation mémoire\n");
            exit(EXIT_FAILURE);
        }
    }
    g->aretes[g->nb_aretes] = a;
    g->nb_aretes++;
    // retourne true si l'arête a bien été ajoutée, false sinon
    return true;
}

int* getIP(Struct_station *s) {
    return s->IP;
}

MAC getMac(Struct_station *s) {
    return s->mac;
}

void afficherIP (Struct_station *s) {
    int* ip_ptr = getIP(s);
    int ip[4];

    for (int i = 0; i < 4; i++) {
        ip[i] = ip_ptr[i];
    }
    printf("IP: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
}

void afficher_mac(Struct_station *s) {
    MAC mac = getMac(s);
    unsigned char octets[6];
    for (int i = 0; i < 6; i++) {
        octets[5 - i] = (mac >> (i * 8)) & 0xFF;
    }

    printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
           octets[0], octets[1], octets[2],
           octets[3], octets[4], octets[5]);
}

uint64_t mac_str_to_uint64(const char* mac_str) {
    uint64_t mac = 0;
    unsigned int octets[6];
    sscanf(mac_str, "%x:%x:%x:%x:%x:%x",
           &octets[0], &octets[1], &octets[2],
           &octets[3], &octets[4], &octets[5]);
    for (int i = 0; i < 6; i++) {
        mac = (mac << 8) | (octets[i] & 0xFF);
    }
    return mac;
}

void ip_str_to_array(const char* ip_str, int ip[4]) {
    sscanf(ip_str, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
}

Struct_station* get_station_by_mac(Equipement* equipements, size_t nb_equipements, MAC mac) {
    for (size_t i = 0; i < nb_equipements; i++) {
        if (equipements[i].type == STATION) {
            Struct_station* station = (Struct_station*)equipements[i].equipement;
            if (station->mac == mac) {
                return station;
            }
        }
    }

    fprintf(stderr, "Erreur : aucune station avec l'adresse MAC spécifiée n'a été trouvée.\n");
    return NULL;
}

Struct_switch* init_machine(const char* mac_str, int nb_ports, int priorite) {
    Struct_switch* sw = malloc(sizeof(Struct_switch));
    if (sw == NULL) {
        perror("Erreur d'allocation mémoire pour Struct_switch");
        exit(EXIT_FAILURE);
    }
    sw->mac = mac_str_to_uint64(mac_str);
    sw->nb_ports = nb_ports;
    sw->priorite = priorite;
    return sw;
}

Struct_station* init_pc(const char* mac_str, const char* ip_str) {
    Struct_station* pc = malloc(sizeof(Struct_station));
    if (pc == NULL) {
        perror("Erreur d'allocation mémoire pour Struct_station");
        exit(EXIT_FAILURE);
    }
    pc->mac = mac_str_to_uint64(mac_str);
    ip_str_to_array(ip_str, pc->IP);
    return pc;
}

graphe creerReseau(const char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[MAX_LINE_LENGTH];
    fgets(ligne, sizeof(ligne), fichier);

    int nb_equipements = 0, nb_liens = 0;
    sscanf(ligne, "%d %d", &nb_equipements, &nb_liens);

    Equipement equipements[MAX_EQUIPMENTS];

    // Lecture des équipements
    for (int i = 0; i < nb_equipements; i++) {
        fgets(ligne, sizeof(ligne), fichier);

        char* type_str = strtok(ligne, ";");
        int type = atoi(type_str);

        if (type == SWITCH) {
            char* mac_str = strtok(NULL, ";");
            char* nb_ports_str = strtok(NULL, ";");
            char* priorite_str = strtok(NULL, ";");

            int nb_ports = atoi(nb_ports_str);
            int priorite = atoi(priorite_str);

            Struct_switch* sw = init_machine(mac_str, nb_ports, priorite);
            equipements[i].type = SWITCH;
            equipements[i].equipement = sw;

        } else if (type == STATION) {
            char* mac_str = strtok(NULL, ";");
            char* ip_str = strtok(NULL, ";\n");

            Struct_station* pc = init_pc(mac_str, ip_str);
            equipements[i].type = STATION;
            equipements[i].equipement = pc;
        }
    }

    // Création du graphe
    graphe g;
    init_graphe(&g);

    for (int i = 0; i < nb_equipements; i++) {
        ajouter_sommet(&g);
    }

    // Lecture des liens
    for (int i = 0; i < nb_liens; i++) {
        fgets(ligne, sizeof(ligne), fichier);

        int from, to, poids;
        sscanf(ligne, "%d;%d;%d", &from, &to, &poids);

        arete a = { .s1 = from, .s2 = to };
        ajouter_arete(&g, a);
        // Si vous stockez le poids, ajoutez le à une structure annexe ou modifiez `arete`
    }


    fclose(fichier);

    // Exemple : afficher les IP des stations
    for (int i = 0; i < nb_equipements; i++) {
        if (equipements[i].type == STATION) {
            Struct_station* s = (Struct_station*)equipements[i].equipement;
            printf("Station %d: IP = %d.%d.%d.%d\n", i, s->IP[0], s->IP[1], s->IP[2], s->IP[3]);
        }
    }

    return g;
}

// Définition du buffer global
uint8_t trame_buffer[TRAME_BUFFER_SIZE];

// Copier une trame dans le buffer global
void copier_trame_dans_buffer(const trame* t) {
    if (t == NULL) {
        fprintf(stderr, "Erreur : trame NULL\n");
        return;
    }

    // Copie de la structure de base dans le buffer global
    memcpy(trame_buffer, t, sizeof(trame));

    // Gestion des champs dynamiques
    trame* buffer_trame = (trame*)trame_buffer;
    if (t->data != NULL) {
        buffer_trame->data = malloc(strlen(t->data) + 1);
        strcpy(buffer_trame->data, t->data);
    }
    if (t->bourrage != NULL) {
        buffer_trame->bourrage = malloc(strlen(t->bourrage) + 1);
        strcpy(buffer_trame->bourrage, t->bourrage);
    }
}

// Récupérer une trame à partir du buffer global
void recuperer_trame_du_buffer(trame* t) {
    if (t == NULL) {
        fprintf(stderr, "Erreur : pointeur de trame NULL\n");
        return;
    }

    // Copie de la structure de base depuis le buffer global
    memcpy(t, trame_buffer, sizeof(trame));

    // Gestion des champs dynamiques
    trame* buffer_trame = (trame*)trame_buffer;
    if (buffer_trame->data != NULL) {
        t->data = malloc(strlen(buffer_trame->data) + 1);
        strcpy(t->data, buffer_trame->data);
    }
    if (buffer_trame->bourrage != NULL) {
        t->bourrage = malloc(strlen(buffer_trame->bourrage) + 1);
        strcpy(t->bourrage, buffer_trame->bourrage);
    }
}

bool stations_meme_graphe(graphe* g, trame* t) {
    if (g == NULL || t == NULL) {
        fprintf(stderr, "Erreur : graphe ou trame NULL\n");
        return false;
    }

    // Récupérer les adresses MAC des deux stations depuis la trame
    MAC mac_station1 = t->adr_depart;
    MAC mac_station2 = t->adr_visee;

    // Vérifier si les deux stations existent dans le graphe
    Struct_station* station1 = get_station_by_mac(g->equipements, g->ordre, mac_station1);
    Struct_station* station2 = get_station_by_mac(g->equipements, g->ordre, mac_station2);

    if (station1 == NULL || station2 == NULL) {
        fprintf(stderr, "Erreur : une ou les deux stations n'existent pas dans le graphe\n");
        return false;
    }

    // Vérifier si les deux stations sont connectées dans le graphe
    for (size_t i = 0; i < g->nb_aretes; i++) {
        arete a = g->aretes[i];
        if ((a.s1 == station1->mac && a.s2 == station2->mac) ||
            (a.s1 == station2->mac && a.s2 == station1->mac)) {
            return true; // Les stations sont connectées
        }
    }

    // Si aucune connexion n'a été trouvée
    return false;
}

void deinit_reseau(graphe* g) {
    if (g == NULL) {
        return; // Rien à libérer si le graphe est NULL
    }

    // Libérer les équipements
    for (size_t i = 0; i < g->ordre; i++) {
        Equipement* equipement = &g->equipements[i];
        if (equipement->type == STATION) {
            Struct_station* station = (Struct_station*)equipement->equipement;
            free(station); // Libérer la mémoire allouée pour la station
        } else if (equipement->type == SWITCH) {
            Struct_switch* sw = (Struct_switch*)equipement->equipement;
            free(sw); // Libérer la mémoire allouée pour le switch
        }
    }

    // Libérer les arêtes
    free(g->aretes);
    g->aretes = NULL;

    // Réinitialiser les champs du graphe
    g->ordre = 0;
    g->nb_aretes = 0;
    g->aretes_capacite = 0;
}

void afficher_reseau(const graphe* g) {
    if (g == NULL) {
        printf("Erreur : graphe NULL\n");
        return;
    }

    printf("=== Réseau ===\n");
    printf("Nombre d'équipements : %zu\n", g->ordre);
    printf("Nombre de liens : %zu\n", g->nb_aretes);

    // Afficher les équipements
    printf("\n--- Équipements ---\n");
    for (size_t i = 0; i < g->ordre; i++) {
        afficher_equipement(&g->equipements[i]);
    }

    // Afficher les liens
    printf("\n--- Liens ---\n");
    for (size_t i = 0; i < g->nb_aretes; i++) {
        afficher_lien(&g->aretes[i]);
    }
}

void afficher_equipement(const Equipement* eq) {
    if (eq == NULL) {
        printf("Erreur : équipement NULL\n");
        return;
    }

    switch (eq->type) {
        case STATION: {
            Struct_station* station = (Struct_station*)eq->equipement;
            printf("Type : Station\n");
            printf("  Nom : %s\n", station->nom ? station->nom : "Inconnu");
            printf("  MAC : %lu\n", station->mac);
            printf("  IP  : %d.%d.%d.%d\n",
                   station->IP[0], station->IP[1], station->IP[2], station->IP[3]);
            break;
        }
        case SWITCH: {
            Struct_switch* sw = (Struct_switch*)eq->equipement;
            printf("Type : Switch\n");
            printf("  Nom : %s\n", sw->nom ? sw->nom : "Inconnu");
            printf("  MAC : %lu\n", sw->mac);
            printf("  Nombre de ports : %d\n", sw->nb_ports);
            printf("  Priorité : %d\n", sw->priorite);
            break;
        }
        default:
            printf("Type d'équipement inconnu\n");
            break;
    }
}

void afficher_lien(const arete* lien) {
    if (lien == NULL) {
        printf("Erreur : lien NULL\n");
        return;
    }

    printf("Lien :\n");
    printf("  Équipement 1 (MAC) : %zu\n", lien->s1);
    printf("  Équipement 2 (MAC) : %zu\n", lien->s2);
}