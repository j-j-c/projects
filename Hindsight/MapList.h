#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "StockNode.h"

using namespace std;

struct MapListNode
{
    string ticker;
    float ret;
    MapListNode* next = nullptr;
    MapListNode* prev = nullptr;
    MapListNode(){};
    MapListNode(float f, string s):ret(f),ticker(s){};
    bool operator<(const MapListNode& r) const 
    {
        if (this->ret == r.ret)
        return ticker < r.ticker;
    else  
        return (this->ret < r.ret);
    };
   bool operator!=(const MapListNode& r) const 
    {
        return (this->ret != r.ret || this->ticker != r.ticker);
    };
};

bool nodecomp (MapListNode left, MapListNode right)
 {
    if (left.ret == right.ret)
        return left.ticker < right.ticker;
    else  
        return (left.ret < right.ret);
}

class MyList
{
        MapListNode* head = nullptr;
    public:
        void insert(MapListNode* n);
        void del(MapListNode* n);
        void print();
        MapListNode* getHead() {return head;};
};

void MyList::insert(MapListNode* n)
{
   if (!this->head)
   {    head = n;
       return;
   }
   
    MapListNode* curr = head;
    if (n->ret > curr->ret)
    {
        head = n;
        n->next = curr;
        curr->prev = n;
        return;
    }
    while (curr->next && curr->next->ret > n->ret)
        curr = curr->next;

    n->next = curr->next;
    n->prev = curr;
    if (curr->next)
        curr->next->prev = n;
    curr->next = n;

    return;
}


void MyList::del(MapListNode* n)
{
    if (n->prev)
        n->prev->next = n->next;
    if (n->next)
        n->next->prev = n->prev;
    
    if (head == n)
        head = n->next;
    
    delete n;
}
void MyList::print()
{   
    MapListNode* curr = head;
    while (curr)
    {
         cout << curr->ticker<<": "<< curr->ret << endl;
         curr = curr->next;
    }
        
};

class MapList
{
        vector<MyList> dateList;
        unordered_map<string, unordered_map<int, MapListNode*>*> mapLists;
        int size = 0;

    public:
        MapList(int s):dateList(s),size(s){};
        bool insert(Stock* stock);
        bool remove(Stock* stock);
        void calcStrat();
        void printStrat();
        float getMax(int i);      
        vector<MapListNode*> strategy;  

};

bool MapList::insert(Stock* stockPt)
{
    Stock& stock = *(stockPt);
    unordered_map<int, MapListNode*>* stockDates = new unordered_map<int, MapListNode*>;
    string ticker = stock.ticker;

    for (int i =0; i < size; i++)
    {
        float r = stock.rets[i];
        MapListNode* n = new MapListNode(r,stock.ticker);
        (*stockDates)[i] = n;
        dateList[i].insert(n);
    }
    mapLists[ticker] = stockDates;
    return true;
}

bool MapList::remove(Stock* stockPt)
{
    Stock& stock = *(stockPt);
   string ticker = stock.ticker;
   unordered_map<int, MapListNode*>* stockNodes = mapLists[ticker];
    for (int i =0; i < size; i++)
    {
        MapListNode* curr = (*stockNodes)[i];
        dateList[i].del(curr);
    }
    delete stockNodes;
    mapLists.erase(ticker);
    return true;
}

 void MapList::printStrat()
{
    for (auto e : strategy)
    {
        cout << e->ticker <<": " << e->ret << endl;
    }
}
 void MapList::calcStrat()
 {
    strategy.clear();
    for (int i =0; i < size; i++)
    {
        MapListNode* maxReturn = (dateList[i]).getHead();
        strategy.push_back(maxReturn);
    }
        
 }