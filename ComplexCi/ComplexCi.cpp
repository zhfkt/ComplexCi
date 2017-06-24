// ComplexCi.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"

#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <iterator>
#include <queue>
#include <algorithm>


using namespace std;


template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

void getNeighbourFrontierAndScope(const vector<vector<int> > &adjListGraph, int scope, int currentNode, unordered_set<int> &currentSet, unordered_set<int>& alreadyAccess)
{
	currentSet.insert(currentNode);
	alreadyAccess.insert(currentNode);

	for (int i = 0; i < scope; i++)
	{
		unordered_set<int> nextSet;
		for (const auto& node : currentSet)
		{
			const vector<int>& neighbourNodeList = adjListGraph[node];

			for (const auto& eachNeighbour : neighbourNodeList)
			{
				if (alreadyAccess.find(eachNeighbour) == alreadyAccess.end())
				{
					nextSet.insert(eachNeighbour);
					alreadyAccess.insert(eachNeighbour);
				}
			}

		}

		currentSet = move(nextSet);
	}
}


long long basicCi(const vector<vector<int> > &adjListGraph, int ballRadius, int currentNode)
{
	if (adjListGraph[currentNode].size() == 0)
	{
		return -1;
	}

	unordered_set<int> currentFrontier;	
	unordered_set<int> dummyValue;

	getNeighbourFrontierAndScope(adjListGraph, ballRadius, currentNode, currentFrontier, dummyValue);

	long long ci = 0;

	for (auto node : currentFrontier)
	{
		ci += (adjListGraph[currentNode].size() - 1);
	}

	ci *= (adjListGraph[currentNode].size() - 1);

	return ci;
}






void deleteNode(vector<vector<int> > &adjListGraph, int node)
{
	/*
		if (adjListGraph[node].size() == 0)
		{
			return;
		} // not must here
	*/ 

	for (auto neighbour: adjListGraph[node])
	{
		adjListGraph[neighbour].erase(remove(adjListGraph[neighbour].begin(), adjListGraph[neighbour].end(), node), adjListGraph[neighbour].end());
	}

	adjListGraph[node].clear();

}

int main(int argc, char* argv[])
{
	unsigned int ballRadius = 1;
	unsigned int updateBatch = 1;
	unsigned int outputNumBatch = 1;


	string path = "";
	string output = "";

	if (argc > 4)
	{
		path = argv[1];
		output = path + "_out";

		ballRadius = stoi(argv[2]);
		updateBatch = stoi(argv[3]);
		outputNumBatch = stoi(argv[4]);

	}
	else
	{
		cout << "at least 3 parameters for csv path" <<endl;
		cout << "e.g. 'C:/Users/zhfkt/Documents/Visual Studio 2013/Projects/ComplexCi/Release/karate.txt' 2 500 500" <<endl;
		return 0;
	}

	string modelID = "";
		
	{
		vector<string> fileName = split(path, '/');
		modelID = split(fileName[fileName.size() - 1], '.')[0];
	}

	std::cout << "path: " << path << endl;
	std::cout << "output: " << output << endl;
	std::cout << "modelID: " << modelID << endl;

	std::cout << "ballRadius: " << ballRadius << endl;
	std::cout << "updateBatch: " << updateBatch << endl;
	std::cout << "outputNumBatch: " << outputNumBatch << endl;


	std::cout << "First Read Start" << endl;

	unordered_set<int> allVex;
	string eachLine;
	ifstream is;
	is.open(path);

	while (is >> eachLine)
	{
		vector<string> csvEachLine = split(eachLine, ',');

		allVex.insert(stoi(csvEachLine[0]));
		allVex.insert(stoi(csvEachLine[1]));

	}
	is.close();
	int totalSize = allVex.size();

	std::cout << "First Read End/Second Read Start" << endl;


	vector<vector<int> > adjListGraph(totalSize);
	is.open(path);
	while (is >> eachLine)
	{
		vector<string> csvEachLine = split(eachLine, ',');

		adjListGraph[stoi(csvEachLine[0])].push_back(stoi(csvEachLine[1]));
		adjListGraph[stoi(csvEachLine[1])].push_back(stoi(csvEachLine[0]));
	}

	std::cout << "Second Read End" << endl;



	//--------------


	
	
	set<pair<long long, int> > allPQ; //ci/currentNode --- long is 32 bit on the win and long long is 64 bit / and long long can be multiple
	unordered_map<int, long long> revereseLoopUpAllPQ;
	cout << "modelID: " << modelID << " First Cal CI" << endl;

	for (int i = 0; i < adjListGraph.size(); i++)
	{
		int currentNode = i;
		// core_ci
		long long ci = basicCi(adjListGraph, ballRadius, currentNode);

		allPQ.insert(make_pair(ci, currentNode));
		revereseLoopUpAllPQ[currentNode] = ci;
	}

	vector<int> finalOutput;
	int loopCount = 0;
	while (true)
	{
		cout << "modelID: " << modelID << " loopCount: " << loopCount << " totalSize: " << totalSize << " maxCi: " << allPQ.rbegin()->first << " node: " << allPQ.rbegin()->second << endl;
		loopCount += updateBatch;

		vector<int> batchList;
		unsigned int batchLimiti = 0;
		for (auto rit = allPQ.rbegin(); batchLimiti < updateBatch && (rit != allPQ.rend()); rit++, batchLimiti++)
		{
			batchList.push_back(rit->second);
			finalOutput.push_back(rit->second);
			allVex.erase(rit->second);  //remove key

			if (rit ->first <= 0)
			{
				// ci algorithm ends
				goto CIEND;
			}
		}

		//cout << "monitor 1" << endl;

		//int debugCount = 0;

		unordered_set<int> candidateUpdateNodes;
		for (int i : batchList)
		{
			unordered_set<int> allScopeInBallRadiusPlusOne;
			unordered_set<int> dummyValue;
			getNeighbourFrontierAndScope(adjListGraph, ballRadius + 1, i, dummyValue, allScopeInBallRadiusPlusOne);
			candidateUpdateNodes.insert(allScopeInBallRadiusPlusOne.begin(), allScopeInBallRadiusPlusOne.end());


			//cout << "monitor 1_2: " << debugCount++ << " " << batchList.size() << " " << candidateUpdateNodes.size() << endl;

		}

		//cout << "monitor 2" << endl;

		for (int i : batchList)
		{
			deleteNode(adjListGraph, i);
			//candidateUpdateNodesWithCi.insert(make_pair(i, -1));// no need to because self will be included in the candidateUpdateNodes and updated in the below
		}

		//cout << "monitor 3" << endl;
		//debugCount = 0;

		for (int i : candidateUpdateNodes)
		{
			long long updatedCi = basicCi(adjListGraph, ballRadius, i);

			long long olderCi = revereseLoopUpAllPQ[i];

			allPQ.erase(make_pair(olderCi, i));
			allPQ.insert(make_pair(updatedCi, i));

			revereseLoopUpAllPQ[i] = updatedCi;


			//cout << "monitor 3_4: " << debugCount++ << " " << candidateUpdateNodes.size() << endl;
		}
		//cout << "monitor 4" << endl;

	}

CIEND:

	//add left random

	cout << "Before Random adding the left CI equals zero: " << finalOutput.size() << endl;
	for (auto leftVex : allVex)
	{
		finalOutput.push_back(leftVex);
	}
	cout << "After Random adding the left CI equals zero: " << finalOutput.size() << endl;

	while (true)
	{
		if (finalOutput.size() % outputNumBatch == 0)
		{
			break;
		}
		else
		{
			finalOutput.push_back(-1);
		}
	}



	//--------------

	std::cout << "Outputing Start.." << endl;

	ofstream os(output);
	

	string output500 = "";
	for (unsigned int i = 0; i < finalOutput.size(); i++)
	{
		if (i % outputNumBatch == 0)
		{
			output500 = modelID;
		}


		if (finalOutput[i] != -1)
		{
			output500 += (',' + std::to_string(finalOutput[i]));
		}
		else
		{
			output500 += ',';
		}

		if (i % outputNumBatch == (outputNumBatch - 1) || i == (finalOutput.size() - 1))
		{
			os << output500 << endl;
			//std::cout << "output500: " << output500 << endl;
			output500.clear();
		}

	}


	os.close();

	std::cout << "Outputing End.." << endl;

	system("pause");
	return 0;
}



