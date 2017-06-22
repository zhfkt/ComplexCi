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


long long basicCi(const unordered_map<int, vector<int> > &adjListGraph, int ballRadius, int currentNode)
{
	if (adjListGraph.at(currentNode).size() == 0)
	{
		return -1;
	}


	unordered_set<int> currentSet;
	currentSet.insert(currentNode);

	unordered_set<int> alreadyAccess;
	alreadyAccess.insert(currentNode);

	for (int i = 0; i < ballRadius; i++)
	{
		unordered_set<int> nextSet;
		for (const auto& node : currentSet)
		{

			const vector<int>& neighbourNodeList = adjListGraph.at(node);

			

			for (const auto& eachNeighbour : neighbourNodeList)
			{
				if (alreadyAccess.find(eachNeighbour) == alreadyAccess.end())
				{
					nextSet.insert(eachNeighbour);
					alreadyAccess.insert(eachNeighbour);
				}
			}


		}

		currentSet = nextSet;
	}

	long long ci = 0;

	for (auto node : currentSet)
	{
		ci += (adjListGraph.at(node).size() - 1);
	}

	ci *= (adjListGraph.at(currentNode).size() - 1);

	return ci;
}


void deleteNode(unordered_map<int, vector<int> > &adjListGraph, int node)
{
	if (adjListGraph[node].size() == 0)
	{
		return;
	}

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


	unordered_map<int, vector<int> > adjListGraph;
	unordered_set<int> allVex;

	ifstream is(path);
	string eachLine;
	while (is >> eachLine)
	{
		vector<string> csvEachLine = split(eachLine, ',');

		adjListGraph[stoi(csvEachLine[0])].push_back(stoi(csvEachLine[1]));
		adjListGraph[stoi(csvEachLine[1])].push_back(stoi(csvEachLine[0]));

		allVex.insert(stoi(csvEachLine[0]));
		allVex.insert(stoi(csvEachLine[1]));

	}

	is.close();

	int totalSize = adjListGraph.size();

	//--------------


	vector<int> finalOutput;
	int loopCount = 0;
	do
	{
		std::cout << "modelID: " << modelID << " start loopCount: " << loopCount << " totalSize: " << totalSize << endl;
		loopCount += updateBatch;

		set<pair<long long,int> > pq; //ci/currentNode --- long is 32 bit on the win and long long is 64 bit

		for (const auto& i : adjListGraph)
		{
			int currentNode = i.first;
			// core_ci
			long long ci = basicCi(adjListGraph, ballRadius, currentNode);

			pq.insert(make_pair(ci, currentNode));
			if (pq.size() > updateBatch)
			{
				pq.erase(pq.begin());
			}
		}

		for (const auto& i : pq)
		{
			deleteNode(adjListGraph, i.second);
		}

		

		for (auto rit = pq.rbegin(); rit != pq.rend(); rit++)
		{
			allVex.erase(rit->second);  //remove key

			finalOutput.push_back(rit->second);

			if (rit->first <= 0)
			{
				std::cout << "modelID: " << modelID << " maxCi: " << pq.rbegin()->first << " node: " << pq.rbegin()->second << endl; //show again

				// ci algorithm ends
				goto CIEND;
			}
		}

		std::cout << "endLoop modelID: " << modelID << " maxCi: " << pq.rbegin()->first << " node: " << pq.rbegin()->second << endl; //show again

		

	} while (true);

CIEND:

	//add left random

	cout << "leftVex:" <<endl;
	for (auto leftVex : allVex)
	{
		finalOutput.push_back(leftVex);
		cout << " " << leftVex ;
	}

	//--------------

	ofstream os(output);
	

	string output500 = "";
	for (unsigned int i = 0; i < finalOutput.size(); i++)
	{
		if (i % outputNumBatch == 0)
		{
			output500 = modelID;
		}


		output500 += (',' + std::to_string(finalOutput[i]));


		if (i % outputNumBatch == (outputNumBatch - 1) || i == (finalOutput.size() - 1))
		{
			os << output500 << endl;
			std::cout << "output500: " << output500 << endl;
			output500.clear();
		}

	}


	os.close();

	system("pause");
	return 0;
}



