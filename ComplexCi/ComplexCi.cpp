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
#include <exception>

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
	int maxRankCount = 0;

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

	int getRank(int rootNode) const
	{
		return rankCount[rootNode];
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

	vector<int>::iterator getBeginItForBfsQueue()
	{
		return bfsQueue.begin();
	}

	vector<int>::iterator getEndItForBfsQueue()
	{
		return bfsQueue.begin() + endIt;
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

	static void recoverAddNode(const vector<vector<int> > &backupCompletedAdjListGraph, vector<bool>& backupAllVex, vector<vector<int> > &adjListGraph, int node, disjointSet &unionSet)
	{
		for (int i = 0; i < backupCompletedAdjListGraph[node].size(); i++)
		{
			int neighbourNode = backupCompletedAdjListGraph[node][i];

			if (backupAllVex[neighbourNode])
			{
				addEdge(adjListGraph, node, neighbourNode);
				unionSet.merge(node, neighbourNode);
			}
		}

		backupAllVex[node] = true;
	}

	static void addEdge(vector<vector<int> > &adjListGraph, int node0, int node1)
	{
		if (((int)adjListGraph.size() - 1) < std::max(node0, node1))
		{
			adjListGraph.resize(std::max(node0, node1) + 1);
		}

		adjListGraph[node0].push_back(node1);
		adjListGraph[node1].push_back(node0);
	}
};


class decreaseComponentStrategy
{
public:

	decreaseComponentStrategy(const string& description)
	{
		cout << "decreaseComponentStrategy: " << description << endl;
	}

	virtual long long decreaseComponentNumIfAddNode(const vector<vector<int> >& backupCompletedAdjListGraph, const vector<bool>& currentAllVex, disjointSet &unionSet, int node) = 0;
	
};


class decreaseComponentRank :public decreaseComponentStrategy
{

public:


	decreaseComponentRank() :decreaseComponentStrategy("decreaseComponentRank"){}
	

	virtual long long decreaseComponentNumIfAddNode(const vector<vector<int> >& backupCompletedAdjListGraph, const vector<bool>& currentAllVex, disjointSet &unionSet, int node)
	{
		unordered_set<int> componentSet;

		for (int i = 0; i < backupCompletedAdjListGraph[node].size(); i++)
		{
			int neighbourNode = backupCompletedAdjListGraph[node][i];

			if (currentAllVex[neighbourNode])
			{
				componentSet.insert(unionSet.findRoot(neighbourNode));
			}
		}

		long long sum = 1;

		for (int eachNode : componentSet)
		{
			sum += unionSet.getRank(eachNode);
		}

		return sum;
	}
};

class decreaseComponentCount : public decreaseComponentStrategy
{
public:

	decreaseComponentCount() :decreaseComponentStrategy("decreaseComponentCount"){}

	virtual long long decreaseComponentNumIfAddNode(const vector<vector<int> >& backupCompletedAdjListGraph, const vector<bool>& currentAllVex, disjointSet &unionSet, int node) override
	{
		unordered_set<int> componentSet;

		for (int i = 0; i < backupCompletedAdjListGraph[node].size(); i++)
		{
			int neighbourNode = backupCompletedAdjListGraph[node][i];

			if (currentAllVex[neighbourNode])
			{
				componentSet.insert(unionSet.findRoot(neighbourNode));
			}
		}

		return (long long)componentSet.size();
	}
};

class decreaseComponentMultiple : public decreaseComponentStrategy
{
public:

	decreaseComponentMultiple() :decreaseComponentStrategy("decreaseComponentMultiple"){}

	virtual long long decreaseComponentNumIfAddNode(const vector<vector<int> >& backupCompletedAdjListGraph, const vector<bool>& currentAllVex, disjointSet &unionSet, int node) override
	{
		unordered_set<int> componentSet;

		for (int i = 0; i < backupCompletedAdjListGraph[node].size(); i++)
		{
			int neighbourNode = backupCompletedAdjListGraph[node][i];

			if (currentAllVex[neighbourNode])
			{
				componentSet.insert(unionSet.findRoot(neighbourNode));
			}
		}

		long long sum = 1;

		for (int eachNode : componentSet)
		{
			sum += unionSet.getRank(eachNode);
		}

		sum *= componentSet.size();

		return sum;
	}
};


class reInsertStrategy
{
protected:

	double biggestComponentCurrentRatio;
	const double biggestComponentEndThreshold;
	int computeComponentInterval, computeComponentIntervalIndice, reinsertEachStep;
	bool isSlowInterval, isContinueReInserted;

	const string& modelID;
	const vector<vector<int> > backupCompletedAdjListGraph; //must invoke copy constructor here
	

	shared_ptr<decreaseComponentStrategy> decreaseStrategy;

public:

	enum reInsertMethod{
		NONE_INSERT,
		COMPONENT_COUNT,
		COMPONENT_RANK,
		FAST_COMPONENT_COUNT,
		FAST_COMPONENT_RANK,
		FIRSTINORDER_COMPONENT_COUNT,
		FIRSTINORDER_COMPONENT_RANK,
		COMPONENT_MULTIPLE,
		FIRSTINORDER_COMPONENT_MULTIPLE,
	};

	reInsertStrategy(bool _isSlowInterval, double _biggestComponentEndThreshold, const vector<vector<int> >& _backupCompletedAdjListGraph, 
		const string& _modelID, shared_ptr<decreaseComponentStrategy> _decreaseStrategy, const string& reInsertStrategyName = "selectRandomLessThanThreshold") :
		isSlowInterval(_isSlowInterval), isContinueReInserted(true),computeComponentIntervalIndice(-1), biggestComponentCurrentRatio(1.0), 
		biggestComponentEndThreshold(_biggestComponentEndThreshold), backupCompletedAdjListGraph(_backupCompletedAdjListGraph), modelID(_modelID),
		decreaseStrategy(_decreaseStrategy)
	{
		cout << "biggestComponentEndThreshold: " << biggestComponentEndThreshold << endl;
		cout << "reInsertStrategyName: " << reInsertStrategyName << endl;
		
		computeComponentInterval = int(backupCompletedAdjListGraph.size() * 0.01);

		if (isSlowInterval)
		{
			if (computeComponentInterval > 200)
			{
				computeComponentInterval = 200;
			}
		}

		if (computeComponentInterval == 0)
		{
			computeComponentInterval = 1;
		}

		//---------------------

		reinsertEachStep = int(backupCompletedAdjListGraph.size() * 0.001);

		if (isSlowInterval)
		{
			if (reinsertEachStep > 20)
			{
				reinsertEachStep = 20;
			}
		}
			
	}

	void tryReInsert(const vector<vector<int> >& adjListGraph, const unordered_set<int>& allVex, int loopCount, vector<int>& finalOutput)
	{
		if (isContinueReInserted)
		{
			int newComputeComponentIntervalIndice = loopCount / computeComponentInterval;


			if (newComputeComponentIntervalIndice > computeComponentIntervalIndice)
			{
				computeComponentIntervalIndice = newComputeComponentIntervalIndice;

				biggestComponentCurrentRatio = disjointSet(adjListGraph).getBiggestComponentCurrentRatio();
			}

			if (biggestComponentCurrentRatio < biggestComponentEndThreshold)
			{
				cout << "Start ReInsert: modelID: " << modelID << " loopCount: " << loopCount << " totalSize: " << adjListGraph.size() << " re-biggestComponentCurrentRatio: " << biggestComponentCurrentRatio << endl;

				isContinueReInserted = false;
				finalOutput = reInsert(finalOutput, adjListGraph, allVex);
			}
		}
	}

	virtual vector<int> reInsert(const vector<int> &beforeOutput, const vector<vector<int> >& adjListGraph, const unordered_set<int>& allVex)
	{
		vector<vector<int> > currentAdjListGraph = adjListGraph;

		vector<bool> currentAllVex(currentAdjListGraph.size(), false);
		for (int eachV : allVex)
		{
			currentAllVex[eachV] = true;
		}

		unordered_set<int> leftOutput(beforeOutput.begin(), beforeOutput.end());
		vector<int> finalOutput;

		disjointSet unionSet(currentAdjListGraph);

		while (leftOutput.size() != 0)
		{
			cout << "modelID: " << modelID << " reInsertCount: " << leftOutput.size() << endl;

			vector<pair<long long, int> >  batchList;

			for (int eachNode : leftOutput)
			{
				//min is better
				long long decreaseValue = decreaseStrategy->decreaseComponentNumIfAddNode(backupCompletedAdjListGraph, currentAllVex, unionSet, eachNode);
				batchList.push_back(make_pair(decreaseValue, eachNode));
			}


			if (reinsertEachStep >= batchList.size())
			{
				reinsertEachStep = (int)batchList.size();
			}
			else
			{
				std::nth_element(batchList.begin(), batchList.begin() + reinsertEachStep, batchList.end());
			}

			for (int i = 0; i < reinsertEachStep; i++)
			{
				finalOutput.push_back(batchList[i].second);
				leftOutput.erase(batchList[i].second);

				graphUtil::recoverAddNode(backupCompletedAdjListGraph, currentAllVex, currentAdjListGraph, batchList[i].second, unionSet);
			}

		}

		std::reverse(finalOutput.begin(), finalOutput.end());

		return finalOutput;

	}

	double getBiggestComponentCurrentRatio()
	{
		return biggestComponentCurrentRatio;
	}


};


class reInsertFirstInOrderStrategy : public reInsertStrategy
{
public:
	reInsertFirstInOrderStrategy(bool _isSlowInterval, double _biggestComponentEndThreshold, const vector<vector<int> >& _backupCompletedAdjListGraph,
		const string& _modelID, shared_ptr<decreaseComponentStrategy> _decreaseStrategy) :
	reInsertStrategy(_isSlowInterval, _biggestComponentEndThreshold, _backupCompletedAdjListGraph, _modelID, _decreaseStrategy, "selectFirstInOrderLessThanThreshold")
	{}

	virtual vector<int> reInsert(const vector<int> &beforeOutput, const vector<vector<int> >& adjListGraph, const unordered_set<int>& allVex)
	{
		vector<vector<int> > currentAdjListGraph = adjListGraph;

		vector<bool> currentAllVex(currentAdjListGraph.size(), false);
		for (int eachV : allVex)
		{
			currentAllVex[eachV] = true;
		}

		unordered_map<int, int> leftOutput;
		for (int i = 0; i < beforeOutput.size(); i++)
		{
			leftOutput[beforeOutput[i]] = (int)beforeOutput.size() - i;
		}
		vector<int> finalOutput;

		disjointSet unionSet(currentAdjListGraph);

		while (leftOutput.size() != 0)
		{
			cout << "modelID: " << modelID << " reInsertCount: " << leftOutput.size() << endl;

			vector<tuple<long long, int, int> >  batchList;

			for (const auto& eachNode : leftOutput)
			{
				//min is better
				long long decreaseValue = decreaseStrategy->decreaseComponentNumIfAddNode(backupCompletedAdjListGraph, currentAllVex, unionSet, eachNode.first);
				batchList.push_back(make_tuple(decreaseValue, eachNode.second, eachNode.first));
			}


			if (reinsertEachStep >= batchList.size())
			{
				reinsertEachStep = (int)batchList.size();
			}
			else
			{
				std::sort(batchList.begin(), batchList.end());
			}

			for (int i = 0; i < reinsertEachStep; i++)
			{
				finalOutput.push_back(std::get<2>(batchList[i]));
				leftOutput.erase(std::get<2>(batchList[i]));

				graphUtil::recoverAddNode(backupCompletedAdjListGraph, currentAllVex, currentAdjListGraph, std::get<2>(batchList[i]), unionSet);
			}

		}

		std::reverse(finalOutput.begin(), finalOutput.end());

		return finalOutput;
	}

};


class basicParametersBuilder
{
public:
	unsigned int ballRadius;
	unsigned int updateBatch;
	unsigned int outputNumBatch;
	const string& path;
	const string& modelID;
	bool isPrintMinPointCausingMinComponent;

public:
	basicParametersBuilder(unsigned int _ballRadius, unsigned int _updateBatch, unsigned int _outputNumBatch,
		const string& _path, const string& _modelID, const bool _isPrintMinPointCausingMinComponent) :
		ballRadius(_ballRadius), updateBatch(_updateBatch), outputNumBatch(_outputNumBatch), 
		path(_path), modelID(_modelID), isPrintMinPointCausingMinComponent(_isPrintMinPointCausingMinComponent){}
};


class parametersBuilder :public basicParametersBuilder
{
public:

	double biggestComponentEndThreshold;
	reInsertStrategy::reInsertMethod rim;

public:
	parametersBuilder(unsigned int _ballRadius, unsigned int _updateBatch, unsigned int _outputNumBatch, 
		const string& _path, const string& _modelID, const bool _isPrintMinPointCausingMinComponent) :
		basicParametersBuilder(_ballRadius, _updateBatch, _outputNumBatch, _path, _modelID, _isPrintMinPointCausingMinComponent),
		biggestComponentEndThreshold(0.01), rim(reInsertStrategy::NONE_INSERT){}

	parametersBuilder& setBiggestComponentEndThreshold(double _biggestComponentEndThreshold)
	{
		biggestComponentEndThreshold = _biggestComponentEndThreshold;
		return *this;
	}

	parametersBuilder& setReInsertMethod(reInsertStrategy::reInsertMethod _rim)
	{
		rim = _rim;
		return *this;
	}

	template<class T>
	T* build()
	{
		return new T(*this);
	}

};




class basicCiAlgo
{

public:
	
	basicCiAlgo(const parametersBuilder& builder) : path(builder.path), modelID(builder.modelID), ballRadius(builder.ballRadius),
		updateBatch(builder.updateBatch), outputNumBatch(builder.outputNumBatch), isPrintMinPointCausingMinComponent(builder.isPrintMinPointCausingMinComponent)
	{
		load();

		shared_ptr<reInsertStrategy> curReInsert;

		if (builder.rim == reInsertStrategy::COMPONENT_COUNT)
		{
			shared_ptr<decreaseComponentStrategy> decreaseStrategy(new decreaseComponentCount());
			curReInsert.reset(new reInsertStrategy(true, builder.biggestComponentEndThreshold, adjListGraph, modelID, decreaseStrategy));
		}
		else if (builder.rim == reInsertStrategy::COMPONENT_RANK)
		{
			shared_ptr<decreaseComponentStrategy> decreaseStrategy(new decreaseComponentRank());
			curReInsert.reset(new reInsertStrategy(true, builder.biggestComponentEndThreshold, adjListGraph, modelID, decreaseStrategy));
		}
		else if (builder.rim == reInsertStrategy::NONE_INSERT)
		{
			cout << "reInsertStrategy and biggestComponentEndThreshold will not be used" << endl;
			//reInsert = nullptr;
		}
		else if (builder.rim == reInsertStrategy::FAST_COMPONENT_COUNT)
		{
			shared_ptr<decreaseComponentStrategy> decreaseStrategy(new decreaseComponentCount());
			curReInsert.reset(new reInsertStrategy(false, builder.biggestComponentEndThreshold, adjListGraph, modelID, decreaseStrategy));
		}
		else if (builder.rim == reInsertStrategy::FAST_COMPONENT_RANK)
		{
			shared_ptr<decreaseComponentStrategy> decreaseStrategy(new decreaseComponentRank());
			curReInsert.reset(new reInsertStrategy(false, builder.biggestComponentEndThreshold, adjListGraph, modelID, decreaseStrategy));
		}
		else if (builder.rim == reInsertStrategy::FIRSTINORDER_COMPONENT_RANK)
		{
			shared_ptr<decreaseComponentStrategy> decreaseStrategy(new decreaseComponentRank());
			curReInsert.reset(new reInsertFirstInOrderStrategy(true, builder.biggestComponentEndThreshold, adjListGraph, modelID, decreaseStrategy));
		}
		else if (builder.rim == reInsertStrategy::FIRSTINORDER_COMPONENT_COUNT)
		{
			shared_ptr<decreaseComponentStrategy> decreaseStrategy(new decreaseComponentCount());
			curReInsert.reset(new reInsertFirstInOrderStrategy(true, builder.biggestComponentEndThreshold, adjListGraph, modelID, decreaseStrategy));
		}
		else if (builder.rim == reInsertStrategy::COMPONENT_MULTIPLE)
		{
			shared_ptr<decreaseComponentStrategy> decreaseStrategy(new decreaseComponentMultiple());
			curReInsert.reset(new reInsertStrategy(true, builder.biggestComponentEndThreshold, adjListGraph, modelID, decreaseStrategy));
		}
		else if (builder.rim == reInsertStrategy::FIRSTINORDER_COMPONENT_MULTIPLE)
		{
			shared_ptr<decreaseComponentStrategy> decreaseStrategy(new decreaseComponentMultiple());
			curReInsert.reset(new reInsertFirstInOrderStrategy(true, builder.biggestComponentEndThreshold, adjListGraph, modelID, decreaseStrategy));
		}
		else
		{
			throw runtime_error("can not find the corresponding reInsertStrategy class");
		}


		if (curReInsert)
		{
			if (std::abs(builder.biggestComponentEndThreshold) < 1e-7)
			{
				fullReInsert = curReInsert;
			}
			else
			{
				reInsert = curReInsert;
			}
		}

		if (reInsert)
		{
			cout << "reInsert enabled" << endl;
		}
		else
		{
			cout << "reInsert not enabled" << endl;
		}

		if (fullReInsert)
		{
			cout << "fullReInsert enabled" << endl;
		}
		else
		{
			cout << "fullReInsert not enabled" << endl;
		}
	}
	

protected:

	unsigned int ballRadius;
	unsigned int updateBatch;
	unsigned int outputNumBatch;
	
	const string& path;
	const string& modelID;
	bool isPrintMinPointCausingMinComponent;
	
	unordered_set<int> allVex;
	vector<vector<int> > adjListGraph;
	shared_ptr<reInsertStrategy> reInsert;
	shared_ptr<reInsertStrategy> fullReInsert;

private:

	void load()
	{
		cout << "Read Start: " << path << endl;

		string eachLine;
		ifstream is;
		is.open(path);

		if (!is.good())
		{
			is.close();
			throw runtime_error((string("Can not open the file: ") + path).c_str());
		}

		while (is >> eachLine)
		{
			vector<string> csvEachLine = util::split(eachLine, ',');

			int node0 = stoi(csvEachLine[0]);
			int node1 = stoi(csvEachLine[1]);

			allVex.insert(node0);
			allVex.insert(node1);

			graphUtil::addEdge(adjListGraph, node0, node1);

		}
		is.close();

		cout << "Read End: " << path << endl;
	}


public:

	virtual vector<int> go()
	{
		set<pair<long long, int> > allPQ; //ci/currentNode --- long is 32 bit on the win and long long is 64 bit / and long long can be multiple
		vector<long long> revereseLoopUpAllPQ(adjListGraph.size());

		graphUtil gu((int)adjListGraph.size());

		cout << "modelID: " << modelID << " First Cal CI" << endl;

		for (int i = 0; i < adjListGraph.size(); i++)
		{
			int currentNode = i;
			// core_ci
			long long ci = gu.basicCi(adjListGraph, ballRadius, currentNode);

			allPQ.insert(make_pair(ci, currentNode));
			revereseLoopUpAllPQ[currentNode] = ci;
		}


		vector<int> finalOutput;
		int loopCount = 0;
		while (true)
		{
			if ((updateBatch != 1) || ((loopCount%outputNumBatch == 0) && (updateBatch == 1)))
			{
				//restrict flood output when updateBatch == 1
				cout << "modelID: " << modelID << " loopCount: " << loopCount << " totalSize: " << adjListGraph.size() << " maxCi: " << allPQ.rbegin()->first
					<< " node: " << allPQ.rbegin()->second;
				if (reInsert)
				{
					cout << " biggestComponentCurrentRatio: " << reInsert->getBiggestComponentCurrentRatio();
				}

				cout << endl;
			}

			loopCount += updateBatch;

			if (reInsert)
			{
				reInsert->tryReInsert(adjListGraph, allVex, loopCount, finalOutput);
			}

			vector<int> batchList;
			unsigned int batchLimiti = 0;

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

			unordered_set<int> candidateUpdateNodesSet;

			for (int i : batchList)
			{
				gu.getNeighbourFrontierAndScope(adjListGraph, ballRadius + 1, i);
				
				for (auto bfsIt = gu.getBeginItForBfsQueue(); bfsIt != gu.getEndItForBfsQueue(); bfsIt++)
				{
					candidateUpdateNodesSet.insert(*bfsIt);
				}
			}


			for (int i : batchList)
			{
				graphUtil::deleteNode(adjListGraph, i);
				//candidateUpdateNodesWithCi.insert(make_pair(i, -1));// no need to because self will be included in the candidateUpdateNodes and updated in the below
			}

			for (auto canIt = candidateUpdateNodesSet.begin(); canIt != candidateUpdateNodesSet.end(); canIt++)
			{
				long long updatedCi = gu.basicCi(adjListGraph, ballRadius, *canIt);

				long long olderCi = revereseLoopUpAllPQ[*canIt];

				allPQ.erase(make_pair(olderCi, *canIt));
				allPQ.insert(make_pair(updatedCi, *canIt));

				revereseLoopUpAllPQ[*canIt] = updatedCi;
			}

		}

	}

	virtual vector<int>& postProcess(vector<int>& finalOutput)
	{
		if (fullReInsert)
		{
			finalOutput = fullReInsert->reInsert(finalOutput, adjListGraph, allVex);
		}

		cout << modelID << " Min Point count reach the Min Component when reinsert enabled or Zero Ci value when reinsert disabled:  " << finalOutput.size() << endl;

		if (!isPrintMinPointCausingMinComponent)
		{
			//add left random
			for (auto leftVex : allVex)
			{
				finalOutput.push_back(leftVex);
			}
			cout << modelID << " Total Point: " << finalOutput.size() << endl;

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

	vector<pair<long long, int> > concurrentCiForBatchList(int threadIndice, int ballRadius, const vector<vector<int> > &adjListGraph, const vector<int>& batchList)
	{
		vector<pair<long long, int> > vectorPQ;

		for (int i = threadIndice; i < batchList.size(); i += threadNum)
		{
			int currentNode = batchList[i];

			// core_ci
			long long ci = gu.basicCi(adjListGraph, ballRadius, currentNode);
			vectorPQ.push_back(make_pair(ci, currentNode));
		}

		return vectorPQ;
	}

	vector<pair<long long, int> > concurrentCiForFirstTime(int threadIndice, int ballRadius, const vector<vector<int> > &adjListGraph)
	{
		vector<pair<long long, int> > vectorPQ;

		for (int i = threadIndice; i < adjListGraph.size(); i += threadNum)
		{
			int currentNode = i;

			// core_ci
			long long ci = gu.basicCi(adjListGraph, ballRadius, currentNode);
			vectorPQ.push_back(make_pair(ci, currentNode));
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

			for (auto bfsIt = gu.getBeginItForBfsQueue(); bfsIt != gu.getEndItForBfsQueue(); bfsIt++)
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

	concurrentBasicCiAlgo(const parametersBuilder& builder) :
		basicCiAlgo(builder) 
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
		bool switchToSingleThread = false;

		set<pair<long long, int> > allPQ; //ci/currentNode --- long is 32 bit on the win and long long is 64 bit / and long long can be multiple
		vector<long long> revereseLoopUpAllPQ(adjListGraph.size());

		graphUtil gu((int)adjListGraph.size());
		vector<future<vector<pair<long long, int> > > > firstThreadPoolFutureGetCi;
		
		cout << "modelID: " << modelID << " First Cal CI" << endl;

		vector<concurrentGraphUtil> cgu(threadNum, concurrentGraphUtil((int)adjListGraph.size(), threadNum));

		//thread start

		for (unsigned int i = 0; i < threadNum; i++)
		{
			firstThreadPoolFutureGetCi.push_back(async(std::launch::async, &concurrentGraphUtil::concurrentCiForFirstTime, cgu[i], i, ballRadius, ref(adjListGraph)));
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
				cout << "modelID: " << modelID << " loopCount: " << loopCount << " totalSize: " << adjListGraph.size() << " maxCi: " << allPQ.rbegin()->first
					<< " node: " << allPQ.rbegin()->second;
				if (reInsert)
				{
					cout << " biggestComponentCurrentRatio: " << reInsert->getBiggestComponentCurrentRatio();
				}

				cout << endl;
			}

			loopCount += updateBatch;

			if (reInsert)
			{
				reInsert->tryReInsert(adjListGraph, allVex, loopCount, finalOutput);

				if (reInsert->getBiggestComponentCurrentRatio() < 0.5)
				{
					switchToSingleThread = true;
				}

			}

			vector<int> batchList;
			unsigned int batchLimiti = 0;

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
	
			
			if (switchToSingleThread)
			{
				unordered_set<int> candidateUpdateNodesSet;

				for (int i : batchList)
				{
					gu.getNeighbourFrontierAndScope(adjListGraph, ballRadius + 1, i);

					for (auto bfsIt = gu.getBeginItForBfsQueue(); bfsIt != gu.getEndItForBfsQueue(); bfsIt++)
					{
						candidateUpdateNodesSet.insert(*bfsIt);
					}
				}


				for (int i : batchList)
				{
					graphUtil::deleteNode(adjListGraph, i);
					//candidateUpdateNodesWithCi.insert(make_pair(i, -1));// no need to because self will be included in the candidateUpdateNodes and updated in the below
				}

				for (auto canIt = candidateUpdateNodesSet.begin(); canIt != candidateUpdateNodesSet.end(); canIt++)
				{
					long long updatedCi = gu.basicCi(adjListGraph, ballRadius, *canIt);

					long long olderCi = revereseLoopUpAllPQ[*canIt];

					allPQ.erase(make_pair(olderCi, *canIt));
					allPQ.insert(make_pair(updatedCi, *canIt));

					revereseLoopUpAllPQ[*canIt] = updatedCi;

				}


			}
			else
			{
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
					updateThreadPoolFutureGetCi.push_back(async(std::launch::async, &concurrentGraphUtil::concurrentCiForBatchList, cgu[i], i, ballRadius, ref(adjListGraph), ref(candidateUpdateNodesVector)));
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

class openSourceParametersBuilder;
class openSourceCiAlgo : public basicCiAlgo
{

private:

	string tempFormatFile;
	bool isReInserted;

public:

	openSourceCiAlgo(const openSourceParametersBuilder& builder);
	virtual vector<int> go();

private:

	void transformToFormat();
};


class openSourceParametersBuilder :public basicParametersBuilder
{
public:
	bool isReInserted;

public:
	openSourceParametersBuilder(unsigned int _ballRadius, unsigned int _updateBatch, unsigned int _outputNumBatch, const string& _path,
		const string& _modelID, const bool _isPrintMinPointCausingMinComponent) :
		basicParametersBuilder(_ballRadius, _updateBatch, _outputNumBatch, _path, _modelID, _isPrintMinPointCausingMinComponent),
		isReInserted(false){}

	openSourceParametersBuilder& setisReInserted(const bool _isReInserted)
	{
		isReInserted = _isReInserted;
		return *this;
	}

	openSourceCiAlgo* build()
	{
		return new openSourceCiAlgo(*this);
	}
};

openSourceCiAlgo::openSourceCiAlgo(const openSourceParametersBuilder& builder) :
basicCiAlgo(parametersBuilder(builder.ballRadius, builder.updateBatch, builder.outputNumBatch, builder.path, builder.modelID, builder.isPrintMinPointCausingMinComponent)),
	isReInserted(builder.isReInserted)
{
	tempFormatFile = "tempFormatFile_" + modelID + ".fmt";
	transformToFormat();

	if (isReInserted)
	{
		cout << "The openSourceCiAlgo will use own reInsertStrategy" << endl;
	}
	else
	{
		cout << "The openSourceCiAlgo will not use own reInsertStrategy" << endl;
	}
}


vector<int> openSourceCiAlgo::go()
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
	listInfluencers = ci::get_influencers(Node, N, Graph, L, isReInserted);

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

void openSourceCiAlgo::transformToFormat()
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





/*

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
*/



int main(int argc, char* argv[])
{
	unsigned int ballRadius = 1;
	unsigned int updateBatch = 1;
	unsigned int outputNumBatch = 1;
	double biggestComponentEndThreshold = 0.01;

	string path = "";
	string output = "";
	int method = 0;
	bool isPrintMinPointCausingMinComponent = false;

	if (argc > 5)
	{
		path = argv[1];
		output = path + "_out";

		ballRadius = stoi(argv[2]);
		updateBatch = stoi(argv[3]);
		outputNumBatch = stoi(argv[4]);
		method = stoi(argv[5]);

		if (argc > 6)
		{
			biggestComponentEndThreshold = atof(argv[6]);
		}

		if (argc > 7)
		{
			int intPrintMinPointCausingMinComponent = stoi(argv[7]);

			if (intPrintMinPointCausingMinComponent == 0)
			{
				isPrintMinPointCausingMinComponent = false;
			}
			else
			{
				isPrintMinPointCausingMinComponent = true;
			}
		}

	}
	else
	{
		cout << "at least 4 parameters for csv path" << endl;
		cout << "e.g. 'C:/Users/zhfkt/Documents/Visual Studio 2013/Projects/ComplexCi/Release/karate.txt' [ballRadius] [updateBatch] [outputNumBatch] [method] {[biggestComponentEndThreshold]}" << endl;
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
	cout << "biggestComponentEndThreshold: " << biggestComponentEndThreshold << endl;
	cout << "isPrintMinPointCausingMinComponent: " << isPrintMinPointCausingMinComponent << endl;
	
	//--------------

	try
	{





		unique_ptr<basicCiAlgo> bca;

		if (method == 0)
		{
			// Just use in the test instead formal release
			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				build<basicCiAlgo>());
		}
		else if (method == 1)
		{
			bca.reset(openSourceParametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setisReInserted(true).
				build());
		}
		else if (method == 2)
		{
			// Just use in the test instead formal release
			bca.reset(openSourceParametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				build());// not use setisReInserted
		}
		else if (method == 3)
		{
			// disable openSourceCiTMAlgo
			cout << "Disabled openSourceCiTMAlgo" << endl;
			return 0;

			//bca.reset(new openSourceCiTMAlgo(ballRadius, updateBatch, outputNumBatch, path, modelID));
		}
		else if (method == 4)
		{
			// Just use in the test instead formal release
			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent)
				.build<concurrentBasicCiAlgo>());
		}
		else if (method == 5)
		{
			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::COMPONENT_COUNT).
				build<basicCiAlgo>());
		}
		else if (method == 6)
		{
			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::COMPONENT_COUNT).
				build<concurrentBasicCiAlgo>());
		}
		else if (method == 7)
		{
			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::COMPONENT_RANK).
				build<basicCiAlgo>());
		}
		else if (method == 8)
		{
			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::COMPONENT_RANK).
				build<concurrentBasicCiAlgo>());
		}
		else if (method == 9)
		{
			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::FAST_COMPONENT_COUNT).
				build<basicCiAlgo>());
		}
		else if (method == 10)
		{
			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::FAST_COMPONENT_RANK).
				build<basicCiAlgo>());
		}
		else if (method == 11)
		{
			// Just use in the test instead formal release

			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::FIRSTINORDER_COMPONENT_COUNT).
				build<concurrentBasicCiAlgo>());
		}
		else if (method == 12)
		{
			// Just use in the test instead formal release

			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::FIRSTINORDER_COMPONENT_RANK).
				build<concurrentBasicCiAlgo>());
		}
		else if (method == 13)
		{
			// Just use in the test instead formal release

			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::COMPONENT_MULTIPLE).
				build<concurrentBasicCiAlgo>());
		}
		else if (method == 14)
		{
			// Just use in the test instead formal release

			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::FIRSTINORDER_COMPONENT_MULTIPLE).
				build<concurrentBasicCiAlgo>());
		}
		
		//15-18 is the single thread version for concurrent version of 11-14
		
		else if (method == 15)
		{
			// Just use in the test instead formal release

			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::FIRSTINORDER_COMPONENT_COUNT).
				build<basicCiAlgo>());
		}
		else if (method == 16)
		{
			// Just use in the test instead formal release

			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::FIRSTINORDER_COMPONENT_RANK).
				build<basicCiAlgo>());
		}
		else if (method == 17)
		{
			// Just use in the test instead formal release

			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::COMPONENT_MULTIPLE).
				build<basicCiAlgo>());
		}
		else if (method == 18)
		{
			// Just use in the test instead formal release

			bca.reset(parametersBuilder(ballRadius, updateBatch, outputNumBatch, path, modelID, isPrintMinPointCausingMinComponent).
				setBiggestComponentEndThreshold(biggestComponentEndThreshold).
				setReInsertMethod(reInsertStrategy::FIRSTINORDER_COMPONENT_MULTIPLE).
				build<basicCiAlgo>());
		}
		
		
		else		
		{
			cout << "Method " << method << " is not defined" << endl;
			return 0;
		}

		high_resolution_clock::time_point t1 = high_resolution_clock::now();

		const vector<int>& finalOutput = bca->go();

		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		auto duration = duration_cast<seconds>(t2 - t1).count();

		cout << modelID << " duration: " << duration << "s" << endl;

		outputFinalOutput ofo(output, modelID, finalOutput, outputNumBatch);
		ofo.outputToFile();
	}
	catch (const runtime_error e)
	{
		cout << "Error: " << e.what() << endl;
	}

	//system("pause");
	
	cerr << modelID << " ends" << endl;
	
	return 0;
}

