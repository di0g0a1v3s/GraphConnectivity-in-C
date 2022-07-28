//COMPILAR: gcc main.c queue.c heap.c -o main
//CORRER: ./main

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "defs.h"
#include "string.h"
#include "queue.h"
#include "heap.h" //adapted from https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


typedef struct Node
{
    int id;
    int type;
    struct Node *next;
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
node** importGraphFromFile(char* , int*, int*);
int getNumberOfNodes(char*);
void free_graph(node**, int);
int* dijkstra(node** ,int , int , int , int);
void dijkstraBGP(node** ,int , int , int);
int extension(int, int);
int invert(int);
int extraxct_user_info(node**, int, char*, int);

void LengthOfBGPCCDF(node** , int);
int* lengthPathsBGP(node**,int, int);

int isCommerciallyAcyclic(node**, int);
int isCommerciallyConnected_Acyclic(node** , int);
void DFS_commercially_acyclic(node**, int, int, int*, int*, int*, int*, int*);
void DFS_CommerciallyConnected_Acyclic(int, int*, int, node**);
int picknode(node**, int, int*, int*);
void free_linked_list(LinkedList*);

void printMatrix(int** , int);
void printArray(int* , int);
void printCCDF(int* , int, int);
void printPDF(int*);
int* dijkstraMinimumPathsOfBGPTypeToDst(node** ,int , int t, int*);
void MinimumPathsOfBGPTypeCCDF(node** , int);
void TypeOfBGP_PDF(node**, int);

int main(void)
{
    int number_of_nodes;
    node** graph;
    char buffer[100];
    char* menu = "\nSelect an option:\n\n0. Display Graph\n1. Algorithm1\n2. Algorithm2\n3. Algorithm3\n4. Exit\n\n> ";
    char* alg1_menu = "\nSelect an option:\n\n0. Source and Destination\n1. PDF\n2. Menu\n\n> ";
	  char* alg2_menu = "\nSelect an option:\n\n0. Source and Destination\n1. CCDF\n2. Menu\n\n> ";
    char* alg3_menu = "\nSelect an option:\n\n0. Source and Destination\n1. CCDF\n2. Menu\n\n> ";
    int input1, input2, input3 = -1;
    int CC_flag = 0;
    char source_destination[12];

    do{
      printf("Network file: ");
      scanf("%s", buffer);
      graph = importGraphFromFile(buffer, &number_of_nodes, &CC_flag);
    } while(graph == NULL);


    while(input1 != 4)
    {
      input2 = -1;
      printf("%s", menu);
      scanf("%d",&input1);
      printf("\n");
      switch (input1)
      {
        case 0:
          display(graph, number_of_nodes);
          break;
        case 1:
          while(input2 != 2)
          {
            input3 = -1;
            printf("%s", alg1_menu);
            scanf("%d",&input2);
            printf("\n");
            switch (input2)
            {
              case 0:
                while(input3 == -1)
                {
                  printf("\nSelecet a source and a destination, S-D (eg. 123-321):\n\n> ");
                  scanf("%s", source_destination);
                  printf("\n");
                  input3 = extraxct_user_info(graph, number_of_nodes, source_destination, CC_flag);
                }
                break;
              case 1:
                TypeOfBGP_PDF(graph, number_of_nodes);
                break;
              case 2:
                break;
              default:
                break;
            }
          }
          break;
        case 2:
		while(input2 != 2)
          {
            input3 = -1;
            printf("%s", alg2_menu);
            scanf("%d",&input2);
            printf("\n");
            switch (input2)
            {
              case 0: ;

                int src;
                int dst;
                int valid = FALSE;
                do{
                  printf("\nSelecet a source and a destination, S-D (eg. 123-321):\n\n> ");
                  scanf("%d-%d", &src,&dst);
                  valid = (src >= 0 && src <= number_of_nodes-1 && dst >= 0 && dst <= number_of_nodes-1);
                  if(!valid)
                    printf("Invalid source and destination! Try again...\n");
                }while(!valid);
                printf("\n");
                int* length = lengthPathsBGP(graph, number_of_nodes, dst);
                printf("\nPath from %d to %d has length %d\n", src, dst, length[src]);
                free(length);
                break;
              case 1: ;
                LengthOfBGPCCDF(graph, number_of_nodes);
                break;
              case 2:
                break;
              default:
                break;
            }
          }
          break;
        case 3:
          while(input2 != 2)
          {
            input3 = -1;
            printf("%s", alg3_menu);
            scanf("%d",&input2);
            printf("\n");
            switch (input2)
            {
              case 0: ;

                int src;
                int dst;
                int valid = FALSE;
                do{
                  printf("\nSelecet a source and a destination, S-D (eg. 123-321):\n\n> ");
                  scanf("%d-%d", &src,&dst);
                  valid = (src >= 0 && src <= number_of_nodes-1 && dst >= 0 && dst <= number_of_nodes-1);
                  if(!valid)
                    printf("Invalid source and destination! Try again...\n");
                }while(!valid);
                printf("\n");
                int* path_types = dijkstra(graph,number_of_nodes,0,dst,FALSE);
                int* distances = dijkstraMinimumPathsOfBGPTypeToDst(graph, number_of_nodes, dst, path_types);
                printf("\nShortest Path from %d to %d with type %d has length %d\n", src,dst,path_types[src], distances[src]);
                free(path_types);
                free(distances);
                break;
              case 1: ;
                MinimumPathsOfBGPTypeCCDF(graph,number_of_nodes);
                break;
              case 2:
                break;
              default:
                break;
            }
          }
          break;
        case 4:
          break;
        default:
          break;
      }
    }
    free_graph(graph, number_of_nodes);
}


//------------------- ALGORITHM I -------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

// It is called only if the graph is commercially connected
// Checks if the source's node is a trivial node
// A trivial node doesn't require an algorithm to provide it's type
// A trivial node only has connections of the same type
// The complexity of checking if a source is trivial:
// O(|V|)
void dijkstraBGP(node** graph,int number_of_nodes, int s, int t)
{
    int cnst_type = 1;
    int type = 0;
    node* aux = graph[s];

    type = aux->type;

    //checks all the source neighbours
    //if the connection type remains constant
    //means every connection is of the same type
    //Then, the source is a trivial node
    while (aux != NULL)
    {
      if (aux->type != type)
        cnst_type = 0;
      aux = aux->next;
    }

    //In case of being trivial dispenses any algorithm
    if (cnst_type)
    {
      switch (type)
      {
        case 1:
          printf("BGP Type of Path: Client\n");
          break;
        case 2:
          printf("BGP Type of Path: Peer\n");
          break;
        case 3:
          printf("BGP Type of Path: Provider\n");
          break;
        default:
          break;
      }
    }

    //Otherwise it must run the Dijkstra algorithm
    else
      dijkstra(graph, number_of_nodes, s, t, TRUE);

}

// The main algorithm of part I (Dijkstra implementation of BGP)
// Gets the type of connections established by BGP
// COMPLEXITY: O(|V|+|E|)
int* dijkstra(node** graph,int number_of_nodes, int s, int t, int verbose)
{
    int* a=(int*)malloc(sizeof(int)*number_of_nodes);
    int u = 0;

    Queue qP = createQueue(); //Provider type nodes
    Queue qR = createQueue(); //Peer type nodes
    Queue qC = createQueue(); //Customer type nodes
    Queue qI = createQueue(); //Invalid type nodes

    // Initialize node's type vector
    for (int i = 0; i < number_of_nodes; ++i)
        a[i] = INT_MAX;

    // Make type value of src vertex
    // as 0 since it's a special node
    // which doesn't have any type
    a[t] = 0;

    // Inserts the source on Customer queue
    // in order to be extracted as soon as possible
    enQueue(qC, t);

    // Stops when any of the queues remains not empty
    // Checks the queues by order of preference (Total Order)
    // Extracts node being analysed from the corresponding queue
    while (!isQueueEmpty(qP) || !isQueueEmpty(qR) || !isQueueEmpty(qC) || !isQueueEmpty(qI))
    {
        if (!isQueueEmpty(qC))
          u = deQueue(qC);

        else if (!isQueueEmpty(qR))
          u = deQueue(qR);

        else if (!isQueueEmpty(qP))
          u = deQueue(qP);

        else
          u = deQueue(qI);

        // Checks the neighbours of the principal node
        node* aux = graph[u];
		    while (aux != NULL)
        {
            int v = aux->id; // Current neighbour

            // If new connection type is prefered to the current neighbours type
            if (a[v] > extension(invert(aux->type), a[u])) //VERIFICAR SE NÓ V JÁ ESTÁ ENCERRADO
            {
				        // Updates the neighbour type
                a[v] = extension(invert(aux->type), a[u]);

                // Inserts neighbour in the corresponding queue
                if (a[v] == 1)
                  enQueue(qC, v);

                else if (a[v] == 2)
                  enQueue(qR, v);

                else if (a[v] == 3)
                  enQueue(qP, v);

                else
                  enQueue(qI, v);
            }
            aux = aux->next;
        }
    }

    // Prints the final type elected by each node
    // In case of being asked for a specific source:
    // Prints the type elected by the source
    if(verbose)
    {
      if (a[s] == 1)
        printf("BGP Type of Path: Client\n");

      if (a[s] == 2)
        printf("BGP Type of Path: Peer\n");

      if (a[s] == 3)
        printf("BGP Type of Path: Provider\n");

      if (a[s] == 4)
        printf("BGP Type of Path: Invalid\n");

      if(a[s] == INT_MAX)
        printf("There's no path between %d and %d\n", s, t);

      if (a[s] == 0)
        printf("Seriously?\n");
    }

    return a;
}

// Necessary function in order to well-define the extension result
// given the type of connection is seen from the point of view
// from the principal node and not the current neighbour which is
// the node being proposed to be updated
int invert(int type)
{
  if (type == 1)
    return 3;

  if (type == 3)
    return 1;

  else
    return type;
}

// Provides the result of the extension between two types of connection types
// Ensures the commercially connection whenever it is possible
int extension(int entry1, int entry2)
{
  if (entry2 == 0)
    return entry1;

  if (entry1 == 1 && entry2 == 1)
    return 1;

  if (entry1 == 2 && entry2 == 1)
    return 2;

  if ((entry1 == 3 && entry2 == 1) || (entry1 == 3 && entry2 == 2) || (entry1 == 3 && entry2 == 3))
    return 3;

  else
    return 4;
}


//Reads the source and destination provided by the user
//Checks if it was given properly
//Depending on the property of the graph being commercially connected or not
//it decides if it calls the Dijkstra algorithm right away or not
int extraxct_user_info(node** graph, int number_of_nodes, char* str, int CC_flag)
{
  char* token;
  const char s[2] = "-";
  int source, destination, k = 0;

  source = atoi(strtok(str, s)); //stores source node ID
  destination = atoi(strtok(NULL, s)); //stores destination node ID

  while (graph[k] == NULL) //searches for the lowest ID value
    k++;

  //Verifies if the provided ID's are valid ID's
  if (source >= number_of_nodes || destination >= number_of_nodes || source == destination || source < k || destination < k)
    printf("\nPay attention...\n");

  //In case of being Commercially Connected
  else if (CC_flag == 1)
    dijkstraBGP(graph, number_of_nodes, source, destination);

  //In case of not being Commercially Connected
  else
    dijkstra(graph, number_of_nodes, source, destination, TRUE);

  return 1;
}


// Prints the PDF (Probability Density Function)
// of the types of paths from every source to every destination
// COMPLEXITY: O(|V|x(|E|+|V|))
void TypeOfBGP_PDF(node** graph, int number_of_nodes)
{
  //stores the frequency of each path type
  //the path types can only take values
  //1 (customer), 2 (peer), 3 (provider), 4 (invalid)
  int* histogram = malloc(sizeof(int)*5);

  // histogram initialization
  for(int i = 0; i < 5; i++)
    histogram[i] = 0;


  printf("Progress: 0%%\n");
  for(int dst = 0; dst < number_of_nodes; dst++) //for each destination node
  {
    if(graph[dst] == NULL)
      continue;

    int status = (int)((float)dst/number_of_nodes * 100);
    if(dst % (number_of_nodes/100+1) == 0)
      printf("Progress: %d%%\n",status);

    //BGP path types from each node to destination dst
    int* path_types = dijkstra(graph, number_of_nodes, 0, dst, FALSE);

    for(int src = 0; src < number_of_nodes; src++) //for each src node
    {
      if(graph[src] == NULL || src==dst)
        continue;

      if(path_types[src] != INT_MAX)
          histogram[path_types[src]]++;
    }

    free(path_types);
  }

  printf("Progress: 100%%\n");

  //print the PDF
  printPDF(histogram);
  free(histogram);
}

// Prints the PDF given a provided histogram
void printPDF(int* histogram)
{
  int i, sum = 0;

  for (i = 1; i <= 4; i++)
    sum += histogram[i];

  printf("\nPDF\n\n");

  for (i = 1; i <= 4; i++)
  {
    if (i == 1)
      printf("(Client's Type:   %f)\n", (float)histogram[i]/sum);

    if (i == 2)
      printf("(Peer's Type:     %f)\n", (float)histogram[i]/sum);

    if (i == 3)
      printf("(Provider's Type: %f)\n", (float)histogram[i]/sum);

    if (i == 4)
      printf("(Invalid's Type:  %f)\n", (float)histogram[i]/sum);
  }
}


//------------------- ALGORITHM II ------------------------------------------

//---------------------------------------------------------------------------

//prints the CCDF(complementay cumulative distribution function)
//of the length of the path computed by BGP over all source-destination
//pairs in an internet
//COMPLEXITY: O( |V| (|E|+|V|log|V|) )
void LengthOfBGPCCDF(node** graph, int number_of_nodes)
{
  //stores the frequency of each path length (in this type of graph, the length is not bigger than |V|)
  int* histogram = malloc(sizeof(int)*(number_of_nodes));
  for(int i = 0; i<number_of_nodes; i++)
  {
    histogram[i] = 0;
  }
  int true_number_of_nodes = 0; //actual number of nodes in the graph (those that have at least one edge)

  printf("Progress: 0%%\n");
  for(int dst = 0; dst < number_of_nodes; dst++) //for each destination node
  {
    if(graph[dst] == NULL)
    {
      continue;
    }

    histogram[0]++; //path from dst to dst has length 0
    true_number_of_nodes++;

    int status = (int)((float)dst/number_of_nodes * 100);
    if( dst % (number_of_nodes/100+1) == 0)
      printf("Progress: %d%%\n",status);


    //length of the path computed by BGP, from every node to dst
    int* distance = lengthPathsBGP(graph, number_of_nodes, dst);

    for(int src = 0; src < number_of_nodes; src++) //for each src node
    {
      if(graph[src] == NULL || src==dst)
        continue;

      if(distance[src] != INT_MAX)  //update the distances in the histogram
          histogram[distance[src]]++;


    }
    free(distance);
  }
  //print the CCDF
  printCCDF(histogram,number_of_nodes,true_number_of_nodes*true_number_of_nodes);
  free(histogram);

}

// The main function that calulates
// distances of BGP paths from all
// vertices to the destination 't'. It has a complexity of O((|E|+|V|)log|V|)
int* lengthPathsBGP(node** graph,int number_of_nodes, int t)
{
	int *length=(int*)(malloc(number_of_nodes*sizeof(int)));
    int a[number_of_nodes]; //path type
    int parent[number_of_nodes];
	int empty=0;
    MinHeap* minHeap = createMinHeap(number_of_nodes);

    // Initialize path type, length and minHeap vertices
    for (int i = 0; i < number_of_nodes; ++i)
    {
        a[i] = INT_MAX;
		length[i]=INT_MAX;
        parent[i]=-1;
        minHeap->array[i] = newMinHeapNode(i, INT_MAX);
        minHeap->pos[i] = i;
    }

    // Make type and length value of dest(t) vertex
    // as 0 so that it is extracted first
    a[t] = 0;
	length[t]=0;

    decreaseKey(minHeap, t, length[t]+a[t]*number_of_nodes);

    // Initially size of min heap
    minHeap->size = number_of_nodes;

    // In the followin loop,
    // min heap contains all nodes
    // whose shortest distance
    // is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with preferable
        // path and minimum length value
        MinHeapNode* minHeapNode = extractMin(minHeap);

        // Store the extracted vertex number
        int u = minHeapNode->v;
        free(minHeapNode);
        node* aux = graph[u];
		while (aux != NULL)
        {
            int v = aux->id;

            // If preferable path with minimum length to v is
            // not finalized yet, and path to v
            // through u is preferable than its
            // previously calculated path or equal
			// and has inferior length
            if ((isInMinHeap(minHeap, v)) && (a[u] != INT_MAX) && (length[u] != INT_MAX) && (a[v] >= extension(invert(aux->type), a[u])) && (extension(invert(aux->type), a[u])!=4)&& (length[v] > length[u]+1 || a[v] > extension(invert(aux->type), a[u])) && (parent[u] != v))
            {
                a[v] = extension(invert(aux->type), a[u]);
                parent[v] = u;
				length[v]=length[u]+1;
                // update preferable path with
                // minimum length in min heap
				decreaseKey(minHeap, v, length[v]+a[v]*number_of_nodes);

            }
            aux = aux->next;
        }
    }
    freeMinHeap(minHeap);
	return length;
}


//------------------- ALGORITHM III -----------------------------------------

//---------------------------------------------------------------------------
//Given a destination node 't', and an array 'path_types' with the type of BGP paths that each
//node takes to reach 't', this function calculates and returns an array with the length of
//the smaller path from each node to 't', that are of the same type as the BGP path.
//It uses the Dijkstra and has a complexity of O(|E|+|V|log|V|)
int* dijkstraMinimumPathsOfBGPTypeToDst(node** graph,int number_of_nodes, int t, int* path_types)
{
    int* dist = (int*)malloc(sizeof(int)*number_of_nodes); //distances of minumum type of type BGP from every node to 't'
    int min_dist_R[number_of_nodes]; //distances of minumum type of type PEER from every node to 't'
    int min_dist_P[number_of_nodes]; //distances of minumum type of type PROVIDER from every node to 't'
    int min_dist_C[number_of_nodes]; //distances of minumum type of type CUSTOMER from every node to 't'

    MinHeap* minHeap = createMinHeap(number_of_nodes);  //heap

    for (int i = 0; i < number_of_nodes; i++)
    {
        //all distances to infinity
        dist[i] = INT_MAX;
        min_dist_R[i] = INT_MAX;
        min_dist_P[i] = INT_MAX;
        min_dist_C[i] = INT_MAX;
        //initialize heap nodes
        minHeap->array[i] = newMinHeapNode(i, dist[i]);
        minHeap->pos[i] = i;
    }
    //destination distances are zero
    min_dist_R[t] = 0;
    min_dist_C[t] = 0;
    min_dist_P[t] = 0;
    dist[t]= 0;
    //update distance in heap
    decreaseKey(minHeap, t, dist[t]);

    // Initially size of min heap is equal to number of nodes
    minHeap->size = number_of_nodes;

    // In the followin loop,
    // min heap contains all nodes
    // whose distance
    // is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the node with
        // minimum distance value
        MinHeapNode* minHeapNode = extractMin(minHeap);

        // Store the extracted node number
        int u = minHeapNode->v;
        free(minHeapNode);

        node* aux = graph[u];
        while (aux != NULL)
        {
            int v = aux->id;

            //Calculate the updated distances from 'v' to 't' for each path type
            int updated_dist_C = min_dist_C[v];
            int updated_dist_P = min_dist_P[v];
            int updated_dist_R = min_dist_R[v];

            //two of the updated distances will remain the same, the only one that may
            //change will be the one corresponding to the type of the link v->u

            //if u is customer of v, the distance from v to t through a CUSTOMER
            //path will be updated to the distance from u to t through a CUSTOMER path plus one
            //(if it is smaller than the current value). Because in a commercial path, a customer link
            //can only be followed by a customer link
            if(invert(aux->type) == TYPE_CUSTOMER && min_dist_C[u] != INT_MAX)
              updated_dist_C = MIN(min_dist_C[v],min_dist_C[u]+1);

            //if u is peer of v, the distance from v to t through a PEER
            //path will be updated to the distance from u to t through a CUSTOMER path plus one
            //(if it is smaller than the current value). Because in a commercial path, a peer link
            //can only be followed by a customer link
            if(invert(aux->type) == TYPE_PEER && min_dist_C[u] != INT_MAX)
              updated_dist_R = MIN(min_dist_R[v],min_dist_C[u]+1);


            //if u is provider of v, the distance from v to t through a PROVIDER
            //path will be updated to the minimum distance from u to t through a pathof ANY kind plus one
            //(if it is smaller than the current value). Because in a commercial path, a provider link
            //can be followed by a link of any other kind
            if(invert(aux->type) == TYPE_PROVIDER)
            {
              int a = min_dist_C[u]+1; if(a<0) a = INT_MAX;
              int b = min_dist_R[u]+1; if(b<0) b = INT_MAX;
              int c = min_dist_P[u]+1; if(c<0) c = INT_MAX;

              int d = MIN(a,b);
              int e = MIN(c,min_dist_P[v]);

              updated_dist_P = MIN(d,e);
              //above code is same as: updated_dist_P = MIN(min_dist_C[u]+1, min_dist_R[u]+1, min_dist_P[u]+1, min_dist_P[v]);
            }

            //cond tests if the current distances can be improved
            int cond = FALSE;
            cond = (min_dist_P[v] > updated_dist_P) || (min_dist_C[v] > updated_dist_C) || (min_dist_R[v] > updated_dist_R);

            //if node v is in the heap and its distances are not yet minimum
            if ((isInMinHeap(minHeap, v)) && cond && (dist[u] != INT_MAX) )
            {
                //update distances (in practice, at most only one of them will change)
                min_dist_R[v] = updated_dist_R;
                min_dist_P[v] = updated_dist_P;
                min_dist_C[v] = updated_dist_C;

                //update the distance corresponding to the BGP type path
                if(path_types[v] == TYPE_PROVIDER)
                  dist[v] = min_dist_P[v];
                else if(path_types[v] == TYPE_PEER)
                  dist[v] = min_dist_R[v];
                else if(path_types[v] == TYPE_CUSTOMER)
                  dist[v] = min_dist_C[v];

                //update the distance in the heap
                decreaseKey(minHeap, v, dist[v]);
            }
            aux = aux->next;
          }

    }
    freeMinHeap(minHeap);
    return dist;

}


//prints the CCDF(complementay cumulative distribution function)
//of the length of the shortest path of the same type as that
//computed by BGP over all source-destination pairs in an internet
//COMPLEXITY: O( |V| (|E|+|V|log|V|) )
void MinimumPathsOfBGPTypeCCDF(node** graph, int number_of_nodes)
{
  //stores the frequency of each path length (in this type of graph, the length is not bigger than |V|)
  int* histogram = malloc(sizeof(int)*(number_of_nodes));
  for(int i = 0; i<number_of_nodes; i++)
  {
    histogram[i] = 0;
  }
  int true_number_of_nodes = 0; //actual number of nodes in the graph (those that have at least one edge)

  printf("Progress: 0%%\n");
  for(int dst = 0; dst < number_of_nodes; dst++) //for each destination node
  {
    if(graph[dst] == NULL)
    {
      continue;
    }

    histogram[0]++; //path from dst to dst has length 0
    true_number_of_nodes++;

    int status = (int)((float)dst/number_of_nodes * 100);
    if( dst % (number_of_nodes/100+1) == 0)
      printf("Progress: %d%%\n",status);

    //BGP path types from each node to dst
    int* path_types = dijkstra(graph,number_of_nodes,0,dst,FALSE);

    //length of the shortest path of the same type as that computed by BGP, from every node to dst
    int* distance = dijkstraMinimumPathsOfBGPTypeToDst(graph,number_of_nodes,dst,path_types);

    for(int src = 0; src < number_of_nodes; src++) //for each src node
    {
      if(graph[src] == NULL || src==dst)
        continue;

      if(distance[src] != INT_MAX)  //update the distances in the histogram
          histogram[distance[src]]++;


    }
    free(distance);
    free(path_types);
  }
  //print the CCDF
  printCCDF(histogram,number_of_nodes,true_number_of_nodes*true_number_of_nodes);
  free(histogram);

}


//prints the CCDF given an histogram
void printCCDF(int* histogram, int number_of_nodes, int number_of_pairs)
{
  int max_distance = 0;
  int number_infinite_distances=0;
  int number_finite_distances=histogram[0];
  for(int i = 1; i<number_of_nodes;i++)
  {
    number_finite_distances+=histogram[i];
    if(histogram[i-1] != 0 && histogram[i] == 0)
      max_distance = i-1;
  }
  number_infinite_distances = (number_of_pairs) - number_finite_distances;
  int* ccdf = (int*)malloc(sizeof(int)*(max_distance+1));
  for(int k = 0; k<=max_distance;k++)
  {
    ccdf[k] = number_infinite_distances;
  }

  for(int i = 0; i<=max_distance;i++)
  {
    for(int j = i; j<=max_distance;j++)
    {
      ccdf[i]+=histogram[j];
    }
  }

  printf("\nCCDF\n");
  for(int k = 0; k<=max_distance;k++)
  {
    printf("\n(%d: CCDF=%f histogram=%d)",k,(float)ccdf[k]/(number_of_pairs),histogram[k]);
  }
  printf("\n(%d...INFINITY: CCDF=%f histogram=0...%d)  ",max_distance+1,(float)number_infinite_distances/(number_of_pairs), number_infinite_distances);
  printf("\n");
  free(ccdf);
}


//------------------ GRAPH FUNCTIONS ----------------------------------------

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
node** importGraphFromFile(char* filename, int* number_of_nodes, int* CC_flag)
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

    // The general algorithm may consume too much time
    // In that case, by default, it assumes it is not Commercialy Connected
    if(isCommerciallyAcyclic(link, *number_of_nodes))
      *CC_flag = isCommerciallyConnected_Acyclic(link, *number_of_nodes);

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

//----------FROM--PROJECT--I-----------------------------
//
//
//-------------------------------------------------------

//determines if the grpah is commercialy acyclic using DFS algortihm
//returns the commercial cycles if not
//COMPLEXITY: O(|V|+|E|)
int isCommerciallyAcyclic(node** graph, int number_of_nodes)
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
    ret = TRUE;
  }

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

//determines if an acyclic graph is commercially connected
//COMPLEXITY: O(|V|+|E|)
int isCommerciallyConnected_Acyclic(node** graph, int number_of_nodes)
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
      ret = FALSE;
      break;
    }

  }

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
//----------------------------------------------------------------------
