/*
 *    Disjoint Sets:
 *
 *    MakeSet: Takes O(1) for each for an element
 *    FindSet: Takes O(log*(n)) log*(n) - inverse of Ackermann function - can be approximated to O(1)
 *    Union: Takes O(1) since it only has to connect the representatives;
 *
 *    Heuristics: 1)path compression/tree shrink:
 *                     After we find the representative of the set in which element x belong to,
 *                     we g back from x to the representative and make all the elements on the way
 *                     have the parent the representative of the set.
 *               2)union after rank:
 *                      When we unite two sets we want to connect them such that the tree that
 *                      has the rank smaller is connected to the tree that has the rank bigger, so
 *                      we don't have to change the rank of the bigger tree. If they are equal
 *                      we can connect them in any way, but the rank of the representative of the
 *                      resulting set increases by one.
 *
 *    Kruskal: Algorithm for determining minimum spanning tree, using a greedy approach, using disjoint sets :
 *          -sort edges based on cost in ascending order
 *          -take each edge, from the edge with the smallest cost, see if it's a frontier edge(the nodes are in different sets)
 *          -if it is a frontier edge, put the two nodes in the same set and add the edge to the solution
 *
 *          N-number of nodes,M-number of edges
 *          O(MlogM) - to sort edges
 *          O(N) - to make the sets of the N nodes
 *          O(Mlog*(N))- for each edge in the sorted list of edges, we have to call the FindSet and Union operations on disjoint sets
 *          final complexity O(MlogM+N+Mlog*(N))
 *          since the graphs in the analysis are connected and M=4*n -> final complexity O(NlogN+Nlog*(N)) -> O(NlogN)
 *
 *          To generate a connected graph with N nodes and 4*N edges, I used the following approach:
 *            1) I created two arrays, one to check if the node is connected to any node that is already connected with another node by an edge
 *            ,and the other array to generate the nodes in a random order so that i get a random graph each time
 *            2) I took the first node in the second array, set is as connected, then I took the next node
 *            connect it with the first one, set is as connected and then connect with other 3 random nodes, then from any node
 *            onward seek a node that is set as connected, if the edge already exists between them connect with other 4 random nodes,
 *            else with 3 random nodes
 *
 *
 *            Graph Analysis:
 *                   --MakeSet operation has O(N) in Kruskal algorithm
 *                   --FindSet operation has O(Nlog*(N))~O(N)
 *                   --Union operation has O(N)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NUM_TEST 10

Profiler p("DisjointSets");
typedef struct List{
    int key;
    int cost;
    struct List *next;
};
typedef struct Graph{
    int cost;
    int sizeGraph;
    struct List *neighbour_list;
};
typedef struct Edge{
    int x;
    int y;
    int cost;
};
int opCMakeSet,opAMakeSet,opAFindSet,opCFindSet,opAUnion,opCUnion;
void MakeSet(int x,int *p,int *rank){
    p[x]=x; //element is its own representative
    rank[x]=0;
    opAMakeSet+=2;
}
int FindSet(int x,int *p){

    int parent,node=x;
    opAFindSet+=1;
    opCFindSet+=1;
    while(p[node]!=node) {   //find representative
        node = p[node];
        opAFindSet+=1;
        opCFindSet+=1;
    }

    opCFindSet+=1;
    while(x!=node){    //shrink the tree
        parent=p[x];
        p[x]=node;
        x=parent;
        opAFindSet+=3;
        opCFindSet+=1;
    }

    return node;

}
void Union(int x,int y,int *p,int *rank){


 int I= FindSet(x,p); //Find representative of x
 int J= FindSet(y,p); //Find representative of y
    opAUnion+=2;

  opCUnion+=1;
 if(rank[I]>rank[J])     //Union after rank
     p[J]=I;
 else p[I]=J;
 opAUnion+=1;

 opCUnion+=1;
 if(rank[I]==rank[J]) {  //If they have the same rank, then the rank of the "tree" increases by one
     rank[J] = rank[J] + 1;
    opAUnion+=1;
 }

}
void PrintSets(int n,int *p){
    for(int i=0;i<n;i++)
        if (p[i] == i) {
            printf("%d:%d", i, i);
            for (int j = 0; j < n; j++)
                if (FindSet(i,p)== FindSet(j,p) && j != i)
                    printf(",%d", j);
            printf("\n");
        }
}
void DemoDisjointSet(){

    printf("------DEMO DISJOINT SETS--------\n");
    int n=10;
    int *p=(int *)malloc(sizeof(int)*n);
    int *rank=(int *) malloc(sizeof(int)*n);


    for(int i=0;i<n;i++)
        MakeSet(i,p,rank);


    printf("Initial sets\n");
    PrintSets(n,p);
    printf("Find set representative of %d:%d\n",2, FindSet(2,p));
    printf("Find set representative of %d:%d\n",4, FindSet(4,p));
    Union(0,3,p,rank);
    printf("------\n");
    printf("%d union %d\n",0,3);
    PrintSets(n,p);
    printf("Find set representative of %d:%d\n",0, FindSet(0,p));
    printf("Find set representative of %d:%d\n",1, FindSet(1,p));
    Union(1,2,p,rank);
    printf("------\n");
    printf("%d union %d\n",1,2);
    PrintSets(n,p);
    printf("Find set representative of %d:%d\n",1, FindSet(1,p));
    printf("Find set representative of %d:%d\n",5, FindSet(5,p));
    Union(3,6,p,rank);
    printf("------\n");
    printf("%d union %d\n",3,6);
    PrintSets(n,p);
    printf("Find set representative of %d:%d\n",6, FindSet(6,p));
    printf("Find set representative of %d:%d\n",7, FindSet(7,p));
    Union(2,8,p,rank);
    printf("------\n");
    printf("%d union %d\n",2,8);
    PrintSets(n,p);
    printf("Find set representative of %d:%d\n",1, FindSet(1,p));
    printf("Find set representative of %d:%d\n",8, FindSet(8,p));
    Union(3,9,p,rank);
    printf("------\n");
    printf("%d union %d\n",3,9);
    PrintSets(n,p);
    printf("Find set representative of %d:%d\n",0, FindSet(0,p));
    printf("Find set representative of %d:%d\n",9, FindSet(9,p));
    Union(1,4,p,rank);
    printf("------\n");
    printf("%d union %d\n",1,4);
    PrintSets(n,p);
    printf("Find set representative of %d:%d\n",7, FindSet(7,p));
    printf("Find set representative of %d:%d\n",4, FindSet(4,p));
    Union(4,9,p,rank);
    printf("------\n");
    printf("%d union %d\n",4,9);
    PrintSets(n,p);
    printf("Find set representative of %d:%d\n",4, FindSet(4,p));
    printf("Find set representative of %d:%d\n",9, FindSet(9,p));

    free(p);
    free(rank);
}
Graph *MakeGraph(int sizeGraph) {

    Graph *G = (Graph *) malloc(sizeof(Graph) * sizeGraph);
    G->sizeGraph=sizeGraph;
    G->cost=0;
    for (int i = 0; i < sizeGraph; i++)
        G[i].neighbour_list = NULL;

    return G;
}
List *FindInList(List *head,int y){
    if(NULL==head)
        return NULL;

    while(NULL!=head){
        if(head->key==y)
            return head;
        head=head->next;
    }

    return NULL;
}
void AddToListAtFront(List **head,int y,int cost){
     if(NULL==(*head)){
         (*head)=(List *) malloc(sizeof(List));
         (*head)->key=y;
         (*head)->cost=cost;
         (*head)->next=NULL;
         return;
     }

     List *temp=(List *) malloc(sizeof(List));
    temp->key=y;
    temp->cost=cost;
    temp->next=(*head);
    (*head)=temp;
}
void PurgeList(List **head){
    if(NULL==(*head))
        return;

    List *temp;
    while((*head)!=NULL){
        temp=(*head);
        (*head)=(*head)->next;
        free(temp);
        temp=NULL;
    }
}
void AddEdge(Graph *G,int x,int y,int cost){
    AddToListAtFront(&(G[x].neighbour_list),y,cost);
}
void PrintNeigbourList(List *head,int x){
    if(NULL==head)
        return;

    while(NULL!=head){
        printf("(%d,%d,%d) ",x,head->key,head->cost);
        head=head->next;
    }
    printf("\n");
}
void DestroyGraph(Graph *G,int sizeGraph){
    for(int i=0;i<sizeGraph;i++){
        PurgeList(&(G[i].neighbour_list));
    }
    free(G);
}
void AddToEdgeList(Edge *edgelist,int *size_edglelist,int x,int y,int cost){
    edgelist[(*size_edglelist)].x=x;
    edgelist[(*size_edglelist)].y=y;
    edgelist[(*size_edglelist)++].cost=cost;
}
void BinaryInsertionSort(Edge *v,int low_arr,int high_arr){
    int ind;
    Edge aux,elem;
    int low,high,mid;
    for(int i=low_arr;i<=high_arr;i++){
        low=0;
        high=i;
        elem=v[i];


        while(low<high){
            mid=low+(high-low)/2;

            if(v[mid].cost<=elem.cost)
                low=mid+1;
            else high=mid;
        }

        if(high!=i) {
            for (int j = i; j > high; j--) {
                v[j] = v[j - 1];
            }
            v[high] = elem;
        }
    }
}
int Partition(Edge *arr,int low,int high){
    Edge aux;
    int pivot=low;
    while(low<=high){

        while(arr[low].cost<arr[pivot].cost) {
            low++;
        }

        while(arr[high].cost>arr[pivot].cost) {
            high--;
        }

        if(low<high){
            aux=arr[low];
            arr[low]=arr[high];
            arr[high]=aux;
            low++;
            high--;
        }
        else break;
    }
    aux=arr[high];
    arr[high]=arr[pivot];
    arr[pivot]=aux;

    return high;
}
void MakeHeap(Edge *Heap,int sizeHeap,int curr_index){
    Edge aux;
    while(curr_index<sizeHeap/2){
        if(Heap[curr_index].cost>=Heap[2*curr_index+1].cost && (curr_index*2+2>sizeHeap || Heap[curr_index].cost>=Heap[curr_index*2+2].cost))
            return;

        if(Heap[2*curr_index+1].cost>=Heap[curr_index].cost && (curr_index*2+2>sizeHeap || Heap[2*curr_index+1].cost>=Heap[2*curr_index+2].cost)){
            aux=Heap[curr_index];
            Heap[curr_index]=Heap[2*curr_index+1];
            Heap[2*curr_index+1]=aux;
            curr_index=(curr_index<<1)+1;
        }
        else{
            aux=Heap[curr_index];
            Heap[curr_index]=Heap[2*curr_index+2];
            Heap[2*curr_index+2]=aux;
            curr_index=(curr_index<<1)+2;
        }
    }
}
void BuildHeapBottomUp(Edge *Heap,int startHeap,int stopHeap){
    for(int i=stopHeap/2;i>=startHeap;i--)
        MakeHeap(Heap,stopHeap,i);
}
void HeapSort(Edge *Heap,int startHeap,int stopHeap){

    BuildHeapBottomUp(Heap,startHeap,stopHeap);
    Edge aux;
    for(int i=startHeap;i<stopHeap-1;i++){
        aux=Heap[stopHeap-(i-startHeap)];
        Heap[stopHeap-(i-startHeap)]=Heap[startHeap];
        Heap[startHeap]=aux;
        MakeHeap(Heap,stopHeap-(i-startHeap)-1,startHeap);
    }
}
void IntroSort(Edge *arr,int low,int high,int threshold,int maxdepth){

    if((high-low)<threshold)
        BinaryInsertionSort(arr,low,high);
    else if(maxdepth==0)
        HeapSort(arr,low,high);
    else{
        int rand_index = rand() % (high - low) + low;
        Edge aux = arr[low];
        arr[low] = arr[rand_index];
        arr[rand_index] = aux;

        int p = Partition(arr, low, high);
        IntroSort(arr,low,p-1,threshold,maxdepth-1);
        IntroSort(arr,p+1,high,threshold,maxdepth-1);
    }
}
void Kruskal(Graph *G){

    int sizeEdgeArr=0;
    int curr_sizeEdgeArr=40001;
    Edge *edgeArr=(Edge *) malloc(sizeof(Edge)*curr_sizeEdgeArr);
    for(int i=0;i<G->sizeGraph;i++)
      for(int j=i+1;j<G->sizeGraph;j++)
          if(NULL!=FindInList(G[i].neighbour_list,j))
              AddToEdgeList(edgeArr,&sizeEdgeArr,i,j,FindInList(G[i].neighbour_list,j)->cost);

    int depth; //depth for introsort
    for(int i=sizeEdgeArr;i>0;i>>=1,depth++);

    printf("array of edges before sorting:");
    for(int i=0;i<sizeEdgeArr;i++)
           printf("(%d,%d,%d) ",edgeArr[i].x,edgeArr[i].y,edgeArr[i].cost);
    printf("\n");

    IntroSort(edgeArr,0,sizeEdgeArr-1,27,depth);

    printf("\n");
    printf("array of edges after sorting:");
    for(int i=0;i<sizeEdgeArr;i++)
        printf("(%d,%d,%d) ",edgeArr[i].x,edgeArr[i].y,edgeArr[i].cost);
    printf("\n");

    int *p=(int *) malloc(sizeof(int)*G->sizeGraph);
    int *rank=(int *) malloc(sizeof(int)*G->sizeGraph);

    for(int i=0;i<G->sizeGraph;i++)
       MakeSet(i,p,rank);

    Graph *MST= MakeGraph(G->sizeGraph);
    for(int i=0;i<sizeEdgeArr;i++) {
        if (FindSet(edgeArr[i].x, p) != FindSet(edgeArr[i].y, p)) {
            Union(edgeArr[i].x, edgeArr[i].y, p, rank);
            AddEdge(MST, edgeArr[i].x, edgeArr[i].y, edgeArr[i].cost);
            AddEdge(MST, edgeArr[i].y, edgeArr[i].x, edgeArr[i].cost);
            MST->cost+=edgeArr[i].cost;
        }
        else printf("%d-%d are already in the same set\n",edgeArr[i].x,edgeArr[i].y);
    }

    printf("\nMinimum spanning tree cost:%d\n",MST->cost);
    printf("Edges in MST(format (node,neighbour,cost)):\n");

    for(int i=0;i<MST->sizeGraph;i++)
        PrintNeigbourList(MST[i].neighbour_list,i);

    free(edgeArr);
    DestroyGraph(MST,MST->sizeGraph);
    free(p);
    free(rank);
}
void DemoKruskal(){

    printf("\n---DEMO KRUSKAL---\n");
    int n=4;
    Graph *G= MakeGraph(n);
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++){
            int cost=rand()%20;
            AddEdge(G,i,j,cost);
            AddEdge(G,j,i,cost);
        }

    printf("\nEDGES(format (node,neighbour,cost))\n");
    for(int i=0;i<n;i++) {
        printf("%d:",i);
        PrintNeigbourList(G[i].neighbour_list, i);
    }

    Kruskal(G);
    DestroyGraph(G,G->sizeGraph);
}
void AnalysisKruskal(){

    Graph *G=NULL;
    bool *isConnected=NULL;
     int *arr_nodes=NULL;

  for(int test=0;test<NUM_TEST;test++) {
      printf("---%d---\n",test);
      for (int n = STEP_SIZE; n <= MAX_SIZE; n+=STEP_SIZE) {
         printf("---%d---\n",n);
          Operation CompMakeSet = p.createOperation("Comparisons MakeSet", n);
          Operation AssignMakeSet = p.createOperation("Assignments MakeSet", n);
          Operation CompFindSet = p.createOperation("Comparisons FindSet", n);
          Operation AssignFindSet = p.createOperation("Assignments FindSet", n);
          Operation CompUnion = p.createOperation("Comparisons Union", n);
          Operation AssignUnion = p.createOperation("Assignments Union", n);
          Operation TotalOps=p.createOperation("TotalOperations",n);


          G = MakeGraph(n);
          arr_nodes = (int *) malloc(sizeof(int) * n);
          isConnected = (bool *) malloc(sizeof(bool) * n);

          for (int i = 0; i < n; i++)
              isConnected[i] = false;

          FillRandomArray(arr_nodes, n, 0, n - 1, true, UNSORTED);

          isConnected[arr_nodes[0]] = true;
          int j = rand() % n, cost, cnt=0;

          while (cnt < 4) {
              j = rand() % n;
              if (NULL == FindInList(G[arr_nodes[0]].neighbour_list, j) && j!=arr_nodes[0]) {
                  cost = rand() % 25 + 1;
                  AddEdge(G, arr_nodes[0], j, cost);
                  AddEdge(G, j, arr_nodes[0], cost);
                  isConnected[j]= true;
                  cnt++;
              }
          }

          for (int i = 1; i < n; i++) {

              cnt = 0;
              while (isConnected[j] == false) {
                  j = rand() % n;
              }

              if (NULL == FindInList(G[arr_nodes[i]].neighbour_list, j) && j!=arr_nodes[i]) {
                  cost = rand() % 25 + 1;
                  AddEdge(G, arr_nodes[i], j, cost);
                  AddEdge(G, j, arr_nodes[i], cost);
                  cnt++;
              }
              isConnected[arr_nodes[i]]= true;

              while (cnt < 4) {
                  j = rand() % n;
                  if (NULL == FindInList(G[arr_nodes[i]].neighbour_list, j) && j!=arr_nodes[i]) {
                      cost = rand() % 25 + 1;
                      AddEdge(G, arr_nodes[i], j, cost);
                      AddEdge(G, j, arr_nodes[i], cost);
                      isConnected[j]= true;
                      cnt++;
                  }
              }

          }

          opCFindSet=0;
          opAUnion=0;
          opAFindSet=0;
          opCUnion=0;
          opAMakeSet=0;
          opCMakeSet=0;

          Kruskal(G);
          CompFindSet.count(opCFindSet);
          AssignFindSet.count(opAFindSet);
          CompMakeSet.count(opCMakeSet);
          AssignMakeSet.count(opAMakeSet);
          CompUnion.count(opCUnion);
          AssignUnion.count(opAUnion);
          TotalOps.count(opAUnion+opAMakeSet+opAFindSet+opCFindSet+opCUnion+opCMakeSet);

          DestroyGraph(G, G->sizeGraph);
          free(arr_nodes);
          free(isConnected);
      }
  }
  p.divideValues("Comparisons MakeSet",NUM_TEST);
  p.divideValues("Assignments MakeSet",NUM_TEST);
  p.divideValues("Comparisons FindSet",NUM_TEST);
  p.divideValues("Assignments FindSet",NUM_TEST);
  p.divideValues("Comparisons Union",NUM_TEST);
  p.divideValues("Assignments Union",NUM_TEST);
  p.divideValues("TotalOperations",NUM_TEST);

  p.addSeries("Total Operations MakeSet","Comparisons MakeSet","Assignments MakeSet");
  p.addSeries("Total Operations FindSet","Comparisons FindSet","Assignments FindSet");
  p.addSeries("Total Operations Union","Comparisons Union","Assignments Union");

  p.showReport();
}
int main() {

    freopen("disjoint.in","r",stdin);
    freopen("disjoint.out","w",stdout);
    srand(time(NULL));
    DemoDisjointSet();
    DemoKruskal();
  //   AnalysisKruskal();
    return 0;
}
