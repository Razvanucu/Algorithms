#include <stdlib.h>
#include <stdio.h>

typedef struct nodeMWayTree{
    int key;
    int currsize,maxsize;
    struct nodeMWayTree **children;
};
typedef struct node{
    int key;
    struct node *left;
    struct node *right;
};
void PrettyPrintParentRepresentation(int *parentArr,int n,int parent,int space){
    for(int i=0;i<n;i++)
        if (parentArr[i]==parent){

        for(int i=0;i<space;i++)
            printf("   ");

            printf("%d\n",i+1);
            PrettyPrintParentRepresentation(parentArr,n,i+1,space+1);
    }
}
void PrettyPrintMultiWayTree(nodeMWayTree *root,int space){
    for(int i=0;i<space;i++)
        printf("   ");
    printf("%d\n",root->key);
    for(int i=0;i<root->currsize;i++)
        PrettyPrintMultiWayTree(root->children[i],space+1);
}
nodeMWayTree *ParentRepToMultiWayTree(int *parentArr,int n){

    int root;
    nodeMWayTree **nodesArr=(nodeMWayTree **) malloc(sizeof(nodeMWayTree *)*n);

    for(int i=0;i<n;i++){
        nodesArr[i]=(nodeMWayTree *) malloc(sizeof(nodeMWayTree));
        nodesArr[i]->key=i+1;
        nodesArr[i]->currsize=0;
        nodesArr[i]->maxsize=3;
        nodesArr[i]->children=(nodeMWayTree **) malloc(sizeof(nodeMWayTree *)*3);
        nodesArr[i]->children[0]=NULL;
        nodesArr[i]->children[1]=NULL;
        nodesArr[i]->children[2]=NULL;
    }
   //O(n) time to create the array of nodes
    for(int i=0;i<n;i++){
        if(parentArr[i]!=-1){
            //increase the size of the children array if it is almost close to the maxsize
            if(nodesArr[parentArr[i]-1]->currsize==nodesArr[parentArr[i]-1]->maxsize-1) {
                nodeMWayTree **nodeFree=(nodeMWayTree **) malloc(sizeof(nodeMWayTree*)*nodesArr[parentArr[i]-1]->currsize);
                nodesArr[parentArr[i]-1]->maxsize+=3;
                nodesArr[parentArr[i] - 1]->children = (nodeMWayTree **) realloc(nodesArr[parentArr[i] - 1]->children,nodesArr[parentArr[i] - 1]->maxsize);
                free(nodeFree);
            }
            //add to the array of children
            nodesArr[parentArr[i]-1]->children[nodesArr[parentArr[i]-1]->currsize]=nodesArr[i];
            nodesArr[parentArr[i]-1]->currsize+=1;
        }
        else root=i; //we found the root
    }

    //printf("%d",nodesArr[root]->children[0]->key);
    free(nodesArr);
    return nodesArr[root];

}
void PrettyPrintBinaryTree(node *root,int space){
    for(int i=0;i<space;i++)
        printf("   ");

    printf("%d\n",root->key);
    if(root->left!=NULL)
    PrettyPrintBinaryTree(root->left,space+1);

    if(root->right!=NULL)
    PrettyPrintBinaryTree(root->right,space);
}
node *MultiWayToBinaryTree(nodeMWayTree *root){
   node *Node=(node *)malloc(sizeof(node));
   Node->key=root->key;
   Node->left=NULL;
   Node->right=NULL;

   if(root->children[0]!=NULL) {
       Node->left= MultiWayToBinaryTree(root->children[0]);
       node *nextNode=(node *) malloc(sizeof(node));
       nextNode=Node->left;
       for (int i = 1; i < root->currsize; i++){
           nextNode->right= MultiWayToBinaryTree(root->children[i]);
           nextNode=nextNode->right;
       }
   }
    return Node;
}
int main() {

    freopen("output.out","w",stdout);

    int n;
    scanf("%d",&n);
    int *parentArr=(int *) malloc(sizeof(int)*n);
    for(int i=0;i<n;i++)
        scanf("%d",&parentArr[i]);

//    9
//    2 7 5 2 7 7 -1 5 2


    printf("---Pretty Print Parent Representation---\n");
    PrettyPrintParentRepresentation(parentArr,n,-1,0);
    nodeMWayTree *root=ParentRepToMultiWayTree(parentArr,n);
    free(parentArr);
    printf("\n---Pretty Print Multi-Way Representation---\n");
    PrettyPrintMultiWayTree(root,0);
    node *rootBinary= MultiWayToBinaryTree(root);
    printf("\n---Pretty Print Binary Tree---\n");
    PrettyPrintBinaryTree(rootBinary,0);

    return 0;
}
