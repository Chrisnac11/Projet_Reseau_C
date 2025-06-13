#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_EQUIPMENTS 100
static const size_t UNKNOWN_INDEX = -1;
#define TRAME_BUFFER_SIZE 256

extern uint8_t trame_buffer[TRAME_BUFFER_SIZE];

typedef size_t sommet;

typedef struct arete
{
    sommet s1;
    sommet s2;
} arete;

// ==================== Structures du réseau ====================

typedef uint64_t MAC;

typedef struct {
    uint8_t adr_depart;
    uint8_t adr_visee;
    int type;
    char* data;
    char* bourrage;
} trame;

void copier_trame_dans_buffer(const trame* t);

void recuperer_trame_du_buffer(trame* t);

typedef struct {
    char* nom;
    MAC mac;
    int IP[4];
} Struct_station;

typedef struct {
    MAC machine;
    uint8_t port;
} Struct_association;

typedef void (*fonction_ptr)(void);

typedef struct {
    char* nom;
    MAC mac;
    int nb_ports;
    int priorite;
    Struct_association commutation[128];
} Struct_switch;


Struct_switch* init_machine(const char* mac_str, int nb_ports, int priorite);
Struct_station* init_pc(const char* mac_str, const char* ip_str);

typedef struct {
    MAC mac_n1;
    MAC mac_n2;
} Struct_arete;

typedef enum { STATION = 1, SWITCH = 2 } TypeEquipement;

typedef struct {
    TypeEquipement type;
    void* equipement;  // Peut pointer vers Struct_station ou Struct_switch
    trame stockage;
} Equipement;



typedef struct graphe
{
    size_t ordre;
    arete* aretes;
    size_t aretes_capacite;
    size_t nb_aretes;
    Equipement equipements[MAX_EQUIPMENTS];
} graphe;



void init_graphe(graphe *g);

int IP(Struct_station *s);
void afficherIP (Struct_station *s);
void ip_str_to_array(const char* ip_str, int ip[4]);

MAC Mac(Struct_station *s);
void afficher_mac(Struct_station *s);
uint64_t mac_str_to_uint64(const char* mac_str);

graphe creerReseau(const char* nom_fichier);

void ajouter_sommet(graphe *g);
size_t index_sommet(graphe const *g, sommet s);
bool existe_arete(graphe const *g, arete a);
bool ajouter_arete(graphe *g, arete a);

void deinit_reseau(graphe* g);

void afficher_reseau(const graphe* g);
void afficher_equipement(const Equipement* eq);
void afficher_lien(const arete* lien);