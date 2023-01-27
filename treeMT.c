#include "treeMT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


TNode* newNode(int v) {
    TNode* node = (TNode*)malloc(sizeof(TNode));
    node->val = v;
    node->left = NULL;
    node->right = NULL;
    pthread_mutex_init(&node->lock,NULL);
    return node;
}


Tree* makeEmptyTree()
{
  /*
    TODO: Create a new Tree, setting the root to NULL. Initialize any necessary mutexes. 

    Return Tree*.
   */
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    tree->root = NULL;
    pthread_mutex_init(&tree->lock,NULL);
    return tree;
}


void insertIntoTree(Tree* t, int val)
{
  /*
    TODO: This function should construct a new Node, traverse the BST to find the correct insertion location, and insert it. If there is already a node with that value, nothing needs to be inserted.

    Use mutexes to prevent threads from interfering with one another.  
   */
    TNode* new = newNode(val);
    pthread_mutex_lock(&t->lock); 
    if (!t->root){
        t->root = new;
        pthread_mutex_unlock(&t->lock);
        return;
    }
    pthread_mutex_lock(&t->root->lock);
    pthread_mutex_unlock(&t->lock);
    TNode* currNode = t->root;
    TNode* nextNode = NULL;
    while(currNode != NULL){
        if (val < currNode->val){
            if (currNode->left == NULL){
                currNode->left = new;
                pthread_mutex_unlock(&currNode->lock);
                return;
            }
            else nextNode  = currNode->left;
        }
        else if (val > currNode->val){
            if (currNode->right == NULL) {
                currNode->right = new;
                pthread_mutex_unlock(&currNode->lock);
                return;
            }
            else nextNode = currNode->right;
        }
        else {
            pthread_mutex_unlock(&currNode->lock);
            return;
        }
        pthread_mutex_lock(&nextNode->lock);
        pthread_mutex_unlock(&currNode->lock);
        currNode = nextNode;
   }
}

void printTreeAux(TNode* root)
{
   if (root == NULL)
      return;
   else {
      printf("(");
      printTreeAux(root->left);
      printf(" %d ",root->val);
      printTreeAux(root->right);
      printf(")");      
   }
}

void printTree(Tree* t)
{
  printTreeAux(t->root);
}

void destroyTreeAux(TNode* root)
{
  if(root != NULL){
    destroyTreeAux(root->left);
    destroyTreeAux(root->right);
    pthread_mutex_destroy(&root->lock);
    free(root);
  }
}

void destroyTree(Tree* t)
{
  destroyTreeAux(t->root);
  pthread_mutex_destroy(&t->lock);
  free(t);
}


