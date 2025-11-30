C Network Simulation


Projet de simulation réseau réalisé en C dans le cadre d'un projet d'études.
L’objectif était de représenter un réseau à partir d’un fichier de configuration, de mettre en place les structures nécessaires à son fonctionnement, et de préparer une simulation du protocole Ethernet ainsi qu’une future extension vers STP.


Le projet consiste à modéliser un réseau composé de différents équipements (switches, hôtes, liens), selon une configuration chargée depuis un fichier externe.
Il vise à poser les bases d’une simulation où des messages pourraient transiter entre équipements, selon les règles du protocole Ethernet.

Implémenté

    création des structures de données représentant le réseau
    gestion des équipements (switches, hôtes, liens)
    chargement complet du fichier de configuration
    représentation correcte de l’état initial du réseau (t = 0)

Ce qui n’est pas implémenté

    simulation du protocole Ethernet
    propagation et stockage des messages
    mécanisme d’apprentissage des tables de commutation
    protocole STP (Spanning Tree Protocol)
    
Objectifs du projet

    représenter un réseau à partir d’un fichier
    préparer une simulation de messages entre équipements
    poser les fondations d’une simulation Ethernet et STP
    


Membres du groupe

    AKGUL Ilian
    ROSMANN Henri

Notes

Projet non terminé : seules les structures de base et le chargement du réseau ont été réalisés.
