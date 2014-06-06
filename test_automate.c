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

int main(){

	Automate * a = creer_automate();
	/* Automate * a2 = creer_automate(); */
	
	printf("\n==========automate(a)==============\n");
	ajouter_transition(a, 1, 'a', 1, 1);
	ajouter_transition(a, 2, 'a', 1, 1);
	ajouter_transition(a, 1, 'a', 1, 2);
	ajouter_transition(a, 2, 'a', 0, 2);
	ajouter_etat_initial(a, 1);
	ajouter_etat_final(a, 1);
	print_automate(a);

	Matrice ma = creer_matrice_transistions(a,'a');
	Matrice ma2 = multiplication(ma,ma);
	Matrice ma3 = multiplication(ma2,ma);

	printf("\n");

	print_matrice(ma);
	print_matrice(ma2);
	print_matrice(ma3);
	
	printf("\n");
	
	printStableMatrice(ma);
	printStableMatrice(ma2);
	printStableMatrice(ma3);

	printf("\n");

	Matrice ma4=matriceNaR(ma2);
	print_matrice(ma4);
	printf("\n");
	printStableMatrice(ma4);
	printf("\n");
	Matrice ma5=eDieze(ma4);
	printStableMatrice(ma5);

	return 0;
}
