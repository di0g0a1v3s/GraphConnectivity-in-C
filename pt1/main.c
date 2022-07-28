//COMPILAR: gcc main.c queue.c -o main
//CORRER: ./main

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "queue.h" //FIFO
#include "string.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


typedef struct Node
{
    int id;
    int type;
    struct Node *next;

    int ignore; //only used in ALGORITHM IV (EXTRA)
} node;


//linked list node
typedef struct LinkedListStruct
{
  int data;
  struct LinkedListStruct * next;
}LinkedList;


int add_edge(node **, int, int);
void output(node **);
void display(node **, int);
node** importGraphFromFile(char* , int*);
int getNumberOfNodes(char*);
int isConnected(node** , int, int);
int isLinkBiconnected(node** , int, int);
void dfs_link_biconnected(int , node** , int* , int* , int* , int * , int* , int* , int);
void free_graph(node**, int);
void dfs(int, node**, int, int*, int*, int*, int*);
void DFS_CommerciallyConnected_Acyclic(int, int*, int, node**);
int isCommerciallyConnected_Acyclic(node** , int, int);
int picknode(node**, int , int* , int* );
void resetIgnore(node**, int);
void DFS_CommerciallyConnected_General(int v, int* , int , node** , node* );
int isCommerciallyConnected_General(node** , int, int);
int isCommerciallyAcyclic(node**, int, int);
void print_cycle(int*, int);
void DFS_commercially_acyclic(node**, int, int, int*, int*, int*, int*, int*);
int isPathCommercial(int , int );
void free_linked_list(LinkedList*);



int main(void)
{
    int number_of_nodes;
    node** graph;
    char buffer[100];
    char* menu = "\nSelect an option:\n0. Display Graph\n1. Is Graph Connected?\n2. Is Graph Link-Biconnected?\n3. Is Graph Commercially-Acyclic?\n4. Is Graph Commercially Connected?\n5. Exit\n";
    do{
      printf("Network file: ");
      scanf("%s", buffer);
      graph = importGraphFromFile(buffer, &number_of_nodes);
    } while(graph == NULL);

    int input = -1;
    while(input != 5)
    {
      printf("%s", menu);
      scanf("%d",&input);
      printf("\n");
      switch (input)
      {
      case 0:
        display(graph, number_of_nodes);
        break;
      case 1:
        isConnected(graph, number_of_nodes, TRUE);
        break;
      case 2:
		if(isConnected(graph, number_of_nodes, FALSE))
		  isLinkBiconnected(graph, number_of_nodes, TRUE);
		else
		{
		  printf("It's not Link-Biconnected\n");
		}
        break;
      case 3:
        isCommerciallyAcyclic(graph, number_of_nodes, TRUE);
        break;
      case 4:
        if(isCommerciallyAcyclic(graph, number_of_nodes, FALSE))
          isCommerciallyConnected_Acyclic(graph,number_of_nodes, TRUE);
        else
        {
          printf("This may take a while...\n");
          isCommerciallyConnected_General(graph,number_of_nodes, TRUE);
        }
        break;
      case 5:
        break;
      default:
        break;
      }
    }

    free_graph(graph, number_of_nodes);

}





//------------------- ALGORITHM I -------------------------------------------

//determines if the graph is connected, using the BFS algorithm
//COMPLEXITY: O(|V|+|E|)
int isConnected(node** graph, int number_of_nodes, int verbose)
{
    int* discovered = (int*)(malloc(number_of_nodes*sizeof(int))); //discovery array
    int i = 0, s = 0;
    for (i = 0; i < number_of_nodes; i++)
    {
        discovered[i] = FALSE;  //initialize all to FALSE
    }
    Queue q = createQueue(); //First In First Out data structure (FIFO)

    while(graph[s]==NULL) //Search for initial node
      s++;

    discovered[s] = TRUE;
    enQueue(q,s); //put s in queue

    while (!isQueueEmpty(q))
    {
        int u = deQueue(q); //take next node out of queue
        node* aux = graph[u];
        while(aux != NULL) //iterate through neighbours of u
        {
            int v = aux->id;
            if(discovered[v] == FALSE)
            {
                discovered[v] = TRUE;
                enQueue(q,v); //put neighbours of u in queue if not yet discovered
            }
            aux = aux->next;
        }

    }

    //graph if connected only if all nodes have been discovered (starting from an arbitrary node,
    //it should be possible to reach every other node of the graph)
    int connected = TRUE;
    for(i = 0; i < number_of_nodes; i++)
    {
        if(discovered[i] == FALSE && graph[i]!=NULL)
            connected = FALSE;
    }

    if(connected == TRUE)
    {
      if(verbose)
        printf("It's Connected\n");
    }
    else
    {
      if(verbose)
        printf("It's not Connected\n");
    }

    free(discovered);
    return connected;
}

//---------------------------------------------------------------------------




//------------------- ALGORITHM II -------------------------------------------

//determines if the graph is link-connected, using the DFS algorithm
//COMPLEXITY: O(|V|+|E|)
int isLinkBiconnected(node** graph, int number_of_nodes, int verbose)
{
	int discovered[number_of_nodes];
	int pred[number_of_nodes];
	int d[number_of_nodes];		//discovery time
	int l[number_of_nodes];		//lowest connection
	int time=0;
	int i = 0, s = 0, flg = 0;

  for (i = 0; i < number_of_nodes; i++)
  {
    discovered[i] = FALSE;  //initialize all to FALSE
    pred[i]=-1;
    d[i]=-1;
    l[i]=-1;
  }

  while(graph[s]==NULL) //Search for initial node
    s++;

  dfs_link_biconnected(s, graph, pred, discovered, d, l, &flg, &time, verbose);

  if(flg)
  {
    if(verbose)
      printf("It's not Link-Biconnected\n");
    return FALSE;
  }
  else
  {
    if(verbose)
      printf("It's Link-Biconnected\n");
    return TRUE;
  }
}

void dfs_link_biconnected(int u, node** graph, int* pred, int* discovered, int* d, int * l, int* flg, int* time, int verbose)
{
    //current node is visited
    discovered[u] = TRUE;

    // Initialize discovery time and low value
    d[u] = l[u] = ++(*time);

    node*aux=graph[u];
    while(aux!=NULL)
    {
        int v = aux->id;  // v is adjacent of u

        // If v is not visited recur for it
        if (!discovered[v])
        {
			    pred[v]=u;
          dfs_link_biconnected(v, graph, pred, discovered, d, l, flg, time, verbose);

			      // If the lowest vertex reachable from subtree
            // under v is  below u in DFS, then u-v
            // is a bridge
            if (l[v] > d[u])
            {
              if(verbose)
                printf("Bridge: %d-%d\n",u,v);
              *flg = 1;
            }

            // Check if the subtree of v has a
            // connection to one of the ancestors of u
            l[u]  = MIN(l[u], l[v]);
        }

        // uv is a back edge
        else if (v != pred[u])
            l[u]  = MIN(l[u], d[v]);

        aux=aux->next;
    }
}

//---------------------------------------------------------------------------




//------------------- ALGORITHM III -------------------------------------------

//determines if the grpah is commercialy acyclic using DFS algortihm
//returns the commercial cycles if not
//COMPLEXITY: O(|V|+|E|)
int isCommerciallyAcyclic(node** graph, int number_of_nodes, int verbose)
{
  int* discovered = (int*)(malloc(number_of_nodes*sizeof(int))); //visited array
  int* finished = (int*)(malloc(number_of_nodes*sizeof(int))); //visited array
  int *cycle = (int*)malloc(number_of_nodes*sizeof(int));
	int flg = -1, i = 0, s = 0, stop = 0;

  for (i = 0; i < number_of_nodes; i++)
  {
      discovered[i] = FALSE;  //initialize all to FALSE
      finished[i] = FALSE;  //initialize all to FALSE
      cycle[i] = -1;  //initialize all to '-1'
  }

  //picks the first node from the graph which's not discovered
  //Runs a DFS starting in that node, s
  while (picknode(graph, number_of_nodes, discovered, &s) != 0 && flg == -1)
    DFS_commercially_acyclic(graph, number_of_nodes, s, discovered, finished, &flg, cycle, &stop);

  int ret = FALSE;

  //flag flg returns the last node reached by the DFS
  //no cycle means flg is returned '-1' from the DFS
  if (flg == -1)
  {
    if(verbose)
      printf("It's Commercially-Acyclic\n");
    ret = TRUE;
  }
  else if(verbose)
    print_cycle(cycle, flg);

  free(discovered);
  free(finished);
  free(cycle);
  return ret;
}

//picks the first node from the graph which's not discovered
int picknode(node** graph, int number_of_nodes, int* discovered, int* node_ID)
{
  for (int i = 0; i < number_of_nodes; i++)
  {
    if (graph[i] != NULL)
    {
      if (discovered[i] == 0)
      {
        *node_ID = i;
        return 1;
      }
    }
  }
  return 0;
}

//prints the cycle discovered in algorithm III
void print_cycle(int* cycle, int flg)
{
  int i = 0, size = 0, k = 0;


  while (cycle[size] != -1) //finds the cycle size
    size++;

  printf("It's not Commercially-Acyclic\n");

  printf("Cycle: %d->", flg);

  for (i = size-1; i >= 0; i--)
    if (cycle[i] != flg)
      printf("%d->", cycle[i]);

  printf("%d\n", flg);
}

void DFS_commercially_acyclic(node** graph, int number_of_nodes, int s, int* discovered, int* finished, int* flg, int* cycle, int* stop)
{
  int i = 0;

  //current node is visited
  discovered[s] = TRUE;

  node* aux = graph[s];

  while (aux != NULL && *flg == -1)
  {
      int v = aux->id;  // v is current adjacent of s

      // in the case a cycle has been discovered; v has already been visited
      if (discovered[v] == TRUE && aux->type == TYPE_CUSTOMER && finished[v] == FALSE)
      {
        *flg = v; // saves the value of the last node's id reached by the DFS
        i = 0;
        while (cycle[i] != -1) // saves the node's id on the cycle
          i++;
        cycle[i] = v;
      }

      // If v is not visited yet, then recur for it
      if (discovered[v] == FALSE && aux->type == TYPE_CUSTOMER)
        DFS_commercially_acyclic(graph, number_of_nodes, v, discovered, finished, flg, cycle, stop);

      // DFS proceeds only if there are no cycles discovered yet
      if (*flg == -1)
        aux=aux->next;
  }

  if (*flg == s)  // stops the saving of node's id to the cycle
    *stop = 1;

  if (*stop != 1 && *flg != -1)
  {
    i = 0;
    while (cycle[i] != -1)  // saves the node's id on the cycle
      i++;
    cycle[i] = s;
  }

  finished[s] = TRUE; // every node ahead of the current one has been visited
}

//---------------------------------------------------------------------------




//------------------- ALGORITHM IV -------------------------------------------

//determines if an acyclic graph is commercially connected
//COMPLEXITY: O(|V|+|E|)
int isCommerciallyConnected_Acyclic(node** graph, int number_of_nodes, int verbose)
{

  int i = 0;
  //build list of tier 1 nodes
  LinkedList* tier_1_nodes = NULL;
  for (i = 0; i < number_of_nodes; i++)
  {
    if(graph[i] != NULL)
    {
      node* aux = graph[i];
      int has_provider = FALSE;
      while(aux != NULL)
      {
        if(aux->type == TYPE_PROVIDER)
        {
          has_provider = TRUE;
          break;
        }
        aux = aux->next;
      }
      if(has_provider == FALSE)
      {
        LinkedList* newNode = (LinkedList*) malloc(sizeof(LinkedList));
        newNode->next = tier_1_nodes;
        newNode->data = i;
        tier_1_nodes = newNode;
      }
    }
  }


  //check if all tier 1 nodes are connected with peer links
  LinkedList* iterator1 = tier_1_nodes;
  while(iterator1 != NULL)
  {
    LinkedList* iterator2 = iterator1->next;
    while(iterator2 != NULL)
    {
      node* aux = graph[iterator1->data];
      int has_peer_connnection = FALSE;
      while(aux != NULL)
      {
        if(aux->id == iterator2->data && aux->type == TYPE_PEER)
        {
          has_peer_connnection = TRUE;
          break;
        }
        aux = aux->next;
      }
      if(has_peer_connnection == FALSE)
      {
        if(verbose)
          printf("It's not Commercially Connected\n");
        free_linked_list(tier_1_nodes);
        return FALSE; //if a pair of tier 1 nodes don't have a peer link, the graph is not commercially connected
      }
      iterator2 = iterator2->next;
    }
    iterator1 = iterator1->next;
  }


  //DFS initialization array
  int* discovered = (int*) malloc(sizeof(int)*number_of_nodes);
  for (int i = 0; i < number_of_nodes; i++)
      discovered[i] = FALSE;


  //start DFS from tier 1 nodes
  LinkedList* iterator = tier_1_nodes;
  while(iterator != NULL)
  {
    int v = iterator->data;
    DFS_CommerciallyConnected_Acyclic(v, discovered, number_of_nodes, graph);
    iterator = iterator->next;
  }

  int ret = TRUE;
  for (int i = 0; i < number_of_nodes; i++)
  {
    if(discovered[i] == FALSE && graph[i] != FALSE)
    {
      if(verbose)
        printf("It's not Commercially Connected\n");
      ret = FALSE;
      break;
    }

  }
  if(ret == TRUE && verbose)
    printf("It's Commercially Connected\n");

  free_linked_list(tier_1_nodes);
  free(discovered);
  return ret;

}

void DFS_CommerciallyConnected_Acyclic(int v, int* discovered, int number_of_nodes, node** graph)
{
  // Mark the current node as visited
  discovered[v] = TRUE;

  // Recur for all the vertices adjacent customers of this one
  node* aux = graph[v];
  while(aux != NULL)
  {
    if(aux->type == TYPE_CUSTOMER && !discovered[aux->id])
    {
      DFS_CommerciallyConnected_Acyclic(aux->id, discovered, number_of_nodes, graph);

    }
    aux = aux->next;
  }

}

//--------------------------------------------------------------------------




//-------------------ALGORITH IV (EXTRA)-------------------------------------

//determines if a graph is commercially connected. Works for any graph.
//COMPLEXITY: O(|V|*(|V|+|E|))
int isCommerciallyConnected_General(node** graph, int number_of_nodes, int verbose)
{
  //DFS initialization array
  int* discovered = (int*) malloc(sizeof(int)*number_of_nodes);
  int ret = TRUE;
  for(int v = 0; v < number_of_nodes; v++)
  {
    if(graph[v] == NULL)
      continue;

    //reset attribute "ignore" for all links (set to FALSE)
    resetIgnore(graph, number_of_nodes);

    for (int i = 0; i < number_of_nodes; i++)
        discovered[i] = FALSE;


    //DFS starting in node v
    DFS_CommerciallyConnected_General(v, discovered, number_of_nodes, graph, NULL);

    for (int i = 0; i < number_of_nodes; i++)
    {
      if(discovered[i] == FALSE && graph[i] != FALSE)
      {
        if(verbose)
          printf("It's not Commercially Connected\n");
        ret = FALSE;
        break;
      }

    }
    if(ret == FALSE)
      break;
  }
  free(discovered);

  if(ret == TRUE && verbose)
    printf("It's Commercially Connected\n");

  return ret;
}

void DFS_CommerciallyConnected_General(int v, int* discovered, int number_of_nodes, node** graph, node* previous_link)
{
  // Mark the current node as visited
  discovered[v]++;

  int previous_link_type = TYPE_NONE;
  if(previous_link != NULL)
  {
    previous_link_type = previous_link->type;
  }

  // Recur for all the vertices connected to v (that keep the path commercial)
  node* aux = graph[v];
  while(aux != NULL)
  {
    if(aux->ignore == FALSE && isPathCommercial(previous_link_type, aux->type))
    {
      aux->ignore = TRUE; //once this link (in this direction) is used once, it can't be used again
      DFS_CommerciallyConnected_General(aux->id, discovered, number_of_nodes, graph, aux);
    }
    aux = aux->next;
  }
}

//set attribute "ignore" of all the graph links to false
void resetIgnore(node** graph, int number_of_nodes)
{
  for(int i = 0; i<number_of_nodes; i++)
  {
    node* aux = graph[i];
    while(aux != NULL)
    {
      aux->ignore = FALSE;
      aux=aux->next;
    }
  }
}

//determines whether, when adding a link to an already commercial path,
//that path remains commercial or not.
//Commercial paths are in the form PRC (P - customer to provider, R  - peer to peer, C - provider to customer)
//with P>=0; R=0,1; C>=0, so the only way a path ceases to remain commercial is if its last link is of types
//R or C and we wish to insert a link of types P or R
//If it is the first link of the path, TYPE_NONE can be used for previous_link_type
int isPathCommercial(int previous_link_type, int next_link_type)
{
    if((previous_link_type == TYPE_PEER || previous_link_type == TYPE_CUSTOMER) //previous link of type R or C
            && (next_link_type == TYPE_PROVIDER || next_link_type == TYPE_PEER)) //new link of type P or R
    {
        return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------------




//---------------GRAPH FUNCTIONS-------------------------------------

//frees all memory of the graph data structure
void free_graph(node** graph, int number_of_nodes)
{
    for(int i = 0; i<number_of_nodes; i++)
    {
        node* iterator = graph[i];
        while(iterator != NULL)
        {
            node* aux = iterator;
            iterator = iterator->next;
            free(aux);
        }
    }
    free(graph);
}

//function that, given a file, returns the corresponding graph in form of a node** data structure,
//as well as its number of nodes (int* number_of_nodes)
node** importGraphFromFile(char* filename, int* number_of_nodes)
{
    *number_of_nodes = getNumberOfNodes(filename);
    if(*number_of_nodes == -1)
      return NULL;

    int tail_id, head_id, type;
    FILE *fp;

    if ((fp = fopen(filename, "r")) == NULL) {
      printf("Error! opening file\n");
      // Program exits if file pointer returns NULL.
      return NULL;
    }

    node** link = (node**)malloc(sizeof(node*)*(*number_of_nodes));

    for(int i = 0; i<(*number_of_nodes); i++)
    {
        link[i] = NULL;
    }

    while(fscanf(fp, "%d %d %d", &tail_id, &head_id, &type) != EOF)
      add_edge(&link[tail_id], head_id, type);

    fclose(fp);
    return link;
}

//returns the number of nodes of a graph, given the file in which it is stored
int getNumberOfNodes(char* filename)
{
    int tail_id, head_id, type;
    int num_of_nodes = 0;
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
      printf("Error! opening file\n");
      // Program exits if file pointer returns NULL.
      return -1;
    }

    while(fscanf(fp, "%d %d %d", &tail_id, &head_id, &type) != EOF)
    {
      num_of_nodes = MAX(tail_id+1, num_of_nodes);
      num_of_nodes = MAX(head_id+1, num_of_nodes);
    }
    return num_of_nodes;

    fclose(fp);
}

//adds an edge to the graph data structure
int add_edge(node **head, int id, int type)
{
    node *new_node = malloc(sizeof(node));
    int success = new_node != NULL;

    if (success)
    {
        new_node->id = id;
        new_node->type = type;
        new_node->next = *head;
        *head = new_node;
    }
    return success;
}

//prints a link of the graph
void output(node **head)
{
    for(node *current = *head; current != NULL; current = current->next)
        printf("->(%d, %d)", current->id, current->type);
}

//prints the graph
void display(node **graph, int n)
{
    for (int i = 0; i < n; i++)
    {
      if(*graph++!=NULL)
      {
        graph--;
        printf("[%d]", i);
        output(graph++);
        putchar('\n');
      }
    }
}

//----------------------------------------------------------------------


void free_linked_list(LinkedList* head)
{
  LinkedList* aux;
  while(head != NULL)
  {
    aux = head;
    head = head->next;
    free(aux);
  }
}
