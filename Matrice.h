#ifndef __MATRICE_H__
#define __MATRICE_H__
#include "arbresyntaxique.h"

#define INFINI 3
#define OMEGA 2
#define FALSE 0
#define TRUE 1

struct _Matrice;
typedef struct _Matrice *Matrice;

Matrice creer_matrice(int taille);
void detruire_matrice(Matrice m);

int get_taille(Matrice m);
tree get_mot(Matrice m);
int ** get_tableau(Matrice m);

void print_matrice_in_R(Matrice m);
int equal_matrice (Matrice m1, Matrice m2);

// Renvoie la matrice des couts associés à une lettre pour un automate
Matrice creer_matrice_transistions(Automate* a, char c);

/* Multiplcation de deux matrices dans l'ensemble R = {0, 1, ω, ∞}
 * avec a+b = min(a, b) et a*b = max(a, b)
 */
Matrice multiplication_in_MnR(Matrice m1,Matrice m2);

//Test si une matrice est idempotente, cad m = m.m
int est_idempotent(Matrice m);

//test si l'element de la matrice est stable(anchored); 
int stable(int i,int j,Matrice m);

//test si la matrice est dans R;
int estMatriceR(Matrice m);

// Calcule et renvoie la matrice # d'une matrice dans R
Matrice creer_matrice_diese(Matrice m);

#endif

