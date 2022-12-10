#pragma once
#include "StockNode.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "MapList.h"
#include "TreeStrat.h"
#include <stdlib.h>  
#include<bits/stdc++.h>
#include <math.h> 
#include <chrono>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

class Strategy
{       
        MapList* mapList = nullptr;
        TreeStrat* treeStrat = nullptr;

        set<string> stocksToInsert;
        set<string> stocksToRemove;
        unordered_map<int, string> tickers;
        vector<string> dates;
        bool datesFilled = false;
        unordered_map<string,Stock*> stockData;
        
        void loadTickers();

    public:
        Strategy();
        bool insertStock();
        bool insertStock(string& s);
        bool removeStock();
        bool calculateStrategy();
        void insertRandomTickers();
        void removeRandomTickers();
        void printAllTickers();
        void printTickersToUpdate();
        void printCurrentTickers();
        void printStrategy();
        void printHelp();
};

Strategy::Strategy()
{
    loadTickers();
}

//Gets user input for ticker symbol input to remove from current strategy
bool Strategy::removeStock()
{
    string stockTicker;
    cout << "Type the ticker you want to remove: " << endl;
    cin >> stockTicker;
    int numOfCurrentTickers = stockData.size();
    if ((stocksToRemove.size()+1) > numOfCurrentTickers)
    {
        cout <<"Trying to remove more stocks than available, try again" << endl;
        return false;
    }
   stocksToRemove.insert(stockTicker);
   return true;
}

//Calculates the optimal trading strategy using both the AA-Tree and MapList
//Displays the execution time for both data structures
bool Strategy::calculateStrategy()
{
    
     /* //used for debugging
    static int callcount=0;
    callcount++;
    */

    int size = dates.size();
    if (!mapList)
       mapList = new MapList(size);
    if (!treeStrat)
         treeStrat = new TreeStrat(size);

    MapList* ml = this->mapList;
    TreeStrat* ts = this->treeStrat;

    auto starttimeml = high_resolution_clock::now();
    for (auto s : stocksToInsert)
    {
        ml->insert(stockData[s]);
    }
    for (auto s: stocksToRemove)
    {
        ml->remove(stockData[s]);
    }
    ml->calcStrat();
    auto stoptimeml = high_resolution_clock::now();
    auto timediffml = duration_cast<microseconds>(stoptimeml - starttimeml);
 
 
    auto starttimets = high_resolution_clock::now();
    for (auto s : stocksToInsert)
    {
        ts->insert(stockData[s]);
    }
    for (auto s: stocksToRemove)
    {
        ts->remove(stockData[s]);
    }
    ts->calcStrat();
    auto stoptimets = high_resolution_clock::now();
    auto timediffts = duration_cast<microseconds>(stoptimets - starttimets);

    for (auto s: stocksToRemove)
    {
        delete stockData[s];
        stockData.erase(s);
    }

    stocksToInsert.clear();
    stocksToRemove.clear();
    printStrategy();
    
    cout << "(tree)Time to calculate strategy: " << timediffts.count() << " micro-seconds" << endl;
    cout << "(maplist)Time to calculate strategy: " << timediffml.count() << " micro-seconds" << endl;
    return true;
}

//Gets user input for ticker symbol input to remove from current strategy
bool Strategy::insertStock()
{
    string stockTicker;
    cout << "Type the ticker you want to insert:" << endl;
    cin >> stockTicker;
    insertStock(stockTicker);
    return true;
}

//Loads the inserted stock data into the strategy
bool Strategy::insertStock(string& s)
{
    bool datesFilled = dates.size() == 0 ? false : true;

    string stockTicker = s;

    fstream iFile;
    iFile.open("stockData/"+stockTicker+".csv",ios::in);
    string line;
    string date;
    float ret;
    vector<float> rets;
    int rowCount = 0;

    while (getline(iFile, line))
    {    
        if (line.empty())
            break;

    rowCount++;
    stringstream ss(line);
    getline(ss, date, ',');
    if (!datesFilled)
        dates.push_back(date);
    ss >> ret;
    rets.push_back(ret);
    }

    float* retArr = new float[rowCount];
    for (int i =0; i < rowCount; i++)
    {
        retArr[i] = rets[i];
    }
    
    Stock* currstock = new Stock(stockTicker, retArr,rowCount);
    stockData[stockTicker] = currstock;

    stocksToInsert.insert(stockTicker);


    return true;

}
//Loads all tickers into the program
 void Strategy::loadTickers()
{
    string line;
    string ticker;

    fstream iFile;
    iFile.open("stocklist.txt",ios::in);

    getline(iFile, line);
    int index = 0;
    while (getline(iFile, line))
    { 
        stringstream ss(line);
        getline(ss, ticker, ',');
        tickers[index] = ticker;
        index++;
    }

}

//Prints all available tickers
void Strategy::printAllTickers()
{
    int colCount = 0;
    
    for (int i =0; i < tickers.size(); i++)
    {
        cout << tickers[i] << ", ";
        colCount++;
        if (colCount%10 == 0)
            cout << endl;
    }
    cout << endl;
}

//Prints tickers that are to be updated
void Strategy::printTickersToUpdate()
{   
    int colCount = 0;
    cout <<"Stocks to insert: " << endl;
    for (auto t : stocksToInsert)
    {
        cout << t << ", ";
        colCount++;
        if (colCount%10 == 0)
            cout << endl;
    }  
    cout << endl;
    colCount = 0;
    cout <<"Stocks to remove: " << endl;
    for (auto t : stocksToRemove)
    {
        cout << t << ", ";
        colCount++;
        if (colCount%10 == 0)
            cout << endl;
    }    
cout << endl;
}

//Prints tickers in the current strategy
void Strategy::printCurrentTickers()
{
    
    cout << "Current Stocks: "<< endl;
    int colCount = 0;
    for (auto s : stockData)
    {
        cout << s.first <<", ";
        colCount++;
        if (colCount%10 == 0)
            cout << endl;
    }
    cout << endl;
    cout << "# Stocks: " << stockData.size() << ". # Data Points: " << stockData.size()*dates.size() << endl;

}

//Inserts random number of ticker symbols
void Strategy::insertRandomTickers()
{
    int n;
    cout << "Enter number of random stocks to insert" << endl;
    cin >> n;

    int numOfTickers = tickers.size();
    if (n + stockData.size() > tickers.size())
        return;
    srand (time(NULL));
    int randomNum; 
    
    while (stocksToInsert.size() < n)
    {
        randomNum = rand() % numOfTickers;
        string randomTicker = tickers[randomNum];
        if (stockData.count(randomTicker))
            continue;

        stocksToInsert.insert(tickers[randomNum]);
    }
    for (auto s : stocksToInsert)
        insertStock(s);

}

//Removes random number of ticker symbols
void Strategy::removeRandomTickers()
{   
    int numToRemove;
    int numOfCurrentTickers = stockData.size();
    cout << "Enter number of random stocks to remove. (Currently, up to " << numOfCurrentTickers <<" can be removed.)" <<  endl;
    cin >> numToRemove;
   
    if (numToRemove+(stocksToRemove.size()) > numOfCurrentTickers)
    {
        cout <<"Trying to remove more stocks than available, try again" << endl;
        return;
    }
       

    srand (time(NULL));
   
    vector<string> currTickers;

    for (auto s: stockData)
        currTickers.push_back(s.first);

    int randomNum;
    while (stocksToRemove.size() < numToRemove)
    {
        randomNum = rand() % currTickers.size();
        stocksToRemove.insert(currTickers[randomNum]);
    }

}

//Displays the optimal trading strategy with the chosen tickers
void Strategy::printStrategy()
{
    vector<TreeNode*>& treeStrategy = treeStrat->strategy;
    vector<MapListNode*>& mapListStrategy = mapList->strategy;
    int colCount = 0;
    double returnTotal = 0.0;
    for (int i =0; i < dates.size(); i++)
    {
        if (treeStrategy[i]->ret !=mapListStrategy[i]->ret )
            cout<<"NOT EQUAL" << endl;
        returnTotal += mapListStrategy[i]->ret;
        cout << dates[i] <<": " << mapListStrategy[i]->ticker <<" : " <<  fixed << setprecision(2) << mapListStrategy[i]->ret <<  ", ";
        colCount++;
        if (colCount%5 ==0)
            cout << endl;
    }
    cout << endl;
    cout << "total log returns: " << returnTotal << endl;

    double realReturn = exp(returnTotal);
    double percentChange = (realReturn-1.0)*100;
    bool gain = percentChange > 0 ? true : false;
    cout << "Percent Gain/Loss: " <<  fixed << setprecision(2) << percentChange <<"% ";
    if (gain)
        cout << "gain" << endl;
    else   
        cout << "loss" << endl;

}

//Prints all available commands
void Strategy::printHelp()
{
    cout <<"i: insert a stock" << endl;
    cout << "r: remove a stock" << endl;
    cout << "ir: insert random stocks" << endl;
    cout << "rr: remove random stocks" << endl;
    cout << "up: list stocks to be updated" << endl;
    cout << "curr: list current stocks in strategy" << endl;
    cout <<"$: Compute optimal trading strategy" << endl;
    cout << "all: list all available ticker symbols" << endl;
}