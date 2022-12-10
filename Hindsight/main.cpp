#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "StockNode.h"
#include "MapList.h"
#include <string>
#include "Strategy.h"

using namespace std;

enum commandCode
{
    insertStock,
    removeStock,
    calculateStrategy,
    printAllTickers,
    printTickersToUpdate,
    printCurrentTickers,
    insertRandomTickers,
    removeRandomTickers,
    reset,
    help,
    invalid
};
commandCode stringToCommand (string& s)
{
    if (s =="h") return help;
    if (s == "i") return insertStock;
    if (s == "r") return removeStock;
    if (s == "$") return  calculateStrategy;
    if (s == "all") return  printAllTickers;
    if (s == "up")   return printTickersToUpdate;
    if (s == "curr") return printCurrentTickers;
    if (s == "ir") return insertRandomTickers;
    if (s == "rr") return  removeRandomTickers;
    if (s == "x") return reset;

    return invalid;
}


int main()
{
    Strategy strategy;

    cout <<"Welcome to Hindsight!" << endl;
    cout <<"Type 'h' for a full list of commands"<<endl;
    cout << "Insert/Remove stocks from the SP500 to compute the best daytrading strategy from the past 2 years!" << endl; 
    
    while (true)
    {
        
        string input;

        cout <<"Enter a command (type 'h' for full command list)" << endl;
        cin >> input;

        commandCode cmd = stringToCommand(input);
        switch (cmd)
        {
            case help:
                strategy.printHelp();
                break;
            case insertStock:
                strategy.insertStock();
                break;
            case removeStock:
                strategy.removeStock();
                break;
            case calculateStrategy:
                strategy.calculateStrategy();
                break;
            case printAllTickers:
                strategy.printAllTickers();
                break;
            case printTickersToUpdate:
                strategy.printTickersToUpdate();
                break;
            case printCurrentTickers:
                strategy.printCurrentTickers();
                break;
            case insertRandomTickers:
                strategy.insertRandomTickers();
                break;
            case removeRandomTickers:
                strategy.removeRandomTickers();
                break;
            case reset:
                break;
            default:
                cout <<"invalid command, try again" << endl;
                break;
        }
    }

    return 0;
}

