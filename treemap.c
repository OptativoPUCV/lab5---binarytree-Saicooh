#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode *createTreeNode(void *key, void *value)
{
  TreeNode *new = malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new -> pair = malloc(sizeof(Pair));
    new -> pair -> key = key;
    new -> pair -> value = value;
    new -> parent = new -> left = new -> right = NULL;
  
    return new;
}

TreeMap *createTreeMap(int (*lower_than) (void *key1, void *key2))
{
  TreeMap *mapa = malloc(sizeof(TreeMap));
  mapa -> root = NULL;
  mapa -> current = NULL;
  mapa -> lower_than = lower_than;

  return mapa;
}

Pair *searchTreeMap(TreeMap *tree, void *key)
{
  tree -> current = tree -> root;

  while(tree -> current)
  {
    if(is_equal(tree, tree -> current -> pair -> key, key))
      return tree -> current -> pair;
      
    else if (tree -> lower_than(tree -> current -> pair -> key, key))
      tree -> current = tree -> current -> right;

    else tree -> current = tree -> current -> left;
  }
  
  return NULL;
}


void insertTreeMap(TreeMap * tree, void* key, void * value)
{
  TreeNode *nuevo = createTreeNode(key, value);
  
  if (!tree -> root) 
  {
    tree -> root = nuevo;
    tree -> current = nuevo;
     
    return;
  }

  TreeNode *current = tree -> root;
  TreeNode *parent = NULL;

  while(current)
  {
    if(is_equal(tree, key, current -> pair -> key)) return;

    parent = current;

    if(!tree -> lower_than(key,current -> pair -> key)) current = current -> right;
    else current = current -> left;
  }
  
  nuevo -> parent = parent;
  
  if (!tree -> lower_than(key, parent -> pair -> key)) parent -> right = nuevo;
  else parent -> left = nuevo;
    
  tree -> current = nuevo;
}

TreeNode *minimum(TreeNode *x)
{
  while(x -> left) x = x -> left;
  return x;
}

void removeNode(TreeMap *tree, TreeNode *node)
{
  if(node == NULL) return;

  if(node -> left == NULL && node -> right == NULL)
  {
    if(!tree -> lower_than(node -> pair -> key, node -> parent -> pair -> key)) node -> parent -> right = NULL;
    else node -> parent -> left = NULL;
    
    free(node);
    
    return;
  }

  else if(node -> left == NULL || node -> right == NULL)
  {
    TreeNode *hijo;
    if (node -> left) hijo = node -> left;
    else hijo = node -> right;
    
    if (node -> parent -> left == node) node -> parent -> left = hijo;
    else node -> parent -> right = hijo;
    hijo -> parent = node -> parent;
    
    free(node);

    return;
  }
  
  else
  {
    TreeNode *min = minimum(node -> right);
    node -> pair -> key = min -> pair -> key;
    node -> pair -> value = min -> pair -> value;
    
    removeNode(tree, min);
  }
}

void eraseTreeMap(TreeMap *tree, void *key)
{
  if (tree == NULL || tree -> root == NULL) return;

  if(searchTreeMap(tree, key) == NULL) return;
  TreeNode *node = tree -> current;
  removeNode(tree, node);
}

Pair *firstTreeMap(TreeMap * tree)
{
  tree -> current = minimum(tree -> root);
  return tree -> current -> pair;
}

Pair *nextTreeMap(TreeMap *tree)
{
  if (tree -> current == NULL) return NULL;

  if (tree -> current -> right)
  {
    TreeNode *current = tree -> current -> right;
    while (current -> left) current = current -> left;

    tree -> current = current;
    return current -> pair;
  }

  TreeNode *padre = tree -> current -> parent;
  
  while (padre && tree -> current == padre -> right) 
  {
    tree -> current = padre;
    padre = padre -> parent;
  }

  tree -> current = padre;
  if (padre == NULL) return NULL;
  
  return padre -> pair;
}

Pair *upperBound(TreeMap *tree, void *key) 
{
  TreeNode *nodoMayor = NULL;
  tree -> current = tree -> root;

  while(tree -> current)
  {
    if(is_equal(tree, tree -> current -> pair -> key, key)) return tree -> current -> pair;
      
    else if(tree -> lower_than(tree -> current -> pair -> key, key)) tree -> current = tree -> current -> right;
      
    else
    {
      nodeMayor = tree -> current;
      tree -> current = tree -> current -> left;
    }  
  }
  
  if(nodeMayor) return nodeMayor -> pair;
  return NULL;
}