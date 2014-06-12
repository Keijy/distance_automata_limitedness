#ifndef __MATRICE_H__
#define __MATRICE_H__
#include "arbresyntaxique.h"

struct _Matrice;
typedef struct _Matrice *Matrice;


Matrice creer_matrice(int taille);
void detruire_matrice(Matrice m);
Matrice creer_matrice_transistions(Automate* a, char c);
void print_matrice_in_R(Matrice m);

Matrice multiplication_in_MnR(Matrice m1,Matrice m2);
int stable(int i,int j,Matrice m);
int est_stable_matrice(Matrice m);
int estMatriceR(Matrice m);
Matrice matriceNaR(Matrice m);
void printStableMatrice(Matrice m);

Matrice creer_matrice_dieze(Matrice m);
int est_idempotent(Matrice m);
int get_taille(Matrice m);
tree get_mot(Matrice m);
int equal_matrice(Matrice m1,Matrice m2);

#endif

