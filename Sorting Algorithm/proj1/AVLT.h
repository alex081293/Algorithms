#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

// Data structure node
struct node {
    string teamName;
    int index;
    node *left;
    node *right;

    // Constructor
    node(string item, int num) {
    	teamName = item;
    	index = num;
    	left = 0;
    	right = 0;
    }
};
typedef node *nodePointer;
class BST {
private:
    nodePointer myRoot;
public:
	BST();
    // Search
    nodePointer searchForIndex(const string &item) const;
    // insert
    void insert(string &item, int &index);
    // insert used inside of insert()
    nodePointer insert2(nodePointer &subtreeRoot, string &item, int &index);
    // delete/clear the tree
    void deleteTree();
    // used inside deleteTree()
    void deleteTree2(nodePointer&);
    // Graphs the tree
    // Used for testing
    void graph();
    void graphAux(ostream &out, int indent, nodePointer subtreeRoot);
    // all used for height balancing the tree
    nodePointer rrRotation(nodePointer);
    nodePointer llRotation(nodePointer);
    nodePointer lrRotation(nodePointer);
    nodePointer rlRotation(nodePointer);
    nodePointer balance(nodePointer);
    // Determines the current heigh of that node
    int height(nodePointer);
    // Determines the difference in hight compare to the rest of the tree
    int diff(nodePointer);
};