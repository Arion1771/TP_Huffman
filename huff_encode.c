
#include "arbrebin.h"
#include "bfile.h"
#include "fap.h"
#include "huffman_code.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
    int tab[256];
} TableOcc_t;

struct code_char HuffmanCode[256];

void ConstruireTableOcc(FILE *fichier, TableOcc_t *TableOcc) {

    int c;
    int j=0;
    while (j<256) {
        TableOcc->tab[j]=0;
        j++;
    }

    c = fgetc(fichier);
    while (c != EOF) {
        TableOcc->tab[c]++;
        c = fgetc(fichier);
    };


    int i;
    for (i = 0; i < 256; i++) {
        if (TableOcc->tab[i] != 0)
            printf("Occurences du caractere %c (code %d) : %d\n", i, i,
                   TableOcc->tab[i]);
    }
}

fap InitHuffman(TableOcc_t *TableOcc) {
    fap file=creer_fap_vide();
    int i = 0;
    while (i<256) {
        if (TableOcc->tab[i] != 0) {
            Arbre a = ArbreVide();
            a = NouveauNoeud(ArbreVide(),i,ArbreVide());
            file=inserer(file,a,TableOcc->tab[i]);
        }
        i++;
    }   
    return file;
}

Arbre ConstruireArbre(fap file) {
    if (est_fap_vide(file)) {
        return NULL;
    }
    while (1) {
        Arbre a = ArbreVide();
        Arbre b = ArbreVide();
        Arbre c = ArbreVide();
        int i;
        int j;
        file=extraire(file,&a,&i);
        if (est_fap_vide(file)) {
            return a;
        }
        file=extraire(file,&b,&j);
        c=NouveauNoeud(a,65,b);
        file=inserer(file,c,i+j);
    }
}


void ConstruireCode(Arbre huff) {
    
}

void Encoder(FILE *fic_in, FILE *fic_out, Arbre ArbreHuffman) {
    /* A COMPLETER */
    printf("Programme non realise (Encoder)\n");
}

int main(int argc, char *argv[]) {

    TableOcc_t TableOcc;
    FILE *fichier;
    FILE *fichier_encode;

    fichier = fopen(argv[1], "r");
    /* Construire la table d'occurences */
    ConstruireTableOcc(fichier, &TableOcc);
    fclose(fichier);

    /* Initialiser la FAP */
    fap file = InitHuffman(&TableOcc);

    /* Construire l'arbre d'Huffman */
    Arbre ArbreHuffman = ConstruireArbre(file);

        AfficherArbre(ArbreHuffman);

    /* Construire la table de codage */
    ConstruireCode(ArbreHuffman);

    /* Encodage */
    fichier = fopen(argv[1], "r");
    fichier_encode = fopen(argv[2], "w");

    Encoder(fichier, fichier_encode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier);

    return 0;
}
