/*
 *   To construct the tree with elements 1,2,3...n, using a divide et impera approach, I have the following approach:
 *      1.Consider set A={1,2,3...,n}
 *      2.while(A is not empty) do the following
 *      3.pick the element in the middle of the set
 *      4.create a node and store the middle in the node
 *      5.split A into two parts, A'={1,2,..,mid-1} and A''={mid+1,mid+2,...n}
 *      6.To the right of the node will be the node with the middle key of A'
 *        To the left of the node will be the node with the middle key of A''
 *      7.repeat from step 2 with A<-A' then A<-A''
 *      O(n) time algorithm since we do the get middle ( O(1) ) and create node ( O(1) ) for each element in the set A
 *      Also we store in each node the size(dimension) i.e. the number of elements in the tree rooted at the node
 *      which is the size of the current A
 *      Note:This algorithm also construct a PBT (Perfect Binary tree) since the set is always splitting half
 *      thus making the number elements on the left equal to, or off by one, with the number of elements on the right
 *
 *   For OS_SELECT, since we already have the dimension of each node stored we do the traversal on the tree like a normal BST
 *   but instead of comparing the key values, we compare the rank in the tree rooted at that node with the index we want to find.
 *   If the node we are currently at has the rank, in the tree rooted at that node, greater than the index we want to find , that
 *   means the node with the index is in the left subtree, if the rank is less than the index that means the node is in the right subtree
 *   with the index=(index-rank_of_current_node). If the rank is equal with the index, we stop and return the node we are at.
 *   this algorithm should take O(logn) since it is a PBT from the start
 *
 *   For OS_DELETE, we delete like in a normal BST, but again we compare rank of the current node with the index of the
 *   element we want to delete.We treat all cases like in a normal BST delete, but in the case when the node we want to delete
 *   has two children, I chose to select the minimum node of the right subtree, but we can do this by select the element with index 1
 *   int the right subtree(using OS_SELECT)
 *   This should take O(logn) since it is balanced from the beginning
 *
 *   Graph Analysis:
 *     From the graphs we can see that the total number of operations for all elements of the tree takes O(nlogn) ( we can see
 *     the slight curve upwards of the graph) and we can see that on average, an operation takes O(logn) since it is a PBT from the start
 *     The operations from the build tree should be O(n) as in the graph.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Profiler.h"

#define STEP_SIZE 100
#define NUM_TEST 100
#define MAX_SIZE 10000

Profiler p("OS_Tree");
typedef struct Node{
    int key;
    int balance;
    int dimension;
    int height; //this field will be used only for the AVL implementation
    struct Node *right;
    struct Node *left;
    struct Node *parent;
};
unsigned long long opComp,opAssign;
int Maximum(int a,int b){
    return (a>b) ? a:b;
}
void PrettyPrint(Node *root, int space){
    if(NULL!=root) {

        for (int i = 0; i < space; i++)
            printf("   ");
        printf("%d\n", root->key);
        PrettyPrint(root->right, space + 1);
        PrettyPrint(root->left, space + 1);
    }
}
Node *GetMinimum(Node *root){

    opComp+=1;
    while(NULL!=root->left) {
        root = root->left;
        opAssign+=1;
        opComp+=1;
    }
    return root;
}
Node *GetMaximum(Node *root){
    opComp+=1;
    while(NULL!=root->right) {
        root = root->right;
        opAssign+=1;
        opComp+=1;
    }
    return root;
}
int HeightTree(Node *root){

    if(NULL==root)
        return -1;

    return root->height;
}
int DimensionTree(Node *root){
    if(NULL==root)
        return 0;

    return root->dimension;
}
Node *rightRotation(Node *node){

    Node *leftChild=node->left;
    Node *gama=leftChild->right;

    leftChild->right=node;
    leftChild->parent=node->parent;

    node->parent=leftChild;
    node->left=gama;

    node->height=1+ Maximum(HeightTree(node->left), HeightTree(node->right));
    leftChild->height=1+ Maximum(HeightTree(leftChild->left), HeightTree(leftChild->right));


    node->dimension=1+ DimensionTree(node->left)+ DimensionTree(node->right);
    leftChild->dimension=1+DimensionTree(leftChild->left)+ DimensionTree(leftChild->right);

    return leftChild;
}
Node *leftRotation(Node *node){

    Node *rightChild=node->right;
    Node *beta=rightChild->left;

    rightChild->left=node;
    rightChild->parent=node->parent;

    node->parent=rightChild;
    node->right=beta;

    node->height=1+ Maximum(HeightTree(node->left), HeightTree(node->right));
    rightChild->height=1+ Maximum(HeightTree(rightChild->left), HeightTree(rightChild->right));

    node->dimension=1+ DimensionTree(node->left)+ DimensionTree(node->right);
    rightChild->dimension=1+DimensionTree(rightChild->left)+ DimensionTree(rightChild->right);

    return rightChild;
}
Node *ConstructTree(int low, int high){
    if(low<=high){
        Node *node=(Node *) malloc(sizeof (Node));
        int mid=(high+low)/2;
        node->key=mid;
        node->parent=NULL;
        node->balance=(high-mid)-(mid-low);
        node->dimension=high-low+1;
        opAssign+=5;
        opComp+=1;
        if(low<=mid-1) {
            node->left=ConstructTree(low, mid - 1);
            node->left->parent=node;
            opAssign+=1;
        }
        else node->left = NULL;
        opAssign+=1;

        opComp+=1;
        if(mid+1<=high) {
            node->right=ConstructTree(mid + 1, high);
            node->right->parent=node;
            opAssign+=1;
        }
        else node->right=NULL;
        opAssign+=1;

        node->height=1+ Maximum(HeightTree(node->left), HeightTree(node->right));
        opAssign+=1;
        return node;
    }

}
Node *BuildTree(int n){
    Node *root= ConstructTree(1, n);
    return root;

}
int OS_RANK(Node *root,Node *node){
    int r=1;

    opComp+=1;
    if(NULL!=node->left) {
        r += node->left->dimension;
       opAssign+=1;
    }

    Node *aux_node=node;
    opComp+=1;
    while(aux_node!=root){
        opComp+=1;
        if(aux_node==aux_node->parent->right) {
            r+=1;
            opComp+=1;
            if(NULL!=aux_node->parent->left) {
                r += aux_node->parent->left->dimension;
            opAssign+=1;
            }
        }
        aux_node=aux_node->parent;
        opAssign+=1;
        opComp+=1;
    }
    return r;
}
Node *OS_SELECT(Node *node,int index){

    int r;
    opComp+=1;
    if(NULL==node->left)
        r=1;
    else {
        r = node->left->dimension + 1;
        opAssign+=1;
    }

    opComp+=1;
    if(index==r) return node;
    else {
        opComp+=1;
        if (index < r)
            return OS_SELECT(node->left, index);
        else {
            opComp+=1;
            if (index > r) {
                return OS_SELECT(node->right, index - r);
            }
        }
    }
}
Node *OS_DELETE(Node *node,int index){
   opComp+=1;
    if(NULL==node) return node;
    else {
        int r;
           opComp+=1;
        if (NULL == node->left)
            r = 1;
        else {
            r = node->left->dimension + 1;
           opAssign+=1;
        }
           opComp+=1;
        if (index < r) {
            node->dimension -= 1;
            node->balance+=1;
            node->left = OS_DELETE(node->left, index);
            opAssign+=3;
        }
        else if (index > r) {
            opComp+=1;
            node->dimension -= 1;
            node->balance-=1;
            node->right = OS_DELETE(node->right, index - r);
            opAssign+=3;
        }
        else {
            opComp+=1; //previous if statement, didn't want to restructure the code

           opComp+=2;
            if (NULL == node->right && NULL == node->left) {
                free(node);
                node = NULL;
               opAssign+=1;
            }
            else {
                opComp+=1;
                if (NULL == node->right) {
                    Node *temp = node;
                    node = node->left;
                    free(temp);
                    temp = NULL;
                    opAssign+=3;
                }
                else{
                    opComp+=1;
                    if (NULL == node->left) {
                        Node *temp = node;
                        node = node->right;
                        free(temp);
                        temp = NULL;
                        opAssign+=3;
                    }
                    else {
                        Node *temp = OS_SELECT(node->right,1); //GetMinimum(node->right);
                       opAssign+=1;
                        //  int rank = OS_RANK(node->right, temp);
                        node->key = temp->key;
                        node->dimension -= 1;
                        node->balance-=1;
                        opAssign+=3;
                        //  node->right = OS_DELETE(node->right, rank);
                        node->right = OS_DELETE(node->right, 1);
                       opAssign+=1;
                    }
                }
            }
        }
        return node;
    }
}
void DEMO(){
    int n=11;
    Node *root= BuildTree(n);

    printf("--Generated tree--\n");
    PrettyPrint(root,0);

    printf("  ---OS_SELECT DEMO---\n");
    srand(time(NULL));
    int rand_index=rand()%n+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);


    rand_index=rand()%n+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);

    rand_index=rand()%n+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);


    printf("   --OS_DELETE DEMO--\n");
    rand_index=rand()%n+1;
    printf("rand index %d\n",rand_index);
    root= OS_DELETE(root,rand_index);
    printf("--Pretty Print after deletion--\n");
    PrettyPrint(root,0);
    rand_index=rand()%(n-1)+1;
    printf("rand index %d\n",rand_index);
    root= OS_DELETE(root,rand_index);
    printf("--Pretty Print after deletion--\n");
    PrettyPrint(root,0);
    rand_index=rand()%(n-2)+1;
    printf("rand index %d\n",rand_index);
    root= OS_DELETE(root,rand_index);
    printf("--Pretty Print after deletion--\n");
    PrettyPrint(root,0);

    printf("   ---OS_SELECT DEMO after delete---\n");
    srand(time(NULL));
    rand_index=rand()%(n-3)+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);

    rand_index=rand()%(n-3)+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);

    rand_index=rand()%(n-3)+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);

}
void Analysis(){

    srand(time(NULL));
    int rand_index;
    Node *root,*node;
    for(int n=STEP_SIZE;n<=MAX_SIZE;n++){

        Operation AssignSelect=p.createOperation("OS_SELECT Assignments",n);
        Operation CompSelect=p.createOperation("OS_SELECT Comparisons",n);
        Operation AssignDelete=p.createOperation("OS_DELETE Assignments",n);
        Operation CompDelete=p.createOperation("OS_DELETE Comparisons",n);
        Operation OpSelect=p.createOperation("Average OS_SELECT FOR ONE ELEMENT",n);
        Operation OpDelete=p.createOperation("Average OS_DELETE FOR ONE ELEMENT",n);

         for(int test=0;test<NUM_TEST;test++){
             root = BuildTree(n);
             opComp=0;
             opAssign=0;
             for(int q=0;q<n;q++){
                 rand_index=rand()%n+1;
                 node= OS_SELECT(root,rand_index);
             }
             AssignSelect.count(opAssign);
             CompSelect.count(opComp);
             OpSelect.count((opAssign+opComp)/n);

             opComp=0;
             opAssign=0;
             for(int q=0;q<n;q++){
                 rand_index=rand()%(n-q)+1;
                 root= OS_DELETE(root,rand_index);
             }
             AssignDelete.count(opAssign);
             CompDelete.count(opComp);
             OpDelete.count((opComp+opAssign)/n);
         }
    }
      p.divideValues("OS_SELECT Assignments",NUM_TEST);
      p.divideValues("OS_SELECT Comparisons",NUM_TEST);
      p.divideValues("OS_DELETE Assignments",NUM_TEST);
      p.divideValues("OS_DELETE Comparisons",NUM_TEST);
      p.divideValues("Average OS_SELECT FOR ONE ELEMENT",NUM_TEST);
      p.divideValues("Average OS_DELETE FOR ONE ELEMENT",NUM_TEST);
      p.addSeries("OS_SELECT TOTAL","OS_SELECT Assignments","OS_SELECT Comparisons");
      p.addSeries("OS_DELETE TOTAL","OS_DELETE Assignments","OS_DELETE Comparisons");

      for(int n=STEP_SIZE;n<=MAX_SIZE;n++) {
          Operation CompBuild=p.createOperation("BUILD_TREE Comparisons",n);
          Operation AssignBuild=p.createOperation("BUILD_TREE Assign",n);
          for (int test = 0; test < NUM_TEST; test++) {
              opComp = 0;
              opAssign = 0;
              root = BuildTree(n);
              CompBuild.count(opComp);
              AssignBuild.count(opAssign);

              for (int q = 0; q < n; q++)
                  root = OS_DELETE(root, 1);

          }
      }

    p.divideValues("BUILD_TREE Comparisons",NUM_TEST);
    p.divideValues("BUILD_TREE Assign",NUM_TEST);

  p.showReport();
}
Node *InsertAVL(Node *node,int key){

    if(node==NULL){
        node=(Node *)malloc(sizeof(Node));
        node->left=NULL;
        node->right=NULL;
        node->key=key;
        node->parent=NULL;
        node->balance=0;
        node->height=0;
        node->dimension=1;
        return node;
    }

    if(node->key==key)
        return node;
    else  if(key<node->key) {
        node->left = InsertAVL(node->left, key);
        if(NULL==node->left->parent)
            node->left->parent=node;
    }
    else  if(key>node->key){
        node->right= InsertAVL(node->right,key);
        if(NULL==node->right->parent)
            node->right->parent=node;
    }

    node->height=1+ Maximum(HeightTree(node->left), HeightTree(node->right));
    node->dimension=1+ DimensionTree(node->left)+ DimensionTree(node->right);
    node->balance= HeightTree(node->left)- HeightTree(node->right);


    if (node->balance > 1 && key < node->left->key)
        return rightRotation(node);

    if (node->balance > 1 && key > node->left->key) {
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }

    if (node->balance < -1 && key > node->right->key)
        return leftRotation(node);

    if (node->balance < -1 && key < node->right->key) {
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }


    return node;
}
Node *DeleteAVL(Node *node,int index){

    if(node==NULL)
        return NULL;

    int r=1;
    r = DimensionTree(node->left) + 1;

    if(index<r) {
        node->left = DeleteAVL(node->left, index);
    }
    else if(index>r) {
        node->right = DeleteAVL(node->right, index - r);
    }
    else{

        if(NULL==node->left && NULL==node->right){
            free(node);
            node=NULL;
            return node;
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
            Node *temp= OS_SELECT(node->right,1);
            node->key=temp->key;
            node->right= DeleteAVL(node->right,1);
        }
    }

    node->dimension= DimensionTree(node->left)+1+ DimensionTree(node->right);
    node->balance= HeightTree(node->left)- HeightTree(node->right);

    if(node->balance>1 && HeightTree(node->left->left)- HeightTree(node->left->right)>=0)
        return rightRotation(node);

    if(node->balance>1 && HeightTree(node->left->left)- HeightTree(node->left->right)<0){
        node->left= leftRotation(node->left);
        return rightRotation(node);
    }

    if(node->balance<-1 && HeightTree(node->right->left)- HeightTree(node->right->right)<=0)
        return leftRotation(node);

    if(node->balance<-1 && HeightTree(node->right->left)- HeightTree(node->right->right)>0){
        node->right= rightRotation(node->right);
        return leftRotation(node);
    }

    return node;
}
void AVLDemo(){
    int n=11;
   Node *root=NULL;
    for(int i=1;i<=n;i++)
        root= InsertAVL(root,i);

    printf("\n--Generated AVL tree--\n");
    PrettyPrint(root,0);

    printf("  ---OS_SELECT DEMO AVL tree---\n");
    srand(time(NULL));
    int rand_index=rand()%n+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);


    rand_index=rand()%n+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);

    rand_index=rand()%n+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);


    printf("   --OS_DELETE DEMO AVL tree--\n");
    rand_index=rand()%n+1;
    printf("rand index %d\n",rand_index);
    root= DeleteAVL(root,rand_index);
    printf("--Pretty Print after deletion--\n");
    PrettyPrint(root,0);
    rand_index=rand()%(n-1)+1;
    printf("rand index %d\n",rand_index);
    root= DeleteAVL(root,rand_index);
    printf("--Pretty Print after deletion--\n");
    PrettyPrint(root,0);
    rand_index=rand()%(n-2)+1;
    printf("rand index %d\n",rand_index);
    root= DeleteAVL(root,rand_index);
    printf("--Pretty Print after deletion--\n");
    PrettyPrint(root,0);

    printf("   ---OS_SELECT DEMO after delete AVL Tree---\n");
    srand(time(NULL));
    rand_index=rand()%(n-3)+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);

    rand_index=rand()%(n-3)+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);

    rand_index=rand()%(n-3)+1;
    printf("selected index=%d  key=%d\n",rand_index, OS_SELECT(root,rand_index)->key);
}
int main() {

    freopen("output.out","w",stdout);
    //Analysis();
    DEMO();

    AVLDemo();


    return 0;
}
