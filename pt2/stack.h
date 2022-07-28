#ifndef _STACK_H_
#define _STACK_H_

typedef struct StackNode* Stack;

Stack createStack();
int isStackEmpty(Stack s);
void push(Stack* root, int data);
int pop(Stack* root);
int peek(Stack root);

#endif