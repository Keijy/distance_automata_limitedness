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
    fprintf(f, "%d ", (int)get_element( it));
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
      fprintf(f,"%d",(int)get_origine_cle(cle));
    
      fprintf(f," -> ");
      fprintf(f,"%d",(int)fin);

      fprintf(f," [ label =\"");  
      char lettre=get_lettre_cle(cle);
      fprintf(f,"%c",lettre);

    
      fprintf(f, ":");
      fprintf(f,"%d",get_cout_cle(cle));

      fprintf(f,"\"];\n\t");
    }
  }   
  fprintf(f,"node [shape = point];\n\t");
  for(it = premier_iterateur_ensemble(get_initiaux(a));
      ! iterateur_ensemble_est_vide( it );
      it = iterateur_suivant_ensemble( it )){
    fprintf(f,"i%d ",(int)get_element(it));
  } 
  fprintf(f,";\n\t");
  for(it = premier_iterateur_ensemble(get_initiaux(a));
      ! iterateur_ensemble_est_vide( it );
      it = iterateur_suivant_ensemble( it )){
    fprintf(f,"i%d",(int)get_element(it));
    fprintf(f," -> ");
    fprintf(f,"%d",(int)get_element(it));

    fprintf(f," [ label =\"start\" ];\n\t");
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
  printf("Voulez vous voir le graphe de cet automate? (y/n)\n");
  char entree;
  scanf("%c", &entree);
  if(entree == 'y'){
    system("dot -Tpng auto.gv -o auto.png");
    system("xdg-open auto.png &");  //open avec le logiciel par default
    /* system("firefox auto.png >/dev/null 2>&1 &"); */  //open avec firefox
  }

  printf("\n");

  printf("\n==========L'automate a est-il limité ?==============\n");
  printf("Calcul de l'automate des matrices...\n");
  clock_t time = clock();
  Mautomate * maut = creer_mautomate(a);
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
  liberer_mautomate(maut);
  liberer_automate(a);//c'est bon
  return 0;
}




