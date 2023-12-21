#include "bfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMITEUR '@'
#define READ 0
#define WRITE 1

BFILE *bstart(FILE *fichier, const char *mode) {
    BFILE *resultat;

    resultat = (BFILE *)malloc(sizeof(BFILE));
    if (resultat == NULL)
        return NULL;
    resultat->fichier = fichier;
    if (strcmp(mode, "r") == 0) {
        resultat->mode = READ;
        resultat->last = 0;
        resultat->nombre = 0;
    } else if (strcmp(mode, "w") == 0) {
        resultat->mode = WRITE;
        resultat->nombre = 0;
        resultat->donnees = 0;
    } else {
        free(resultat);
        return NULL;
    }
    return resultat;
}

int bstop(BFILE *fichier) {
    int resultat = 0;

    if (fichier->mode == WRITE) {
        /* protocole de fin (directive de taille restante puis donnees) */
        fputc(DELIMITEUR, fichier->fichier);
        fputc(fichier->nombre + '0', fichier->fichier);
        if (fichier->nombre)
            resultat = fputc(fichier->donnees, fichier->fichier);
    }
    free(fichier);

    return resultat;
}

int bitread(BFILE *fichier) {
    int resultat;

    /* On remplit le buffer si necessaire */
    if ((fichier->nombre == 0) && !beof(fichier)) {
        fichier->last = 1;
        resultat = fgetc(fichier->fichier);
        if (resultat == EOF)
            return -1;
        /* Decodage du protocole de trames */
        if (resultat == DELIMITEUR) {
            resultat = fgetc(fichier->fichier);
            if (resultat == EOF)
                /* Bug, ceci ne devrait pas arriver */
                return -1;
            if (resultat == DELIMITEUR) {
                fichier->nombre = 8;
            } else {
                resultat -= '0';
                if ((resultat < 1) || (resultat > 8))
                    /* Bug si resultat > 8 */
                    return -1;
                int nombre;
                nombre = resultat;
                resultat = fgetc(fichier->fichier);
                if (resultat == EOF)
                    /* Bug, ne devrait pas arriver */
                    return -1;
                fichier->nombre = nombre;
            }
        } else {
            fichier->nombre = 8;
        }
        fichier->donnees = resultat;
        if (fichier->nombre > 0)
            fichier->last = 0;
    }
    /* Si on arrive jusqu'ici, c'est qu'il y a encore des donnees */
    fichier->nombre--;
    resultat = (fichier->donnees >> fichier->nombre) & 1;
    return resultat;
}

int bitwrite(BFILE *fichier, int bit) {
    int resultat;

    fichier->donnees <<= 1;
    fichier->donnees |= bit;
    fichier->nombre++;
    if (fichier->nombre == 8) {
        /* gestion du protocole des directives */
        if (fichier->donnees == DELIMITEUR) {
            resultat = fputc(DELIMITEUR, fichier->fichier);
            if (resultat == EOF)
                /* Erreur d'ecriture */
                return -1;
        }
        resultat = fputc(fichier->donnees, fichier->fichier);
        if (resultat == EOF)
            /* Erreur d'ecriture */
            return -1;
        fichier->nombre = 0;
        fichier->donnees = 0;
    }
    return 0;
}

int beof(BFILE *fichier) { return fichier->last; }
