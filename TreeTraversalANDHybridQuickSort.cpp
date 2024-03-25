/*
 * Traversals Comparison:
 *   From the graphs we can see that there is no difference between the number of prints done by
 *   the Iterative implementation or Recursive implementation of the traversals. What makes the Iterative implementation more
 *   desirable is that it is not recursive(doesn't involve calls to stack such as PUSH,POP,RET) and it is more compact(All traversals can
 *   be done in one function).It uses constant additional memory since we only need a node pointer to the current node we are at, and an
 *   integer to remember where we are coming from and signaling where we should go.
 *    All traversals take O(n) (since every node is visited once)
 *
 *  Determination of threshold for HybridQuickSort:
 *    Best threshold is 27 since it performs the best in the worst case, running time wise, and it's
 *    pretty good compared to the others from the number of operations perspective. I took the threshold from the
 *    running time perspective because it has less recursive calls than the others which have low number of
 *    operations, so we don't need to keep pushing and poping from the stack
 *
 *  QuickSort vs HybridQuickSort:
 *     Average case:they perform almost the same
 *     Worst case: Quicksort's worst case is O(n^2) while for the Hybrid form is O(nlogn) (always,even though it has a worst case,i.e. when the array is sorted in descending order
 *     because of insertion sort)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NUM_TEST 10

Profiler p("Traversal");
enum Traversal{Post,Pre,In};

typedef struct Node{
    int key;
    struct Node *right;
    struct Node *left;
    struct Node *parent;
};
int op,opA,opC;
void PrettyPrint(Node *node,int space){

    if(NULL!=node){

        for(int i=0;i<space;i++)
            printf("   ");

        printf("%d\n",node->key);
        PrettyPrint(node->left,space+1);
        PrettyPrint(node->right,space+1);
    }

}
void IterativeTraversal(Node **root,Traversal traversal){
  int direction=1;
  Node *node=(*root);
  do{
      if(direction==1){

          if(traversal==Pre){
              op+=1;
              printf("%d ",node->key);
          }

          if(NULL!=node->left)
              node=node->left;
          else direction=2;
      }
      if(direction==2){

          if(traversal==In){
              op+=1;
              printf("%d ",node->key);
          }

          if(NULL!=node->right) {
              node = node->right;
              direction=1;
          }
          else direction=3;
      }
      if(direction==3){

          if(traversal==Post){
              op+=1;
             printf("%d ",node->key);
          }

          if(NULL!=node->parent){
              if(node==node->parent->left)
                  direction=2;

              node=node->parent;
          }
      }
  }
  while((*root)!=node || direction!=3);

    if(traversal==Post){
        op+=1;
        printf("%d ",node->key);
    }

}
void PreorderTraversal(Node *node){
    if(NULL==node)
        return;

    op+=1;
    printf("%d ",node->key);
    PreorderTraversal(node->left);
    PreorderTraversal(node->right);
}
void PostorderTraversal(Node *node){
    if(NULL==node)
        return;

    PostorderTraversal(node->left);
    PostorderTraversal(node->right);
    printf("%d ",node->key);
    op+=1;
}
void InorderTraversal(Node *node){
    if(NULL==node)
        return;

    InorderTraversal(node->left);
    op+=1;
    printf("%d ",node->key);
    InorderTraversal(node->right);
}
void InsertInTree(Node **root, int key){

    if(NULL==(*root)){
        Node *temp = (Node *) malloc(sizeof(Node));
        temp->parent=NULL;
        temp->right=NULL;
        temp->left=NULL;
        temp->key=key;
        (*root)=temp;
        return;
    }

    Node *curr_node=(*root);
    int ok=false;
    while(!ok){
        if(key>curr_node->key) {

            if (NULL == curr_node->right) {
                Node *temp = (Node *) malloc(sizeof(Node));
                temp->parent=curr_node;
                temp->right=NULL;
                temp->left=NULL;
                temp->key=key;
                curr_node->right=temp;
                curr_node=curr_node->right;
                ok= true;
            }
            else curr_node=curr_node->right;
        }
        else if(key<curr_node->key){

            if (NULL == curr_node->left) {
                Node *temp = (Node *) malloc(sizeof(Node));
                temp->parent=curr_node;
                temp->right=NULL;
                temp->left=NULL;
                temp->key=key;
                curr_node->left=temp;
                curr_node=curr_node->left;
                ok= true;
            }
            else curr_node=curr_node->left;
        }
    }
}
Node *GetMinimum(Node *root){
    while(root->left!=NULL)
        root=root->left;

    return root;
}
Node *DeleteInTree(Node *node,int key){

    if(node==NULL)
        return node;

    if(key>node->key){
        node->right= DeleteInTree(node->right,key);
    }
    else if(key<node->key){
        node->left= DeleteInTree(node->left,key);
    }
    else{

        if(NULL==node->left && NULL==node->right){
            free(node);
            node=NULL;
        }
        else if(NULL==node->left){
            Node *temp=node;
            node=node->right;
            free(temp);
            temp=NULL;
        }
        else if(NULL==node->right){
            Node *temp=node;
            node=node->left;
            free(temp);
            temp=NULL;
        }
        else{
            Node *temp= GetMinimum(node->right);
            node->key=temp->key;
            node->right= DeleteInTree(node->right,temp->key);
        }
    }
    return node;

}
void Analysis(){

    int arr[MAX_SIZE];
    Node *root=NULL;
    for(int n=STEP_SIZE;n<=MAX_SIZE;n+=STEP_SIZE){
        Operation IterativePre=p.createOperation("Iterative Preorder",n);
        Operation IterativePost=p.createOperation("Iterative Postorder",n);
        Operation IterativeIn=p.createOperation("Iterative Inorder",n);
        Operation RecursivePre=p.createOperation("Recursive Preorder",n);
        Operation RecursivePost=p.createOperation("Recursive Postorder",n);
        Operation RecursiveIn=p.createOperation("Recursive Inorder",n);
        for(int test=0;test<NUM_TEST;test++){
            FillRandomArray(arr,n,10,50000,true,UNSORTED);

            for(int i=0;i<n;i++)
                InsertInTree(&root,arr[i]);

            op=0;
            IterativeTraversal(&root,Pre);
            IterativePre.count(op);

            op=0;
            IterativeTraversal(&root,Post);
            IterativePost.count(op);

            op=0;
            IterativeTraversal(&root,In);
            IterativeIn.count(op);

            op=0;
            PreorderTraversal(root);
            RecursivePre.count(op);

            op=0;
            PostorderTraversal(root);
            RecursivePost.count(op);

            op=0;
            InorderTraversal(root);
            RecursiveIn.count(op);

            for(int i=0;i<n;i++)
                root= DeleteInTree(root,arr[i]);
        }
    }

    p.divideValues("Iterative Preorder",NUM_TEST);
    p.divideValues("Iterative Postorder",NUM_TEST);
    p.divideValues("Iterative Inorder",NUM_TEST);
    p.divideValues("Recursive Preorder",NUM_TEST);
    p.divideValues("Recursive Postorder",NUM_TEST);
    p.divideValues("Recursive Inorder",NUM_TEST);
    p.showReport();

}
void Demo(){

    int arr[13];
    FillRandomArray(arr,13,10,45,true,UNSORTED);
    Node *root=NULL;
    for(int i=0;i<13;i++)
        InsertInTree(&root,arr[i]);

    printf("\n---Pretty Print---\n");
    PrettyPrint(root,0);

    printf("\n--Recursive Preorder---\n");
    PreorderTraversal(root);

    printf("\n--Recursive Postorder---\n");
    PostorderTraversal(root);

    printf("\n--Recursive Inorder---\n");
    InorderTraversal(root);

    printf("\n--Iterative Preorder---\n");
    IterativeTraversal(&root,Pre);

    printf("\n--Iterative Postorder---\n");
    IterativeTraversal(&root,Post);

    printf("\n--Iterative Inorder---\n");
    IterativeTraversal(&root,In);



}
void InsertionSort(int *v,int low,int high){
    int ind;
    int aux,elem;
    for(int i=low;i<high;i++){
        ind=i;
        elem=v[ind+1];
      opA+=1;//Assignment

      opC+=1;//Comparison
        while(ind>=low && v[ind]>elem){
            v[ind+1]=v[ind];
            ind--;
          opA+=1;//Assignment
         opC+=1;//Comparison
        }

        if(ind!=i) {
            v[ind + 1] = elem;
           opA+=1;//Assignment
        }
    }
}
void BinaryInsertionSort(int *v,int low_arr,int high_arr){
    int ind;
    int aux,elem,low,high,mid;
    for(int i=low_arr;i<=high_arr;i++){
        low=0;
        high=i;
        elem=v[i];
      opA+=1;//Assignment

        while(low<high){
            mid=low+(high-low)/2;

           opC+=1;//Comparison
            if(v[mid]<=elem)
                low=mid+1;
            else high=mid;
        }

        if(high!=i) {
            for (int j = i; j > high; j--) {
                v[j] = v[j - 1];
             opA+=1;//Assignment
            }
            v[high] = elem;
         opA+=1;//Assignment
        }
    }
}
int Partition(int *arr,int low,int high){
    int aux;
    int pivot=low;
  while(low<=high){

      opC+=1;//Comparison
      while(arr[low]<arr[pivot]) {
          low++;
       opC+=1;
      }

     opC+=1;//Comparison
      while(arr[high]>arr[pivot]) {
          high--;
        opC+=1;
      }

      if(low<high){
          aux=arr[low];
          arr[low]=arr[high];
          arr[high]=aux;
          low++;
          high--;

          opA+=3;//Assignment
      }
      else break;
  }
  aux=arr[high];
  arr[high]=arr[pivot];
  arr[pivot]=aux;
  opA+=3;//Assignment
  return high;
}
void QuickSort(int *arr,int low,int high){
  if(low<high){

          int p = Partition(arr, low, high);
          QuickSort(arr, low, p - 1);
          QuickSort(arr, p + 1, high);
  }
}
void HybridQuickSortLinearInsertionSort(int *arr,int low,int high,int threshold){
    if(low<high){
        if(high-low<threshold)
            InsertionSort(arr,low,high);
        else {

            int rand_index = rand() % (high - low) + low;
            int aux = arr[low];
            arr[low] = arr[rand_index];
            arr[rand_index] = aux;
            opA+=3;//Assignment
            int p = Partition(arr, low, high);
            HybridQuickSortLinearInsertionSort(arr, low, p - 1,threshold);
            HybridQuickSortLinearInsertionSort(arr, p + 1, high,threshold);
        }
    }
}
void HybridQuickSortBinaryInsertionSort(int *arr,int low,int high,int threshold){
    if(low<high){
        if(high-low<threshold)
            BinaryInsertionSort(arr,low,high);
        else {

            int rand_index = rand() % (high - low) + low;
            int aux = arr[low];
            arr[low] = arr[rand_index];
            arr[rand_index] = aux;
             opA+=3;//Assignment
            int p = Partition(arr, low, high);
            HybridQuickSortBinaryInsertionSort(arr, low, p - 1,threshold);
            HybridQuickSortBinaryInsertionSort(arr, p + 1, high,threshold);
        }
    }
}
void DemoQuickSort(){

    int N=23;
    int arr[N];
    FillRandomArray(arr,N,10,50,false,UNSORTED);
    for(int i=0;i<23;i++)
        printf("%d ",arr[i]);
    printf("\n");
    QuickSort(arr,0,N-1);
    for(int i=0;i<23;i++)
        printf("%d ",arr[i]);
}
void DemoHybridQuickSortLinearInsertion(){
    int N=23;
    int arr[N];
    FillRandomArray(arr,N,10,50,false,UNSORTED);
    for(int i=0;i<23;i++)
        printf("%d ",arr[i]);
    printf("\n");
    HybridQuickSortLinearInsertionSort(arr,0,N-1,30);
    for(int i=0;i<23;i++)
        printf("%d ",arr[i]);
}
void DemoHybridQuickSortBinaryInsertion(){
    int N=23;
    int arr[N];
    FillRandomArray(arr,N,10,50,false,UNSORTED);
    for(int i=0;i<23;i++)
        printf("%d ",arr[i]);
    printf("\n");
    HybridQuickSortBinaryInsertionSort(arr,0,N-1,30);
    for(int i=0;i<23;i++)
        printf("%d ",arr[i]);
}
void DeterminationOptimalThreshold(){


    int *arr=(int *)malloc(sizeof (int)*10000);
    for(int threshold=5;threshold<=50;threshold++){
        printf("--%d--\n",threshold);
        Operation opAssign=p.createOperation("Total Assignments Hybrid Quicksort Linear Insertion",threshold);
        Operation opComp=p.createOperation("Total Comparisons Hybrid Quicksort Linear Insertion",threshold);
        Operation opTotal=p.createOperation("Total Ops Hybrid Quicksort Linear Insertion",threshold);

        for(int test=0;test<100;test++){
            FillRandomArray(arr,10000,10,50000,false,UNSORTED);
            opC=0;
            opA=0;
            HybridQuickSortLinearInsertionSort(arr,0,9999,threshold);
            opAssign.count(opA);
            opComp.count(opC);
            opTotal.count(opC+opA);
        }


    }
    p.divideValues("Total Assignments Hybrid Quicksort Linear Insertion",100);
    p.divideValues("Total Comparisons Hybrid Quicksort Linear Insertion",100);
    p.divideValues("Total Ops Hybrid Quicksort Linear Insertion",100);


    for(int threshold=5;threshold<=50;threshold++) {
        printf("--%d--\n",threshold);
         p.startTimer("Time QuickSortHybrid Hybrid Quicksort Linear Insertion",threshold);
        for(int test=0;test<1000;test++){
            FillRandomArray(arr,10000,10,50000,false,UNSORTED);
            opA=0;
            opC=0;
            HybridQuickSortLinearInsertionSort(arr,0,9999,threshold);
        }
        p.stopTimer("Time QuickSortHybrid Hybrid Quicksort Linear Insertion",threshold);

    }

    for(int threshold=5;threshold<=50;threshold++) {
        printf("--%d--\n",threshold);
        p.startTimer("Time QuickSortHybrid Hybrid Quicksort Binary Insertion",threshold);
        for(int test=0;test<1000;test++){
            FillRandomArray(arr,10000,10,50000,false,UNSORTED);
            opA=0;
            opC=0;
            HybridQuickSortBinaryInsertionSort(arr,0,9999,threshold);
        }
        p.stopTimer("Time QuickSortHybrid Hybrid Quicksort Binary Insertion",threshold);

    }

    for(int threshold=5;threshold<=50;threshold++){
        printf("--%d--\n",threshold);
        Operation opAssign=p.createOperation("Worst Total Assignments Hybrid Quicksort Linear Insertion",threshold);
        Operation opComp=p.createOperation("Worst Total Comparisons Hybrid Quicksort Linear Insertion",threshold);
        Operation opTotal=p.createOperation("Worst Total Ops Hybrid Quicksort Linear Insertion",threshold);

        for(int test=0;test<100;test++){
            FillRandomArray(arr,10000,10,50000,false,DESCENDING);
            opC=0;
            opA=0;
            HybridQuickSortLinearInsertionSort(arr,0,9999,threshold);
            opAssign.count(opA);
            opComp.count(opC);
            opTotal.count(opC+opA);
        }


    }
    p.divideValues("Worst Total Assignments Hybrid Quicksort Linear Insertion",100);
    p.divideValues("Worst Total Comparisons Hybrid Quicksort Linear Insertion",100);
    p.divideValues("Worst Total Ops Hybrid Quicksort Linear Insertion",100);


    for(int threshold=5;threshold<=50;threshold++) {
        printf("--%d--\n",threshold);
        p.startTimer("Worst Time QuickSortHybrid Hybrid Quicksort Linear Insertion",threshold);
        for(int test=0;test<1000;test++){
            FillRandomArray(arr,10000,10,50000,false,DESCENDING);
            opA=0;
            opC=0;
            HybridQuickSortLinearInsertionSort(arr,0,9999,threshold);
        }
        p.stopTimer("Worst Time QuickSortHybrid Hybrid Quicksort Linear Insertion",threshold);

    }

    for(int threshold=5;threshold<=50;threshold++) {
        printf("--%d--\n",threshold);
        p.startTimer("Worst Time QuickSortHybrid Hybrid Quicksort Binary Insertion",threshold);
        for(int test=0;test<1000;test++){
            FillRandomArray(arr,10000,10,50000,false,DESCENDING);
            opA=0;
            opC=0;
            HybridQuickSortBinaryInsertionSort(arr,0,9999,threshold);
        }
        p.stopTimer("Worst Time QuickSortHybrid Hybrid Quicksort Binary Insertion",threshold);

    }

    free(arr);

    p.showReport();

}
void QuicksortVsHybridQuickSort(){

    for(int n=STEP_SIZE;n<=MAX_SIZE;n+=STEP_SIZE){
        printf("--%d--\n",n);
        Operation opAssign=p.createOperation("Total Assignments Quicksort",n);
        Operation opComp=p.createOperation("Total Comparisons Quicksort",n);
        Operation opTotal=p.createOperation("Total Ops Quicksort",n);
        Operation opAssign1=p.createOperation("Total Assignments Hybrid Quicksort Linear Insertion",n);
        Operation opComp1=p.createOperation("Total Comparisons Hybrid Quicksort Linear Insertion",n);
        Operation opTotal1=p.createOperation("Total Ops Hybrid Quicksort Linear Insertion",n);
        int *arr=(int *) malloc(sizeof(int)*n);
        int *arr1=(int *) malloc(sizeof(int)*n);
        for(int test=0;test<10;test++){
            FillRandomArray(arr,n,10,50000,false,UNSORTED);
            CopyArray(arr1,arr,n);
            opA=0;
            opC=0;
            QuickSort(arr,0,n-1);
            opAssign.count(opA);
            opComp.count(opC);
            opTotal.count(opA+opC);

            opA=0;
            opC=0;
            HybridQuickSortLinearInsertionSort(arr1,0,n-1,27);
            opAssign1.count(opA);
            opComp1.count(opC);
            opTotal1.count(opA+opC);
        }

        free(arr);
        free(arr1);
    }
    p.divideValues("Total Assignments Quicksort",10);
    p.divideValues("Total Comparisons Quicksort",10);
    p.divideValues("Total Ops Quicksort",10);
    p.divideValues("Total Assignments Hybrid Quicksort Linear Insertion",10);
    p.divideValues("Total Comparisons Hybrid Quicksort Linear Insertion",10);
    p.divideValues("Total Ops Hybrid Quicksort Linear Insertion",10);
    p.createGroup("Quick vs Hybrid Assign","Total Assignments Quicksort","Total Assignments Hybrid Quicksort Linear Insertion");
    p.createGroup("Quick vs Hybrid Comp","Total Comparisons Quicksort","Total Comparisons Hybrid Quicksort Linear Insertion");
    p.createGroup("Quick vs Hybrid Total","Total Ops Quicksort","Total Ops Hybrid Quicksort Linear Insertion");

    for(int n=STEP_SIZE;n<=MAX_SIZE;n+=STEP_SIZE){
        printf("--%d--\n",n);
        int *arr=(int *) malloc(sizeof(int)*n);
        p.startTimer("Time QuickSortHybrid Hybrid Quicksort Linear Insertion",n);
        for(int test=0;test<1000;test++){
            FillRandomArray(arr,n,10,50000,false,UNSORTED);
            HybridQuickSortLinearInsertionSort(arr,0,n-1,27);
        }
        p.stopTimer("Time QuickSortHybrid Hybrid Quicksort Linear Insertion",n);

        p.startTimer("Time QuickSort",n);
        for(int test=0;test<1000;test++){
            FillRandomArray(arr,n,10,50000,false,UNSORTED);
            QuickSort(arr,0,n-1);
        }
        p.stopTimer("Time QuickSort",n);

        free(arr);
    }

    p.createGroup("Time","Time QuickSortHybrid Hybrid Quicksort Linear Insertion","Time QuickSort");

    for(int n=STEP_SIZE;n<=MAX_SIZE;n+=STEP_SIZE){
        printf("--%d--\n",n);
        Operation opAssign=p.createOperation("Worst Total Assignments Quicksort",n);
        Operation opComp=p.createOperation("Worst Total Comparisons Quicksort",n);
        Operation opTotal=p.createOperation("Worst Total Ops Quicksort",n);
        Operation opAssign1=p.createOperation("Worst Total Assignments Hybrid Quicksort Linear Insertion",n);
        Operation opComp1=p.createOperation("Worst Total Comparisons Hybrid Quicksort Linear Insertion",n);
        Operation opTotal1=p.createOperation("Worst Total Ops Hybrid Quicksort Linear Insertion",n);
        int *arr=(int *) malloc(sizeof(int)*n);
        int *arr1=(int *) malloc(sizeof(int)*n);
        for(int test=0;test<10;test++){
            FillRandomArray(arr,n,10,50000,false,DESCENDING);
            CopyArray(arr1,arr,n);
            opA=0;
            opC=0;
            QuickSort(arr,0,n-1);
            opAssign.count(opA);
            opComp.count(opC);
            opTotal.count(opA+opC);

            opA=0;
            opC=0;
            HybridQuickSortLinearInsertionSort(arr1,0,n-1,27);
            opAssign1.count(opA);
            opComp1.count(opC);
            opTotal1.count(opA+opC);
        }

        free(arr);
        free(arr1);
    }
    p.divideValues("Worst Total Assignments Quicksort",10);
    p.divideValues("Worst Total Comparisons Quicksort",10);
    p.divideValues("Worst Total Ops Quicksort",10);
    p.divideValues("Worst Total Assignments Hybrid Quicksort Linear Insertion",10);
    p.divideValues("Worst Total Comparisons Hybrid Quicksort Linear Insertion",10);
    p.divideValues("Worst Total Ops Hybrid Quicksort Linear Insertion",10);
    p.createGroup("Worst Quick vs Hybrid Assign","Worst Total Assignments Quicksort","Worst Total Assignments Hybrid Quicksort Linear Insertion");
  //  p.createGroup("Worst Quick vs Hybrid Comp","Worst Total Comparisons Quicksort","Worst Total Comparisons Hybrid Quicksort Linear Insertion");
   // p.createGroup("Worst Quick vs Hybrid Total","Worst Total Ops Quicksort","Worst Total Ops Hybrid Quicksort Linear Insertion");

    for(int n=STEP_SIZE;n<=MAX_SIZE;n+=STEP_SIZE){
        printf("--%d--\n",n);
        int *arr=(int *) malloc(sizeof(int)*n);
        p.startTimer("Worst Time QuickSortHybrid Hybrid Quicksort Linear Insertion",n);
        for(int test=0;test<1000;test++){
            FillRandomArray(arr,n,10,50000,false,DESCENDING);
            HybridQuickSortLinearInsertionSort(arr,0,n-1,27);
        }
        p.stopTimer("Worst Time QuickSortHybrid Hybrid Quicksort Linear Insertion",n);

        p.startTimer("Worst Time QuickSort",n);
        for(int test=0;test<1000;test++){
            FillRandomArray(arr,n,10,50000,false,DESCENDING);
            QuickSort(arr,0,n-1);
        }
        p.stopTimer("Worst Time QuickSort",n);

        free(arr);
    }

    p.createGroup("Worst Time","Worst Time QuickSortHybrid Hybrid Quicksort Linear Insertion","Worst Time QuickSort");

    p.showReport();
}
int main() {

    //Analysis();
    freopen("algsort.out","w",stdout);
   Demo();

    printf("\nDemo Quicksort\n");
   DemoQuickSort();
    printf("\nDemo HybridQuicksort with Linear Insertion Sort\n");
   DemoHybridQuickSortLinearInsertion();
    printf("\nDemo HybridQuicksort with Binary Insertion Sort\n");
    DemoHybridQuickSortBinaryInsertion();

 //DeterminationOptimalThreshold();
//  QuicksortVsHybridQuickSort();

   return 0;
}
