#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "AVLTree.h"
#include <algorithm>

using namespace std;

//checks if the string only contains alphabet chars and/or spaces.
bool isValidString(string s)
{
	for(char c: s)
	{
		if (!isalpha(c) && c !=' ')
		return false;
	}
	return true;
}

//checks if input string is an 8 digit number
bool isValidDigit(string s)
{
	if (s.length() != 8)
		return false;

	for(char c: s)
	{
		if (!isdigit(c))
		return false;
	}
	return true;	
}

void printSuccessful(){cout << "successful" << endl; }
void printUnsuccessful(){cout << "unsuccessful" << endl;}
void print(string output){cout << output << endl;}

//prints zero-leading 8 digit numbers
void print(vector<int>& outputList)
{
	for (auto i : outputList)
		cout << setfill('0') << setw(8) << i << endl;
}

//prints contents of vector separated by commas
void printCommaSeparatedList(vector<string>& outputList)
{
	if (outputList.size() > 1)
	{
		for (auto iter = outputList.begin(); iter != outputList.end()-1; iter++)
		{
			cout  << *iter;
			if (iter+1 != outputList.end())
				cout << ", ";
			
		}
	}
	cout << outputList.back() << endl;
}

enum class CommandName {invalid,insert, remove, search,printInorder,printPreorder,printPostorder,printLevelCount, removeInorder };


class Command 
{
	CommandName name;
	int intParam=0;
	string strParam;
	bool validCommand = false;
	
    public:
        Command(string full);
		bool isValid(){return validCommand;}
		CommandName getCommandName(){return name;}
		string getStrParam(){return strParam;}
		int getIntParam(){return intParam;}
		bool runCommand(AVLTree& tree);

};

Command::Command(string fullCommand)
{
	
	istringstream inStream(fullCommand);
	string command;
	string firstParam;
	string secondParam;

	inStream >> command;
	
	//sets enum to name of command
	if (command == "insert")
		this->name = CommandName::insert;
	else if (command == "remove")
		this->name = CommandName::remove;
	else if (command == "search")
		this->name = CommandName::search;
	else if (command == "printInorder")
		this->name = CommandName::printInorder;
	else if (command == "printPreorder")
		this->name = CommandName::printPreorder;
	else if (command == "printPostorder")
		this->name = CommandName::printPostorder;
	else if (command == "printLevelCount")
		this->name = CommandName::printLevelCount;
	else if (command == "removeInorder")
		this->name = CommandName::removeInorder;
	else 
		this->name = CommandName::invalid;
	
	//if the command is valid, store the input parameters.
	//only the insert command takes 2 parameters
	inStream.ignore();
	if (this->name != CommandName::invalid)
	{
		if (inStream.peek() == '\"')
		{
			inStream.ignore();
			getline(inStream, firstParam,'\"');
			inStream >> secondParam;
		}
		inStream >> firstParam;
	}

	//parses parameters and validates input
	switch (this->name)
	{
	case CommandName::insert:
		//remove parentheses and check for valid parameters
		firstParam.erase(remove(firstParam.begin(),firstParam.end(),'\"'),firstParam.end());
		if (isValidString(firstParam) && isValidDigit(secondParam))
		{
			strParam = firstParam;
			intParam = stoi(secondParam);
		} else
			this->name = CommandName::invalid;
		break;

	case CommandName::remove:
		if (isValidDigit(firstParam))
			intParam = stoi(firstParam);
		 else
			this->name = CommandName::invalid;
		break;

	case CommandName::search:
		firstParam.erase(remove(firstParam.begin(),firstParam.end(),'\"'),firstParam.end());
		 if (isValidDigit(firstParam))
		{
			intParam = stoi(firstParam);
		}
		else if (isValidString(firstParam))
		{
			strParam = firstParam;
		} 
		else
			this->name = CommandName::invalid;
		break;
	case CommandName::printInorder:
		break;
	case CommandName::printPreorder:
		break;
	case CommandName::printPostorder:
		break;
	case CommandName::printLevelCount:
		break;
	case CommandName::removeInorder:
		intParam = stoi(firstParam);
		break;
	default:
		break;
	}

}

//executes the command. Returns true if command ran successfully
bool Command::runCommand(AVLTree& tree)
{
	string outputSingleStr;
	vector<int> outputInt;
	vector<string> outputStr;

	bool isSuccess = false;
	switch (this->name)
	{
	case CommandName::insert:
		 isSuccess = tree.insert(strParam,intParam);
		 if (isSuccess)
			printSuccessful();
		else 
			printUnsuccessful();
		break;

	case CommandName::remove:
		isSuccess = tree.remove(intParam);
		 if (isSuccess)
			printSuccessful();
		else 
			printUnsuccessful();
		break;

	case CommandName::search:
		if (!strParam.empty())
			isSuccess = tree.search(strParam, outputInt);
		else
			isSuccess = tree.search(intParam, outputSingleStr);
		 if (isSuccess)
		{	
			if (!outputInt.empty())
				print(outputInt);
			else
				print(outputSingleStr);
		}
		else
			printUnsuccessful();
		break;

	case CommandName::printInorder:
		isSuccess = tree.printInorder(outputStr);
		if (isSuccess)
			printCommaSeparatedList(outputStr);
		break;

	case CommandName::printPreorder:
		isSuccess = tree.printPreorder(outputStr);
		if (isSuccess)
			printCommaSeparatedList(outputStr);
		break;

	case CommandName::printPostorder:
		isSuccess = tree.printPostorder(outputStr);
		if (isSuccess)
			printCommaSeparatedList(outputStr);
		break;

	case CommandName::printLevelCount:
		isSuccess = tree.printLevelCount(outputSingleStr);
		if (isSuccess)
			print(outputSingleStr);
		break;

	case CommandName::removeInorder:
		isSuccess = tree.removeInorder(intParam);
		 if (isSuccess)
			printSuccessful();
		else 
			printUnsuccessful();
		break;

	default:
		printUnsuccessful();
		isSuccess = false;
		break;
	}
	return isSuccess;
}
