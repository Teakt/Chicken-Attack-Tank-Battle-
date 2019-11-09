#include <stdlib.h>
#include <stdio.h>

#include "librairies.h"
#include "constantes.h"
#include "jeu.h"






int main(int argc, char *argv[])

{
    freopen ("logs.txt","w",stdout);
    SDL_Surface *ecran = NULL, *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;


    int continuer = 1;

    Mix_Music *musique ;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_DOUBLEBUF); // TIMER pour pouvoir utiliser le tps


    SDL_WM_SetIcon(IMG_Load("Images/TankTest.png"), NULL); // L'ic�ne doit �tre charg�e avant SDL_SetVideoMode

    ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);






    SDL_WM_SetCaption("Tank_Battles", NULL);


    menu = IMG_Load("Images/AccueilTest.png");

    positionMenu.x = 0;

    positionMenu.y = 0;


    while (continuer)

    {

        SDL_WaitEvent(&event);

        switch(event.type)

        {

            case SDL_QUIT:

                continuer = 0;

                break;

            case SDL_KEYDOWN:

                switch(event.key.keysym.sym)

                {

                    case SDLK_ESCAPE: // Veut arr�ter le jeu

                        continuer = 0;

                        break;

                    case SDLK_KP1: // Demande � jouer


                        play(ecran);

                        break;

                    case SDLK_KP2:

                        playHARD(ecran);

                        break;

                }

                break;

        }


        // Effacement de l'�cran

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

        SDL_BlitSurface(menu, NULL, ecran, &positionMenu);

        SDL_Flip(ecran);

    }


    SDL_FreeSurface(menu);



    SDL_Quit();


    fclose (stdout); //pour les logs

    return EXIT_SUCCESS;

}
