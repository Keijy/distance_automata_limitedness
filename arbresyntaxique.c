#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "arbresyntaxique.h"


tree tree_creat(void){
  tree t=malloc(sizeof(struct tree_t));
  return t;
}


void tree_destroy(tree t){
  free(t);
}

tree tree_concatenation(tree t1,tree t2){
  tree t=tree_creat();
  t->c='.';
  t->l=t1;
  t->r=t2;
  return t;
}

tree tree_diese(tree t){
  tree td=tree_creat();
  td->c='#';
  td->l=t;
  return td;
}


void print_tree(tree t){
  if(t->c=='.'){
    print_tree(t->l);
    print_tree(t->r);
  }
  
  else if(t->c=='#'){
    printf("(");
    print_tree(t->l);
    printf(")#");
  }
  else
    printf("%c",t->c);
}
  
void tree_set(tree t,char c){
  t->c=c;
}






