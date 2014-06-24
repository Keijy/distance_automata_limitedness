/*
 *   Ce fichier fait parti d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux 1
 *
 *   Copyright (C) 2014 Adrien Boussicault
 *
 *    This Library is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This Library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this Library.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "automate.h"
#include "Matrice.h"
#include "outils.h"
#include "arbresyntaxique.h"
#include <stdarg.h>

void creergraphe(Automate *a);


void creergraphe(Automate *a){
  FILE* f=fopen("./auto.gv","w+");
  fprintf(f, "digraph graphe {\n\trankdir=S;\n\tsize=\"8,5\"\n\tnode [shape = doublecircle];\n\t");
  Ensemble_iterateur it;
  for(it = premier_iterateur_ensemble(get_finaux(a));
      ! iterateur_ensemble_est_vide( it );
      it = iterateur_suivant_ensemble( it )){
    int tmp = (int)get_element( it);
    char num_etat[32];
    sprintf(num_etat," %d ", tmp);
    fprintf(f, num_etat);
  }
  fprintf(f,";\n\t");
  fprintf(f,"node [shape = point ];\n\t");
  for(it = premier_iterateur_ensemble(get_initiaux(a));
      ! iterateur_ensemble_est_vide( it );
      it = iterateur_suivant_ensemble( it )){
    int tmp=get_element(it);
    char num_etat[32];
    sprintf(num_etat," i%d ", tmp);
    fprintf(f,num_etat);
  }
  fprintf(f,";\n\t");
  fprintf(f,"\n\tnode [shape = circle];\n\t"); 
  Table_iterateur it2;
  for( it2 = premier_iterateur_table(get_transitions( a ));
       ! iterateur_ensemble_est_vide( it2 );
       it2 = iterateur_suivant_ensemble( it2 )
       ){
    Cle * cle = (Cle*) get_cle( it2 );
    Ensemble * fins = (Ensemble*) get_valeur( it2 );
    for( it = premier_iterateur_ensemble( fins );
	 ! iterateur_ensemble_est_vide( it );
	 it = iterateur_suivant_ensemble( it )
	 ){ 
      int fin = get_element( it );
      int origine=get_origine_cle(cle);
      char num_etat[32];
      sprintf(num_etat," %d ", origine);
      fprintf(f,num_etat);
    
      fprintf(f," -> ");
      fprintf(f,"%d",fin);

      fprintf(f," [ label =");
      char lettre=get_lettre_cle(cle);
      char num_etat1[32];
      sprintf(num_etat1,"\" %c ", lettre);
      fprintf(f,num_etat1);

    
      fprintf(f, ":");
      int cout=get_cout_cle(cle);
      char num_etat2[32];
      sprintf(num_etat2," %d\"", cout);
      fprintf(f,num_etat2);

      fprintf(f," ];\n\t");
    }
  }    
  fprintf(f,"}\n");
  fclose(f);
}



int main(){

  Automate * a = creer_automate();
  /* Automate * a2 = creer_automate(); */
	

  /* printf("\n==========automate(a)==============\n"); */
  /* ajouter_transition(a, 1, 'a', 1, 1); */
  /* ajouter_transition(a, 2, 'a', 1, 1); */
  /* ajouter_transition(a, 1, 'a', 1, 2); */
  /* ajouter_transition(a, 2, 'a', 0, 2); */
  /* ajouter_transition(a, 3, 'a', 3, 3); */

  /* ajouter_transition(a, 1, 'b', 1, 1); */
  /* ajouter_transition(a, 1, 'b', 1, 3); */
  /* ajouter_transition(a, 3, 'b', 1, 1); */
  /* ajouter_transition(a, 3, 'b', 0, 3); */

  /* ajouter_etat_initial(a, 1); */
  /* ajouter_etat_final(a, 1); */
	
  printf("\n==========automate(a)==============\n");
  ajouter_transition(a, 0, 'a', 0, 0);
  ajouter_transition(a, 0, 'b', 0, 0);
  ajouter_transition(a, 0, 'b', 0, 1);
  ajouter_transition(a, 1, 'a', 1, 1);
  ajouter_transition(a, 1, 'b', 0, 2);
  ajouter_transition(a, 2, 'a', 0, 2);
  ajouter_transition(a, 2, 'b', 0, 2);
  ajouter_transition(a, 3, 'a', 0, 3);
  ajouter_transition(a, 3, 'b', 1, 3);

  ajouter_etat_initial(a, 0);
  ajouter_etat_initial(a, 1);
  ajouter_etat_initial(a, 3);

  ajouter_etat_final(a, 1);
  ajouter_etat_final(a, 2);
  ajouter_etat_final(a, 3);

  creergraphe(a);
  print_automate(a);
  printf("\n");

  printf("\n==========L'automate a est-il limité ?==============\n");
  printf("Calcul de l'automate des matrices...\n");
  clock_t time = clock();
  Mautomate * maut = creer_automate_des_matrices(a);
  time= clock();
  printf("Temps de calcul : %.3f seconde\n", (double)time/CLOCKS_PER_SEC);
  printf("Calcul des limites...\n");
  Matrice res = (Matrice)est_limite(a, maut);
  if(res==NULL)
    printf("L'automate est limité.\n");
  else{
    printf("L'automate n'est pas limité, voici la premiere matrice qui cause le cout infini :\n");
    print_matrice_in_R(res);
  }
  print_mautomate(maut);
	
  return 0;

}




