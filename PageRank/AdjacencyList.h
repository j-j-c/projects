#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <utility> 
#include <set>
#include <unordered_set>
#include <iomanip>

using namespace std;



class AdjacencyList 
{ 
// set<string> orderedNames;
 unordered_map<string, int> outGoingCount;
 map<string, vector<pair<string,int>>> websites;
 unordered_map<string, float> pageRanks;

 public: 
 void insertEdge(string from, string to);
 void setInitialValues();
  void PageRank(int n);
  void printVals();
};

  void AdjacencyList::insertEdge(string from, string to)
 {
    
    websites[to].push_back(make_pair(from,0));
   

    if (!websites.count(from))
        websites[from] = {};

    outGoingCount[from]++;
    //orderedNames.insert(from);
    //orderedNames.insert(to);

 };

 void AdjacencyList::setInitialValues()
 {
    int totalWebsites = websites.size();
    float initialRank = 1.0/totalWebsites;
    
    for (auto& ws: websites)
    {
        pageRanks[ws.first] = initialRank;
        for (auto& e : ws.second)
            e.second = outGoingCount[e.first];
    }


 }

void AdjacencyList::PageRank(int n)
  {
    while (--n > 0)
    {
        unordered_map<string, float> newRanks;
        for (const auto& ws: websites)
        {
            float newRank = 0.0;
            for (const auto& fromSitesPairs: ws.second)
            {
                newRank += pageRanks[fromSitesPairs.first]*(1.0/fromSitesPairs.second);
                
            }
            newRanks[ws.first] = newRank;
        }

        for (const auto& nr: newRanks)
            pageRanks[nr.first] = nr.second;
    }
} ;

void AdjacencyList::printVals()
{
    for (const auto&  ws: websites)
    {
        cout << fixed << showpoint;
        cout<< setprecision(2);
        cout <<  ws.first << " " << pageRanks[ws.first] << endl;
    }
}
