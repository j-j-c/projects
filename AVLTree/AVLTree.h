#pragma once
#include <string>
#include <stack>
#include <queue>
#include <vector>

using namespace std;

//A Node is composed of a name and a unique id.
struct Node
{
    string name;
    int id = 0;
    Node* left = nullptr;
    Node* right = nullptr;
    int height = 1;

    Node();
    Node(string s, int x):name(s),id(x){}
    Node(string s, int x, Node* l, Node*r):name(s),id(x),left(l),right(r){}
    ~Node()
    {
       // cout << this->name << " was deleted" << endl;
    }

    static int getHeight(Node* node)
    {
        if (node == NULL)
            return 0;
        return node->height;
    }
    void updateHeight() {this->height = max(Node::getHeight(this->left),Node::getHeight(this->right))+1;}
};

class AVLTree
{
        Node* head = nullptr;
        Node* insertHelper(Node* root,string name,int id, bool& isSuccess);
        Node* removeHelper(Node* root, int key, bool& isSuccess);
        Node* removeInorderHelper(Node* root,int& currCount, int removeNumber, bool& isSuccess);
        void searchHelper(Node* root, string key,vector<int>& output);
        void searchHelper(Node* root, int key, string& output);
        void printInorderHelper(Node* root,vector<string>& output);
        void printPreorderHelper(Node* root,vector<string>& output);
        void printPostorderHelper(Node* root,vector<string>& output);
        void printLevelCountHelper(Node* root,string& output);
        void TreeDeleter(Node* node);
        Node* getInorderSuccessor(Node* root);
        Node* rotateLeft(Node* node);
        Node* rotateRight(Node* node);

    public:
        AVLTree(){};
        ~AVLTree();
        bool insert(string name,int id);
        bool remove(int key);
        bool removeInorder(int x);
        bool search(string name, vector<int>& output);
        bool search(int key, string& output);
        bool printInorder(vector<string>& output);
        bool printPreorder(vector<string>& output);
        bool printPostorder(vector<string>& output);
        bool printLevelCount(string& output);
};

//initiates node deletion
AVLTree::~AVLTree()
{
    TreeDeleter(this->head);
}

//recursively deletes nodes
void AVLTree::TreeDeleter(Node* root)
{
    if (root == nullptr)
        return;

    TreeDeleter(root->left);
    TreeDeleter(root->right);
    delete root;
    return;      
}

//Deletes the n-th inorder node.
bool AVLTree::removeInorder(int x)
{
    int currCount = 0;
    bool isSuccess = false;
    this->head = removeInorderHelper(head, currCount, x, isSuccess);
    return isSuccess;
}

Node* AVLTree::removeInorderHelper(Node* root,int& currCount, int removeNumber, bool& isSuccess)
{   
    if (root == nullptr)
        return root;


    root->left = removeInorderHelper(root->left,currCount,removeNumber,isSuccess);
    
    if (currCount == removeNumber)
    {
        currCount++;
        isSuccess = true;
        Node* inOrderSuccessor = getInorderSuccessor(root);
        delete root;
        return inOrderSuccessor;
    }
    //If the node has been deleted (or end of nodes reached), this code executes, preventing more recursive calls when returning up the stack
    else if (currCount > removeNumber)
    {
        root->updateHeight();
        return root;
    }

    currCount++;
    root->right = removeInorderHelper(root->right,currCount,removeNumber,isSuccess);
    root->updateHeight();
    return root;

}

bool AVLTree::insert(string name, int id)
{
    bool isSuccess = true;
    this->head = insertHelper(this->head, name, id, isSuccess);
    return isSuccess;
}

Node* AVLTree::insertHelper(Node* root,string name, int id, bool& isSuccess)
{
    if (root == NULL)
    {
        Node* n = new Node(name,id);
        return n;
    }

    //if id already exists
    if (root->id == id)
    {
        isSuccess = false;
        return root;
    }
        
    if(id < root->id)
        root->left = insertHelper(root->left,name,id,isSuccess);
    else if(id > root->id)
        root->right = insertHelper(root->right,name,id,isSuccess);

    //Parent's Right child is longer than left. Need left rotation 
    if (Node::getHeight(root->left) - Node::getHeight(root->right) < -1)
    {   
        //Check for Right-Left case. If so, perform right rotation on right child
        if (Node::getHeight(root->right->left) - Node::getHeight(root->right->right) == 1)
            root->right = rotateRight(root->right);

        root = rotateLeft(root);
    }

    //Parent's Left child is longer than right. Need right rotation 
    else if (Node::getHeight(root->left) - Node::getHeight(root->right) > 1)
    {   
        //Check for Left-Right case. If so, perform left rotation on left child
        if (Node::getHeight(root->left->left) - Node::getHeight(root->left->right) == -1)
            root->left = rotateLeft(root->left);

        root = rotateRight(root);
    }

    root->updateHeight();
    return root;
}

//searches all nodes for the string and stores their ID's in the output vector.
bool AVLTree::search(string key, vector<int>& output)
{
    bool isSuccess = false;
    searchHelper(this->head,key,output);
    if (!output.empty())
        isSuccess = true;
 return isSuccess;   
}

void AVLTree::searchHelper(Node* root, string key, vector<int>& output)
{
    if (root == NULL)
        return ;

    if (root->name == key)
        output.push_back(root->id);
    
    searchHelper(root->left, key, output);
    searchHelper(root->right, key, output);
    return;
}

//performs binary search for the node with the ID key and stores the name in the output string.
bool AVLTree::search(int key, string& output)
{
    bool isSuccess = false; 
     searchHelper(this->head,key, output);  
    if (!output.empty())
        isSuccess = true;
 return isSuccess;  
}

void AVLTree::searchHelper(Node* root, int key, string& output)
{

    if (root == NULL)
        return;

    //key found, set the output to root name and terminate search
    if (root->id == key)
    {
        output = root->name;
        return;
    }
    
   else if (key < root->id)
        searchHelper(root->left, key, output);
    else
        searchHelper(root->right, key, output);
    return;
    
}

//Removes the node with the associated key.
bool AVLTree::remove(int key)
{
    bool isSuccess = false;
    this->head = removeHelper(this->head,key,isSuccess);
    return isSuccess;
}

Node* AVLTree::removeHelper(Node* root, int key, bool& isSuccess)
{   
    if (root == nullptr)
        return nullptr;

    if (root->id == key)
    {
        isSuccess = true;
        Node* inOrderSuccessor =  getInorderSuccessor(root);
        delete root;
        return inOrderSuccessor;
    }
    else if (key < root->id)
    {
        root->left = removeHelper(root->left,key,isSuccess);
        root->updateHeight();
    }
    else 
    {
        root->right = removeHelper(root->right,key,isSuccess);
        root->updateHeight();
    }

    return root;
}

//Finds and returns the inorder successor of root node
Node* AVLTree::getInorderSuccessor(Node* root)
{
    stack<Node*> nodesNeedUpdateHeight;

    //two-child case. Finds left-most child of the right child.
    if (root->left != nullptr && root->right != nullptr)
    {
        Node* successor = root->right;
        Node* parentofSuc = nullptr;

        //travels down left child of root->right. Keeps track of parent
        while (successor->left != nullptr)
        {  
            parentofSuc = successor;
            nodesNeedUpdateHeight.push(parentofSuc);
            successor = successor->left;
        }
        if (parentofSuc != nullptr)
        {
            parentofSuc->left = successor->right;
        }
        successor->left = root->left;

        //Prevents successor from referencing itself
        if (successor != root->right)
            successor->right = root->right;

        while(!nodesNeedUpdateHeight.empty())
        {
            nodesNeedUpdateHeight.top()->updateHeight();
            nodesNeedUpdateHeight.pop();
        }

        successor->updateHeight();
        return successor;
    }

    //one child cases:
    else if (root->left != nullptr)
        return root->left;

    else return root->right;

}
//updates vector of strings with the nodes from an inorder traversal
bool AVLTree::printInorder(vector<string>& output)
{
    printInorderHelper(this->head,output);
    if (!output.empty())
        return true;
    return false;
}
//updates vector of strings with the nodes from a preorder traversal
bool AVLTree::printPreorder(vector<string>& output)
{
    printPreorderHelper(this->head,output);
    if (!output.empty())
        return true;
    return false;
}
//updates vector of strings with the nodes from an postorder traversal
bool AVLTree::printPostorder(vector<string>& output)
{
    printPostorderHelper(this->head,output);
    if (!output.empty())
        return true;
    return false;
}

void AVLTree::printInorderHelper(Node* root, vector<string>& output)
{
    if (root == NULL)
        return ;
    
    printInorderHelper(root->left,output);
    output.push_back(root->name);
    printInorderHelper(root->right,output);
    return ;
}

void AVLTree::printPreorderHelper(Node* root, vector<string>& output)
{
    if (root == NULL)
        return ;
    
    output.push_back(root->name);
    printPreorderHelper(root->left,output);
    printPreorderHelper(root->right,output);
    return ;
}
void AVLTree::printPostorderHelper(Node* root, vector<string>& output)
{
    if (root == NULL)
        return ;
    
    printPostorderHelper(root->left,output);
    printPostorderHelper(root->right,output);
    output.push_back(root->name);
    return ;
}

//updates string with the # of levels in tree or 0 if tree is empty
bool AVLTree::printLevelCount(string& output)
{
    bool isSuccess = false;
    printLevelCountHelper(this->head, output);  
    if (!output.empty())
        isSuccess = true;   
     return isSuccess; 

}
void AVLTree::printLevelCountHelper(Node* root,string& output)
{
    if (root == nullptr)
    {
        output = "0";
        return;
    }
    queue<Node*> nodes;
    nodes.push(root);
    int currSize = 0;
    int count = 0;
    while (!nodes.empty())
    {
        currSize = nodes.size();
        for (int i =0; i < currSize; i++)
        {
            Node* currLeft = nodes.front()->left;
            Node* currRight = nodes.front()->right;

            if (currLeft != nullptr)
                nodes.push(currLeft);
            if (currRight != nullptr)
                nodes.push(currRight);

            nodes.pop();
        }
        count++;
    }
    output = to_string(count);
}

//Performs left-rotation on the root node
Node* AVLTree::rotateLeft(Node *root)
{
    Node* rightChild = root->right;
    Node* leftGc = rightChild->left;

    rightChild->left = root;
    root->right = leftGc;
    root->updateHeight();

    return rightChild;
}
//Performs right-rotation on the root node
Node* AVLTree::rotateRight(Node *root)
{
    Node* leftChild = root->left;
    Node* rightGc = leftChild->right;

    leftChild->right = root;
    root->left = rightGc;
    root->updateHeight();

    return leftChild;
}
