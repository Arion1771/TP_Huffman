#include <stdio.h>

#include "arbrebin.h"
#include "assert.h"
#include "bfile.h"

void Decoder(FILE *fichier_encode, Arbre ArbreHuffman) {
        AfficherArbre(ArbreHuffman);
    /* A COMPLETER */
    printf("Programme non realise (Decoder)\n");
}

int main(int argc, char *argv[]) {
    FILE *fichier_encode = fopen(argv[1], "r");

    Arbre ArbreHuffman = LireArbre(fichier_encode);

    Decoder(fichier_encode, ArbreHuffman);

    fclose(fichier_encode);

    return 0;
}
