#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <utility> 
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

class AdjacencyList 
{ 
    unordered_map<string, int> outGoingCount;
    map<string, vector<pair<string,int>>> websites;
    unordered_map<string, float> pageRanks;

 public: 
    void insertEdge(string from, string to);
    void setInitialValues();
    void pageRank(int n);
    void printRanks();
};

/*
-Adds both the "from" website and "to" website to the map of websites. 
-"to" websites contain vector of incoming nodes with corresponding out_degree initially set to 0
-Also counts number of outgoing links each website has by incrementing the associated integer in outGoingCount unordered_map
*/
void AdjacencyList::insertEdge(string from, string to)
 {
    websites[to].push_back(make_pair(from,0));
   
   //initialize "from" website if it hasn't been already
    if (!websites.count(from))
        websites[from] = {};

    //increment from's outgoing degree by 1.
    outGoingCount[from]++;
 }

/*
-Initializes the page ranks and sets the out_degree for each websites incoming nodes
-Calculates inverse total # of websites. Sets each website's pagerank to this in the pageRank unordered_map.
-Iterates through each website's incoming nodes and sets the corresponding out_degree by reading values from outGoingCount
*/
 void AdjacencyList::setInitialValues()
 {
    int totalWebsites = websites.size();
    float initialRank = 1.0/totalWebsites;
    
    for (auto& ws: websites)
    {
        pageRanks[ws.first] = initialRank;
        for (auto& v : ws.second)
            v.second = outGoingCount[v.first];
    }
 }

/*
-Performs pagerank operation p times. For p == 1, pageRanks correspond to r(0) and will not execute inner while loop.
-Creates copy of pageRanks named oldRanks
-Iterates through all websites and their vector of incoming nodes, multiplying the inverse out_degree by its oldRank value and adding this
sum to the newRank value.
-Updates pageRank with the new rank.
*/
void AdjacencyList::pageRank(int p)
  {
    while (--p > 0)
    {
        unordered_map<string, float> oldRanks = pageRanks;
        for (const auto& ws: websites)
        {
            float newRank = 0.0;
            for (const auto& fromSitesPairs: ws.second)
            {
                string incomingWebsite = fromSitesPairs.first;
                int outDegree = fromSitesPairs.second;

                //( rank of website) = sum(kr/out_degree(k)) for all k incoming nodes.
                newRank += oldRanks[incomingWebsite]*(1.0/outDegree);
            }
            //updates pageRank with new rank
            pageRanks[ws.first] = newRank;
        }
    }
} 

/*
-Lists page website names in alphabetical order with corresponding page rank
-Since pageRanks are stored in unordered_map, we iterate through the website map names since the map keeps them alphabetized
-We use the website name as the key to lookup the rank in the pageRank unordered_map in O(1) time.
*/
void AdjacencyList::printRanks()
{
    for (auto iter = websites.begin(); iter != websites.end(); iter++)
    {
        cout << fixed << showpoint;
        cout<< setprecision(2);
        cout <<  iter->first << " " << pageRanks[iter->first];

        //only prints newline for elements before the last element 
        if (iter != --websites.end())
            cout << endl;
    }

    /*
    for (const auto&  ws: websites)
    {
        cout << fixed << showpoint;
        cout<< setprecision(2);
        cout <<  ws.first << " " << pageRanks[ws.first] << endl;
    }*/
}

int main()
{
    AdjacencyList graph;

    int numCommands = 0;
    int power_iterations = 0;
   
	string input;
    
	getline(cin, input);
    istringstream inStream(input);

    //number of commands to read before terminating while loop
    inStream >> numCommands;
    //number of power iterations to perform with page rank
    inStream >> power_iterations;
	
    //stores the "from" website string and "to" website string for inserting edges
    string from, to;

	while (numCommands-- > 0 )
	{
		getline(cin, input);
        istringstream in(input);
        in >> from;
        in >> to;
		graph.insertEdge(from, to);
	}
    graph.setInitialValues();
    graph.pageRank(power_iterations);
    graph.printRanks();
	return 0;
} 
