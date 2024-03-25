/*
 *   Bubble Sort implementation:
 *     We consider the array sorted, then we start going through the array and check if adjacent elements are ordered. If they are nor
 *     swap them and mark the array as unsorted. We always decrease the search's range of the array when we finish searching because it is guaranteed
 *     that the maximum element, in the range, will be placed at the end of the range. If we haven't done any swaps, we stop because the
 *     array is sorted
 *
 *   Selection Sort implementation:
 *     We always look for the ith smallest element in the array i=0...size-1(because the maximum element will be in the right place).
 *     We consider the element on position i as being the ith smallest and search from i+1 to the end of the array for an element smaller than this one.
 *     If there is we store it's position and compare with the remaining elements in the array. At the end we swap the element from the ith position with
 *     the ith smallest(if there is one).
 *
 *    Insertion Sort implementation:
 *       We have a destination(part of the array) that is sorted and we want to place from the unsorted part each element in the rigt position of the
 *       destination. At the beggining the destination is 0, then we take the very next element outside of the destination's range and store it in x.
 *       We start from the end of the destination and shift all elements by one to the right, but only shift elements which are bigger then x.
 *       When we haven't done any shifts we stop and we put x in place.
 *
 *    Binary Insertion Sort implementation:
 *     Same as Insertion Sort, but instead of searching for the place linearly, we take advantage of the fact that the part of the array is sorted
 *     and use binary search to find the first position of the smallest of the elements bigger than x.
 *
 *     Graph Interpretetion:
 *       Average case: Although selection sort does very little assignments it still has to go through the array for each element
 *       and compare elements while both bubble and insertion stop once the array is sorted or when we find the right place to insert
 *       the element, thus making the number of comparisons significantly smaller.
 *
 *       Best Case: Bubble sort outperforms the other sorting methods because it goes once through the array and sees that it is
 *       sorted, while Insertion sort still has to check for every element and we have to store the element we want to insert.
 *       Selection sort performs almost the same but does no assignments.
 *
 *       Worst Case: For Bubble sort, the worst case is when the array is sorted in descending order(if we want it in ascending order).
 *       Insertion sort performs the best in the worst case and selection sort performs the same.
 *       From the graphs we can see that selection sort works pretty much the same for any case.
 *       Bubble sort is the best sorting algorithm when the array is sorted(almost sorted).
 *       Insertion sort is the best algorithm in average and worst case.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Profiler.h"
#include <time.h>

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NUM_TEST 5

Profiler p("DirectSortingMethods");
void PrintArray(int *v,unsigned int size){

    for(int i=0;i<size;i++)
        printf("%d ",v[i]);
    printf("\n");
}
void BubbleSort(int *v,unsigned int size){
    Operation Comparisons=p.createOperation("Bubblecomp",size);
    Operation Assignments=p.createOperation("Bubbleass",size);
    bool ok=true;
    int aux,j=0;
    while(ok){
        ok=false;
        for(unsigned int i=0;i<size-j-1;i++){
            Comparisons.count();
            if(v[i]>v[i+1]){
                aux=v[i];
                v[i]=v[i+1];
                v[i+1]=aux;
                Assignments.count(3);
                ok=true;
            }
        }
        j++;
    }
}
void SelectionSort(int *v,unsigned int size){
    Operation Comparisons=p.createOperation("Selectioncomp",size);
    Operation Assignments=p.createOperation("Selectionass",size);
      unsigned int minpos;
      int aux;
    for(unsigned int i=0;i<size;i++){
        minpos=i;
        for(unsigned int j=i+1;j<size;j++){
            Comparisons.count();
            if(v[j]<v[minpos])
             minpos=j;
        }

        if(minpos!=i) {
            aux = v[i];
            v[i] = v[minpos];
            v[minpos] = aux;
            Assignments.count(3);
        }
   }
}
void InsertionSort(int *v,unsigned int size){
    Operation Comparisons=p.createOperation("Insertioncomp",size);
    Operation Assignments=p.createOperation("Insertionass",size);
    int ind;
    int aux,elem;
    for(int i=0;i<size-1;i++){
        ind=i;
        elem=v[ind+1];
        Assignments.count();
        Comparisons.count();
        while(ind>=0 && v[ind]>elem){
            v[ind+1]=v[ind];
            Assignments.count();
            ind--;
            Comparisons.count();
        }

        if(ind!=i) {
            v[ind + 1] = elem;
            Assignments.count();
        }
    }
}
void BinaryInsertionSort(int *v,unsigned int size){
    //Operation Comparisons=p.createOperation("BinaryInsertioncomp",size);
  //  Operation Assignments=p.createOperation("BinaryInsertionass",size);
    int ind;
    int aux,elem,low,high,mid;
    for(int i=0;i<size;i++){
        low=0;
        high=i;
        elem=v[i];
      //  Assignments.count();
        while(low<high){
            mid=low+(high-low)/2;
            //Comparisons.count();
            if(v[mid]<=elem)
                low=mid+1;
            else high=mid;
        }

        if(high!=i) {
            for (int j = i; j > high; j--) {
                v[j] = v[j - 1];
              //  Assignments.count();
            }
            v[high] = elem;
          //  Assignments.count();
        }
    }
}
bool CheckIsSorted(int *v,unsigned int size){

    for(unsigned int i=0;i<size-1;i++)
        if(v[i]>v[i+1])
            return false;
    return true;
}
void demoBubbleSort(){

    int v[10];
    FillRandomArray(v,10,10,1000,false,UNSORTED);
    printf("Bubble sort:\n");
    PrintArray(v,10);
    BubbleSort(v,10);
    PrintArray(v,10);
}
void demoInsertionsort(){
    int v[10];
    FillRandomArray(v,10,10,1000,false,UNSORTED);
    printf("InsertionSort:\n");
    PrintArray(v,10);
    InsertionSort(v,10);
   PrintArray(v,10);
}
void demoSelectionSort(){

    int v[10];
    FillRandomArray(v,10,10,1000,false,UNSORTED);
    printf("Selection sort:\n");
    PrintArray(v,10);
    SelectionSort(v,10);
    PrintArray(v,10);
}
void demoBinaryInsertionSort(){
    int v[10];
    FillRandomArray(v,10,10,1000,false,UNSORTED);
    printf("Binary Insertion sort:\n");
   PrintArray(v,10);
    BinaryInsertionSort(v,10);
   PrintArray(v,10);
 /*   if(true==CheckIsSorted(v,10))
        printf("SORTED");
    else printf("UNSORTED");*/
}
void AverageCase(){

    int arr1[MAX_SIZE],arr2[MAX_SIZE],arr3[MAX_SIZE];
        for(int n=STEP_SIZE;n<=MAX_SIZE;n+=STEP_SIZE)
            for(int q=0;q<NUM_TEST;q++){
            FillRandomArray(arr1,n,10,50000,false,UNSORTED);
            for(unsigned int i=0;i<n;i++){
                arr2[i]=arr1[i];
                arr3[i]=arr1[i];
            }
            BubbleSort(arr1,n);
            SelectionSort(arr2,n);
            InsertionSort(arr3,n);
        }
      p.divideValues("Bubbleass",NUM_TEST);
      p.divideValues("Bubblecomp",NUM_TEST);
      p.divideValues("Insertionass",NUM_TEST);
      p.divideValues("Insertioncomp",NUM_TEST);
      p.divideValues("Selectionass",NUM_TEST);
      p.divideValues("SelectionComp",NUM_TEST);
      p.addSeries("BubbleTotal","Bubbleass","Bubblecomp");
      p.addSeries("InsertionTotal","Insertionass","Insertioncomp");
      p.addSeries("SelectionTotal","Selectionass","Selectioncomp");
      p.createGroup("AVGAssignment","Bubbleass","Insertionass");
      p.createGroup("AVGComparisons","Bubblecomp","Insertioncomp","Selectioncomp");
      p.createGroup("AVGTotal","BubbleTotal","InsertionTotal","SelectionTotal");

}
void BestCase(){

    p.reset("DirectSortingMethods");
    int arr1[MAX_SIZE],arr2[MAX_SIZE],arr3[MAX_SIZE];
    for(int n=STEP_SIZE;n<=MAX_SIZE;n+=STEP_SIZE){
            FillRandomArray(arr1,n,10,50000,true,ASCENDING);
            for(unsigned int i=0;i<n;i++){
                arr2[i]=arr1[i];
                arr3[i]=arr1[i];
            }
            BubbleSort(arr1,n);
            SelectionSort(arr2,n);
            InsertionSort(arr3,n);
        }
    p.addSeries("BubbleTotal","Bubbleass","Bubblecomp");
    p.addSeries("InsertionTotal","Insertionass","Insertioncomp");
    p.addSeries("BestSelectionTotal","Selectionass","Selectioncomp");
    p.createGroup("BestAssignment","Bubbleass","Insertionass","Selectionass");
    p.createGroup("BestComparisons","Bubblecomp","Insertioncomp");
    p.createGroup("BestTotal","BubbleTotal","InsertionTotal");

}
void WorstCase(){

    int x;
    p.reset("DirectSortingMethods");
    int arr1[MAX_SIZE],arr2[MAX_SIZE],arr3[MAX_SIZE];
    for(int n=STEP_SIZE;n<=MAX_SIZE;n+=STEP_SIZE){
            FillRandomArray(arr1,n,10,50000,true,DESCENDING);
            for(unsigned int i=0;i<n;i++){
                arr2[i]=arr1[i];
                arr3[i]=arr1[i];
            }
        BinaryInsertionSort(arr3,n);
            x=arr3[n-1];
            for(int i=n-1;i>0;i--)
              arr3[i]=arr3[i-1];
            arr3[0]=x;
            BubbleSort(arr1,n);
            SelectionSort(arr2,n);
            InsertionSort(arr3,n);
        }

    p.addSeries("BubbleTotal","Bubbleass","Bubblecomp");
    p.addSeries("InsertionTotal","Insertionass","Insertioncomp");
    p.addSeries("SelectionTotal","Selectionass","Selectioncomp");
    p.createGroup("WORSTAssignment","Insertionass","Selectionass");
    p.createGroup("WORSTComparisons","Bubblecomp","Selectioncomp");
    p.createGroup("WORSTTotal","BubbleTotal","SelectionTotal");
    p.showReport();

}
int main() {

    freopen("output.out","w",stdout);
   // AverageCase();
   // BestCase();
   // WorstCase();
    demoBubbleSort();
    demoSelectionSort();
    demoInsertionsort();
    demoBinaryInsertionSort();

 /*   clock_t start = clock();

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);

    start=clock();
    demoInsertionsort();
    stop = clock();
    elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);*/
    return 0;
}
