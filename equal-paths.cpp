#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool height(Node* root, int* leafh, int nodeh);
bool equalPaths(Node * root)

{
    // Add your code below
    int leafh=0;
    int nodeh=0;
    return height(root, &leafh, nodeh);

}


bool height(Node* root, int* leafh, int nodeh) {
    if(root == NULL) {
        return true;
    }
    
    
    if(root->left == NULL && root->right == NULL) {
        
        if(*leafh == 0) {
            *leafh = nodeh;
            return true;
        }
        
        
        if(nodeh== *leafh){
            return true;
        } 
        else{
            return false;
        } 
    }
    
    return height(root->right, leafh, nodeh + 1) && height(root->left, leafh, nodeh + 1);
}
