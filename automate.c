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

struct _Automate {
  Ensemble * vide;
  Ensemble * etats;
  Ensemble * alphabet;
  Table * transitions;
  Ensemble * initiaux;
  Ensemble * finaux;
};

struct _Cle {
  int origine;
  int lettre;
  int cout;
};

/* automate : automate des matrices
 * t_matrices_etats : table qui à un état de l'automate, associe sa matrice dans R
 * t_matrices_lettres : table qui à une lettre associe sa matrice dans R
 */
struct _Matrice_Automate{
  Automate * automate;
  Table* t_matrices_etats;
  Table* t_matrices_lettres;
};

int get_origine_cle(Cle* c){
  return c->origine;
}

int get_lettre_cle(Cle* c){
  return c->lettre;
}

int get_cout_cle(Cle* c){
  return c->cout;
}

void supprimer_etat( Automate * automate, int etat );
void ajouter_transitions( Automate * automate, Table * transitions );

int comparer_cle(const Cle * a, const Cle * b){
  if( a->origine < b->origine )
    return -1;
  if( a->origine > b->origine )
    return 1;
  if( a->lettre < b->lettre )
    return -1;
  if( a->lettre > b->lettre )
    return 1;
  if( a->cout < b->cout )
    return -1;
  if( a->cout > b->cout)
    return 1;
  return 0;
}

void print_cle( const Cle * a){
  printf( "{%d} --> (%c : %d)" , a->origine, (char) (a->lettre), a->cout );
}

/* retourne un état non utilisé correspond au plus petit entier
 * disponible l'automate
 */
int get_etat_libre(Automate * automate){
  int i = 0;
  for ( ; est_un_etat_de_l_automate( automate, i); i++);
  return i;
}

/* retourne une lettre non utilisée dans l'ensemble des
 * minuscule (a-z) et majuscule(A-Z)
 * retourne -1 si toutes les lettres sont prises
 */
int get_lettre_libre(Automate * automate){
  int lettre;
  for(lettre = 97; lettre < 123; lettre++){
    if(!est_dans_l_ensemble(get_alphabet(automate), (intptr_t)lettre))
      return lettre;
  }
  for(lettre = 65; lettre < 91; lettre++){
    if(!est_dans_l_ensemble(get_alphabet(automate), (intptr_t)lettre))
      return lettre;
  }
  return -1;
}

void supprimer_cle( Cle * cle ){
  xfree( cle );
}

void initialiser_cle( Cle * cle, int origine, char lettre, int cout ){
  cle->origine = origine;
  cle->lettre = (int) lettre;
  cle->cout = cout;
}

Cle * creer_cle( int origine, char lettre, int cout ){
  Cle * result = xmalloc( sizeof(Cle) );
  initialiser_cle( result, origine, lettre, cout );
  return result;
}

Cle * copier_cle( const Cle * cle ){
  return creer_cle( cle->origine, cle->lettre, cle->cout );
}

Automate * creer_automate(){
  Automate * automate = xmalloc( sizeof(Automate) );
  automate->etats = creer_ensemble( NULL, NULL, NULL );
  automate->alphabet = creer_ensemble( NULL, NULL, NULL );
  automate->transitions = 
    creer_table(( int(*)(const intptr_t, const intptr_t) ) comparer_cle,
		( intptr_t(*)(const intptr_t) ) copier_cle,
		( void(*)(intptr_t) ) supprimer_cle );
  automate->initiaux = creer_ensemble( NULL, NULL, NULL );
  automate->finaux = creer_ensemble( NULL, NULL, NULL );
  automate->vide = creer_ensemble( NULL, NULL, NULL ); 
  return automate;
}

void liberer_automate( Automate * automate ){
  liberer_ensemble( automate->vide );
  liberer_ensemble( automate->finaux );
  liberer_ensemble( automate->initiaux );
  pour_toute_valeur_table( automate->transitions,
			   ( void(*)(intptr_t) ) liberer_ensemble );
  liberer_table( automate->transitions );
  liberer_ensemble( automate->alphabet );
  liberer_ensemble( automate->etats );
  xfree(automate);
}

const Ensemble * get_etats( const Automate * automate ){
  return automate->etats;
}

/* Getteur de l'ensemble des etats initiaux.
 */
const Ensemble * get_initiaux( const Automate * automate ){
  return automate->initiaux;
}

/* Getteur de l'ensemble des etats finaux.
 */
const Ensemble * get_finaux( const Automate * automate ){
  return automate->finaux;
}

/* Getteur de l'alphabet de l'automate.
 */
const Ensemble * get_alphabet( const Automate * automate ){
  return automate->alphabet;
}

const Table * get_transitions( const Automate * automate ){
  return automate->transitions;
}

void ajouter_etat( Automate * automate, int etat ){
  ajouter_element( automate->etats, etat );
}

/* On ajoute la lettre à l'alphabet de l'automate.
 * Comme pour l'ajout d'un etat il n'est pas necessaire de regarder
 * si la lettre est déjà dans l'ensemble.
 */
void ajouter_lettre( Automate * automate, char lettre ){
  ajouter_element( automate->alphabet, lettre );
}

void ajouter_transition( Automate * automate,
			 int origine,
			 char lettre,
			 int cout,
			 int fin
			 ){
  ajouter_etat( automate, origine );
  ajouter_etat( automate, fin );
  ajouter_lettre( automate, lettre );

  Cle cle;
  initialiser_cle( &cle, origine, lettre, cout );
  Table_iterateur it = trouver_table( automate->transitions,
				      (intptr_t) &cle );
  Ensemble * ens;
  if( iterateur_est_vide( it ) ){
    ens = creer_ensemble( NULL, NULL, NULL );
    add_table( automate->transitions,
	       (intptr_t) &cle,
	       (intptr_t) ens );
  } else {
    ens = (Ensemble*) get_valeur( it );
  }
  ajouter_element( ens, fin );
}

/* On test si l'etat fait ne fais pas déjà partie de l'automate.
 * Dans ce cas on l'ajoute à l'ensemble des états de l'automate
 * puis on le rend final.
 */
void ajouter_etat_final( Automate * automate, int etat_final ){
  if ( !est_un_etat_de_l_automate( automate, etat_final ))
    ajouter_etat( automate, etat_final );
  ajouter_element( automate->finaux, etat_final );
}

/* On test si l'etat fait ne fais pas déjà partie de l'automate.
 * Dans ce cas on l'ajoute à l'ensemble des états de l'automate
 * puis on le rend initial.
 */
void ajouter_etat_initial( Automate * automate, int etat_initial ){
  if ( !est_un_etat_de_l_automate( automate, etat_initial ))
    ajouter_etat( automate, etat_initial );
  ajouter_element( automate->initiaux, etat_initial );
}

void pour_toute_transition( const Automate * automate,
			    void (* action )( int origine, 
					      char lettre,
					      int fin,
					      void* data ),
			    void * data
			    ){
  Table_iterateur it1;
  Ensemble_iterateur it2;
  for( it1 = premier_iterateur_table( automate->transitions );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    Cle * cle = (Cle*) get_cle( it1 );
    Ensemble * fins = (Ensemble*) get_valeur( it1 );

    for( it2 = premier_iterateur_ensemble( fins );
	 ! iterateur_ensemble_est_vide( it2 );
	 it2 = iterateur_suivant_ensemble( it2 )
	 ){
      int fin = get_element( it2 );
      action( cle->origine, cle->lettre, fin, data );
    }
  }
}

Automate* copier_automate( const Automate * automate ){
  Automate * res = creer_automate();
  Ensemble_iterateur it1;
  // On ajoute les états de l'automate
  for( it1 = premier_iterateur_ensemble( get_etats( automate ) );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    ajouter_etat( res, get_element( it1 ) );
  }
  // On ajoute les états initiaux
  for( it1 = premier_iterateur_ensemble( get_initiaux( automate ) );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    ajouter_etat_initial( res, get_element( it1 ) );
  }
  // On ajoute les états finaux
  for( it1 = premier_iterateur_ensemble( get_finaux( automate ) );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    ajouter_etat_final( res, get_element( it1 ) );
  }
  // On ajoute les lettres
  for( it1 = premier_iterateur_ensemble( get_alphabet( automate ) );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    ajouter_lettre( res, (char) get_element( it1 ) );
  }
  // On ajoute les transitions
  Table_iterateur it2;
  for( it2 = premier_iterateur_table( automate->transitions );
       ! iterateur_ensemble_est_vide( it2 );
       it2 = iterateur_suivant_ensemble( it2 )
       ){
    Cle * cle = (Cle*) get_cle( it2 );
    Ensemble * fins = (Ensemble*) get_valeur( it2 );
    for( it1 = premier_iterateur_ensemble( fins );
	 ! iterateur_ensemble_est_vide( it1 );
	 it1 = iterateur_suivant_ensemble( it1 )
	 ){
      int fin = get_element( it1 );
      ajouter_transition( res, cle->origine, cle->lettre, cle->cout, fin );
    }
  }
  return res;
}

/* Renvoie l'automate destination, dont le nom de tous les états est
 * différent du nom des états de l'automate source.
 */
Automate * creer_automate_etat_different( const Automate * src,
					  const Automate * dst
					  ){
    Automate * res = creer_automate();
    Automate * copie_src = copier_automate(src);
    Table * table_corresp_etats = creer_table(NULL, NULL, NULL);

    // On ajoute les états de l'automate
    Ensemble_iterateur it1;
    for( it1 = premier_iterateur_ensemble( get_etats( dst ) );
	 ! iterateur_ensemble_est_vide( it1 );
	 it1 = iterateur_suivant_ensemble( it1 )
	 ){
	int new_etat = get_etat_libre(copie_src);
	add_table( table_corresp_etats, get_element( it1 ), new_etat);
	ajouter_etat( res, new_etat );
	ajouter_etat( copie_src, new_etat );
      
	if( est_un_etat_final_de_l_automate( dst, get_element( it1 )))
	    ajouter_etat_final( res, new_etat );
	if( est_un_etat_initial_de_l_automate( dst, get_element( it1 )))
	    ajouter_etat_initial( res, new_etat );
    }

    // On ajoute les transitions
    Table_iterateur it2;
    for( it2 = premier_iterateur_table( dst->transitions );
	 ! iterateur_ensemble_est_vide( it2 );
	 it2 = iterateur_suivant_ensemble( it2 )
	 ){
	Cle * cle = (Cle*) get_cle( it2 );
	Ensemble * fins = (Ensemble*) get_valeur( it2 );
	for( it1 = premier_iterateur_ensemble( fins );
	     ! iterateur_ensemble_est_vide( it1 );
	     it1 = iterateur_suivant_ensemble( it1 )
	     ){
	    int fin = get_element( it1 );
	    Table_iterateur t_origine = trouver_table(table_corresp_etats, cle->origine);
	    Table_iterateur t_fin = trouver_table(table_corresp_etats, fin);
	    ajouter_transition( res,
				get_valeur(t_origine),
				cle->lettre,
				cle->cout,
				get_valeur(t_fin));
	}
    }
    liberer_table(table_corresp_etats);
    liberer_automate(copie_src);
    return res;
}

/* Renvoie l'automate dont dont les états ont tous été renommé
 * de 0 à n-1, où n est le nombre d'états de l'automate
 */
Automate * creer_automate_etats_0_n (Automate* a){
  Automate* m=creer_automate();
  Automate* res=creer_automate_etat_different(m, a);
  liberer_automate(m);
  return res;
}

Automate * translater_etat( const Automate * automate, int n ){
  Automate * res = creer_automate();

  Ensemble_iterateur it;
  for( it = premier_iterateur_ensemble( get_etats( automate ) );
       ! iterateur_ensemble_est_vide( it );
       it = iterateur_suivant_ensemble( it )
       ){
    ajouter_etat( res, get_element( it ) + n );
  }
    
  Table_iterateur it1;
  Ensemble_iterateur it2;
  for( it1 = premier_iterateur_table( automate->transitions );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    Cle * cle = (Cle*) get_cle( it1 );
    Ensemble * fins = (Ensemble*) get_valeur( it1 );

    for( it2 = premier_iterateur_ensemble( fins );
	 ! iterateur_ensemble_est_vide( it2 );
	 it2 = iterateur_suivant_ensemble( it2 )
	 ){
      int fin = get_valeur( it2 );
      ajouter_transition( res,
			  cle->origine + n,
			  cle->lettre,
			  cle->cout,
			  fin + n );
    }
  }
  return res;
}

void action_get_max_etat( const intptr_t element, void * data ){
  int * max = (int*) data;
  if( *max < element ) *max = element;
}

int get_max_etat( const Automate * automate ){
  int max = INT_MIN;
  pour_tout_element( automate->etats, action_get_max_etat, &max );
  return max;
}


void action_get_min_etat( const intptr_t element, void * data ){
  int * min = (int*) data;
  if( *min > element ) *min = element;
}

int get_min_etat( const Automate * automate ){
  int min = INT_MAX;
  pour_tout_element( automate->etats, action_get_min_etat, &min );
  return min;
} 

int est_un_etat_de_l_automate( const Automate * automate, int etat ){
  return est_dans_l_ensemble( get_etats( automate ), etat );
}

int est_un_etat_initial_de_l_automate( const Automate * automate, int etat ){
  return est_dans_l_ensemble( get_initiaux( automate ), etat);
}

int est_un_etat_final_de_l_automate( const Automate * automate, int etat ){
  return est_dans_l_ensemble( get_finaux( automate ), etat);
}

int est_une_lettre_de_l_automate( const Automate * automate, char lettre ){
  return est_dans_l_ensemble( get_alphabet( automate ), lettre);
}

void print_ensemble_2( const intptr_t ens ){
  print_ensemble( (Ensemble*) ens, NULL );
}

void print_lettre( intptr_t c ){
  printf("%c", (char) c );
}

void print_automate( const Automate * automate ){
  printf("- Etats :       ");
  print_ensemble( get_etats( automate ), NULL );
  printf("\n- Initiaux :    ");
  print_ensemble( get_initiaux( automate ), NULL );
  printf("\n- Finaux :      ");
  print_ensemble( get_finaux( automate ), NULL );
  printf("\n- Alphabet :    ");
  print_ensemble( get_alphabet( automate ), print_lettre );
  printf("\n- Transitions : ");
  print_table( 
	      automate->transitions,
	      ( void (*)( const intptr_t ) ) print_cle, 
	      ( void (*)( const intptr_t ) ) print_ensemble_2,
	      ""
	       );
  printf("\n");
}

void print_mautomate(Mautomate* a){
  int nb_etats = taille_ensemble(a->automate->etats);
  int nb_lettres = taille_ensemble(a->automate->alphabet);
  int nb_entree;
  printf("Voulez-vous afficher l'automate des matrice ? (y/n)\n (%d états, %d lettres)\n", nb_etats, nb_lettres);

  //vider le tampon
  int c;
  while((c=getchar()) != '\n' && c != EOF);
  char entree;

  scanf("%c", &entree);
  if(entree == 'y'){
    printf("LOL\n");
    print_automate(a->automate);
  }

  //vider le tampon
  while((c=getchar()) != '\n' && c != EOF);

  printf("Voulez-vous afficher les matrice de l'automate ? (y/n)\n (%d matrices)\n", nb_etats-1);
  scanf("%c", &entree);  
  if(entree == 'y'){
    printf("Combien de matrices souhaitez-vous afficher ?(%d matrices)\n", nb_etats-1);
    scanf("%d", &nb_entree); 
    
    printf("\nMatrices correspondant aux états de l'automate :\n\n");
    Table_iterateur it;
    int i;
    for( it = premier_iterateur_table( a->t_matrices_etats ), i = 0;
	 (!iterateur_ensemble_est_vide( it ) && i<nb_entree);
	 it = iterateur_suivant_ensemble( it ), i++
	 ){
      printf("Etat n°%d :\n", (int)get_cle(it));
      print_matrice_in_R((Matrice)get_valeur(it));
    }
  }
}

/* Supprime un de l'automate, un état passé en paramètre.
 */
void supprimer_etat(Automate * automate, int etat){
  // on maintient un ensemble des transitions à supprimer.
  Ensemble * trans_a_suppr = creer_ensemble(NULL, NULL, NULL);
  Table_iterateur it1;
  Ensemble_iterateur it2;

  retirer_element(automate->etats, etat);
  retirer_element(automate->initiaux, etat);
  retirer_element(automate->finaux, etat);

  // on met à jour cet ensemble
  for( it1 = premier_iterateur_table( automate->transitions );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    Cle * cle = (Cle*) get_cle( it1 );
    Ensemble * fins = (Ensemble*) get_valeur( it1 );
    if (cle->origine == etat) {
      ajouter_element(trans_a_suppr, (intptr_t) cle);
    } else {
      retirer_element(fins, etat);
    }
  }
  // On supprime dans une autre boucle car on ne peut pas supprimer
  // un élément pendant qu'on utilise sa structure iterateur.
  for( it2 = premier_iterateur_ensemble( trans_a_suppr );
       ! iterateur_ensemble_est_vide( it2 );
       it2 = iterateur_suivant_ensemble( it2 )
       ){
    delete_table(automate->transitions, get_element( it2 ));
  }
  liberer_ensemble(trans_a_suppr);
}

/* Ajoute un ensemble de transitions dans l'automate automate.
 */
void ajouter_transitions( Automate * automate, Table * transitions ){
  Table_iterateur it1;
  Ensemble_iterateur it2;

  for( it1 = premier_iterateur_table( transitions );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    Cle * cle = (Cle*) get_cle( it1 );
    Ensemble * fins = (Ensemble*) get_valeur( it1 );
    for( it2 = premier_iterateur_ensemble( fins );
	 ! iterateur_ensemble_est_vide( it2 );
	 it2 = iterateur_suivant_ensemble( it2 )
	 ){
      ajouter_transition(automate, cle->origine, cle->lettre, cle->cout, get_element( it2 ));
    }
  }
}

/* renvoie TRUE si il existe une transition (etat, lettre)-> X 
 * où X est un ensemble d'etat non vide de l'automate.
 * renvoie FALSE sinon
 */
int est_transition_automate(Automate* a, int etat, int lettre){
  Table_iterateur it2;
  int res = FALSE;
  for( it2 = premier_iterateur_table( a->transitions );
       (! iterateur_ensemble_est_vide( it2 ) && !res);
       it2 = iterateur_suivant_ensemble( it2 )
       ){
    Cle * cle = (Cle*) get_cle( it2 );
    if(cle->origine == etat && cle->lettre == lettre)
      res = TRUE;
  }
  return res;
}

//Renvoie TRUE si l'automate est complet, FALSE sinon
int est_complet(Automate* a){
  Ensemble_iterateur it1, it2;
 
  for( it1 = premier_iterateur_ensemble( get_etats( a ) );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    int etat = get_element(it1);
    for( it2 = premier_iterateur_ensemble( get_alphabet( a ) );
	 ! iterateur_ensemble_est_vide( it2 );
	 it2 = iterateur_suivant_ensemble( it2 )
	 ){
      int lettre = get_element(it2);
      if(!est_transition_automate(a, etat, lettre))
	return FALSE;
    }
  }
  return TRUE;
}

/* Complete l'automate en effectuant toutes les multiplications matricielles
 * necessaires pour chaque états, pour chaque lettre de l'automate.
 * Le nombre d'états est fini puisque le nombre de matrice l'est.
*/
void completer_automate_des_matrices(Mautomate * a){
  Automate * ma = a->automate;
  Table * t_matrices_etats = a->t_matrices_etats;
  Table * t_matrices_lettres = a->t_matrices_lettres;
  Ensemble_iterateur it1, it2;
  Table_iterateur it3;
  while (!est_complet(ma)){
    for( it1 = premier_iterateur_ensemble( get_etats( ma ) );
	 ! iterateur_ensemble_est_vide( it1 );
	 it1 = iterateur_suivant_ensemble( it1 )
	 ){
      int etat = get_element(it1);
      for( it2 = premier_iterateur_ensemble( get_alphabet( ma ) );
	   ! iterateur_ensemble_est_vide( it2 );
	   it2 = iterateur_suivant_ensemble( it2 )
	   ){
	int lettre = get_element(it2);
	if(!est_transition_automate(ma, etat, lettre)){
	  Matrice m1 = (Matrice)get_valeur(trouver_table(t_matrices_etats, (intptr_t)etat));
	  Matrice m2 = (Matrice)get_valeur(trouver_table(t_matrices_lettres, (intptr_t)lettre));
	  Matrice new_m = multiplication_in_MnR(m1, m2);

	  int dest = get_etat_libre(ma);
	  int find = FALSE;
	  for( it3 = premier_iterateur_table( t_matrices_etats );
	       (!iterateur_ensemble_est_vide( it3 ) && !find);
	       it3 = iterateur_suivant_ensemble( it3 )
	       ){
	    int cle = (int) get_cle( it3 );
	    Matrice m_tmp = (Matrice) get_valeur( it3 );
	    if(equal_matrice(m_tmp, new_m)){
	      find = TRUE;
	      detruire_matrice(new_m);
	      dest = cle;
	    }
	  }
	  if(!find)
	    add_table(t_matrices_etats, (intptr_t)dest, (intptr_t)new_m);
	  ajouter_transition(ma, etat, lettre, 0, dest);
	    
	}
      }      
    }
  }

}


/* Calcule les dieses des matrices idempotente et complete l'automate si besoin
 */
void calculer_diese_mautotmate(Mautomate * ma){
  Ensemble_iterateur it1;
  for( it1 = premier_iterateur_ensemble( get_etats( ma->automate ) );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    int etat = get_element(it1);
    if(etat != 0){
      Matrice m = (Matrice)get_valeur(trouver_table(ma->t_matrices_etats, (intptr_t)etat));
      if(est_idempotent(m)){
	Matrice md = creer_matrice_diese(m);
	if(!equal_matrice(m, md)){
	  int lettre = get_lettre_libre(ma->automate);
	  int etat = get_etat_libre(ma->automate);
	  add_table(ma->t_matrices_etats, (intptr_t)etat, (intptr_t)md);
	  add_table(ma->t_matrices_lettres, (intptr_t)lettre, (intptr_t)md);
	  ajouter_transition(ma->automate, 0, lettre, 0, etat);
	  completer_automate_des_matrices(ma);
	}
	else
	  detruire_matrice(md);
	
      }
    }
  }
  
}

/* Renvoie NULL si l'automate est limité
 * Renvoie la premiere matrice qui cause le cout infini sinon
 */
void * est_limite(Automate* a, Mautomate * ma){
  Table_iterateur it1;
  for( it1 = premier_iterateur_table( ma->t_matrices_etats );
       !iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    Matrice m_tmp = (Matrice) get_valeur( it1 );
    int** tab = get_tableau(m_tmp);
    int min = -1;
    int i, j;
    for(i = 0; i < get_taille(m_tmp); i++){
      for(j = 0; j < get_taille(m_tmp); j++){
	if(est_dans_l_ensemble(a->initiaux, (intptr_t)i) && est_dans_l_ensemble(a->finaux, (intptr_t)j)){
	  if(min == -1 || min > tab[i][j]){
	    min = tab[i][j];
	  }
	}
      }
    }
    if(min == OMEGA || min == INFINI){
      return m_tmp;
    }
  }
  return NULL;
}


/* Renvoie un "mautomate" qui contient un automate complet où chaque état 
 * correspond à une matrice de transition pour tous les chemins menant à cet état.
 * La structure contient également deux table qui permettent d'acceder au matrices rapidement.
 */

Mautomate* creer_mautomate(Automate* a){
  Mautomate *m = malloc(sizeof(*m));
  m->automate = creer_automate();
  m->t_matrices_etats = creer_table(NULL, NULL, NULL); //table qui, à une état, associe sa matrice dans R
  m->t_matrices_lettres = creer_table(NULL, NULL, NULL); //table qui, à une lettre associe sa matrice dans R

  Automate * abis = creer_automate_etats_0_n(a);

  ajouter_etat(m->automate, 0);

  Ensemble_iterateur it1;
  for( it1 = premier_iterateur_ensemble( get_alphabet(abis) );
       ! iterateur_ensemble_est_vide(it1);
       it1 = iterateur_suivant_ensemble(it1)
       ){
    int new_state = get_etat_libre(m->automate);
    char lettre = get_element(it1);
    Matrice tmp = creer_matrice_transistions(abis, lettre);
    add_table(m->t_matrices_etats, (intptr_t)new_state, (intptr_t)tmp);
    add_table(m->t_matrices_lettres, (intptr_t)lettre, (intptr_t)tmp);
    ajouter_transition(m->automate, 0, lettre, 0, new_state);
  }

  completer_automate_des_matrices(m);
  calculer_diese_mautotmate(m);

  liberer_automate(abis);
  
  return m;
}

void liberer_mautomate(Mautomate* a){
  liberer_automate(a->automate);
  Table_iterateur it1;

  for( it1 = premier_iterateur_table(a->t_matrices_etats);
       !iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
    Matrice tmp = (Matrice) get_valeur( it1 );
    detruire_matrice(tmp);
  }
  liberer_table(a->t_matrices_etats);
  liberer_table(a->t_matrices_lettres);
  free(a);
}
