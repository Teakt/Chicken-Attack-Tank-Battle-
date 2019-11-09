#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED


    #define TAILLE_BLOC         100 // Taille d'un bloc (carr�) en pixels
    #define NB_BLOCS_LARGEUR    13
    #define NB_BLOCS_HAUTEUR    7
    #define LARGEUR_FENETRE     TAILLE_BLOC * NB_BLOCS_LARGEUR
    #define HAUTEUR_FENETRE     TAILLE_BLOC * NB_BLOCS_HAUTEUR
    #define DEP_ENNEMI TAILLE_BLOC
    #define VITESSE_OBUS 3
    enum {HAUT, BAS, GAUCHE, DROITE};
    enum {VIDE, MUR, MURBLINDE, PLAYER , TITI , PROJECTILE , TANKENNEMI};
    enum {PLAYER_UN, PLAYER_DEUX, ENNEMI};


#endif // CONSTANTES_H_INCLUDED
