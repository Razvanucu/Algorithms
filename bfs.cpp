#include <stdlib.h>
#include <string.h>
#include "bfs.h"

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
    int dx[4]={0,1,0,-1};
    int dy[4]={1,0,-1,0};
    int size=0;
    for(int direction=0;direction<4;direction++){
        int next_x=p.col+dx[direction];
        int next_y=p.row+dy[direction];

        if(next_x>=0 && next_x<grid->cols && next_y>=0 && next_y<grid->rows && grid->mat[next_y][next_x]==0){
            neighb[size].col=next_x;
            neighb[size++].row=next_y;
        }
    }

    return size;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{

    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    s->dist=0;
    s->color=COLOR_BLACK;
    s->parent=NULL;
    Node *queue[10001];
    int start_queue=0,end_queue=0;

    queue[end_queue++]=s;
    if(op != NULL) op->count();


    if(op != NULL) op->count();
    while(start_queue<end_queue){

        Node *curr_node=queue[start_queue++];
        if(op != NULL) op->count();
        //  printf("curr:%d %d\n",curr_node->position.row,curr_node->position.col);
        for(int i=0;i<curr_node->adjSize;i++) {
            Node *next_node=curr_node->adj[i];
            if(op != NULL) op->count();
            //    printf("next:%d %d\n",next_node->position.row,next_node->position.col);
            if(op != NULL) op->count();
            if(next_node->color!=COLOR_BLACK) {
                queue[end_queue++] = next_node;
                next_node->dist = curr_node->dist + 1;
                next_node->color = COLOR_BLACK;
                next_node->parent = curr_node;
                if(op != NULL) op->count(4);
            }
        }

    }

}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        int stack[100001];
        int spaces[100001];
        int end_stack=0;
        for(int i=0;i<n;i++)
            if(-1==p[i]){
                spaces[end_stack]=0;
                stack[end_stack++]=i;
                break;
            }

        while(0<end_stack){

            int node=stack[end_stack-1];
            int space=spaces[end_stack-1];
            end_stack--;

            for(int i=0;i<space;i++)
                printf("   ");
            printf("(%d,%d)\n",repr[node].row,repr[node].col);

            for(int i=0;i<n;i++)
                if(p[i]==node){
                    stack[end_stack]=i;
                    spaces[end_stack++]=space+1;
                }
        }
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    bfs(graph,start);
    int sizePath=end->dist;
    int cntSize=end->dist;
    Node *curr_node=end;
    while(NULL!=curr_node){
        path[--cntSize]=curr_node;
        curr_node=curr_node->parent;
    }

    return sizePath;
}


void performance()
{
    int n,i;
    Profiler p("bfs");

    srand(time(NULL));
    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        printf("---%d---\n",n);
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
            graph.v[i]->adj=(Node **) malloc(graph.nrNodes * sizeof(Node*));
            for(int k=0;k<graph.nrNodes;k++)
                graph.v[i]->adj[i]=(Node *) malloc(sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        int j,cnt,maxcnt=n/100,total_edges=0;
        bool isNeighb=false;
        bool *isConnected=(bool *) malloc(sizeof(bool)*100);
        int *arr_nodes=(int *) malloc(sizeof(int)*100);
        for(int i=0;i<100;i++)
            isConnected[i]=false;

        FillRandomArray(arr_nodes,100,0,99,true,UNSORTED);
        isConnected[arr_nodes[0]]=true;
        cnt=0;

        while(cnt<maxcnt){
            j=rand()%100;
            isNeighb=false;
            for(int k=0;k<graph.v[arr_nodes[0]]->adjSize;k++)
                if(graph.v[arr_nodes[0]]->adj[k]==graph.v[j])
                    isNeighb=true;

            if(!isNeighb && j!=arr_nodes[0]){
                graph.v[arr_nodes[0]]->adj[graph.v[arr_nodes[0]]->adjSize++]=graph.v[j];
                graph.v[j]->adj[graph.v[j]->adjSize++]=graph.v[arr_nodes[0]];
                cnt++;
                total_edges++;
                isConnected[j]=true;
            }
        }

        for(i=1;i<100;i++){
            cnt=0;
            j=rand()%100;
            while(isConnected[j]==false)
                j=rand()%100;

            isNeighb=false;
            for(int k=0;k<graph.v[arr_nodes[i]]->adjSize;k++)
                if(graph.v[arr_nodes[i]]->adj[k]==graph.v[j])
                    isNeighb=true;

            if(!isNeighb && j!=arr_nodes[i]){
                graph.v[arr_nodes[i]]->adj[graph.v[arr_nodes[i]]->adjSize++]=graph.v[j];
                graph.v[j]->adj[graph.v[j]->adjSize++]=graph.v[arr_nodes[i]];
                total_edges++;
                cnt++;
            }
            isConnected[arr_nodes[i]]=true;
        }

        while(total_edges<n){
            i=rand()%100;
            j=rand()%100;

            isNeighb=false;
            for(int k=0;k<graph.v[i]->adjSize;k++)
                if(graph.v[i]->adj[k]==graph.v[j])
                    isNeighb=true;

            if(!isNeighb && j!=i){
                graph.v[i]->adj[graph.v[i]->adjSize++]=graph.v[j];
                total_edges++;
                isConnected[j]=true;
            }
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
        free(isConnected);
        free(arr_nodes);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        printf("---%d---\n",n);
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
            graph.v[i]->adj=(Node **) malloc(graph.nrNodes * sizeof(Node*));
            for(int k=0;k<graph.nrNodes;k++)
                graph.v[i]->adj[i]=(Node *) malloc(sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        int j,cnt,maxcnt=4500/n,total_edges=0;
        bool isNeighb=false;
        bool *isConnected=(bool *) malloc(sizeof(bool)*n);
        int *arr_nodes=(int *) malloc(sizeof(int)*n);
        for(int i=0;i<n;i++)
            isConnected[i]=false;

        FillRandomArray(arr_nodes,n,0,n-1,true,UNSORTED);
        isConnected[arr_nodes[0]]=true;
        cnt=0;

        while(cnt<maxcnt){
            j=rand()%n;
            isNeighb=false;
            for(int k=0;k<graph.v[arr_nodes[0]]->adjSize;k++)
                if(graph.v[arr_nodes[0]]->adj[k]==graph.v[j])
                    isNeighb=true;

            if(!isNeighb && j!=arr_nodes[0]){
                graph.v[arr_nodes[0]]->adj[graph.v[arr_nodes[0]]->adjSize++]=graph.v[j];
                cnt++;
                isConnected[j]=true;
                total_edges++;
            }
        }

        for(i=1;i<n;i++){
            cnt=0;
            j=rand()%n;
            while(isConnected[j]==false)
                j=rand()%n;

            isNeighb=false;
            for(int k=0;k<graph.v[arr_nodes[i]]->adjSize;k++)
                if(graph.v[arr_nodes[i]]->adj[k]==graph.v[j])
                    isNeighb=true;

            if(!isNeighb && j!=arr_nodes[i]){
                graph.v[arr_nodes[i]]->adj[graph.v[arr_nodes[i]]->adjSize++]=graph.v[j];
                cnt++;
                total_edges++;
            }
            isConnected[j]=true;
        }

        while(total_edges<4500){
            i=rand()%n;
            j=rand()%n;

            isNeighb=false;
            for(int k=0;k<graph.v[i]->adjSize;k++)
                if(graph.v[i]->adj[k]==graph.v[j])
                    isNeighb=true;

            if(!isNeighb && j!=i){
                graph.v[i]->adj[graph.v[i]->adjSize++]=graph.v[j];
                total_edges++;
                isConnected[j]=true;
            }
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
        free(isConnected);
        free(arr_nodes);
    }

    p.showReport();
}
