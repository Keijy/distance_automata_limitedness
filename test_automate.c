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


#include "automate.h"
#include "Matrice.h"
#include "outils.h"

int main(){

	Automate * a = creer_automate();
	/* Automate * a2 = creer_automate(); */
	
	printf("\n==========automate(a)==============\n");
	ajouter_transition(a, 5, 'a', 1, 5);
	ajouter_transition(a, 9, 'a', 1, 5);
	ajouter_transition(a, 5, 'a', 1, 9);
	ajouter_transition(a, 9, 'a', 0, 9);
	ajouter_etat_initial(a, 5);
	ajouter_etat_final(a, 5);
	print_automate(a);

	printf("\n==========Matrices(a)==============\n");	
	Matrice ma = creer_matrice_transistions(a,'a');
	print_matrice(ma);
	Matrice ma2 = multiplication(ma,ma);
	print_matrice(ma2);
	Matrice ma3 = multiplication(ma2,ma);
	print_matrice(ma3);

	printf("\n==========idempotent(ma)==============\n");	
	int idem = est_idempotent(ma);
	if(idem)
	  printf("La matrice M(%s) est idempotente\n", get_mot(ma));
	else
	  printf("La matrice M(%s) n'est pas idempotente\n", get_mot(ma));



	
	return 0;
}
