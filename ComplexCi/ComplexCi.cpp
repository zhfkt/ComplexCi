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
#include <chrono>
#include <bitset>

using namespace std;
using namespace std::chrono;


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

/*void getNeighbourFrontierAndScope(const vector<vector<int> > &adjListGraph, int scope, int currentNode, unordered_set<int> &currentSet, unordered_set<int>& alreadyAccess)
{
//alreadyAccess.reserve(100000);

currentSet.insert(currentNode);
alreadyAccess.insert(currentNode);

for (int i = 0; i < scope; i++)
{
unordered_set<int> nextSet;
//nextSet.reserve(30000);

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
}*/

bitset<2000000> alreadyAccessBool;
vector<int> bfsQueue;
int startIt = 0;
int endIt = 1;

void getNeighbourFrontierAndScope(const vector<vector<int> > &adjListGraph, int scope, int currentNode)
{
	startIt = 0;
	endIt = 1;

	//startIt and endIt will never execeed bfsQueue.size();
	//alreadyAccess is between 0 and endIt
	//fontier is between startIt and endIt

	bfsQueue[0] = currentNode;
	alreadyAccessBool[currentNode] = 1;

	for (int i = 0; i < scope; i++)
	{
		int lastEndIt = endIt;
		while (startIt != lastEndIt)
		{
			const vector<int>& neighbourNodeList = adjListGraph[bfsQueue[startIt++]];

			for (const auto& eachNeighbour : neighbourNodeList)
			{
				if (!alreadyAccessBool.test(eachNeighbour))
				{
					bfsQueue[endIt++] = eachNeighbour;
					alreadyAccessBool[eachNeighbour] = 1;
				}
			}
		}
	}
	
	for (int i = 0; i < endIt; i++)
	{
		alreadyAccessBool.reset(bfsQueue[i]);
	}



}



long long basicCi(const vector<vector<int> > &adjListGraph, int ballRadius, int currentNode)
{
	if (adjListGraph[currentNode].size() == 0)
	{
		return -1;
	}

	getNeighbourFrontierAndScope(adjListGraph, ballRadius, currentNode);

	long long ci = 0;

	for (int i = startIt; i < endIt; i++)
	{
		ci += (adjListGraph[bfsQueue[i]].size() - 1);
	}

	ci *= (adjListGraph[currentNode].size() - 1);


	/*
	if (currentNode % 500 == 0)
	{
	cout << currentNode << " currentFrontier size :" << currentFrontier.size() << endl;
	}
	*/

	/*
	if (currentNode == 1715132 && (ci == 0 || ci == 2))
	{
	cout << "currentNode: " << currentNode << " adjListGraph[currentNode].size(): " << adjListGraph[currentNode].size() << "/ ";


	for (auto node : adjListGraph[currentNode])
	{
	cout << node << " " ;
	}

	cout << endl;


	cout << "ci: " << ci << endl;

	cout << "currentFrontier: " << endl;

	for (auto node : currentFrontier)
	{
	cout << node << " " << adjListGraph[node].size() << endl;
	}

	cout << "dummyValue: " << endl;

	for (auto node : dummyValue)
	{
	cout << node << " node1: ";

	for (auto node1 : adjListGraph[node])
	{
	cout <<  node1 << " ";
	}
	cout << endl;
	}
	cout << endl;

	}
	*/

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

	for (auto neighbour : adjListGraph[node])
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
		cout << "at least 3 parameters for csv path" << endl;
		cout << "e.g. 'C:/Users/zhfkt/Documents/Visual Studio 2013/Projects/ComplexCi/Release/karate.txt' 2 500 500" << endl;
		return 0;
	}

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

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

	bfsQueue.resize(totalSize, -1);

	//--------------




	set<pair<long long, int> > allPQ; //ci/currentNode --- long is 32 bit on the win and long long is 64 bit / and long long can be multiple
	vector<long long> revereseLoopUpAllPQ(totalSize);
	cout << "modelID: " << modelID << " First Cal CI" << endl;

	for (int i = 0; i < adjListGraph.size(); i++)
	{
		int currentNode = i;
		// core_ci
		long long ci = basicCi(adjListGraph, ballRadius, currentNode);

		allPQ.insert(make_pair(ci, currentNode));
		revereseLoopUpAllPQ[currentNode] = ci;
	}

	bitset<2000000> candidateUpdateNodesBool;
	vector<int> candidateUpdateNodesVector(totalSize, -1);
	
	int candidateEnd = 0;

	vector<int> finalOutput;
	int loopCount = 0;
	while (true)
	{
		cout << "modelID: " << modelID << " loopCount: " << loopCount << " totalSize: " << totalSize << " maxCi: " << allPQ.rbegin()->first << " node: " << allPQ.rbegin()->second << endl;
		loopCount += updateBatch;

		candidateEnd = 0;

		vector<int> batchList;
		unsigned int batchLimiti = 0;

		pair<long long, int> debugPreviousMax = *(allPQ.rbegin());

		for (auto rit = allPQ.rbegin(); batchLimiti < updateBatch && (rit != allPQ.rend()); rit++, batchLimiti++)
		{
			batchList.push_back(rit->second);
			finalOutput.push_back(rit->second);
			allVex.erase(rit->second);  //remove key

			if (rit->first <= 0)
			{
				// ci algorithm ends
				goto CIEND;
			}
		}

		/*
		if (loopCount == 600000)
		{
		cout << " revereseLoopUpAllPQ[1715132] " << revereseLoopUpAllPQ[1715132] << endl;
		}
		*/


		//cout << "monitor 1" << endl;

		//int debugCount = 0;

		
		for (int i : batchList)
		{
			getNeighbourFrontierAndScope(adjListGraph, ballRadius + 1, i);

			for (auto bfsIt = bfsQueue.begin(); bfsIt != bfsQueue.begin() + endIt; bfsIt++)
			{
				if (!candidateUpdateNodesBool.test(*bfsIt))
				{
					candidateUpdateNodesVector[candidateEnd++] = (*bfsIt);
					candidateUpdateNodesBool[*bfsIt] = 1;
				}
			}
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

		for (auto canIt = candidateUpdateNodesVector.begin(); canIt != (candidateUpdateNodesVector.begin() + candidateEnd); canIt++)
		{
			long long updatedCi = basicCi(adjListGraph, ballRadius, *canIt);

			long long olderCi = revereseLoopUpAllPQ[*canIt];

			allPQ.erase(make_pair(olderCi, *canIt));
			allPQ.insert(make_pair(updatedCi, *canIt));

			revereseLoopUpAllPQ[*canIt] = updatedCi;

			candidateUpdateNodesBool[*canIt] = 0; //recover candidateUpdateNodesBool to zero

			//cout << "monitor 3_4: " << debugCount++ << " " << candidateUpdateNodes.size() << endl;
		}
		//cout << "monitor 4" << endl;

		/*if ((debugPreviousMax.first) < (allPQ.rbegin()->first))
		{
		cout << "monitor 5: " << debugPreviousMax.first << " " << allPQ.rbegin()->first << endl;;
		}
		*/


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

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(t2 - t1).count();

	cout << "duration: " << duration << "s" << endl;

	system("pause");
	return 0;
}

