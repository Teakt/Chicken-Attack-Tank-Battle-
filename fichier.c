#include "librairies.h"
#include "constantes.h"
#include "fichier.h"

int chargerNiveau(int niveau[][NB_BLOCS_HAUTEUR])
{
    FILE* fichier = NULL;
    char ligneFichier[NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR ] = {0};
    int i = 0, j = 0;

    fichier = fopen("mapfacile2.lvl", "r");
    if (fichier == NULL)
        return 0;

    fgets(ligneFichier, NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR , fichier);

    for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
    {
        for (j = 0 ; j < NB_BLOCS_HAUTEUR  ; j++)
        {
            switch (ligneFichier[(j * NB_BLOCS_LARGEUR) + i])
            {
                case '0':
                    niveau[i][j] = VIDE;
                    break;
                case '1':
                    niveau[i][j] = MUR;
                    break;
                case '2':
                    niveau[i][j] = MURBLINDE;
                    break;
                case '3':
                    niveau[i][j] = 3;
                    break;
                case '4':
                    niveau[i][j] = 4;
                    break;
                case '5':
                    niveau[i][j] = 5;

                    break;

            }
        }
    }

    fclose(fichier);
    return 1;
}

int chargerNiveauDifficile(int niveau[][NB_BLOCS_HAUTEUR])
{
    FILE* fichier = NULL;
    char ligneFichier[NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR ] = {0};
    int i = 0, j = 0;

    fichier = fopen("MapDifficile.lvl", "r");
    if (fichier == NULL)
        return 0;

    fgets(ligneFichier, NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR , fichier);

    for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
    {
        for (j = 0 ; j < NB_BLOCS_HAUTEUR  ; j++)
        {
            switch (ligneFichier[(j * NB_BLOCS_LARGEUR) + i])
            {
                case '0':
                    niveau[i][j] = VIDE;
                    break;
                case '1':
                    niveau[i][j] = MUR;
                    break;
                case '2':
                    niveau[i][j] = MURBLINDE;
                    break;
                case '3':
                    niveau[i][j] = 3;
                    break;
                case '4':
                    niveau[i][j] = 4;
                    break;
                case '5':
                    niveau[i][j] = 5;

                    break;

            }
        }
    }

    fclose(fichier);
    return 1;
}
