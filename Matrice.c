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
  tree mot;
  int taille;
};

/*************************************************************************/
/************************* Creation, Destruction *************************/
/*************************************************************************/

Matrice creer_matrice(int taille){
  Matrice m = malloc(sizeof(*m));
  m->tab = malloc(taille * sizeof(int*));
  m->taille = taille;

  int i;
  for(i = 0; i<taille; i++)
    m->tab[i] = malloc(taille * sizeof(int));
  
  return m;
}

void detruire_matrice(Matrice m){
  if(m == NULL)
    return;
  else{
    int i;
    for(i = 0; i<m->taille; i++){
      free(m->tab[i]);
    }
    free(m->tab);
    tree_destroy(m->mot);
    free(m);  
  }
}

/*************************************************************************/
/****************************** Accesseurs *******************************/
/*************************************************************************/

int get_taille(Matrice m){
  return m->taille;
}

tree get_mot(Matrice m){
  return m->mot;
}

int ** get_tableau(Matrice m){
  return m->tab;
}

/*************************************************************************/
/******************************* Affichage *******************************/
/*************************************************************************/


void print_matrice_in_R(Matrice m){
  printf("M(");
  print_tree(m->mot);
  printf(")");
  printf("\n\t");
  int i, j;
  for(i = 0; i<m->taille; i++){
    printf("|");
    for(j = 0; j<m->taille; j++){
      switch(m->tab[i][j])
	{
	case OMEGA : printf(" %s ", "ω"); break;
	case INFINI : printf(" %s ", "∞"); break;
	default: printf(" %d ", m->tab[i][j]); break;
	}
    }
    printf("|\n\t");
  }
  printf("\n");
}



/*************************************************************************/
/************************ Fonctions utilitaires **************************/
/*************************************************************************/

int equal_size (Matrice m1,Matrice m2){
  return m1->taille==m2->taille;
}

int equal_matrice (Matrice m1, Matrice m2){
  if (!equal_size(m1, m2))
      return FALSE;
  int i, j;
  for(i = 0; i < m1->taille; i++){
    for(j = 0; j < m1->taille; j++){
      if (m1->tab[i][j] != m2->tab[i][j])
	return FALSE;
    }
  }
  return TRUE;
}

int min (int n1,int n2){
  if(n1<n2)
    return n1;
  return n2;
}

int max (int n1,int n2){
  if(n1>n2)
    return n1;
  return n2;
}

/*************************************************************************/
/************************* Fonctions principales *************************/
/*************************************************************************/


Matrice creer_matrice_transistions(Automate* a, char c){
  Automate * abis = creer_automate_etats_0_n(a);
  int n = taille_ensemble(get_etats(abis));	
  Matrice m = creer_matrice(n);
  tree t=tree_creat();
  tree_set(t, c);
  m->mot = t;

  int i, j;
  for(i = 0; i < m->taille; i++){
    for(j = 0; j< m->taille; j++){
      m->tab[i][j] = INFINI;
    }
  } 

  Table_iterateur it1;
  Ensemble_iterateur it2;
  for ( it1 = premier_iterateur_table(get_transitions(abis));
	! iterateur_ensemble_est_vide(it1);
	it1 = iterateur_suivant_ensemble(it1)
	){
    Cle * cle = (Cle*) get_cle(it1);
    Ensemble * fins = (Ensemble*) get_valeur(it1);
    int tmp = get_lettre_cle(cle);
  
    if(tmp == (int)c){
      for( it2 = premier_iterateur_ensemble(fins);
	   ! iterateur_ensemble_est_vide(it2);
	   it2 = iterateur_suivant_ensemble(it2)
	   ){
	int f = get_element(it2);
	int cout = get_cout_cle(cle)==0 ? 0: 1;
	m->tab[get_origine_cle(cle)][f] = cout;
      }
    }
  }
  liberer_automate(abis);
  return m;
}

Matrice multiplication_in_MnR(Matrice m1,Matrice m2){
  if(!equal_size(m1,m2)){
    printf("sont pas de meme taille");
    return NULL;
  }
  
  Matrice m3=creer_matrice(m1->taille);
  m3->mot = tree_concatenation(m1->mot,m2->mot);

  int i,j,k;
  for(i=0;i<m1->taille;i++)
    for(j=0;j<m1->taille;j++){
      m3->tab[i][j]=-1;
    }  
  for(i=0;i<m1->taille;i++)
    for(j=0;j<m1->taille;j++)   
      for(k=0;k<m1->taille;k++){
	if(m3->tab[i][j] == -1)
	  m3->tab[i][j]= max(m1->tab[i][k],m2->tab[k][j]);
	else
	  m3->tab[i][j]=min(max(m1->tab[i][k],m2->tab[k][j]), m3->tab[i][j]);
      }
  return m3;
}


int est_idempotent(Matrice m){
  Matrice tmp = multiplication_in_MnR(m, m);
  int res = equal_matrice(m, tmp);
  detruire_matrice(tmp);
  return res;
}


//test si l'element de la matrice est stable(anchored); 
int stable(int i,int j,Matrice m){ 
  if(m->taille<=0){
    printf("erreur\n");
    return 0;
  }
  else{
    int k; 
    for(k=0; k < m->taille; k++){
      if(m->tab[k][k]==0 && m->tab[k][k] <= m->tab[i][j] && m->tab[i][j] == max(m->tab[i][k],m->tab[k][j]))
	return TRUE;
    }
    return FALSE;
  }
}

//test si la matrice est dans R;
int estMatriceR(Matrice m){
  int i,j;
  for(i=0;i<m->taille;i++)
    for(j=0;j<m->taille;j++){
      if(m->tab[i][j]>3)
	return FALSE;
    }
  return TRUE;
}

// Calcule et renvoie la matrice # d'une matrice dans R
Matrice creer_matrice_diese(Matrice m){
  if(!estMatriceR(m)){
    printf("erreur");
    return NULL;
  }
  Matrice ma=creer_matrice(m->taille);
  ma->mot = tree_diese(m->mot);
  
  int i,j;
  for(i=0;i<m->taille;i++)
    for(j=0;j<m->taille;j++){
      if(m->tab[i][j]==1 && !stable(i,j,m))
	ma->tab[i][j]=OMEGA;
      else
	ma->tab[i][j]=m->tab[i][j];
    }
  return ma;
}

