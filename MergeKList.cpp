/*
 *  The complexity of the algorithm implemented is O(nlogk) which represents a family of curves.
 * If n is constant the complexity is O(k) ( because we have to build the heap, and the fastest method is O(n) using
 * the Heapify function) and if k is constant the complexity is O(n) ( each element has to be passed through and at some point
 * to be put in the final list)
 *
 *  There are two possible methods to solve this problem in O(nlogk):
 *    First is to merge the list in pairs T(n,k)=T(2n,k/2)+2n (every time the problem is split in half)
 *    O(nlogk) complexity based on operations but O(kn) additional space needed (we need to allocate always a new list of size nk, k=1...n)
 *
 *    Second method is to use a heap, because we always insert the minimum of the lists, so we always keep track of the minimum out of
 *    all the lists at their current indexes, and always move the index of the list after we insert the min in the final list.
 *    This method performs in O(nlogk) because each element has to be inserted in the heap and bubbled up, and we always delete the minimum and
 *    then heapify, both operations having a complexity of O(logk) applied for each of the n elements
 */
#include <stdlib.h>
#include <stdio.h>
#include  <time.h>
#include  "Profiler.h"
#include <time.h>

#define Max_Size 10000
#define Step_Size 100
#define Num_test 10

Profiler p("MergeLists");

typedef struct NodeLL{
    int key;
    struct NodeLL *next;
};
typedef struct List{
    int size;
    NodeLL *first;
    NodeLL *last;
};
typedef struct Heap{
    int key,listIndex;
};

int ops;

void Heapify(Heap *heap,int HeapSize,int index){
    Heap aux;
    while(index<=(HeapSize>>1)){
               ops+=2;
        if(heap[index].key<=heap[index<<1].key && (heap[index].key<=heap[(index<<1)+1].key || (index<<1)+1>HeapSize))
            index=HeapSize;
        else{
               ops+=2;
            if(heap[index].key>=heap[index<<1].key && (heap[(index<<1)+1].key>=heap[index<<1].key || (index<<1)+1>HeapSize)){
                aux=heap[index];
                heap[index]=heap[index<<1];
                heap[index<<1]=aux;
                   ops+=3;
                index<<=1;
            }
            else{
                aux=heap[index];
                heap[index]=heap[(index<<1)+1];
                heap[(index<<1)+1]=aux;
                    ops+=3;
                index=(index<<1)+1;
            }
        }
    }
}
void BubbleUpHeap(Heap *heap,int index){
    Heap aux;
    ops+=1;
    while(heap[index].key<heap[index>>1].key && (index^1)){
        aux=heap[index];
        heap[index]=heap[index>>1];
        heap[index>>1]=aux;
           ops+=3;
        index>>=1;
        ops+=1;
    }
}
void InsertInHeap(Heap *heap, int *HeapSize,int key,int listIndex){
    heap[++(*HeapSize)].key=key;
       ops++;
    heap[(*HeapSize)].listIndex=listIndex;
    BubbleUpHeap(heap,(*HeapSize));
}
void DeleteMinInHeap(Heap *heap,int *HeapSize){
    heap[1]=heap[(*HeapSize)--];
       ops++;
    Heapify(heap,(*HeapSize),1);
}
void BuildHeapBottomUp(Heap *heap,int HeapSize){
    for(int i=(HeapSize>>1);i>=1;i--)
        Heapify(heap,HeapSize,i);
}
void DeleteAtfront(List *Sentinel){
    if(NULL==Sentinel->first)
        return;

    NodeLL *temp=Sentinel->first;
    Sentinel->first=Sentinel->first->next;
    Sentinel->size--;
    free(temp);
}
void InsertAtEnd(List *Sentinel,int key){

    if(NULL==Sentinel->first){
        NodeLL *temp=(NodeLL *) malloc(sizeof(NodeLL));
        temp->next=NULL;
        temp->key=key;
        Sentinel->first=temp;
        Sentinel->last=temp;
        Sentinel->size++;
        return;
    }
    NodeLL *temp=(NodeLL *) malloc(sizeof(NodeLL));
    temp->next=NULL;
    temp->key=key;
    Sentinel->last->next=temp;
    Sentinel->last=temp;
    Sentinel->size++;
}
void PurgeList(List *Sentinel){
    NodeLL *node;
    while(NULL!=Sentinel->first){
        node=Sentinel->first;
        Sentinel->first=Sentinel->first->next;
        free(node);
    }
}
void PrintList(List *Sentinel){

    NodeLL *node=Sentinel->first;
    while(NULL!=node){
        printf("%d ",node->key);
        node=node->next;
    }
    printf("\n");
}
void PrintHeap(Heap *heap,int HeapSize){
    for(int i=1;i<=HeapSize;i++)
        printf("(%d,%d) ",heap[i].key,heap[i].listIndex);
    printf("\n");
}
List **GenerateRandomSortedLists(int k,int n){

   List **Sentinels=(List **) malloc(sizeof(List*)*k);

   for(int i=0;i<k;i++) {
       Sentinels[i] = (List *) malloc(sizeof(List));
       Sentinels[i]->size=0;
       Sentinels[i]->first=NULL;
       Sentinels[i]->last=NULL;
   }

   int cnt=0,atList=0,x;
   while(cnt<n){
       if(NULL==Sentinels[atList]->first)
           InsertAtEnd(Sentinels[atList],rand());
       else {
           while ((x=rand())<Sentinels[atList]->last->key);
           InsertAtEnd(Sentinels[atList],x);
       }
       atList++;
       cnt++;
       if(atList==k)
           atList=0;
   }

    return Sentinels;
}
void MergeTwoLists(){

    srand(time(NULL));
    int n=11;
    List **Sentinels= GenerateRandomSortedLists(2,n);
    printf("First list:");
    PrintList(Sentinels[0]);
    printf("Second list:");
    PrintList(Sentinels[1]);
    List *MergedLists=(List *) malloc(sizeof(List));
    MergedLists->size=0;
    MergedLists->first=NULL;
    MergedLists->last=NULL;
    while(NULL!=Sentinels[0]->first && NULL!=Sentinels[1]->first){
        if(Sentinels[0]->first->key>Sentinels[1]->first->key) {
            InsertAtEnd(MergedLists, Sentinels[1]->first->key);
            DeleteAtfront(Sentinels[1]);
        }
        else{
            InsertAtEnd(MergedLists, Sentinels[0]->first->key);
            DeleteAtfront(Sentinels[0]);
        }
    }

    while(NULL!=Sentinels[0]->first){
        InsertAtEnd(MergedLists, Sentinels[0]->first->key);
        DeleteAtfront(Sentinels[0]);
    }

    while(NULL!=Sentinels[1]->first){
        InsertAtEnd(MergedLists, Sentinels[1]->first->key);
        DeleteAtfront(Sentinels[1]);
    }
    printf("Merged lists:");
    PrintList(MergedLists);
    PurgeList(Sentinels[1]);
    PurgeList(Sentinels[0]);
    PurgeList(MergedLists);
    free(Sentinels);
    free(MergedLists);
}
void MergeKLists(int k,int n){

    //clock_t prog=clock();
    List **Sentinels= GenerateRandomSortedLists(k,n);
    Heap *heap=(Heap *)malloc(sizeof(Heap)*(k+1));
    //clock_t merge=clock();
    int HeapSize=k;
    for(int i=1;i<=k;i++) {
        heap[i].listIndex=i-1;
        heap[i].key = Sentinels[i-1]->first->key;
        ops+=1;
        printf("%d list:",i);
    PrintList(Sentinels[i-1]);
    }

    printf("Array before heapify:");
 PrintHeap(heap,HeapSize);

    for(int i=k/2;i>=1;i--)
        Heapify(heap,HeapSize,i);

  printf("Array after heapify:");
   PrintHeap(heap,HeapSize);

     List *MergedLists=(List *) malloc(sizeof(List));
    MergedLists->first=NULL;
    MergedLists->last=NULL;
    MergedLists->size=0;

    ops=0;
    int listIndex=-1;
    while(HeapSize^0){
        listIndex=heap[1].listIndex;
        ops++;
        InsertAtEnd(MergedLists,Sentinels[listIndex]->first->key);
        DeleteAtfront(Sentinels[listIndex]);

        if(NULL==Sentinels[listIndex]->first)
           listIndex=-1;

        DeleteMinInHeap(heap,&HeapSize);

        if(-1!=listIndex) {
            ops++;
            InsertInHeap(heap, &HeapSize, Sentinels[listIndex]->first->key, listIndex);
        }
    }
   free(heap);
  PrintList(MergedLists);
   for(int i=0;i<k;i++)
        PurgeList(Sentinels[i]);

    PurgeList(MergedLists);
    free(Sentinels);
    free(MergedLists);

   //clock_t end=clock();

   // printf("Generating:%lf \n Merging:%lf",(double)(end-prog),(double)(end-merge));
}
void GraphsKConst(){

    int k=5;


     for(int n=Step_Size;n<=Max_Size;n+=Step_Size) {
         Operation TotalOps = p.createOperation("Operations k=5", n);
         ops = 0;
         MergeKLists(k, n);
         TotalOps.count(ops);
     }

    k=10;


        for(int n=Step_Size;n<=Max_Size;n+=Step_Size) {
            Operation TotalOps = p.createOperation("Operations k=10", n);
            ops = 0;
            printf("---%d---\n",n);
            MergeKLists(k, n);
            TotalOps.count(ops);
        }

        k=100;

        for(int n=Step_Size;n<=10000;n+=Step_Size) {
            Operation TotalOps = p.createOperation("Operations k=100", n);
            ops = 0;
            MergeKLists(k, n);
            TotalOps.count(ops);
        }
     p.divideValues("Operations k=100",10);
     p.divideValues("Operations k=10",10);
     p.divideValues("Operations k=5",10);


    int n=10000;

        for(k=10;k<=500;k+=10){
            Operation TotalOps = p.createOperation("Operations n=10000", k);
            ops = 0;
            MergeKLists(k, n);
            TotalOps.count(ops);
        }

    p.divideValues("Operations n=10000",10);
    p.showReport();
}
int main() {

  /*  List *Sentinel=(List *) malloc(sizeof(List));
    Sentinel->size=0;
    Sentinel->first=NULL;
    Sentinel->last=NULL;

    Heap heap[20];
    int HeapSize=0;*/
    freopen("output.out","w",stdout);
    srand(time(NULL));
    printf("---MERGE TWO LISTS DEMO---\n");
    MergeTwoLists();
    printf("\n---MERGE LISTS DEMO---\n");
    MergeKLists(6,21);
   // GraphsKConst();

    return 0;
}
