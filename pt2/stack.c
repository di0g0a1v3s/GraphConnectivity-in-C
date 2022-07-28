// C program for linked list implementation of stack 
#include <limits.h> 
#include <stdlib.h> 
#include "stack.h"
  
// A structure to represent a stack 
struct StackNode { 
    int data; 
    struct StackNode* next; 
}; 

// A utility function to create an empty queue 
struct StackNode* createStack() 
{ 
    return NULL;
} 
  
struct StackNode* newStackNode(int data) 
{ 
    struct StackNode* stackNode = (struct StackNode*)malloc(sizeof(struct StackNode)); 
    stackNode->data = data; 
    stackNode->next = NULL; 
    return stackNode; 
} 
  
int isStackEmpty(struct StackNode* root) 
{ 
    return !root; 
} 
  
void push(struct StackNode** root, int data) 
{ 
    struct StackNode* stackNode = newStackNode(data); 
    stackNode->next = *root; 
    *root = stackNode; 
} 
  
int pop(struct StackNode** root) 
{ 
    if (isStackEmpty(*root)) 
        return INT_MIN; 
    struct StackNode* temp = *root; 
    *root = (*root)->next; 
    int popped = temp->data; 
    free(temp); 
  
    return popped; 
} 
  
int peek(struct StackNode* root) 
{ 
    if (isStackEmpty(root)) 
        return INT_MIN; 
    return root->data; 
}