/*
 * --QUICKSORT, FIRST ELEMENT AS PIVOT--
 *From the graphs we can see the behaviour like a quadratic function of the operations performed by the Quicksort algorithm,in the worst case.
 * In the best case it does a lot less comparisons, having the number of operations performed smaller by a magnitude, because
 * the array is always split in two balanced parts.
 * The worst case for Quicksort is when the array is sorted(ascending or descending).
 * The algorithm performs close to the best case,in the average case, in terms of the number of operations.
 * Quicksort is usually a good algorithm that can be used in practice(can be optimized to be in the best case always, using a random pivot,
 * median of medians pivot,or a one of these two with a combination of a direct sorting method for small slices), it is not optimal if the data
 * comes in mostly sorted(the number of inversion is small or big - close to 1 or n(n-1)/2 - ).
 * It outperforms Heapsort in the average case, although both have the complexity of θ(nlogn), Heapsort has a bigger constant because it has
 * to first turn the array into a min-heap ( max-heap).
 */
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#include <time.h>

#define MAX_SIZE 10000
#define NUM_TEST 20
#define StepSize 100
Profiler p("QuickSort");

void PrintHeap(int *Heap,int HeapSize){

    for(int i=1;i<=HeapSize;i++)
        printf("%d ",Heap[i]);
    printf("\n");
}
void PrintArray(int *A,int size){


    for(int i=0;i<size;i++)
        printf("%d ",A[i]);
    printf("\n");

}
void GenerateBest(int *BestArray,int *Array,int low,int high,int *sizebest){

    if(low<=high) {
        BestArray[(*sizebest)++] = Array[(low + high) / 2];

        if (low < high) {
            GenerateBest(BestArray, Array, low, (low + high) / 2 - 1, sizebest);
            GenerateBest(BestArray, Array, (low + high) / 2 + 1, high, sizebest);
        }
    }
}
int Partition(int *A,int low,int high,int piv,int size){
    Operation QsAssing=p.createOperation("QsAssign",size);
    Operation QsComp=p.createOperation("QsComp",size);
    int pivot=piv;
    int i=low;
    int j=high;
    int aux;
    while(i<j) {

        QsComp.count();
        while (A[i] <= A[pivot]) {
            i++;
            QsComp.count();
        }

        QsComp.count();
        while (A[j] > A[pivot]) {
            j--;
            QsComp.count();
        }

        if (i < j) {
            aux = A[i];
            A[i] = A[j];
            A[j] = aux;
            QsAssing.count(3);
            i++;
            j--;
        }
    }

    QsComp.count();
  if(A[j]>A[pivot])
        j--;

    aux=A[j];
    A[j]=A[pivot];
    A[pivot]=aux;
    QsAssing.count(3);

    return j;
}
void QuickSortv1(int *A,int low,int high,int size){

    if(low<high) {
        int pivot = Partition(A, low, high, low,size);
        QuickSortv1(A, low, pivot-1,size);
        QuickSortv1(A, pivot + 1, high,size);
    }

}
void AverageCase(){

    int Array[100000];
    for(int size=StepSize;size<=MAX_SIZE;size+=StepSize)
        for(int q=0;q<NUM_TEST;q++){
            FillRandomArray(Array,size,10,50000,false,UNSORTED);
            QuickSortv1(Array,0,size-1,size);
        }

    p.divideValues("QsComp",NUM_TEST);
    p.divideValues("QsAssign",NUM_TEST);
    p.addSeries("TotalQsort","QsAssign","QsComp");

}
void WorstCase(){

    p.reset();
    int Array[100000];
    for(int size=StepSize;size<=MAX_SIZE;size+=StepSize){
            FillRandomArray(Array,size,10,50000,false,ASCENDING);
            QuickSortv1(Array,0,size-1,size);
        }

    p.createGroup("WorstComp","QsComp");
    p.createGroup("WorstAssign","QsAssign");
    p.addSeries("WorstTotalQsort","QsAssign","QsComp");

}

void BestCase(){
    p.reset();
    int Array[100000],BestArray[100000];
    int sizebest=0;
    for(int size=StepSize;size<=MAX_SIZE;size+=StepSize){
        FillRandomArray(Array,size,10,50000,false,ASCENDING);
        sizebest=0;
        GenerateBest(BestArray,Array,0,size-1,&sizebest);
        QuickSortv1(BestArray,0,size-1,size);
    }
    p.createGroup("BestComp","QsComp");
    p.createGroup("BestAssign","QsAssign");
    p.addSeries("BestTotalQsort","QsAssign","QsComp");
}
void Heapify(int *Heap,int HeapSize,int index,int size){
    Operation HeapAssignment=p.createOperation("BottomUpAssign",size);
    Operation HeapComparison=p.createOperation("BottomUpComparison",size);

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
        Heapify(Heap,HeapSize,i,HeapSize);
}
void HeapSort(int *Heap,int HeapSize){

    int aux;
    for(int i=HeapSize;i>=2;i--){
        aux=Heap[1];
        Heap[1]=Heap[i];
        Heap[i]=aux;
        Heapify(Heap,i-1,1,HeapSize);
    }
}
void DemoQuickSort()
{

    int size=14;
    int *A=(int *)malloc(sizeof(int)*size);
    FillRandomArray(A,size,10,100,false,UNSORTED);
    printf("Inital array:");
    PrintArray(A,size);
    QuickSortv1(A,0,size-1,size);
    printf("Array after using quicksort:");
    PrintArray(A,size);
}
void DemoBuildHeap()
{
    int size=14;
    int *Heap=(int *)malloc(sizeof(int)*(size+1));
    FillRandomArray(Heap,size+1,10,100,false,UNSORTED);
    printf("Inital array:");
    PrintHeap(Heap,size);
    BuildHeapBottomUp(Heap,size);
    printf("Array after heapifying:");
    PrintHeap(Heap,size);
    HeapSort(Heap,size);
    printf("Array after using Heapsort:");
    PrintHeap(Heap,size);
}
void HeapsortVSQuicksort()
{
    p.reset();
    int Array[100000],Heap[100001];
    for(int size=StepSize;size<=MAX_SIZE;size+=StepSize)
        for(int q=0;q<NUM_TEST;q++){
            FillRandomArray(Array,size,10,50000,false,UNSORTED);

            for(int i=1;i<=size;i++)
                Heap[i]=Array[i-1];

            BuildHeapBottomUp(Heap,size);
            HeapSort(Heap,size);
            QuickSortv1(Array,0,size-1,size);
        }

    p.divideValues("BottomUpComparison",NUM_TEST);
    p.divideValues("BottomUpAssign",NUM_TEST);
    p.divideValues("QsAssign",NUM_TEST);
    p.divideValues("QsComp",NUM_TEST);
    p.addSeries("TotalHeapsort","BottomUpAssign","BottomUpComparison");
    p.addSeries("TotalQsort","QsAssign","QsComp");
    p.createGroup("Assignments","BottomUpAssign","QsAssign");
    p.createGroup("Comparisons","BottomUpComparison","QsComp");
    p.createGroup("Total","TotalQsort","TotalHeapsort");
}
void IterativeLinearInsertionSort(int *Array,int size){
    //Operation InsertionAssign=p.createOperation("IterativeInsertionAssign",size);
   // Operation InsertionComp=p.createOperation("IterativeInsertionComp",size);

    int aux,index;
    for(int i=0;i<size-1;i++){

        index=i;
        aux=Array[index+1];
       // InsertionAssign.count();
       // InsertionComp.count();
        while(index>=0 && Array[index]>aux){
            Array[index+1]=Array[index];
          //  InsertionAssign.count();
            index--;
          //  InsertionComp.count();
        }

        if(index!=i){
            Array[index+1]=aux;
           // InsertionAssign.count();
        }

    }
}
void RecursiveLinearInsertionSort(int *Array,int ind,int size,int *RInsertionAssign,int *RInsertionComp){


    if(ind==0)
        return;

    RecursiveLinearInsertionSort(Array,ind-1,size,RInsertionAssign,RInsertionComp);

    int index=ind-1;
    int aux=Array[ind];
   // (*RInsertionAssign)+=1;

  //  (*RInsertionComp)+=1;
    while(index>=0 && Array[index]>aux){
        Array[index+1]=Array[index];
      //  (*RInsertionAssign)+=1;
        index--;
       // (*RInsertionComp)+=1;
    }

   if(index!=ind-1){
       Array[index+1]=aux;
      // (*RInsertionAssign)+=1;
   }
}
void DemoInsertionSort(){

    int size=14;
    int *A=(int *)malloc(sizeof(int)*size);
    FillRandomArray(A,size,10,100,false,UNSORTED);
    printf("Inital array:");
    PrintArray(A,size);
    IterativeLinearInsertionSort(A,size);
    printf("Array after using linear insertion sort:");
    PrintArray(A,size);
    printf("\n");
    FillRandomArray(A,size,10,100,false,UNSORTED);
    printf("Inital array:");
    PrintArray(A,size);
    int t=0,z=0;
    RecursiveLinearInsertionSort(A,size-1,size,&t,&z);
    printf("Array after using recursive linear insertion sort:");
    PrintArray(A,size);


}
void IterativeVSRecursive(){
   p.reset();

    int Array[100000],B[100001];
   for(int size=StepSize;size<=MAX_SIZE;size+=StepSize) {
        Operation RInsertionAssign=p.createOperation("RecursiveInsertionAssign",size);
        Operation RInsertionComp=p.createOperation("RecursiveInsertionComp",size);
        for (int q = 0; q < NUM_TEST; q++) {
            FillRandomArray(Array,size,10,50000,false,UNSORTED);
            CopyArray(B,Array,size);
            IterativeLinearInsertionSort(Array,size);

            int Assign=0,Comps=0;
            RecursiveLinearInsertionSort(B,size-1,size,&Assign,&Comps);
            RInsertionComp.count(Comps);
            RInsertionAssign.count(Assign);
        }
    }

    p.divideValues("RecursiveInsertionAssign",NUM_TEST);
    p.divideValues("RecursiveInsertionComp",NUM_TEST);
    p.divideValues("IterativeInsertionAssign",NUM_TEST);
    p.divideValues("IterativeInsertionComp",NUM_TEST);
    p.addSeries("IterativeInsertiontotal","IterativeInsertionAssign","IterativeInsertionComp");
    p.addSeries("RecursiveInsertiontotal","RecursiveInsertionAssign","RecursiveInsertionComp");
    p.createGroup("Assignments","RecursiveInsertionAssign","IterativeInsertionAssign");
    p.createGroup("Comparisons","IterativeInsertionComp","RecursiveInsertionComp");
    p.createGroup("Total","IterativeInsertiontotal","RecursiveInsertiontotal");

    p.reset();

    int z=0,t=0;
    for(int size=StepSize;size<=MAX_SIZE;size+=StepSize) {
        p.startTimer("IterativeInsertionSort",size);
        for (int q = 0; q < 100; q++) {
            FillRandomArray(Array, size, 10, 50000, false, UNSORTED);
            IterativeLinearInsertionSort(Array, size);

        }
        p.stopTimer("IterativeInsertionSort",size);

        p.startTimer("RecursiveInsertionSort",size);
        for (int q = 0; q < 100; q++) {
            FillRandomArray(Array, size, 10, 50000, false, UNSORTED);
            z=0;
            t=0;
            RecursiveLinearInsertionSort(Array, size - 1, size,&z,&t);
        }
        p.stopTimer("RecursiveInsertionSort",size);
    }

    p.createGroup("ExecutionTime","RecursiveInsertionSort","IterativeInsertionSort");
    p.showReport();
}
int QuickSelectPartition(int *Array,int low,int high){
     int aux;
     int pivot=Array[low];
     int i=low+1;
     int j=high;
     while(i<=j){

         while(i<=high && Array[i]<=pivot)
             i++;
         while(j>=low && Array[j]>pivot)
             j--;

         if(i<j){
             aux=Array[i];
             Array[i]=Array[j];
             Array[j]=aux;
         }
     }

    aux=Array[low];
    Array[low]=Array[j];
    Array[j]=aux;

    return j;
}
int QuickSelect(int *Array,int low,int high,int  target,int size){


    if(low==high) return Array[low];
    if(low<high){

        int piv=rand()%(high-low)+low;
        int aux=Array[low];
        Array[low]=Array[piv];
        Array[piv]=aux;
        piv= QuickSelectPartition(Array,low,high);
        if(piv==target) return Array[piv];
        if(piv<target) return QuickSelect(Array,piv+1,high,target,size);
        return QuickSelect(Array,low,piv-1,target,size);
    }

}
void DemoQuickSelect(){
    int n,target;
    int *Array=(int *)malloc(sizeof(int)*n);
    int *BArray=(int *)malloc(sizeof(int)*n);
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&Array[i]);
        BArray[i]=Array[i];
    }
    scanf("%d",&target);
    PrintArray(Array,n);
    printf("rez=%d\n", QuickSelect(Array,0,n-1,target-1,n));
    PrintArray(Array,n);
    QuickSortv1(BArray,0,n-1,n);
    PrintArray(BArray,n);
}
int main() {

     freopen("algsort.in","r",stdin);
     freopen("algsort.out","w",stdout);

    srand(time(NULL));
      //AverageCase();
     //WorstCase();
    //BestCase();
   //HeapsortVSQuicksort();
   // IterativeVSRecursive();
    DemoQuickSort();
    printf("\n");
    DemoBuildHeap();
    printf("\n");
    DemoInsertionSort();
    printf("\n -- DEMO QUICKSELECT-- \n");
    DemoQuickSelect();



    return 0;
}

