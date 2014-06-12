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
#include "automate.h"
#include "Matrice.h"
#include "outils.h"
#include "arbresyntaxique.h"

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

	print_automate(a);
	
	
	Matrice ma = creer_matrice_transistions(a,'a');
	Matrice mb = creer_matrice_transistions(a,'b');

	Matrice ma2 = multiplication_in_MnR(ma,ma);
	Matrice ma3 = multiplication_in_MnR(ma2,ma);
	Matrice mab = multiplication_in_MnR(ma,mb);

	printf("\n");

	print_matrice_in_R(ma);
	print_matrice_in_R(ma2);
	print_matrice_in_R(ma3);
	print_matrice_in_R(mab);

	printf("\n==========idempotent(ma)==============\n");	
	int idem = est_idempotent(ma);
	if(idem){
	  printf("La matrice M(");
	  print_tree(get_mot(ma));
	  printf(") est idempotente\n"); 
	}
	else{
	  printf("La matrice M(");
	  print_tree(get_mot(ma));
	  printf(") n'est pas idempotente\n");
	}
	printf("\n");

	printf("\n==========M(a)#==============\n");	
	Matrice md = creer_matrice_dieze(ma);
	print_matrice_in_R(md);

	/* printf("\n==========Automate des matrices==============\n"); */
	/* Mautomate * a2 = creer_automate_des_matrices(a); */
	/* print_mautomate(a2, 10); */

	printf("\n==========L'automate a est-il limité ?==============\n");
	//Matrice res = est_limite(a);
	if(est_limite(a))
	  printf("L'automate est limité.\n");
	else{
	  printf("L'automate n'est pas limité, voici la premiere matrice qui cause le cout infini :\n");
	  //print_matrice_in_R(res);
	}

	return 0;
}
