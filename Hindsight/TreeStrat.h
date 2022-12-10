#pragma once
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include "stockNode.h"
#include "AATree.h"
using namespace std;

class TreeStrat
{
        vector<AATree> trees;
        int size = 0;

    public:
        TreeStrat(int s):size(s),trees(s){};
        bool insert(Stock* stock);
        bool remove(Stock* stock);
        void calcStrat();
        void printStrat();
        float getMax(int i);        
        vector<TreeNode*> strategy;

};

bool TreeStrat::insert(Stock* stockPt)
{
    Stock& stock = *(stockPt);
    string ticker = stock.ticker;

    for (int i =0; i < stock.size; i++)
    {
        float ret = stock.rets[i];
        TreeNode curr = TreeNode(ret, ticker);
        trees[i].insert(curr);
    }
    
    return true;
}
bool TreeStrat::remove(Stock* stockPt)
{
   Stock& stock = *(stockPt);
   string ticker = stock.ticker;
    for (int i =0; i < size; i++)
    {
        float ret = stock.rets[i];
        TreeNode curr = TreeNode(ret, ticker);
        trees[i].remove(curr);       
    }

    return true;
}

 void TreeStrat::calcStrat()
 {
    strategy.clear();
    for (int i =0; i < size; i++)
    {
        TreeNode* maxNode = trees[i].getMaxRetNode();
        strategy.push_back(maxNode);
    }      
 }

  void TreeStrat::printStrat()
{
    int colCount = 0;
    for (auto e : strategy)
    {
        if (e)
         cout << e->ticker <<": " << e->ret << endl;
         else
            cout << "N/A" <<": " << "N/A" << endl;
    }
}