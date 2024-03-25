/*
 *From the graphs we conclude, that the bottom-up approach is the best for building the heap. It has a better performance
 * than the top-down
 **/
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define  NUM_TEST 10
#define  MAX_SIZE 100000
#define  STEP_SIZE 100
enum Bool{False,True};
Profiler p("Heap");
void Heapify(int *Heap,int HeapSize,int index){
    Operation HeapAssignment=p.createOperation("BottomUpAssign",HeapSize);
    Operation HeapComparison=p.createOperation("BottomUpComparison",HeapSize);

    int aux;
    while(index<=HeapSize/2){
            HeapComparison.count(2);
        if(Heap[index]>=Heap[index*2] && (Heap[index]>=Heap[index*2+1] || index*2+1>HeapSize)){
            index=HeapSize;
        }
        else{
                  HeapComparison.count(2);
            if(Heap[index*2]>=Heap[index] && (Heap[index*2]>=Heap[index*2+1] || index*2+1>HeapSize) ) {
                aux=Heap[index];
                Heap[index]=Heap[index*2];
                Heap[index*2]=aux;
                   HeapAssignment.count(3);
                index*=2;
            }
            else {
                aux=Heap[index];
                Heap[index]=Heap[index*2+1];
                Heap[index*2+1]=aux;
                  HeapAssignment.count(3);
                index = index * 2 + 1;
            }
        }
    } 
}
void BuildHeapBottomUp(int *Heap,int HeapSize){
    for(int i=HeapSize/2;i>=1;i--)
        Heapify(Heap,HeapSize,i);
}
void BuildHeapTopDown(int *Heap,int HeapSize){
    Operation HeapAssignment=p.createOperation("TopDownAssign",HeapSize);
    Operation HeapComparison=p.createOperation("TopDownComparison",HeapSize);

    int aux,index;
    for(int i=2;i<=HeapSize;i++){
          index=i;
                 HeapComparison.count();
          while(index>1 && Heap[index]>Heap[index/2]){
              aux=Heap[index];
              Heap[index]=Heap[index/2];
              Heap[index/2]=aux;
                 HeapAssignment.count(3);
              index=(index>>1);
                 HeapComparison.count();
          }
    }
}
Bool CheckHeapProperty(int *Heap,int HeapSize){

    for(int i=1;i<=HeapSize/2;i++)
        if(Heap[i]<Heap[i*2] || (Heap[i]<Heap[i*2+1] || i*2+1>HeapSize))
            return False;

    return True;
}
Bool CheckIsSorted(int *v,int size){

    for(int i=1;i<size;i++)
        if(v[i]>v[i+1])
            return False;

    return True;
}
void HeapSort(int *Heap,int HeapSize){

    int aux;
    for(int i=HeapSize;i>=2;i--){
        aux=Heap[1];
        Heap[1]=Heap[i];
        Heap[i]=aux;
        Heapify(Heap,i-1,1);
    }
}
void PrintHeap(int *Heap,int HeapSize){

    for(int i=1;i<=HeapSize;i++)
        printf("%d ",Heap[i]);
    printf("\n");
}
void AverageCase(){

  int heap1[MAX_SIZE+1],heap2[MAX_SIZE+1];
  for(int q=0;q<NUM_TEST;q++)
      for(int n=STEP_SIZE-1;n<MAX_SIZE;n+=STEP_SIZE){

          FillRandomArray(heap1,n+1,10,50000,false,UNSORTED);
          for(int i=1;i<=n;i++)
              heap2[i]=heap1[i];

          BuildHeapBottomUp(heap1,n);
          BuildHeapTopDown(heap2,n);
      }

  p.divideValues("BottomUpAssign",NUM_TEST);
  p.divideValues("BottomUpComparison",NUM_TEST);
  p.divideValues("TopDownAssign",NUM_TEST);
  p.divideValues("TopDownComparison",NUM_TEST);

  p.addSeries("TotalOperationsBottomUp","BottomUpAssign","BottomUpComparison");
  p.addSeries("TotalOperationsTopDown","TopDownAssign","TopDownComparison");

  p.createGroup("Assignments","BottomUpAssign","TopDownAssign");
  p.createGroup("Comparisons","BottomUpComparison","TopDownComparison");
  p.createGroup("Total","TotalOperationsBottomUp","TotalOperationsTopDown");

    p.reset();
}
void WorstCase(){


    int heap1[MAX_SIZE+1],heap2[MAX_SIZE+1];
        for(int n=STEP_SIZE-1;n<MAX_SIZE;n+=STEP_SIZE){

            FillRandomArray(heap1,n+1,10,50000,false,ASCENDING);
            for(int i=1;i<=n;i++)
                heap2[i]=heap1[i];

            BuildHeapBottomUp(heap1,n);
            BuildHeapTopDown(heap2,n);
        }
    p.addSeries("WorstTotalOperationsBottomUp","BottomUpAssign","BottomUpComparison");
    p.addSeries("WorstTotalOperationsTopDown","TopDownAssign","TopDownComparison");

    p.createGroup("WorstAssignments","BottomUpAssign","TopDownAssign");
    p.createGroup("WorstComparisons","BottomUpComparison","TopDownComparison");
    p.createGroup("WorstTotal","WorstTotalOperationsBottomUp","WorstTotalOperationsTopDown");

    p.showReport();

}
void demoHeapSort()
{
    int heap[26];
    FillRandomArray(heap,26,10,60,false,UNSORTED);
    printf("Array before heapify:");
    PrintHeap(heap,25);
    BuildHeapTopDown(heap,25);
    printf("Array after heapify:");
    PrintHeap(heap,25);
    if(True==CheckHeapProperty(heap,25))
        printf("This is a heap\n");
    else printf("This is not a heap\n");
    HeapSort(heap,25);
    printf("Sorted array using heapsort:");
    PrintHeap(heap,25);
    if(True== CheckIsSorted(heap,25))
        printf("This is sorted");

}
void demoBuildHeapBottomUp(){

    int heap[12];
    FillRandomArray(heap,12,10,71,false,UNSORTED);
    printf("Array before building heap using Bottom-Up approach:");
    PrintHeap(heap,11);
    BuildHeapBottomUp(heap,11);
    printf("Array after building heap:");
    PrintHeap(heap,11);
}
void demoBuildHeapTopDown(){

    int heap[12];
    FillRandomArray(heap,12,10,71,false,UNSORTED);
    printf("Array before building heap using Top-Down approach:");
    PrintHeap(heap,11);
    BuildHeapBottomUp(heap,11);
    printf("Array after building heap:");
    PrintHeap(heap,11);
}
int main() {

    freopen("output.out","w",stdout);
  // AverageCase();
   //WorstCase();
   printf(" BOTTOMUP\n");
   demoBuildHeapBottomUp();
    printf("\n TOPDOWN \n");
  demoBuildHeapTopDown();
    printf("\n HeapSort \n");
   demoHeapSort();
    return 0;
}
