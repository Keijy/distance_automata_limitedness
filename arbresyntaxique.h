#ifndef ARBRESYNTAXIQUE_H
#define ARBRESYNTAXIQUE_H
/* Header file for the map abstract data type (map.h) */

typedef struct tree_t * tree;
struct tree_t{
  tree l;
  tree r;
  char c;
};

/* create an empty tree */
tree tree_creat(void);


void tree_destroy(tree t);


tree tree_concatenation(tree t1,tree t2);


tree tree_diese(tree t);


void print_tree(tree t);


void tree_set(tree t,char c);

#endif 
