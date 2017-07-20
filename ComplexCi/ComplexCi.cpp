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
#include <mutex>
#include <memory>
#include <thread>
#include <future>
#include <algorithm>

#include "CITM.cpp"
#include "CI_HEAP.cpp"

using namespace std;
using namespace chrono;




class util
{
private:
	util(){}

public:

	template<typename Out>
	static void split(const string &s, char delim, Out result) {
		stringstream ss;
		ss.str(s);
		string item;
		while (getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	static vector<string> split(const string &s, char delim) {
		vector<string> elems;
		split(s, delim, back_inserter(elems));
		return elems;
	}

};


class disjointSet
{

private:



	void makeSet(const vector<vector<int> >& backupAdjListGraph)
	{
		for (int i = 0; i < unionSet.size(); i++)
		{
			unionSet[i] = i;
			rankCount[i] = 1;
		}

		for (int i = 0; i < backupAdjListGraph.size(); i++)
		{
			for (int j = 0; j < backupAdjListGraph[i].size(); j++)
			{
				merge(i, backupAdjListGraph[i][j]);
			}
		}
	}


protected:
	vector<int> unionSet;
	vector<int> rankCount;
	int maxRankCount = -1;

public:
	disjointSet(const vector<vector<int> >& backupAdjListGraph) : unionSet(backupAdjListGraph.size()), rankCount(backupAdjListGraph.size())
	{
		makeSet(backupAdjListGraph);
	}

	int findRoot(int node)
	{
		if (node != unionSet[node])
		{
			int rootNode = findRoot(unionSet[node]);
			unionSet[node] = rootNode;
			return rootNode;
		}
		else
		{
			return node;
		}
	}

	void merge(int node1, int node2)
	{
		int node1Root = findRoot(node1);
		int node2Root = findRoot(node2);

		if (node1Root != node2Root)
		{
			if (rankCount[node2Root] > rankCount[node1Root])
			{
				unionSet[node1Root] = node2Root;
				rankCount[node2Root] += rankCount[node1Root];

				if (rankCount[node2Root] > maxRankCount)
				{
					maxRankCount = rankCount[node2Root];
				}


			}
			else
			{
				unionSet[node2Root] = node1Root;
				rankCount[node1Root] += rankCount[node2Root];


				if (rankCount[node1Root] > maxRankCount)
				{
					maxRankCount = rankCount[node1Root];
				}

			}
		}
	}

	double getBiggestComponentCurrentRatio() const
	{
		return double(maxRankCount) / double(rankCount.size());
	}

		


};

class graphUtil
{
private:

	vector<bool> alreadyAccessBool;
	vector<int> bfsQueue;
	int startIt;
	int endIt;

public:
	graphUtil(int totalSize) : startIt(0),endIt(1)
	{
		bfsQueue.resize(totalSize, -1);
		alreadyAccessBool.resize(totalSize, 0);	
	}

	vector<int>& getBfsQueue()
	{
		return bfsQueue;
	}

	int getEndIt()
	{
		return endIt;
	}


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
					if (!alreadyAccessBool[eachNeighbour])
					{
						bfsQueue[endIt++] = eachNeighbour;
						alreadyAccessBool[eachNeighbour] = 1;
					}
				}
			}
		}

		for (int i = 0; i < endIt; i++)
		{
			alreadyAccessBool[bfsQueue[i]] = 0;
		}
	}



	long long basicCi(const vector<vector<int> > &adjListGraph, int ballRadius, int currentNode)
	{
		if (adjListGraph[currentNode].size() == 0)
		{
			return -1;
		}

		if (ballRadius == 0)
		{
			return adjListGraph[currentNode].size();
		}

		getNeighbourFrontierAndScope(adjListGraph, ballRadius, currentNode);

		long long ci = 0;

		for (int i = startIt; i < endIt; i++)
		{
			ci += (adjListGraph[bfsQueue[i]].size() - 1);
		}

		ci *= (adjListGraph[currentNode].size() - 1);

		return ci;
	}

	static void deleteNode(vector<vector<int> > &adjListGraph, int node)
	{
		for (auto neighbour : adjListGraph[node])
		{
			adjListGraph[neighbour].erase(remove(adjListGraph[neighbour].begin(), adjListGraph[neighbour].end(), node), adjListGraph[neighbour].end());
		}

		adjListGraph[node].clear();
	}

	static void recoverAddNode(const vector<vector<int> > &backupCompletedAdjListGraph, unordered_set<int>& backupAllVex, vector<vector<int> > &adjListGraph, int node, disjointSet &unionSet)
	{
		for (int i = 0; i < backupCompletedAdjListGraph[node].size(); i++)
		{
			int neighbourNode = backupCompletedAdjListGraph[node][i];

			if (backupAllVex.find(neighbourNode) != backupAllVex.end())
			{
				addEdge(adjListGraph, node, neighbourNode);
				unionSet.merge(node, neighbourNode);
			}
		}

		backupAllVex.insert(node);

	}

	static void addEdge(vector<vector<int> > &adjListGraph, int node1, int node2)
	{
		adjListGraph[node1].push_back(node2);
		adjListGraph[node2].push_back(node1);
	}

	static int decreaseComponentNumIfAddNode(const vector<vector<int> >& backupCompletedAdjListGraph, const unordered_set<int>& backupAllVex, disjointSet &unionSet, int node)
	{
		unordered_set<int> componentSet;

		for (int i = 0; i < backupCompletedAdjListGraph[node].size(); i++)
		{
			int neighbourNode = backupCompletedAdjListGraph[node][i];

			if (backupAllVex.find(neighbourNode) != backupAllVex.end())
			{
				componentSet.insert(unionSet.findRoot(neighbourNode));
			}
		}

		return componentSet.size();
	}


};




class basicCiAlgo
{

protected:

	unsigned int ballRadius;
	unsigned int updateBatch;
	unsigned int outputNumBatch;
	
	const string& path;
	const string& modelID;
	
	unordered_set<int> allVex;
	vector<vector<int> > adjListGraph;
	vector<vector<int> > backupCompletedAdjListGraph;
	int totalSize;
	bool isInserted;

	double biggestComponentCurrentRatio;
	const double biggestComponentEndThreshold;
	int computeComponentInterval;


private:

	void load()
	{
		cout << "First Read Start" << endl;

		string eachLine;
		ifstream is;
		is.open(path);

		while (is >> eachLine)
		{
			vector<string> csvEachLine = util::split(eachLine, ',');

			allVex.insert(stoi(csvEachLine[0]));
			allVex.insert(stoi(csvEachLine[1]));

		}
		is.close();

		totalSize = (int)allVex.size();
		adjListGraph.resize(totalSize);

		cout << "First Read End/Second Read Start" << endl;

		is.open(path);
		while (is >> eachLine)
		{
			vector<string> csvEachLine = util::split(eachLine, ',');
			graphUtil::addEdge(adjListGraph, stoi(csvEachLine[0]), stoi(csvEachLine[1]));
		}

		cout << "Second Read End" << endl;

		backupCompletedAdjListGraph = adjListGraph;
	}


public:

	basicCiAlgo(unsigned int _ballRadius, unsigned int _updateBatch, unsigned int _outputNumBatch, const string& _path, const string& _modelID, bool _isInserted) :
		ballRadius(_ballRadius), updateBatch(_updateBatch), outputNumBatch(_outputNumBatch), path(_path), modelID(_modelID), isInserted(_isInserted), biggestComponentCurrentRatio(1.0), 
		biggestComponentEndThreshold(0.01)
	{	
		load();
		
		computeComponentInterval = adjListGraph.size() * 0.001;
		if (computeComponentInterval > 500)
		{
			computeComponentInterval = 500;
		}
		if (computeComponentInterval == 0)
		{
			computeComponentInterval = 1;
		}

	}

	virtual vector<int> go()
	{
		set<pair<long long, int> > allPQ; //ci/currentNode --- long is 32 bit on the win and long long is 64 bit / and long long can be multiple
		vector<long long> revereseLoopUpAllPQ(totalSize);

		graphUtil gu(totalSize);

		cout << "modelID: " << modelID << " First Cal CI" << endl;

		for (int i = 0; i < adjListGraph.size(); i++)
		{
			int currentNode = i;
			// core_ci
			long long ci = gu.basicCi(adjListGraph, ballRadius, currentNode);

			allPQ.insert(make_pair(ci, currentNode));
			revereseLoopUpAllPQ[currentNode] = ci;
		}

		vector<bool> candidateUpdateNodesBool(totalSize, 0);
		vector<int> candidateUpdateNodesVector(totalSize, -1);

		int candidateEnd = 0;

		vector<int> finalOutput;
		int loopCount = 0;
		while (true)
		{
			if ((updateBatch != 1) || ((loopCount%outputNumBatch == 0) && (updateBatch == 1)))
			{
				//restrict flood output when updateBatch == 1
				cout << "modelID: " << modelID << " loopCount: " << loopCount << " totalSize: " << totalSize << " maxCi: " << allPQ.rbegin()->first << " node: " << allPQ.rbegin()->second << " biggestComponentCurrentRatio: " << biggestComponentCurrentRatio << endl;
			}

			loopCount += updateBatch;

			candidateEnd = 0;

			vector<int> batchList;
			unsigned int batchLimiti = 0;

			if (isInserted && (loopCount%computeComponentInterval == 0))
			{
				biggestComponentCurrentRatio = disjointSet(adjListGraph).getBiggestComponentCurrentRatio();

				if (biggestComponentCurrentRatio < biggestComponentEndThreshold)
				{
					cout << "Start ReInsert: modelID: " << modelID << " loopCount: " << loopCount << " totalSize: " << totalSize << " maxCi: " << allPQ.rbegin()->first << " node: " << allPQ.rbegin()->second << " re-biggestComponentCurrentRatio: " << biggestComponentCurrentRatio << endl;

					finalOutput = reInsert(finalOutput);
					isInserted = false;
				}
			}

			for (auto rit = allPQ.rbegin(); batchLimiti < updateBatch && (rit != allPQ.rend()); rit++, batchLimiti++)
			{
				if (rit->first < 0)//try -1 and batchList is the min point causing Zero Component
				{
					// ci algorithm ends
					return postProcess(finalOutput);
				}

				batchList.push_back(rit->second);
				finalOutput.push_back(rit->second);
				allVex.erase(rit->second);  //remove key
			}

			for (int i : batchList)
			{
				gu.getNeighbourFrontierAndScope(adjListGraph, ballRadius + 1, i);
				const vector<int>& bfsQueue = gu.getBfsQueue();
				int endIt = gu.getEndIt();

				for (auto bfsIt = bfsQueue.begin(); bfsIt != bfsQueue.begin() + endIt; bfsIt++)
				{
					if (!candidateUpdateNodesBool[*bfsIt])
					{
						candidateUpdateNodesVector[candidateEnd++] = (*bfsIt);
						candidateUpdateNodesBool[*bfsIt] = 1;
					}
				}
			}


			for (int i : batchList)
			{
				graphUtil::deleteNode(adjListGraph, i);
				//candidateUpdateNodesWithCi.insert(make_pair(i, -1));// no need to because self will be included in the candidateUpdateNodes and updated in the below
			}

			for (auto canIt = candidateUpdateNodesVector.begin(); canIt != (candidateUpdateNodesVector.begin() + candidateEnd); canIt++)
			{
				long long updatedCi = gu.basicCi(adjListGraph, ballRadius, *canIt);

				long long olderCi = revereseLoopUpAllPQ[*canIt];

				allPQ.erase(make_pair(olderCi, *canIt));
				allPQ.insert(make_pair(updatedCi, *canIt));

				revereseLoopUpAllPQ[*canIt] = updatedCi;

				candidateUpdateNodesBool[*canIt] = 0; //recover candidateUpdateNodesBool to zero

			}

		}

	}

protected:

	vector<int> reInsert(const vector<int> &beforeOutput)
	{
		vector<vector<int> > backupAdjListGraph = adjListGraph;
		unordered_set<int> backupAllVex = allVex;

		int eachStep = computeComponentInterval;
		if (eachStep == 0)
		{
			eachStep = 1;
		}

		unordered_set<int> leftOutput(beforeOutput.begin(), beforeOutput.end());
		vector<int> finalOutput;
		
		disjointSet unionSet(backupAdjListGraph);

		while (leftOutput.size() != 0)
		{
			cout << "modelID: " << modelID << " reInsertCount: " << leftOutput.size() << endl;
			
			vector<pair<int, int> >  batchList;

			for (int eachNode : leftOutput)
			{
				//min is better
				int decreaseValue = graphUtil::decreaseComponentNumIfAddNode(backupCompletedAdjListGraph, backupAllVex, unionSet, eachNode);
				batchList.push_back(make_pair(decreaseValue, eachNode));
			}

			
			if (eachStep >= batchList.size())
			{
				eachStep = batchList.size();
			}
			else
			{
				std::nth_element(batchList.begin(), batchList.begin() + eachStep, batchList.end());
			}

			for (int i = 0; i < eachStep; i++)
			{
				finalOutput.push_back(batchList[i].second);
				leftOutput.erase(batchList[i].second);

				graphUtil::recoverAddNode(backupCompletedAdjListGraph, backupAllVex, backupAdjListGraph, batchList[i].second, unionSet);
			}
			
		}

		std::reverse(finalOutput.begin(), finalOutput.end());

		return finalOutput;

	}

	vector<int>& postProcess(vector<int>& finalOutput)
	{
		//add left random

		cout << modelID << " Before Random adding the left CI equals zero: " << finalOutput.size() << endl;
		for (auto leftVex : allVex)
		{
			finalOutput.push_back(leftVex);
		}
		cout << modelID << " After Random adding the left CI equals zero: " << finalOutput.size() << endl;

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

		return finalOutput;
	}

};


class concurrentGraphUtil
{
private:
	graphUtil gu;
	int threadNum;


public:
	concurrentGraphUtil(int totalSize, int _threadNum) : gu(totalSize), threadNum(_threadNum){}

	vector<pair<long long, int> > concurrentCi(int threadIndice, int ballRadius, const vector<vector<int> > &adjListGraph, const vector<int>& batchList)
	{
		vector<pair<long long, int> > vectorPQ;

		if (batchList.size() == 0)
		{
			for (int i = threadIndice; i < adjListGraph.size(); i += threadNum)
			{
				int currentNode = i;

				// core_ci
				long long ci = gu.basicCi(adjListGraph, ballRadius, currentNode);
				vectorPQ.push_back(make_pair(ci, currentNode));
			}
		}
		else
		{
			for (int i = threadIndice; i < batchList.size(); i += threadNum)
			{
				int currentNode = batchList[i];

				// core_ci
				long long ci = gu.basicCi(adjListGraph, ballRadius, currentNode);
				vectorPQ.push_back(make_pair(ci, currentNode));
			}


		}

		return vectorPQ;
	}

	
	unordered_set<int> concurrentGetNeighbourFrontierAndScope(int threadIndice, int ballRadius, const vector<vector<int> > &adjListGraph, const vector<int>& batchList)
	{
		unordered_set<int> candidateUpdateNodesSet;

		for (int i = threadIndice; i < batchList.size(); i += threadNum)
		{
			int currentNode = batchList[i];

			// core_ci
			gu.getNeighbourFrontierAndScope(adjListGraph, ballRadius + 1, currentNode);
			const vector<int>& bfsQueue = gu.getBfsQueue();
			int endIt = gu.getEndIt();

			for (auto bfsIt = bfsQueue.begin(); bfsIt != bfsQueue.begin() + endIt; bfsIt++)
			{
				candidateUpdateNodesSet.insert(*bfsIt);
			}
		}
	
		return candidateUpdateNodesSet;

	}


};

class concurrentBasicCiAlgo : public basicCiAlgo
{
protected:

	unsigned int threadNum;

public:
	concurrentBasicCiAlgo(unsigned int _ballRadius, unsigned int _updateBatch, unsigned int _outputNumBatch, const string& _path, const string& _modelID, bool _isInserted) :
		basicCiAlgo(_ballRadius, _updateBatch, _outputNumBatch, _path, _modelID, _isInserted)
	{
		threadNum = thread::hardware_concurrency();

		if (threadNum == 0)
		{
			threadNum = 1;
		}

		cout << "auto detect hardware concurrency: " << threadNum << endl;
	
	}

	virtual vector<int> go()
	{

		set<pair<long long, int> > allPQ; //ci/currentNode --- long is 32 bit on the win and long long is 64 bit / and long long can be multiple
		vector<long long> revereseLoopUpAllPQ(totalSize);

		graphUtil gu(totalSize);
		vector<future<vector<pair<long long, int> > > > firstThreadPoolFutureGetCi;
		
		cout << "modelID: " << modelID << " First Cal CI" << endl;

		vector<concurrentGraphUtil> cgu(threadNum, concurrentGraphUtil(totalSize, threadNum));

		//thread start

		for (unsigned int i = 0; i < threadNum; i++)
		{
			vector<int> dummyList;
			firstThreadPoolFutureGetCi.push_back(async(std::launch::async, &concurrentGraphUtil::concurrentCi, cgu[i], i, ballRadius, ref(adjListGraph), ref(dummyList)));
		}
		
		//wait thread to end

		
		for (unsigned int i = 0; i < threadNum; i++)
		{
			for (const pair<long long, int>& eachPQ : firstThreadPoolFutureGetCi[i].get() )
			{
				allPQ.insert(make_pair(eachPQ.first, eachPQ.second));
				revereseLoopUpAllPQ[eachPQ.second] = eachPQ.first;
			}
		}

		vector<int> finalOutput;
		int loopCount = 0;
		while (true)
		{
			if ((updateBatch != 1) || ((loopCount%outputNumBatch == 0) && (updateBatch == 1)))
			{
				//restrict flood output when updateBatch == 1
				cout << "modelID: " << modelID << " loopCount: " << loopCount << " totalSize: " << totalSize << " maxCi: " << allPQ.rbegin()->first << " node: " << allPQ.rbegin()->second << endl;
			}

			loopCount += updateBatch;

			vector<int> batchList;
			unsigned int batchLimiti = 0;

			if (isInserted && (loopCount%outputNumBatch == 0))
			{
				biggestComponentCurrentRatio = disjointSet(adjListGraph).getBiggestComponentCurrentRatio();

				if (biggestComponentCurrentRatio < biggestComponentEndThreshold)
					//if (loopCount == 280000)
				{
					cout << "Start ReInsert: modelID: " << modelID << " loopCount: " << loopCount << " totalSize: " << totalSize << " maxCi: " << allPQ.rbegin()->first << " node: " << allPQ.rbegin()->second << " re-biggestComponentCurrentRatio: " << biggestComponentCurrentRatio << endl;

					finalOutput = reInsert(finalOutput);
					isInserted = false;
				}
			}

			for (auto rit = allPQ.rbegin(); batchLimiti < updateBatch && (rit != allPQ.rend()); rit++, batchLimiti++)
			{
				if (rit->first < 0)//try -1 and batchList is the min point causing Zero Component
				{
					// ci algorithm ends
					return postProcess(finalOutput);
				}

				batchList.push_back(rit->second);
				finalOutput.push_back(rit->second);
				allVex.erase(rit->second);  //remove key
			}
	
			

			vector<future<unordered_set<int> > >  threadPoolFutureGetBall;
			unordered_set<int> candidateUpdateNodesSet;
			
			//thread start
			for (unsigned int i = 0; i < threadNum; i++)
			{
				threadPoolFutureGetBall.push_back(async(std::launch::async, &concurrentGraphUtil::concurrentGetNeighbourFrontierAndScope, cgu[i], i, ballRadius, ref(adjListGraph), ref(batchList)));
			}

			//wait thread to end

			for (unsigned int i = 0; i < threadNum; i++)
			{
				for (int candidateNode : threadPoolFutureGetBall[i].get())
				{
					candidateUpdateNodesSet.insert(candidateNode);
				}
			}

			for (int i : batchList)
			{
				graphUtil::deleteNode(adjListGraph, i);
			}

			vector<int> candidateUpdateNodesVector(candidateUpdateNodesSet.begin(), candidateUpdateNodesSet.end());
			vector<future<vector<pair<long long, int> > > > updateThreadPoolFutureGetCi;

			//thread start
			for (unsigned int i = 0; i < threadNum; i++)
			{
				updateThreadPoolFutureGetCi.push_back(async(std::launch::async, &concurrentGraphUtil::concurrentCi, cgu[i], i, ballRadius, ref(adjListGraph), ref(candidateUpdateNodesVector)));
			}

			//wait thread to end

			for (unsigned int i = 0; i < threadNum; i++)
			{
				for (const pair<long long, int>& eachPQ : updateThreadPoolFutureGetCi[i].get())
				{
					long long olderCi = revereseLoopUpAllPQ[eachPQ.second];
					allPQ.erase(make_pair(olderCi, eachPQ.second));
					allPQ.insert(make_pair(eachPQ.first, eachPQ.second));
					revereseLoopUpAllPQ[eachPQ.second] = eachPQ.first;

				}
			}

		}

	}

};





class outputFinalOutput
{
private:

	const string& output;
	const string& modelID;
	const vector<int>& finalOutput;
	const int outputNumBatch;
	
public:

	outputFinalOutput(const string& _output, const string& _modelID, const vector<int>& _finalOutput, const int _outputNumBatch) :
		output(_output), modelID(_modelID), finalOutput(_finalOutput), outputNumBatch(_outputNumBatch)
	{}

	void outputToFile()
	{
		cout << "Outputing Start.." << endl;

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
				output500 += (',' + to_string(finalOutput[i]));
			}
			else
			{
				output500 += ',';
			}

			if (i % outputNumBatch == (outputNumBatch - 1) || i == (finalOutput.size() - 1))
			{
				os << output500 << endl;
				output500.clear();
			}

		}


		os.close();

		cout << "Outputing End.." << endl;
	}
};

class openSourceCiAlgo : public basicCiAlgo
{
public:
	openSourceCiAlgo(unsigned int _ballRadius, unsigned int _updateBatch, unsigned int _outputNumBatch, const string& _path, const string& _modelID, bool _isInserted) :
		basicCiAlgo(_ballRadius, _updateBatch, _outputNumBatch, _path, _modelID, _isInserted)
	{
		tempFormatFile = "tempFormatFile_" + modelID + ".fmt";
		transformToFormat();

	}

	virtual vector<int> go()
	{

		ci::int_t i, N, **Graph, *listInfluencers;
		int L;
		const char *network;

		network = tempFormatFile.c_str();
		L = ballRadius;

		N = ci::get_num_nodes(network);
		ci::varNode *Node;
		Node = (ci::varNode *)calloc(N + 1, sizeof(ci::varNode));
		Graph = ci::makeRandomGraph(network, N);

		fprintf(stdout, "\n\n\t\t\t\t### COMPUTING ###\n\n");
		fflush(stdout);

		//GET INFLUENCERS
		listInfluencers = ci::get_influencers(Node, N, Graph, L, isInserted);

		fprintf(stdout, "\t\t\t\t  ### NETWORK DONE ###\n\n");
		fflush(stdout);

		vector<int> finalOutput;

		for (i = 1; i <= listInfluencers[0]; i++)
		{
			finalOutput.push_back(listInfluencers[i] - 1);
			allVex.erase(listInfluencers[i] - 1);
		}

		free(listInfluencers);

		return postProcess(finalOutput);
	}

protected:

	string tempFormatFile;

	
private:

	void transformToFormat()
	{
		ofstream os(tempFormatFile);

		for (int i = 0; i < adjListGraph.size(); i++)
		{
			os << (i + 1) << " ";

			for (int j = 0; j < adjListGraph[i].size(); j++)
			{

				os << adjListGraph[i][j] + 1 << " ";

			}
			os << endl;
		}

		os.close();
	}
};
	




class openSourceCiTMAlgo : public openSourceCiAlgo
{
public:
	openSourceCiTMAlgo(unsigned int _ballRadius, unsigned int _updateBatch, unsigned int _outputNumBatch, const string& _path, const string& _modelID) :
		openSourceCiAlgo(_ballRadius, _updateBatch, _outputNumBatch, _path, _modelID,  true)
	{}

	virtual vector<int> go()
	{
		srand((unsigned int)time(NULL));

		int L = ballRadius;
		const char *network = tempFormatFile.c_str();;

		int N = citm::get_num_nodes(network);
		citm::int_t **Graph = citm::makeRandomGraph(network, N);

		citm::varNode *Node;
		double **listInfluencers;
		double *threshold;
		threshold = (double *)calloc(N + 1, sizeof(double));
		for (int i = 1; i <= N; i++)
		{
			threshold[i] = 0.5;//the fractional threshold is 0.5
			//threshold[i] = 1;
		}
			
		Node = (citm::varNode *)calloc(N + 1, sizeof(citm::varNode));
		listInfluencers = get_influencers_CITM(Node, N, Graph, threshold, L);

		vector<int> finalOutput;

		for (int i = 1; i <= N; i++)
		{
			finalOutput.push_back((int)round(listInfluencers[i][1]) - 1);
			allVex.erase((int)round(listInfluencers[i][1]) - 1);
		}

		return postProcess(finalOutput);
	}

};




int main(int argc, char* argv[])
{
	unsigned int ballRadius = 1;
	unsigned int updateBatch = 1;
	unsigned int outputNumBatch = 1;


	string path = "";
	string output = "";
	int method = 0;

	if (argc > 5)
	{
		path = argv[1];
		output = path + "_out";

		ballRadius = stoi(argv[2]);
		updateBatch = stoi(argv[3]);
		outputNumBatch = stoi(argv[4]);
		method = stoi(argv[5]);
	}
	else
	{
		cout << "at least 4 parameters for csv path" << endl;
		cout << "e.g. 'C:/Users/zhfkt/Documents/Visual Studio 2013/Projects/ComplexCi/Release/karate.txt' [ballRadius] [updateBatch] [outputNumBatch] [method]" << endl;
		return 0;
	}

	vector<string> fileName = util::split(path, '/');
	string modelID = util::split(fileName[fileName.size() - 1], '.')[0];

	cout << "path: " << path << endl;
	cout << "output: " << output << endl;
	cout << "modelID: " << modelID << endl;

	cout << "ballRadius: " << ballRadius << endl;
	cout << "updateBatch: " << updateBatch << endl;
	cout << "outputNumBatch: " << outputNumBatch << endl;
	cout << "method: " << method << endl;
	
	//--------------

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	shared_ptr<basicCiAlgo> bca;

	if (method == 0)
	{
		bca.reset(new basicCiAlgo(ballRadius, updateBatch, outputNumBatch, path, modelID, false));
	}
	else if (method == 1)
	{
		bca.reset(new openSourceCiAlgo(ballRadius, updateBatch, outputNumBatch, path, modelID, true));
	}
	else if (method == 2)
	{
		bca.reset(new openSourceCiAlgo(ballRadius, updateBatch, outputNumBatch, path, modelID, false));
	}
	else if (method == 3)
	{
		bca.reset(new openSourceCiTMAlgo(ballRadius, updateBatch, outputNumBatch, path, modelID));
	}
	else if (method == 4)
	{
		bca.reset(new concurrentBasicCiAlgo(ballRadius, updateBatch, outputNumBatch, path, modelID, false));
	}
	else if (method == 5)
	{
		bca.reset(new basicCiAlgo(ballRadius, updateBatch, outputNumBatch, path, modelID, true));
	}
	else if (method == 6)
	{
		bca.reset(new concurrentBasicCiAlgo(ballRadius, updateBatch, outputNumBatch, path, modelID, true));
	}
	else
	{
		cout << "Method " << method << " is not defined" << endl;
		return 0;
	}

	const vector<int>& finalOutput = bca->go();

	//--------------

	outputFinalOutput ofo(output,modelID,finalOutput,outputNumBatch);
	ofo.outputToFile();

	

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(t2 - t1).count();

	cout << modelID << " duration: " << duration << "s" << endl;

	system("pause");
	return 0;
}

