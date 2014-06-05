#include "automate.h"
#include "table.h"
#include "ensemble.h"
#include "outils.h"
#include "Matrice.h"

#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> 

#include <math.h>


struct _Matrice {
  int** tab;
  char* mot;
  int taille;
};

Matrice creer_matrice(char* mot, int taille){
  Matrice m = malloc(sizeof(*m));
  m->tab = malloc(taille * sizeof(int*));
  m->mot = mot;
  m->taille = taille;

  int i;
  for(i = 0; i<taille; i++)
    m->tab[i] = malloc(taille * sizeof(int));
  
  return m;
}

void detruire_matrice(Matrice m){
  if(m == NULL)
    return;
  
  int i;
  for(i = 0; i<m->taille; i++)
    free(m->tab+i);
  free(m->tab);
  free(m);  
}

Matrice creer_matrice_transistions(Automate* a, char l){
  a = creer_automate_etats_0_n(a);
  int n = taille_ensemble(get_etats(a));
  Matrice m = creer_matrice(&l, n);

  Table_iterateur it1;
  Ensemble_iterateur it2;
  for ( it1 = premier_iterateur_table(get_transitions(a));
	! iterateur_ensemble_est_vide(it1);
	it1 = iterateur_suivant_ensemble(it1)
	){
    Cle * cle = (Cle*) get_cle(it1);
    Ensemble * fins = (Ensemble*) get_valeur(it1);
    int tmp = get_lettre_cle(cle);
  
    if(tmp == (int)l){
      for( it2 = premier_iterateur_ensemble(fins);
	     ! iterateur_ensemble_est_vide(it2);
	     it2 = iterateur_suivant_ensemble(it2)
	     ){
	    int f = get_element(it2);
	    m->tab[get_origine_cle(cle)][f] = get_cout_cle(cle);
      }
    }
  }
  liberer_automate(a);
  return m;
}

void print_matrice(Matrice m){
  int i, j;
  for(i = 0; i<m->taille; i++){
    printf("|");
    for(j = 0; j<m->taille; j++){
      printf(" %d ", m->tab[i][j]);
    }
    printf("|\n");
  }
    printf("|\n");
}

/* Condition 1 : pour tout i,j,k  e(i,j) <= max(e(i,k), e(k,j))
 * Condition 2 : pour tout i,j, il existe k tq e(i,j) = max(e(i,k), e(k,j))
 * Condition 1 && Condition 2 <=> m est idempotent
 */

int est_idempotent(Matrice m){
  
}
