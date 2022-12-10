#pragma once
#include <iostream>
#include <algorithm>
#include <deque>
#include <string>
using namespace std;

struct TreeNode
{
    string ticker;
    float ret;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    int level = 1;
    TreeNode(float f, string s):ret(f),ticker(s){};
    TreeNode(const TreeNode& r)
    {
        ret = r.ret;
        ticker = r.ticker;
    }
    static int getLevel(TreeNode* node)
    {
        if (!node)
            return 0;
        
        return node->level;
    }
    bool operator<(const TreeNode& r) const 
    {
        if (this->ret == r.ret)
        return ticker < r.ticker;
    else  
        return (this->ret < r.ret);
    };
    bool operator>(const TreeNode& r) const 
    {
        if (this->ret == r.ret)
        return ticker > r.ticker;
    else  
        return (this->ret > r.ret);
    };
    TreeNode& operator=(const TreeNode& r)  
    {
        ticker = r.ticker;
        ret = r.ret;
        return *this;
    };
    
};

class AATree
{
    TreeNode* head = nullptr;
    TreeNode* removeHelp(TreeNode* root, TreeNode& key);
    TreeNode* insertHelp(TreeNode* root, TreeNode& key);
    void pInorderHelp(TreeNode* root);
    void pPreorderHelp(TreeNode* root);
    TreeNode* Rebalance(TreeNode* root); 
    void updateLevel(TreeNode* root);
    TreeNode* getSuccessor(TreeNode* root);
    TreeNode* maxRetNode = nullptr;
    void updateMaxRetNode();
 public:
    void insert(TreeNode& key);
    void remove(TreeNode& key);
    TreeNode* skew(TreeNode* root);
    TreeNode* split(TreeNode* root);
    void pInorder();
    void pPreorder();
    void pLevels();
    void printHeight(){cout << this->head->level<<endl;};
    TreeNode* getMaxRetNode() {return maxRetNode;}

};

void AATree::updateMaxRetNode()
{   
    TreeNode* curr = head;
    while (curr && curr->right)
        curr = curr->right;
    maxRetNode = curr;
}

void AATree::insert(TreeNode& key)
{
    head = insertHelp(head, key);
    if (!maxRetNode || key.ret > maxRetNode->ret )
        updateMaxRetNode();
}

TreeNode* AATree::insertHelp(TreeNode* root, TreeNode& key)
{
    if (!root)
    {
        TreeNode* forInsert = new TreeNode(key);
        return forInsert;
    }

    if (key < *root)
    {
        root->left = insertHelp(root->left,key);
    }
    else if (key > *root)
    {
        root->right = insertHelp(root->right, key);
    }
    else
    {
        return root;
    }
    root = skew(root);
    root = split(root);
    return root;
    
}


void AATree::remove(TreeNode& key)
{
    head = removeHelp(head, key);
    if (!maxRetNode)
        updateMaxRetNode();
}

TreeNode* AATree::removeHelp(TreeNode* root, TreeNode& key)
{
    if (!root)
        return root;

    if (key < *root)
    {
        root->left = removeHelp(root->left,key);
    }
    else if (key > *root)
    {
        root->right = removeHelp(root->right, key);
    }
    else
    {
        if (maxRetNode && maxRetNode == root)
            maxRetNode = nullptr;

        if (!root->left && !root->right)
        {            
            delete root;
            return nullptr;   
        }
        else
        {   
            //root->val = getSuccessor(root->right)->val;
           
            TreeNode* succ = getSuccessor(root->right);
            root->ticker = succ->ticker;
            root->ret = succ->ret;
            //root->right = removeHelp(root->right,root->val);
            TreeNode toRemove = TreeNode(root->ret,root->ticker);
            root->right = removeHelp(root->right,toRemove);

        }
    }
    root = Rebalance(root);
    return root;
    
}

TreeNode* AATree::Rebalance(TreeNode* root)
{
    updateLevel(root);
    root = skew(root);
    
    if (root->right)
    {
         root->right = skew(root->right);

        if (root->right->right)
            root->right->right = skew(root->right->right);
    }
       
    root = split(root);
    root->right = split(root->right);
    
    return root;

}

void AATree::updateLevel(TreeNode* root)
{
    int minLevel = min(TreeNode::getLevel(root->left),TreeNode::getLevel(root->right))+1;
    if (root->level > minLevel)
        root->level--;
    if (root->right && root->right->level > minLevel)
        root->right->level--;
}


 TreeNode* AATree::skew(TreeNode* root)
 {
    if (!root)
        return root;

    if (root->left && root->left->level == root->level)
    {
        TreeNode* Left = root->left;
        root->left = Left->right;
        Left->right = root;
        root = Left;
    }

    return root;

 }
TreeNode* AATree::split(TreeNode* root)
{
    if (!root)
        return root;

    if (root->right && root->right->right && root->right->right->level==root->level)
    {
        TreeNode* right = root->right;
        root->right = right->left;
        right->left = root;
        right->level++;
        root = right;
    }
    return root;
}
TreeNode* AATree::getSuccessor(TreeNode* root)
{
    if (!root)
        return root;

    while(root->left)
        root = root->left;
    
    return root;
}


void AATree::pInorder()
{
    pInorderHelp(head);

}
void AATree::pPreorder()
{
    pPreorderHelp(head);

}

void AATree::pInorderHelp(TreeNode* root)
{
    if (!root)
        return;
    pInorderHelp(root->left);

   cout << root->ticker << ": " << root->ret <<", ";

    //cout << root->val << " H: " << root->level <<", ";
    //cout << root->val << " - ";

    pInorderHelp(root->right);
}
void AATree::pPreorderHelp(TreeNode* root)
{
    if (!root)
        return;
    cout << root->ticker << ": " << root->ret <<", ";

   // cout << root->val << " H: " << root->level <<", ";
    //cout << root->val << " - ";

    pPreorderHelp(root->left);
    pPreorderHelp(root->right);
}