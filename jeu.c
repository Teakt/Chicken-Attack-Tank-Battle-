#include "librairies.h"
#include "constantes.h"
#include "jeu.h"
#include "math.h"



typedef enum { false , true } bool ;
//bool reset = true ;


int titiMort = false ;
//srand(time(NULL));
int timer = 0 ;

typedef struct obus OBUS;
struct obus
{
    int obus_ennemi ; // verifie si l obus est tire par l ennemi ou pas
    int direction ;
    bool actif ;

    SDL_Rect positionObus; // pour recup la position de l'obus
    int Type; /*’M’ => mon tank, ’E’ => tank ennemi*/
    SDL_Surface * obusActuel ;

    struct obus * suivant; /*Pointeur vers un prochain obus*/
};

typedef struct tank TANK;
struct tank {
    int direction ; /*N => Nord, S => Sud, E => EST, O => OUEST*/

    int Blindage; /*niveau de blindage en cours du tank (0 => rien, 1 => blindé, 2 => ultra-blindé)*/
    int Blindage_orig; /*Blindage d’origine*/


    bool actif ;

    OBUS obus ;

    SDL_Rect positionJoueur;
    SDL_Surface *tankActuel ;
    SDL_Surface * image_ennemi[4] ;

    int hp; /*Nombre de fois que le tank est touché*/
    //char Carrosserie [5][28];/*Carrosserie du tank, servira pour l’affichage du tank à tout moment*/
    //int Type; /*’M’ => mon tank, ’E’ => tank ennemi*/
    int Etat; /*État du tank 1 => actif, 2 => en destruction, 3 => inactif*/
    //int Mise_a_jour; /*utile pour la suppression du tank en tenant compte d’un delay*/
    struct tank * suivant; /*Pointeur vers un prochain tank*/ /*Vous pouvez rajouter d’autres variables si nécessaire */
};



typedef struct ennemies ENNEMIES; // Liste chainée
struct ennemies
{
    int nbMaxEnnemies ;
    int nbRestantsEnnemies ; // indique le nombre restants d'ennemis sur la map
    TANK * premier ;
    TANK * dernier ;
};

SDL_Rect positionTiti ;


TANK * player_one;
SDL_Surface *titi = NULL  ;

int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};


TANK * init_player(TANK * player)
{

    player = malloc(sizeof(TANK));
    player->actif = true ;
    player->obus.actif = false;

    player->direction = 0 ;
    player->hp = 3 ;
    return player;






}

ENNEMIES *initialisation_ennemies()
{

    ENNEMIES *ennemies = malloc(sizeof(ENNEMIES));
    ennemies->premier = NULL;
    ennemies->dernier = NULL;

    ennemies->nbMaxEnnemies = 0;
    ennemies->nbRestantsEnnemies = 12;



    return ennemies;

}







 void tirer(TANK * player)
{
        player->obus.actif = true;
        player->obus.Type = PLAYER_UN ;
        player->obus.obusActuel = IMG_Load("Images/oeuf.png");


        player->obus.direction = player->direction ;
        player->obus.positionObus.x = player->positionJoueur.x ;
        player->obus.positionObus.y = player->positionJoueur.y ;




}

void tirerEnnemi(TANK * player)
{
        player->obus.actif = true ;
        player->obus.Type = ENNEMI ;
        player->obus.obusActuel = IMG_Load("Images/dog_food.png");


        player->obus.direction = player->direction ;
        player->obus.positionObus.x = player->positionJoueur.x ;
        player->obus.positionObus.y = player->positionJoueur.y ;




}

bool collision(SDL_Rect A, SDL_Rect B, SDL_Surface * AS, SDL_Surface * BS)
{
    int AW, AH, BW, BH;



    AW = TAILLE_BLOC;
    AH = TAILLE_BLOC;
    BW = TAILLE_BLOC;
    BH = TAILLE_BLOC;

    if((A.x >= B.x + BW)
       || (A.x + AW <= B.x)
       || (A.y >= B.y + BH)
       || (A.y + AH <= B.y))
            return false;
        else
            return true;
}


void impact(ENNEMIES * liste_ennemies, TANK * player)
{

    TANK *actuel = liste_ennemies->premier;

    while (actuel != NULL)
    {

                //Collision entre Titi et les  obus des TANKS Ennemies

                if(collision(positionTiti, actuel->obus.positionObus,titi, actuel->obus.obusActuel))
                {

                    titiMort = true ;


                }

                //Collision entre Titi et les TANKS Ennemies

                if(collision(positionTiti, actuel->positionJoueur,titi, actuel->tankActuel))
                {

                    titiMort = true ;


                }
                // Collision entre les ennemis et l'obus du joueur



                if(collision(player->obus.positionObus, actuel->positionJoueur,player->obus.obusActuel, actuel->tankActuel))
                {

                    player->obus.actif=false;
                    actuel->actif=false;
                    player->obus.positionObus = player->positionJoueur ;

                    liste_ennemies->nbRestantsEnnemies = liste_ennemies->nbRestantsEnnemies - 1  ;
                    liste_ennemies->nbMaxEnnemies = liste_ennemies->nbMaxEnnemies  - 1  ;


                }
                // Collision entre les obus ennemis et notre joueur

                if(collision(actuel->obus.positionObus, player->positionJoueur,actuel->obus.obusActuel, player->tankActuel))
                {



                    actuel->obus.actif=false;
                    printf("hp : %d\n", player->hp);
                    if(timer == 180%4){
                        player->hp--;
                    }

                    printf("hp apres : %d\n", player->hp);
                    if(player->hp <= 0)
                    {
                         player->actif=false; // joueur devient inactif
                         printf("hp QUAND HP ZERO : %d\n", player->hp);
                    }

                    actuel->obus.positionObus = actuel->positionJoueur ;
                }


        actuel = actuel->suivant;
    }
}

void deplacementObus(TANK * player)
{






    switch(player->obus.direction)
        {

        case HAUT:
            if(player->obus.positionObus.y < 0 )
            {
                player->obus.actif = false;
                //player->obus.reset = false ;
                break;
            }
            if(carte[((player->obus.positionObus.x )/TAILLE_BLOC)][((player->obus.positionObus.y )/TAILLE_BLOC)] == MUR
               || carte[((player->obus.positionObus.x + TAILLE_BLOC )/TAILLE_BLOC)][((player->obus.positionObus.y )/TAILLE_BLOC)] == MUR
               || carte[((player->obus.positionObus.x )/TAILLE_BLOC) ][((player->obus.positionObus.y)/TAILLE_BLOC)  ] == MURBLINDE
               || carte[((player->obus.positionObus.x + TAILLE_BLOC )/TAILLE_BLOC)][((player->obus.positionObus.y )/TAILLE_BLOC)] == MURBLINDE )
            {
                switch(carte[((player->obus.positionObus.x )/TAILLE_BLOC)][((player->obus.positionObus.y )/TAILLE_BLOC)])
                {


                    case 1 :
                        carte[((player->obus.positionObus.x )/TAILLE_BLOC) ][((player->obus.positionObus.y )/TAILLE_BLOC)] = 0 ;
                        ;
                        break;
                    case 2 :

                            //carte[((player->obus.positionObus.x )/TAILLE_BLOC) ][((player->obus.positionObus.y )/TAILLE_BLOC)] = 1 ;




                        break;
                    default :

                        break;
                }
                switch(carte[((player->obus.positionObus.x + TAILLE_BLOC  )/TAILLE_BLOC) ][((player->obus.positionObus.y  )/TAILLE_BLOC) ])
                {


                    case 1 :
                        carte[((player->obus.positionObus.x + TAILLE_BLOC )/TAILLE_BLOC) ][((player->obus.positionObus.y )/TAILLE_BLOC) ] = 0 ;
                        //reset = false ;
                        break;
                    case 2 :
                        //carte[((player_un->obus.positionObus.x + TAILLE_BLOC )/TAILLE_BLOC) ][((player_un->obus.positionObus.y )/TAILLE_BLOC) ] =  1 ;
                        //reset = false ;
                        break;
                    default :

                        break;
                }
                //player->obus.reset = false ;
                player->obus.actif = false ;
                break;
            }


            if( player->obus.actif == true )
                player->obus.positionObus.y = player->obus.positionObus.y - VITESSE_OBUS;
            break;


        case BAS:
            if(player->obus.positionObus.y > HAUTEUR_FENETRE)
            {
                player->obus.actif = false;
                //player->obus.reset = false ;
                break;
            }
            if(carte[((player->obus.positionObus.x )/TAILLE_BLOC)][((player->obus.positionObus.y )/TAILLE_BLOC)] == MUR
                || carte[((player->obus.positionObus.x + TAILLE_BLOC )/TAILLE_BLOC)][((player->obus.positionObus.y )/TAILLE_BLOC) + 1] == MUR
                || carte[((player->obus.positionObus.x )/TAILLE_BLOC) ][((player->obus.positionObus.y )/TAILLE_BLOC)  ] == MURBLINDE
                || carte[((player->obus.positionObus.x + TAILLE_BLOC )/TAILLE_BLOC)][((player->obus.positionObus.y )/TAILLE_BLOC) + 1] == MURBLINDE )
            {
                 switch(carte[((player->obus.positionObus.x )/TAILLE_BLOC)][((player->obus.positionObus.y )/TAILLE_BLOC)])
                {


                    case 1 :
                        carte[((player->obus.positionObus.x )/TAILLE_BLOC) ][((player->obus.positionObus.y )/TAILLE_BLOC)] = 0 ;
                        //reset = false ;
                        break;
                    case 2 :
                        //carte[((player_un->obus.positionObus.x )/TAILLE_BLOC) ][((player_un->obus.positionObus.y )/TAILLE_BLOC)] = 1 ;
                        //reset = false ;
                        break;
                    default :

                        break;
                }
                switch(carte[((player->obus.positionObus.x + TAILLE_BLOC  )/TAILLE_BLOC) ][((player->obus.positionObus.y  )/TAILLE_BLOC) + 1])
                {


                    case 1 :
                        carte[((player->obus.positionObus.x + TAILLE_BLOC )/TAILLE_BLOC) ][((player->obus.positionObus.y )/TAILLE_BLOC) + 1] = 0 ;
                        //reset = false ;
                        break;
                    case 2 :
                        //carte[((player_un->obus.positionObus.x + TAILLE_BLOC )/TAILLE_BLOC) ][((player_un->obus.positionObus.y )/TAILLE_BLOC) + 1] =  1 ;
                        //reset = false ;
                        break;
                    default :

                        break;
                }
                //player->obus.reset = false ;
                player->obus.actif = false ;
                break;
            }


            if( player->obus.actif == true )
                player->obus.positionObus.y = player->obus.positionObus.y + VITESSE_OBUS ;
            break;


        case GAUCHE:
            if(player->obus.positionObus.x < 0){
                player->obus.actif = false;
                //player->obus.reset = false ;
                break;
            }
            if(carte[((player->obus.positionObus.x  )/TAILLE_BLOC)][((player->obus.positionObus.y )/TAILLE_BLOC)] == MUR
                || carte[((player->obus.positionObus.x  )/TAILLE_BLOC)][((player->obus.positionObus.y + TAILLE_BLOC )/TAILLE_BLOC)] == MUR
                || carte[((player->obus.positionObus.x  )/TAILLE_BLOC) ][((player->obus.positionObus.y )/TAILLE_BLOC)  ] == MURBLINDE
                || carte[((player->obus.positionObus.x  )/TAILLE_BLOC)][((player->obus.positionObus.y + TAILLE_BLOC )/TAILLE_BLOC)] == MURBLINDE)
            {
                 switch(carte[((player->obus.positionObus.x )/TAILLE_BLOC)][((player->obus.positionObus.y )/TAILLE_BLOC)])
                {


                    case 1 :
                        carte[((player->obus.positionObus.x )/TAILLE_BLOC) ][((player->obus.positionObus.y )/TAILLE_BLOC)] = 0 ;
                        //reset = false ;
                        break;
                    case 2 :
                        //carte[((player->obus.positionObus.x )/TAILLE_BLOC) ][((player->obus.positionObus.y )/TAILLE_BLOC)] = 1 ;
                        //reset = false ;
                        break;
                    default :

                        break;
                }
                switch(carte[((player->obus.positionObus.x  )/TAILLE_BLOC) ][((player->obus.positionObus.y + TAILLE_BLOC )/TAILLE_BLOC)])
                {


                    case 1 :
                        carte[((player->obus.positionObus.x  )/TAILLE_BLOC) ][((player->obus.positionObus.y + TAILLE_BLOC )/TAILLE_BLOC)] = 0 ;
                        //reset = false ;
                        break;
                    case 2 :
                        //carte[((player->obus.positionObus.x  )/TAILLE_BLOC) ][((player->obus.positionObus.y + TAILLE_BLOC )/TAILLE_BLOC)] = 1 ;
                        //reset = false ;
                        break;

                }

                //player->obus.reset = false ;
                player->obus.actif = false ;
                break;
            }


            if( player->obus.actif == true )
                player->obus.positionObus.x = player->obus.positionObus.x - VITESSE_OBUS;
            break;


        case DROITE:

            if(player->obus.positionObus.x > LARGEUR_FENETRE - TAILLE_BLOC){
                player->obus.actif = false;
                //player->obus.reset = false ;
                break;
            }
            if(carte[((player->obus.positionObus.x )/TAILLE_BLOC) + 1][((player->obus.positionObus.y )/TAILLE_BLOC)] == MUR
                || carte[((player->obus.positionObus.x  )/TAILLE_BLOC) + 1][((player->obus.positionObus.y + TAILLE_BLOC )/TAILLE_BLOC)] == MUR
                || carte[((player->obus.positionObus.x  )/TAILLE_BLOC) + 1 ][((player->obus.positionObus.y )/TAILLE_BLOC)  ] == MURBLINDE
                || carte[((player->obus.positionObus.x  )/TAILLE_BLOC) + 1][((player->obus.positionObus.y + TAILLE_BLOC )/TAILLE_BLOC)] == MURBLINDE)
            {



                switch(carte[((player->obus.positionObus.x )/TAILLE_BLOC) + 1][((player->obus.positionObus.y )/TAILLE_BLOC)])
                {


                    case 1 :
                        carte[((player->obus.positionObus.x )/TAILLE_BLOC) + 1][((player->obus.positionObus.y )/TAILLE_BLOC)] = 0 ;
                        //reset = false ;
                        break;
                    case 2 :
                        //carte[((player->obus.positionObus.x )/TAILLE_BLOC) + 1][((player->obus.positionObus.y )/TAILLE_BLOC)] = 1 ;
                        //reset = false ;
                        break;
                    default :

                        break;
                }
                switch(carte[((player->obus.positionObus.x  )/TAILLE_BLOC) + 1][((player->obus.positionObus.y + TAILLE_BLOC )/TAILLE_BLOC)])
                {


                    case 1 :
                        carte[((player->obus.positionObus.x  )/TAILLE_BLOC) + 1][((player->obus.positionObus.y + TAILLE_BLOC )/TAILLE_BLOC)] = 0 ;
                        //reset = false ;
                        break;
                    case 2 :
                        //carte[((player->obus.positionObus.x  )/TAILLE_BLOC) + 1][((player->obus.positionObus.y + TAILLE_BLOC )/TAILLE_BLOC)] = 1 ;
                        //reset = false ;
                        break;
                    default :

                        break;
                }

                //player->obus.reset = false ;
                player->obus.actif = false ;
                break;
            }

            if(player->obus.actif == true )
            {
                 player->obus.positionObus.x = player->obus.positionObus.x + VITESSE_OBUS ;

            }



            break;
    }

}

void spawn(ENNEMIES * liste_ennemies,int direction, int x , int y , int hp , int Etat )
{
    TANK * ennemi = malloc(sizeof(TANK));


    //ennemi->Blindage = Blindage; /*niveau de blindage en cours du tank (0 => rien, 1 => blindé, 2 => ultra-blindé)*/
    //ennemi->Blindage_orig = Blindage_orig; /*Blindage d’origine*/

    ennemi->direction=DROITE ;

    ennemi->actif = 1 ;
    //ennemi->positionJoueur = positionJoueur;
    ennemi->positionJoueur.x = x ;
    ennemi->positionJoueur.y = y ;
    //ennemi->tankActuel = tankActuel ;
    ennemi->hp = hp; /*Nombre de fois que le tank est touché*/


    ennemi->Etat = Etat; /*État du tank 1 => actif, 2 => en destruction, 3 => inactif*/
    ennemi->image_ennemi[BAS] = IMG_Load("Images/tank_ennemi_leger_bas.png");

    ennemi->image_ennemi[GAUCHE] = IMG_Load("Images/tank_ennemi_leger_gauche.png");

    ennemi->image_ennemi[HAUT] = IMG_Load("Images/tank_ennemi_leger_haut.png");
    ennemi->image_ennemi[DROITE] = IMG_Load("Images/tank_ennemi_leger_droite.png");


    ennemi->tankActuel = ennemi->image_ennemi[DROITE];
    liste_ennemies->nbMaxEnnemies = liste_ennemies->nbMaxEnnemies + 1;

    ennemi->suivant = liste_ennemies->premier ;
    liste_ennemies->premier = ennemi;




}

void efface(ENNEMIES * liste_ennemies)
{
    TANK * actuel = liste_ennemies->premier->suivant;
    TANK * precedent = liste_ennemies->premier;

    if(precedent->actif==false)
    {

        liste_ennemies->premier = actuel;


        free(precedent);
    }
    else{
        while(actuel != NULL)
        {
            if(actuel->actif==false)
            {


                TANK *aSupprimer = actuel;
                precedent->suivant = actuel->suivant;

                actuel = actuel->suivant;

                free(aSupprimer);


            }
            else{
                precedent=actuel;
                actuel = actuel->suivant;
            }

        }
    }
}

void afficher(ENNEMIES * liste , SDL_Surface * ecran)
{
    /*
    if(liste == NULL)
    {
        exit(EXIT_FAILURE);

    }
    */
    TANK * actuel = liste->premier ;

    //printf("8\n");


    while (actuel != NULL)

    {
        //printf("9\n");
        SDL_BlitSurface(actuel->tankActuel, NULL, ecran, &actuel->positionJoueur);
        //printf("10\n");
        actuel = actuel->suivant;

    }
    //printf("11\n");
}
/*
void clip(SDL_Rect *pos, SDL_Rect *oldpos, SDL_Surface *terrain , SDL_Surface *sprite)
{
    SDL_Rect pos_Terrain_T ;

    clipper.h = sprite->h;
    clipper.w = sprite->

}
*/

void deplacementEnnemis(ENNEMIES * liste)
{
    TANK * actuel = liste->premier ;

    int i ;


            while(actuel!=NULL)
            {


                int r = rand() % 4; //entre 0 et 3
                if(timer == 180 % 4)
                {




                switch(r)
                {



                case BAS :
                    for( i = 0 ; i < DEP_ENNEMI ; i++)
                    {

                    actuel->direction = BAS ;
                    deplacerJoueur(carte, &(actuel->positionJoueur), BAS);

                    }

                    actuel->tankActuel = actuel->image_ennemi[BAS];

                    break;
                case HAUT :
                    for( i = 0 ; i < DEP_ENNEMI ; i++)
                    {
                    actuel->direction = HAUT ;
                    deplacerJoueur(carte, &(actuel->positionJoueur), HAUT);

                    }
                    actuel->tankActuel = actuel->image_ennemi[HAUT];
                    break;
                case GAUCHE :
                    for( i = 0 ; i < DEP_ENNEMI ; i++)
                    {
                    actuel->direction = GAUCHE ;
                    deplacerJoueur(carte, &(actuel->positionJoueur), GAUCHE);

                    }

                    actuel->tankActuel = actuel->image_ennemi[GAUCHE];
                    break;
                case DROITE :
                    for( i = 0 ; i < DEP_ENNEMI ; i++)
                    {
                    actuel->direction = DROITE ;
                    deplacerJoueur(carte, &(actuel->positionJoueur), DROITE);


                    }

                    actuel->tankActuel = actuel->image_ennemi[DROITE];

                    break;


                }

                }

                actuel = actuel->suivant;




            }
}

void tirer_liste(ENNEMIES  *liste)
{
    /*
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }
    */
    TANK *actuel = liste->premier;

    while(actuel != NULL )
    {
        tirerEnnemi(actuel);

        actuel=actuel->suivant;
    }
}

void deplacementObus_liste(ENNEMIES  *liste)
{


    TANK *actuel = liste->premier;

    while(actuel != NULL)
    {

        deplacementObus(actuel);

        actuel=actuel->suivant;
    }
}

void play(SDL_Surface* ecran)
{








    SDL_Surface *tank[4] ; // 4 surfaces pour chacune des directions du tank
    SDL_Surface *mur = NULL, *murblinde = NULL ,    * win = NULL , * loose = NULL ;
    //SDL_Rect position, positionJoueur;

    SDL_Rect position ;

    SDL_Event event;

    player_one = init_player(player_one);


    int cpt = 0 ;



    int continuer = 1 ;
    int i = 0 , j = 0 ;

    ENNEMIES * liste_ennemies = initialisation_ennemies();


    //Uint32 delay = (330 / 100) * 10;
    //SDL_TimerID timer; /* Variable pour stocker le numéro du timer */


    // Chargement des sprites (décors, personnage...)
    mur = IMG_Load("Images/mur.png");
    murblinde= IMG_Load("Images/murblinde.png");
    titi = IMG_Load("Images/titi.png");
    win = IMG_Load("Images/win.png") ;
    loose =  IMG_Load("Images/loose.png") ;
    tank[BAS] = IMG_Load("Images/poule_faible_bas.png");
    tank[GAUCHE] = IMG_Load("Images/poule_faible_gauche.png");
    tank[HAUT] = IMG_Load("Images/poule_faible_haut.png");
    tank[DROITE] = IMG_Load("Images/poule_faible_droite.png");




    player_one->tankActuel = tank[HAUT]; // Mario sera dirigé vers le bas au départ




    //spawn(liste_ennemies,HAUT,-11 * TAILLE_BLOC ,-10* TAILLE_BLOC  ,10 , ENNEMI) ;
    //liste_ennemies->nbMaxEnnemies  = liste_ennemies->nbMaxEnnemies - 1 ;
    //spawn(liste_ennemies,HAUT,10 * TAILLE_BLOC ,10 * TAILLE_BLOC  ,15 , ENNEMI) ;
    //spawn(liste_ennemies,HAUT,15 * TAILLE_BLOC ,0 * TAILLE_BLOC  ,20 , ENNEMI) ;
    //spawn(liste_ennemies,HAUT,4 * TAILLE_BLOC ,4 * TAILLE_BLOC  ,30 , ENNEMI) ;
    //spawn(liste_ennemies,HAUT,4 * TAILLE_BLOC ,4 * TAILLE_BLOC  ,30 , ENNEMI) ;
    //spawn(liste_ennemies,HAUT,4 * TAILLE_BLOC ,4 * TAILLE_BLOC  ,30 , ENNEMI) ;


    //printf("1");
    // Chargement du niveau
    if (!chargerNiveau(carte))
        exit(EXIT_FAILURE); // On arrête le jeu si on n'a pas pu charger le niveau

    // Recherche de la position de Mario au départ

    player_one->positionJoueur.x = 6 * TAILLE_BLOC;
    player_one->positionJoueur.y =4  * TAILLE_BLOC;


    positionTiti.x = 6 * TAILLE_BLOC ;
    positionTiti.y = 2 * TAILLE_BLOC ;


    //printf("2\n");


    while (continuer)
    {



        timer++;

        if(timer>=180)
        {
            timer=0;
        }




        SDL_PollEvent(&event); /* On utilise PollEvent et non WaitEvent pour ne pas bloquer le programme */

        while (liste_ennemies->nbMaxEnnemies<4 && !(liste_ennemies->nbRestantsEnnemies <=  0) )
        {
            printf("nbMax : %d et nbRestants %d\n", liste_ennemies->nbMaxEnnemies , liste_ennemies->nbRestantsEnnemies);
            int r = rand() % 4; //entre 0 et 3
            switch(r)
            {
            case 0 :

                spawn(liste_ennemies,HAUT,0 ,0 ,10 , ENNEMI) ;
                break;
            case 1 :
                spawn(liste_ennemies,BAS,LARGEUR_FENETRE - TAILLE_BLOC,HAUTEUR_FENETRE,10 , ENNEMI) ;
                break;
            case 2 :
                spawn(liste_ennemies,GAUCHE,0 ,HAUTEUR_FENETRE - TAILLE_BLOC ,10 , ENNEMI) ;
                break;
            case 3 :
                spawn(liste_ennemies,DROITE,LARGEUR_FENETRE - TAILLE_BLOC,0 ,10 , ENNEMI) ;
                break;
            default:
                break;
            }


        }

        switch(event.type)
        {

        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    continuer = 0;

                    break;
                case SDLK_UP:
                    player_one->direction = 0 ; //VERS LE HAUT egal 0
                    player_one->tankActuel = tank[HAUT];
                    deplacerJoueur(carte, &(player_one->positionJoueur), HAUT);

                    break;
                case SDLK_DOWN:
                    player_one->direction = 1 ; //VERS LE BAS egal 1
                    player_one->tankActuel =tank[BAS];
                    deplacerJoueur(carte, &(player_one->positionJoueur), BAS);
                    break;
                case SDLK_RIGHT:
                    player_one->direction = 3 ; //VERS LA DROITE egal 3
                    player_one->tankActuel = tank[DROITE];
                    deplacerJoueur(carte, &(player_one->positionJoueur), DROITE);
                    break;
                case SDLK_LEFT:
                    player_one->direction = 2 ; //VERS LA GAUCHE egal 2
                    player_one->tankActuel = tank[GAUCHE];
                    deplacerJoueur(carte, &(player_one->positionJoueur), GAUCHE);
                    break;
                case SDLK_SPACE:

                    if(player_one->obus.actif == false && player_one->actif == true)
                    {


                        tirer(player_one);
                    }





                    break;








            }

            break;
        }



            //srand(time(NULL));



            efface(liste_ennemies);

            deplacementEnnemis(liste_ennemies);
            if(player_one->actif == true){

            if(timer == 180 % 30){
                tirer_liste(liste_ennemies);

            }



            }

            deplacementObus_liste(liste_ennemies);

            deplacementObus(player_one);


            impact(liste_ennemies,player_one);
            impact(liste_ennemies,titi);


            // Effacement de l'écran
            SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,238, 209, 83));


            //SDL_BlitSurface(background, NULL, ecran, &positionBackground);


            // Placement des objets à l'écran
            //objectifsRestants = 0;
            //printf("4\n");
            for (i = 0 ; i < NB_BLOCS_LARGEUR  ; i++)
            {
                for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
                {
                    position.x = i * TAILLE_BLOC;
                    position.y = j * TAILLE_BLOC;

                    switch( carte[i][j])
                    {


                        case MUR:
                            SDL_BlitSurface(mur, NULL, ecran, &position);
                            break;

                        case MURBLINDE:
                            SDL_BlitSurface(murblinde, NULL, ecran, &position);
                            break;

                        //case TITI:
                            //SDL_BlitSurface(titi, NULL, ecran, &position);
                            //break;
                        case VIDE:
                            //SDL_BlitSurface(grass, NULL , ecran , &position) ;
                            break;
                        //case PROJECTILE:
                            //SDL_BlitSurface(player_un->obus.obusActuel, NULL, ecran, &position);
                            //break;

                    }
                }
            }

            position.x = positionTiti.x ;
            position.y = positionTiti.y ;
            SDL_BlitSurface(titi, NULL, ecran, &positionTiti);


        if(player_one->actif == true)
        {
            position.x = player_one->positionJoueur.x ;
            position.y = player_one->positionJoueur.y ;
            SDL_BlitSurface(player_one->tankActuel, NULL, ecran, &position);
        }
        else{
            // Ecran de game over
        }
        if(player_one->obus.actif == true)
        {
            position.x = player_one->obus.positionObus.x ;
            position.y = player_one->obus.positionObus.y ;
            SDL_BlitSurface(player_one->obus.obusActuel, NULL, ecran, &position);

        }





        TANK *actuel = liste_ennemies->premier;

        while (actuel != NULL)
        {

            if(actuel->obus.actif==true){
                position.x = actuel->obus.positionObus.x ;
                position.y = actuel->obus.positionObus.y ;
                SDL_BlitSurface(actuel->obus.obusActuel, NULL, ecran, &position);
            }

            actuel = actuel->suivant;
        }














        afficher(liste_ennemies,ecran);

         if(player_one->actif == false || titiMort)
        {
            position.x = 0 ;
            position.y = 0 ;
           SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,238, 209, 83));
           SDL_BlitSurface(loose, NULL, ecran, &position);

        }
        if(liste_ennemies->nbRestantsEnnemies <= 1)
        {
            position.x = 0 ;
            position.y = 0 ;
           SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,238, 209, 83));
           SDL_BlitSurface(win, NULL, ecran, &position);

        }

        SDL_Flip(ecran);


    }


    // Libération des surfaces chargées

    SDL_FreeSurface(mur);
    SDL_FreeSurface(murblinde);
    SDL_FreeSurface(titi);
    SDL_FreeSurface(player_one->tankActuel);
    SDL_FreeSurface(player_one->obus.obusActuel);
    SDL_FreeSurface(win);
    SDL_FreeSurface(loose);

    TANK *actuel = liste_ennemies->premier;
    while (actuel != NULL)
    {

        SDL_FreeSurface(actuel->tankActuel);
        SDL_FreeSurface(actuel->obus.obusActuel);

        actuel = actuel->suivant;
    }
    free(liste_ennemies) ;
    free(player_one) ;


}


// MODE DIFFICILE

void playHARD(SDL_Surface* ecran)
{








    SDL_Surface *tank[4] ; // 4 surfaces pour chacune des directions du tank
    SDL_Surface *mur = NULL, *murblinde = NULL ,    * win = NULL , * loose = NULL ;
    //SDL_Rect position, positionJoueur;

    SDL_Rect position ;

    SDL_Event event;

    player_one = init_player(player_one);


    int cpt = 0 ;



    int continuer = 1 ;
    int i = 0 , j = 0 ;

    ENNEMIES * liste_ennemies = initialisation_ennemies();


    //Uint32 delay = (330 / 100) * 10;
    //SDL_TimerID timer; /* Variable pour stocker le numéro du timer */


    // Chargement des sprites (décors, personnage...)
    mur = IMG_Load("Images/mur.png");
    murblinde= IMG_Load("Images/murblinde.png");
    titi = IMG_Load("Images/titi.png");
    win = IMG_Load("Images/win.png") ;
    loose =  IMG_Load("Images/loose.png") ;
    tank[BAS] = IMG_Load("Images/poule_faible_bas.png");
    tank[GAUCHE] = IMG_Load("Images/poule_faible_gauche.png");
    tank[HAUT] = IMG_Load("Images/poule_faible_haut.png");
    tank[DROITE] = IMG_Load("Images/poule_faible_droite.png");




    player_one->tankActuel = tank[HAUT]; // Mario sera dirigé vers le bas au départ




    //spawn(liste_ennemies,HAUT,-11 * TAILLE_BLOC ,-10* TAILLE_BLOC  ,10 , ENNEMI) ;
    //liste_ennemies->nbMaxEnnemies  = liste_ennemies->nbMaxEnnemies - 1 ;
    //spawn(liste_ennemies,HAUT,10 * TAILLE_BLOC ,10 * TAILLE_BLOC  ,15 , ENNEMI) ;
    //spawn(liste_ennemies,HAUT,15 * TAILLE_BLOC ,0 * TAILLE_BLOC  ,20 , ENNEMI) ;
    //spawn(liste_ennemies,HAUT,4 * TAILLE_BLOC ,4 * TAILLE_BLOC  ,30 , ENNEMI) ;
    //spawn(liste_ennemies,HAUT,4 * TAILLE_BLOC ,4 * TAILLE_BLOC  ,30 , ENNEMI) ;
    //spawn(liste_ennemies,HAUT,4 * TAILLE_BLOC ,4 * TAILLE_BLOC  ,30 , ENNEMI) ;


    //printf("1");
    // Chargement du niveau
    if (!chargerNiveauDifficile(carte))
        exit(EXIT_FAILURE); // On arrête le jeu si on n'a pas pu charger le niveau

    // Recherche de la position de Mario au départ

    player_one->positionJoueur.x = 6 * TAILLE_BLOC;
    player_one->positionJoueur.y =4  * TAILLE_BLOC;


    positionTiti.x = 6 * TAILLE_BLOC ;
    positionTiti.y = 2 * TAILLE_BLOC ;


    //printf("2\n");


    while (continuer)
    {



        timer++;

        if(timer>=60)
        {
            timer=0;
        }




        SDL_PollEvent(&event); /* On utilise PollEvent et non WaitEvent pour ne pas bloquer le programme */

        while (liste_ennemies->nbMaxEnnemies<8 && !(liste_ennemies->nbRestantsEnnemies <=  0) )
        {
            printf("nbMax : %d et nbRestants %d\n", liste_ennemies->nbMaxEnnemies , liste_ennemies->nbRestantsEnnemies);
            int r = rand() % 4; //entre 0 et 3
            switch(r)
            {
            case 0 :

                spawn(liste_ennemies,HAUT,0 ,0 ,10 , ENNEMI) ;
                break;
            case 1 :
                spawn(liste_ennemies,BAS,LARGEUR_FENETRE - TAILLE_BLOC,HAUTEUR_FENETRE,10 , ENNEMI) ;
                break;
            case 2 :
                spawn(liste_ennemies,GAUCHE,0 ,HAUTEUR_FENETRE - TAILLE_BLOC ,10 , ENNEMI) ;
                break;
            case 3 :
                spawn(liste_ennemies,DROITE,LARGEUR_FENETRE - TAILLE_BLOC,0 ,10 , ENNEMI) ;
                break;
            default:
                break;
            }


        }

        switch(event.type)
        {

        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    continuer = 0;

                    break;
                case SDLK_UP:
                    player_one->direction = 0 ; //VERS LE HAUT egal 0
                    player_one->tankActuel = tank[HAUT];
                    deplacerJoueur(carte, &(player_one->positionJoueur), HAUT);

                    break;
                case SDLK_DOWN:
                    player_one->direction = 1 ; //VERS LE BAS egal 1
                    player_one->tankActuel =tank[BAS];
                    deplacerJoueur(carte, &(player_one->positionJoueur), BAS);
                    break;
                case SDLK_RIGHT:
                    player_one->direction = 3 ; //VERS LA DROITE egal 3
                    player_one->tankActuel = tank[DROITE];
                    deplacerJoueur(carte, &(player_one->positionJoueur), DROITE);
                    break;
                case SDLK_LEFT:
                    player_one->direction = 2 ; //VERS LA GAUCHE egal 2
                    player_one->tankActuel = tank[GAUCHE];
                    deplacerJoueur(carte, &(player_one->positionJoueur), GAUCHE);
                    break;
                case SDLK_SPACE:

                    if(player_one->obus.actif == false && player_one->actif == true)
                    {


                        tirer(player_one);
                    }





                    break;








            }

            break;
        }



            //srand(time(NULL));



            efface(liste_ennemies);

            deplacementEnnemis(liste_ennemies);
            if(player_one->actif == true){

            if(timer == 100 % 30){
                deplacementEnnemis(liste_ennemies);
                tirer_liste(liste_ennemies);

            }



            }

            deplacementObus_liste(liste_ennemies);
            deplacementEnnemis(liste_ennemies);
            deplacementObus(player_one);


            impact(liste_ennemies,player_one);
            impact(liste_ennemies,titi);


            // Effacement de l'écran
            SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,53, 209, 47));


            //SDL_BlitSurface(background, NULL, ecran, &positionBackground);


            // Placement des objets à l'écran
            //objectifsRestants = 0;
            //printf("4\n");
            for (i = 0 ; i < NB_BLOCS_LARGEUR  ; i++)
            {
                for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
                {
                    position.x = i * TAILLE_BLOC;
                    position.y = j * TAILLE_BLOC;

                    switch( carte[i][j])
                    {


                        case MUR:
                            SDL_BlitSurface(mur, NULL, ecran, &position);
                            break;

                        case MURBLINDE:
                            SDL_BlitSurface(murblinde, NULL, ecran, &position);
                            break;

                        //case TITI:
                            //SDL_BlitSurface(titi, NULL, ecran, &position);
                            //break;
                        case VIDE:
                            //SDL_BlitSurface(grass, NULL , ecran , &position) ;
                            break;
                        //case PROJECTILE:
                            //SDL_BlitSurface(player_un->obus.obusActuel, NULL, ecran, &position);
                            //break;

                    }
                }
            }

            position.x = positionTiti.x ;
            position.y = positionTiti.y ;
            SDL_BlitSurface(titi, NULL, ecran, &positionTiti);


        if(player_one->actif == true)
        {
            position.x = player_one->positionJoueur.x ;
            position.y = player_one->positionJoueur.y ;
            SDL_BlitSurface(player_one->tankActuel, NULL, ecran, &position);
        }
        else{
            // Ecran de game over
        }
        if(player_one->obus.actif == true)
        {
            position.x = player_one->obus.positionObus.x ;
            position.y = player_one->obus.positionObus.y ;
            SDL_BlitSurface(player_one->obus.obusActuel, NULL, ecran, &position);

        }





        TANK *actuel = liste_ennemies->premier;

        while (actuel != NULL)
        {

            if(actuel->obus.actif==true){
                position.x = actuel->obus.positionObus.x ;
                position.y = actuel->obus.positionObus.y ;
                SDL_BlitSurface(actuel->obus.obusActuel, NULL, ecran, &position);
            }

            actuel = actuel->suivant;
        }














        afficher(liste_ennemies,ecran);

         if(player_one->actif == false || titiMort)
        {
            position.x = 0 ;
            position.y = 0 ;
           SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,238, 209, 83));
           SDL_BlitSurface(loose, NULL, ecran, &position);

        }
        if(liste_ennemies->nbRestantsEnnemies <= 1)
        {
            position.x = 0 ;
            position.y = 0 ;
           SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,238, 209, 83));
           SDL_BlitSurface(win, NULL, ecran, &position);

        }

        SDL_Flip(ecran);


    }


    // Libération des surfaces chargées

    SDL_FreeSurface(mur);
    SDL_FreeSurface(murblinde);
    SDL_FreeSurface(titi);
    SDL_FreeSurface(player_one->tankActuel);
    SDL_FreeSurface(player_one->obus.obusActuel);
    SDL_FreeSurface(win);
    SDL_FreeSurface(loose);

    TANK *actuel = liste_ennemies->premier;
    while (actuel != NULL)
    {

        SDL_FreeSurface(actuel->tankActuel);
        SDL_FreeSurface(actuel->obus.obusActuel);

        actuel = actuel->suivant;
    }
    free(liste_ennemies) ;
    free(player_one) ;



}

void deplacerJoueur(int carte[][NB_BLOCS_HAUTEUR], SDL_Rect *pos, int direction)
{
    switch(direction)
    {
        case HAUT:
            if (pos->y < 0) // Si le joueur dépasse l'écran, on arrête
                break;
            if (carte[((pos->x  )/TAILLE_BLOC)][((pos->y )/TAILLE_BLOC) ] == MUR
                || carte[((pos->x  + TAILLE_BLOC)/TAILLE_BLOC  ) ][( pos->y)/TAILLE_BLOC ] == MUR
                || carte[((pos->x   )/TAILLE_BLOC) ][((pos->y )/TAILLE_BLOC)  ] == MURBLINDE
                || carte[((pos->x  + TAILLE_BLOC)/TAILLE_BLOC  ) ][( pos->y)/TAILLE_BLOC ] == MURBLINDE )
                break;



            pos->y--; // On peut enfin faire monter le joueur (oufff !)
            break;


        case BAS:
            if (pos->y  >= HAUTEUR_FENETRE - TAILLE_BLOC)
                break;
            if (carte[((pos->x )/TAILLE_BLOC)][((pos->y  )/TAILLE_BLOC) + 1] == MUR
                || carte[((pos->x + TAILLE_BLOC)/TAILLE_BLOC  ) ][( pos->y)/TAILLE_BLOC + 1 ] == MUR
                || carte[((pos->x )/TAILLE_BLOC) ][((pos->y )/TAILLE_BLOC) + 1 ] == MURBLINDE
                || carte[((pos->x + TAILLE_BLOC)/TAILLE_BLOC  ) ][( pos->y)/TAILLE_BLOC + 1] == MURBLINDE )

                break;






            pos->y++;
            break;


        case GAUCHE:
            if (pos->x < 0)
                break;
           if (carte[((pos->x )/TAILLE_BLOC)][pos->y/TAILLE_BLOC] == MUR
               || carte[((pos->x )/TAILLE_BLOC  ) ][( pos->y + TAILLE_BLOC)/TAILLE_BLOC  ] == MUR
               || carte[((pos->x  )/TAILLE_BLOC)][pos->y/TAILLE_BLOC ] == MURBLINDE
               || carte[((pos->x )/TAILLE_BLOC  ) ][( pos->y +  TAILLE_BLOC)/TAILLE_BLOC ] == MURBLINDE )
                break;





            pos->x--;
            break;


        case DROITE:
            if (pos->x  >= LARGEUR_FENETRE - TAILLE_BLOC )
                break;
            if (carte[((pos->x )/TAILLE_BLOC) +1 ][pos->y/TAILLE_BLOC  ] == MUR
                || carte[((pos->x )/TAILLE_BLOC +1  ) ][( pos->y + TAILLE_BLOC)/TAILLE_BLOC  ] == MUR
                || carte[((pos->x )/TAILLE_BLOC) + 1][pos->y/TAILLE_BLOC ] == MURBLINDE
                || carte[((pos->x )/TAILLE_BLOC + 1  ) ][( pos->y + TAILLE_BLOC)/TAILLE_BLOC  ] == MURBLINDE)
                break;



            pos->x++;
            break;
    }




}
