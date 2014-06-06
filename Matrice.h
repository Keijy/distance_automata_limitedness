#ifndef __MATRICE_H__
#define __MATRICE_H__

struct _Matrice;
typedef struct _Matrice *Matrice;

Matrice creer_matrice(char* mot, int taille);
void detruire_matrice(Matrice m);
Matrice creer_matrice_transistions(Automate* a, char l);
void print_matrice(Matrice m);
Matrice multiplication(Matrice m1,Matrice m2);
int stable(int i,int j,Matrice m);
int stableMatrice(Matrice m);
int estMatriceR(Matrice m);
Matrice matriceNaR(Matrice m);
void printStableMatrice(Matrice m);
Matrice eDieze(Matrice m);

#endif

