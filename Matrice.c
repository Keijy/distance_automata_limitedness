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
#include <string.h>

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
	    m->tab[get_origine_cle(cle)-1][f-1] = get_cout_cle(cle);
      }
    }
  }
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
  printf("\n");
}

int min(int n1,int n2){
  if(n1<n2)
    return n1;
  return n2;
}
int max(int n1,int n2){
  if(n1>n2)
    return n1;
  return n2;
}
int equal(Matrice m1,Matrice m2){
  return m1->taille==m2->taille;
}
Matrice multiplication(Matrice m1,Matrice m2){
  if(!equal(m1,m2)){
    printf("sont pas de meme taille");
    return NULL;
  }
  char* c=strcat(m1->mot,m2->mot);  
  Matrice m3=creer_matrice(c,m1->taille);
  int i,j,k;
  for(i=0;i<m1->taille;i++)
    for(j=0;j<m1->taille;j++){
      m3->tab[i][j]=-1;
    }  
  for(i=0;i<m1->taille;i++)
    for(j=0;j<m1->taille;j++)
      for(k=0;k<m1->taille;k++){
	if(m3->tab[i][j] == -1)
	  m3->tab[i][j]= m1->tab[i][k]+m2->tab[k][j];
	else
	  m3->tab[i][j]=min(m1->tab[i][k]+m2->tab[k][j], m3->tab[i][j]);
      }
  return m3;
}


int anchored(int i,int j,Matrice m){
  int k;
  for(k=1;k<m->taille;k++){
    if(m->tab[k][k]==0 && m->tab[k][k] <= m->tab[i][j] && m->tab[i][j] == max(m->tab[i][k],m->tab[k][j]))
      return 1;
  }
  return 0;
}
  
int stable(int i,int j,Matrice m){
  return m->tab[i][j]==1 && anchored(i,j,m)==0;
}
    


