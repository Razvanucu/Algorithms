/*
 *   DFS takes O(|E| + |V|)
 *
 *   Topological sort takes O(|E| + |V|) (using one DFS approach):
 *     In Topological sort, at the end when we finish visiting all neighbours of a current node
 *     we put it on the stack, so we can guarantee that it is placed after a node from which we visit the current one
 *
 *   Tarjan Algorithm for Strongly Connected Components takes O(|E| + |V|):
 *     We use a DFS, update each node's index and low_link fields to the current index,
 *     then we start visiting all neighbours. If one is already visited, but it is on the stack
 *     we update the node's low_link, from which we are visiting, with the minimum of itself and the neighbour's index
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler p("DFS");
enum Colour{White,Gray,Black};
typedef struct Node{
    int key;
    Colour colour;
    int discovery_time,finish_time;
    Node *parent;

    int index;
    int comp;
    int low_link;
    bool onStack;

    int adjListSize;
    Node **adjList;
};
typedef struct Graph{
    int graph_size;
    Node **vertices;
};

int timeDFS=0,numCC=0,cnt=0,indexDFS=0,numSCC=0,stack_end=0;
int ops=0;
int Minimum(int a,int b){
    return (a<b) ? a:b;
}
void DestroyGraph(Graph *graph){
    for(int i=0;i<graph->graph_size;i++){

        free(graph->vertices[i]->parent);
        graph->vertices[i]->parent=NULL;

        free(graph->vertices[i]->adjList);
        graph->vertices[i]->adjList=NULL;

        graph->vertices[i];
        free(graph->vertices[i]);
        graph->vertices[i]=NULL;
    }

    free(graph->vertices);
    graph->vertices=NULL;
}
void DFS_VISIT(Node *current_node,int space,Node **topSortNodes){

    for(int i=0;i<space;i++)
        printf("  ");
    printf("%d\n",current_node->key);

    timeDFS+=1;
    current_node->colour=Gray;
    current_node->discovery_time=timeDFS;
        ops+=3; //Assignments

    for(int i=0;i<current_node->adjListSize;i++) {
             ops+=1;//Comparison
        if (current_node->adjList[i]->colour == White) {
            DFS_VISIT(current_node->adjList[i], space + 1, topSortNodes);
        }
    }

    current_node->colour=Black;
    timeDFS+=1;
    current_node->finish_time=timeDFS;
       ops+=3;//Assignments

    if(NULL!=topSortNodes)
        topSortNodes[--cnt]=current_node;
}
void DFS(Graph *graph,Node **topSortNodes){

    for(int i=0;i<graph->graph_size;i++){
        graph->vertices[i]->colour=White;
        graph->vertices[i]->discovery_time=0;
        graph->vertices[i]->finish_time=0;
        graph->vertices[i]->parent=NULL;
           ops+=4;//Assignments
    }

    timeDFS=0;
    numCC=0;
    cnt=graph->graph_size;
          ops+=3;//Assignments

    for(int i=0;i<graph->graph_size;i++) {
            ops+=1;//comparison
        if (graph->vertices[i]->colour == White) {
            DFS_VISIT(graph->vertices[i], 0, topSortNodes);
            numCC++;
        }
    }

}
void TopologicalSort(Graph *graph){

    Node **topSortNodes=(Node **) malloc(sizeof(Node *)*graph->graph_size);

    DFS(graph,topSortNodes);
    printf("Array of sorted nodes using topological sort:");
    for(int i=0;i<graph->graph_size;i++)
        printf("%d ",topSortNodes[i]->key);
    printf("\n");

    free(topSortNodes);
}
void StrongConnect(Node *current_node,Node **Stack,Node ***SCC,int *SCCsize){
  current_node->index=indexDFS;
  current_node->low_link=indexDFS;
    indexDFS+=1;

  Stack[stack_end++]=current_node;
  current_node->onStack= true;

  for(int i=0;i<current_node->adjListSize;i++) {
      if (current_node->adjList[i]->index == -1) {
          StrongConnect(current_node->adjList[i], Stack,SCC,SCCsize);
          current_node->low_link = Minimum(current_node->low_link, current_node->adjList[i]->low_link);
      } else if (current_node->adjList[i]->onStack)
          current_node->low_link = Minimum(current_node->low_link, current_node->adjList[i]->index);
  }

  if(current_node->low_link==current_node->index){
      cnt=0;
      numSCC+=1;
      while(stack_end>0 && current_node!=Stack[stack_end-1]){
              SCC[numSCC-1][cnt++]=Stack[stack_end-1];
              Stack[stack_end - 1]->onStack = false;
              Stack[stack_end - 1]->comp = numSCC;
              stack_end--;
      }

      if(current_node==Stack[stack_end-1]){
          SCC[numSCC-1][cnt++]=Stack[stack_end-1];
          Stack[stack_end - 1]->onStack = false;
          Stack[stack_end - 1]->comp = numSCC;
          stack_end--;
      }
      SCC[numSCC-1]=(Node **) realloc(SCC[numSCC-1],cnt* sizeof(Node *));
      SCCsize[numSCC-1]=cnt;
  }
}
void Tarjan(Graph *graph){
    indexDFS=0;
    Node **Stack=(Node **) malloc(sizeof(Node *)*graph->graph_size);
    Node ***SCC=(Node ***) malloc(sizeof(Node **)*graph->graph_size);
    int *SCCsize=(int *) malloc(sizeof(int)*graph->graph_size);

    for(int i=0;i<graph->graph_size;i++)
        SCC[i]=(Node **) malloc(sizeof(Node *)*graph->graph_size);

    numSCC=0;
    stack_end=0;

    for(int i=0;i<graph->graph_size;i++){
        graph->vertices[i]->index=-1;
        graph->vertices[i]->onStack=false;
        graph->vertices[i]->comp=0;
        graph->vertices[i]->low_link=-1;
    }

    for(int i=0;i<graph->graph_size;i++)
        if(graph->vertices[i]->index==-1){
            StrongConnect(graph->vertices[i],Stack,SCC,SCCsize);
        }

    printf("Number of Strongly Connected Components:%d\n",numSCC);
    printf("Strongly connected components:\n");
    SCC=(Node ***) realloc(SCC,numSCC* sizeof(Node**));
    SCCsize=(int *) realloc(SCCsize,numSCC* sizeof(int));
    for(int i=0;i<numSCC;i++){
        printf("%d:",i+1);
        for(int j=0;j<SCCsize[i];j++)
            printf("%d ",SCC[i][j]->key);
        printf("\n");
    }

    for(int i=0;i<numSCC;i++)
        free(SCC[i]);

    free(SCC);
    free(SCCsize);
    free(Stack);
}
void DemoDFS(){

    printf("---DEMO DFS---\n");
    Graph graph;
    int n=5,m=5,x,y;


    graph.graph_size=n;
    graph.vertices=(Node **) malloc(sizeof(Node *)*n);
    for(int i=0;i<graph.graph_size;i++) {
        graph.vertices[i] = (Node *) malloc(sizeof(Node));
        graph.vertices[i]->adjListSize=0;
        graph.vertices[i]->key=i+1;
        graph.vertices[i]->adjList=(Node **) malloc(sizeof(Node *));
    }

    int arr1[5]={1,2,2,4,5};
    int arr2[5]={2,3,4,5,3};

    for(int i=0;i<m;i++){
        graph.vertices[arr1[i]-1]->adjList=(Node **) realloc(graph.vertices[arr1[i]-1]->adjList, sizeof(Node *)*(graph.vertices[arr1[i]-1]->adjListSize+1));
        graph.vertices[arr1[i]-1]->adjList[graph.vertices[arr1[i]-1]->adjListSize++]=graph.vertices[arr2[i]-1];
    }

    printf("Graph\n");
    for(int i=0;i<graph.graph_size;i++){
        printf("%d adjacency list:",graph.vertices[i]->key);
        for(int j=0;j<graph.vertices[i]->adjListSize;j++)
            printf("%d ",graph.vertices[i]->adjList[j]->key);

        printf("\n");
    }

    printf("DFS Tree:\n");
    DFS(&graph,NULL);
    DestroyGraph(&graph);
}
void DemoTopSort(){

    printf("\n---DEMO TOPOLOGICAL SORT---\n");
    Graph graph;
    int n=9,m=8,x,y;


    graph.graph_size=n;
    graph.vertices=(Node **) malloc(sizeof(Node *)*n);
    for(int i=0;i<graph.graph_size;i++) {
        graph.vertices[i] = (Node *) malloc(sizeof(Node));
        graph.vertices[i]->adjListSize=0;
        graph.vertices[i]->key=i+1;
        graph.vertices[i]->adjList=(Node **) malloc(sizeof(Node *));
    }

    int arr1[8]={1,1,3,3,5,4,4,4};
    int arr2[8]={2,3,4,5,9,6,7,8};

    for(int i=0;i<m;i++){
        graph.vertices[arr1[i]-1]->adjList=(Node **) realloc(graph.vertices[arr1[i]-1]->adjList, sizeof(Node *)*(graph.vertices[arr1[i]-1]->adjListSize+1));
        graph.vertices[arr1[i]-1]->adjList[graph.vertices[arr1[i]-1]->adjListSize++]=graph.vertices[arr2[i]-1];
    }

    printf("Graph\n");
    for(int i=0;i<graph.graph_size;i++){
        printf("%d adjacency list:",graph.vertices[i]->key);
        for(int j=0;j<graph.vertices[i]->adjListSize;j++)
            printf("%d ",graph.vertices[i]->adjList[j]->key);

        printf("\n");
    }

    printf("DFS Tree:\n");
    TopologicalSort(&graph);
    DestroyGraph(&graph);

}
void DemoTarjan(){
    printf("\n---DEMO TARJAN SCC---\n");
    Graph graph;
    int n=8,m=12,x,y;

    graph.graph_size=n;
    graph.vertices=(Node **) malloc(sizeof(Node *)*n);
    for(int i=0;i<graph.graph_size;i++) {
        graph.vertices[i] = (Node *) malloc(sizeof(Node));
        graph.vertices[i]->adjListSize=0;
        graph.vertices[i]->key=i+1;
        graph.vertices[i]->adjList=(Node **) malloc(sizeof(Node *));
        graph.vertices[i]->low_link=0;
        graph.vertices[i]->index=0;
        graph.vertices[i]->onStack=false;
    }

    int arr1[12]={1,2,6,7,3,3,2,4,5,6,5,8};
    int arr2[12]={2,6,7,6,1,4,3,5,4,5,8,7};

    for(int i=0;i<m;i++){
        graph.vertices[arr1[i]-1]->adjList=(Node **) realloc(graph.vertices[arr1[i]-1]->adjList, sizeof(Node *)*(graph.vertices[arr1[i]-1]->adjListSize+1));
        graph.vertices[arr1[i]-1]->adjList[graph.vertices[arr1[i]-1]->adjListSize++]=graph.vertices[arr2[i]-1];
    }

    printf("Graph\n");
    for(int i=0;i<graph.graph_size;i++){
        printf("%d adjacency list:",graph.vertices[i]->key);
        for(int j=0;j<graph.vertices[i]->adjListSize;j++)
            printf("%d ",graph.vertices[i]->adjList[j]->key);

        printf("\n");
    }

    Tarjan(&graph);
    DestroyGraph(&graph);
}
void Performance(){

    Graph graph;
    int total_edges;
    int num_nodes,num_edges;
    int i,j;
    bool isNeighb;
    for(int test=0;test<100;test++) {
        for (num_edges = 1000; num_edges <= 4500; num_edges += 100) {

            Operation OpEdges = p.createOperation("DFS_Operations_Edge_Vary", num_edges);
            total_edges = 0;
            num_nodes = 100;
            graph.graph_size = num_nodes;
            graph.vertices = (Node **) malloc(sizeof(Node *) * num_nodes);
            for (int i = 0; i < graph.graph_size; i++) {
                graph.vertices[i] = (Node *) malloc(sizeof(Node));
                graph.vertices[i]->adjListSize = 0;
                graph.vertices[i]->key = i + 1;
                graph.vertices[i]->adjList = (Node **) malloc(sizeof(Node *));
            }

            while (total_edges < num_edges) {
                i = rand() % 100;
                j = rand() % 100;
                isNeighb = false;
                for (int k = 0; k < graph.vertices[i]->adjListSize; k++)
                    if (graph.vertices[j] == graph.vertices[i]->adjList[k])
                        isNeighb = true;

                if (false == isNeighb) {
                    graph.vertices[i]->adjList = (Node **) realloc(graph.vertices[i]->adjList, sizeof(Node *) *
                                                                                               (graph.vertices[i]->adjListSize +
                                                                                                1));
                    graph.vertices[i]->adjList[graph.vertices[i]->adjListSize++] = graph.vertices[j];
                    total_edges++;
                }
            }

            ops = 0;
            DFS(&graph, NULL);
            DestroyGraph(&graph);
            OpEdges.count(ops);
        }
    }


    for(int test=0;test<100;test++) {
        for (num_nodes = 100; num_nodes <= 200; num_nodes += 10) {

            Operation OpEdges = p.createOperation("DFS_Operations_Nodes_Vary", num_nodes);
            total_edges = 0;
            num_edges = 4500;
            graph.graph_size = num_nodes;
            graph.vertices = (Node **) malloc(sizeof(Node *) * num_nodes);
            for (int i = 0; i < graph.graph_size; i++) {
                graph.vertices[i] = (Node *) malloc(sizeof(Node));
                graph.vertices[i]->adjListSize = 0;
                graph.vertices[i]->key = i + 1;
                graph.vertices[i]->adjList = (Node **) malloc(sizeof(Node *));
            }

            while (total_edges < num_edges) {
                i = rand() % num_nodes;
                j = rand() % num_nodes;
                isNeighb = false;
                for (int k = 0; k < graph.vertices[i]->adjListSize; k++)
                    if (graph.vertices[j] == graph.vertices[i]->adjList[k])
                        isNeighb = true;

                if (false == isNeighb) {
                    graph.vertices[i]->adjList = (Node **) realloc(graph.vertices[i]->adjList, sizeof(Node *) *
                                                                                               (graph.vertices[i]->adjListSize +
                                                                                                1));
                    graph.vertices[i]->adjList[graph.vertices[i]->adjListSize++] = graph.vertices[j];
                    total_edges++;
                }
            }

            ops = 0;
            DFS(&graph, NULL);
            DestroyGraph(&graph);
            OpEdges.count(ops);
        }
    }

    p.divideValues("DFS_Operations_Nodes_Vary",100);
    p.divideValues("DFS_Operations_Edge_Vary",100);

    p.showReport();

}
int main() {

    freopen("dfs.in","r",stdin);
    freopen("dfs.out","w",stdout);

    DemoDFS();
    DemoTopSort();
    DemoTarjan();

//Performance();


    return 0;
}
