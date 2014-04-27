#include "AVLT.h"
#include <iomanip>
#include <string.h>

BST::BST()
:myRoot(0)
{}

// searches through the tree to find a node where the teamName is 
// equal to the paramater item
nodePointer BST::searchForIndex(const string &item) const {
    // Sets the locPtr to start at the root
    nodePointer locptr = myRoot;
    // Flag used to allow the while loop to end when the two items match
    int flag4 = 0;
    while(flag4 == 0 && locptr != 0) {
        // if the teamName is equal to the item then we have found our node
        if(locptr->teamName.compare(0, item.length(), item) == 0) flag4 = 1;
        // if the item is less than the currentNode teamName and a left child exists, 
        // set the node to be the nodes left node
        else if(item < locptr->teamName && locptr->left != 0) locptr = locptr->left;
        // if the item is greater than the currentNode teamName and a right child exists, 
        // set the node to be the nodes right node
        else if (locptr->teamName < item && locptr->right != 0) locptr = locptr->right;
    }
    // return the node that was found
    return locptr;    
}

// used to inset into the tree
void BST::insert(string &team, int &index) {
    // calls the insert2 will nodePointer being the root
    insert2(myRoot, team, index);
}

// Inserts the node into the tree
// by shifting through the tree until it finds an empty child node
// if moves to it's left or right child based on the comparing the string names
nodePointer BST::insert2(nodePointer &subtreeRoot, string &team, int &index) {
    // If the current node is empty, then fill it in with a new node
    // with the values of the parameters
    if(subtreeRoot == 0) {
    	subtreeRoot = new node(team, index);
        return subtreeRoot;
  	}
    // if the teamName is less than the current node's teamName
    if(team < subtreeRoot->teamName) {
    	subtreeRoot->left = insert2(subtreeRoot->left, team, index);
        subtreeRoot = balance(subtreeRoot);
	}
    // if the current nodes teamName is less than the teamName
    else if(subtreeRoot->teamName < team) {
    	subtreeRoot->right= insert2(subtreeRoot->right, team, index);
        subtreeRoot = balance(subtreeRoot);
	}
    return subtreeRoot;
}

// Deletes/clears the entire tree by calling deleteTree2()
void BST::deleteTree() {
    deleteTree2(myRoot);
}

// Clears/delete the entire tree by recursively calling
// the deleteTree2() algorithm
void BST::deleteTree2(nodePointer &subtreeRoot) {
    if(subtreeRoot) {
        // if the left child exists, call deleteTree2()
        if(subtreeRoot->left != 0) deleteTree2(subtreeRoot->left);
        // if the right child exists, call deleteTree2()
        if(subtreeRoot->right != 0) deleteTree2(subtreeRoot->right);
        // Set the index to be 0 and the root to be null
        subtreeRoot->index = 0;
        subtreeRoot = 0;
    }
    return;
}

// Used for testing
void BST::graphAux(ostream &out, int indent, nodePointer subtreeRoot) {
	if(subtreeRoot != 0) {
		graphAux(out, indent+8, subtreeRoot->right);
		out << setw(indent) << " " << subtreeRoot->teamName << " " << subtreeRoot->index << endl;
		graphAux(out, indent+8, subtreeRoot->left);
	}
}

// Used for testing
void BST::graph() {
	graphAux(cout, 1, myRoot);  
}

// Determines the height of tree
int BST::height(nodePointer subtreeRoot) {
    if(subtreeRoot != 0) {
        int leftHeight = height(subtreeRoot->left);
        int rightHeight = height(subtreeRoot->right);
        return max(leftHeight, rightHeight) + 1;
    }
    return 0;
}

// Used to balance tree
int BST::diff(nodePointer subtreeRoot) {
    int leftHeight = height(subtreeRoot->left);
    int rightHeight = height(subtreeRoot->right);
    return leftHeight-rightHeight;
}

// rotate left left for balance height
nodePointer BST::llRotation(nodePointer nP) {
    nodePointer temp;
    temp = nP->left;
    nP->left = temp->right;
    temp->right = nP;
    return temp;
}

// rotate right right for balance height
nodePointer BST::rrRotation(nodePointer nP) {
    nodePointer temp;
    temp = nP->right;
    nP->right = temp->left;
    temp->left = nP;
    return temp;
}

// rotate left right for balance height
nodePointer BST::lrRotation(nodePointer nP) {
    nodePointer temp;
    temp = nP->left;
    nP->left = rrRotation(temp);
    return llRotation(nP);
}
// rotate right left for balance height
nodePointer BST::rlRotation(nodePointer nP) {
    nodePointer temp;
    temp = nP->right;
    nP->right = llRotation(temp);
    return rrRotation(nP);
}

// Correctly balances out the height of the tree from the subroot down
nodePointer BST::balance(nodePointer temp) {
    int balance = diff(temp);
    if(balance > 1) {
        if(diff(temp->left) > 0) temp = llRotation(temp);
        else temp = lrRotation(temp);
    }
    else if(balance < -1) {
        if(diff(temp->right) > 0) temp = rlRotation(temp);
        else temp = rrRotation(temp);
    }
    return temp;
}
