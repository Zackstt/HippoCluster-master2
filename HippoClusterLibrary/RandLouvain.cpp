#include "stdafx.h"
#include "RandLouvain.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <ppl.h>
#include <algorithm>

using namespace std;

namespace HippoClusterLibrary
{

	RandLouvain::RandLouvain(AdjacencyList* AdjList)
	{
		adjList = AdjList;
		preClustgerGraph = new AdjacencyList;
		postClusterGraph = new AdjacencyList;
		//for (int i = 0; i < adjList->numVertices(); i++)
		//{
			// vector ID represents node ID and the number stored in the vector represents the cluster id
			//RandLouvain::cluster.push_back(i);
			// cluster.nodes.push_back(i);
			// might have to use a vector insted
		//}

		// need to also create the initial cluster here
	}


	// cluster is a vector of nodes
	double RandLouvain::getModularity(int nodeI, int comunity, vector<int> cluster)
	{
		// need a vector of nodes actually in the comunity
		vector<int> nodesInComunity;
		for (int i = 0; i < cluster.size(); i++)
		{
			if (cluster[i] == comunity)
				nodesInComunity.push_back(i);
		}
		sort(nodesInComunity.begin(), nodesInComunity.end());

		double weightOfNetwork = adjList->sumWeights();

		// weightIntoCluster completed
		double weightIntoCluster = 0, weightIntoNode = 0, weightiToCluster = 0;

		// sum of weights from i to Cj
		// --- get all neighbors of i
		vector<std::tuple<int, double, double>> neighborsOfI = adjList->allNeighbors(nodeI);
		// --- get sumation of weights
		for (int i = 0; i < neighborsOfI.size(); i++)
		{
			if(binary_search(nodesInComunity.begin(), nodesInComunity.end(),comunity))
				weightIntoCluster += get<1>(neighborsOfI[i]);
		}

		// sum of weights into ci
		// is the sum of the weights of the edges into nodes in Cj
		
		// need to go through cluster list, find all neighbors of those nodes and 
		vector<int> neighbors;
		vector<std::tuple<int, double, double>> neighborList;
		for (int i = 0; i < nodesInComunity.size(); i++)
		{
			vector<tuple<int, double, double>> tempHolder = adjList->allNeighbors(i);
			// go to node in cluster, add neighbors to neighbors vector.
			neighborList.insert(neighborList.end(), tempHolder.begin(), tempHolder.end());
			tempHolder.clear();
		}
		// take neighborList, only add elements of it that are not in cluster
		// go through and delete all nodes that are in the cluster
		// go through neighbors list and add weights to weightIntoCluster
		for (int i = 0; i < neighborList.size(); i++)
		{
			if (!binary_search(nodesInComunity.begin(), nodesInComunity.end(), get<0>(neighborList[i])))
				weightIntoCluster += get<1>(neighborList[i]);
		}



		// sum of weights into i
		vector<tuple<int, double, double>> iNeighbors = adjList->allNeighbors(nodeI);
		for (int i = 0; i < iNeighbors.size(); i++)
		{
			weightIntoNode += get<1>(iNeighbors[i]);
		}

		// final equation
		return((weightiToCluster / (2 * weightOfNetwork)) -
			((weightIntoCluster*weightIntoNode)/(2 * weightOfNetwork*weightOfNetwork)) );
	}
	
	void RandLouvain::clusterAlgorithm(AdjacencyList *clusterGraph)
	{
		// ** fails in this part somewhere
		// General idea: 1 make an initial cluster with a vector (reason for this is because we can use the 
		//                  cluster as the value and the node it belongs to as the id of the vector)
		//                  we need to do this because changing the nodes cluster is very quick
		//               2 maximize modularity of clusters
		//               3 compare the initial cluster to final cluster. 
		//                  If initial cluster is better make it initial by creating new adjlist. 
		//                  else its the same graph is complete
		vector<int> clusters;
		clusters.resize(adjList->numVertices(),-1);
		// new AdjacencyList* preClusterGraph; // preform clustering algorithm on this graph, place it into post
		//new AdjacencyList* postClusterGraph; // check and see if this graph is equal to prior graph

		AdjacencyList* preClusterGraph = new AdjacencyList;  
		AdjacencyList* postClusterGraph = new AdjacencyList;

		// step 1 build initial clusters
		for (int i = 0; i < adjList->numVertices(); i++)
		{
			clusters[i] = i;
			cout << i << " " << adjList->getVertexName(i) << endl;
		}

		//while (graphIsNotTheSame)
		//{
		//	// step 2 move nodes between clusters until max modularity is reached
		//	// --- remove node i from its comunity
		//	// --- get random set of neighbors of node i
		//	// --- run getModularity on the list.
		//	// --- if max mod > 0, add to that community, if not return node to its own community
		//	for (int i = 0; i < adjList->numVertices(); i++)
		//	{
		//		clusters[i] = -1;
		//		// get random set of neighbors here
		//		// run getModularity on each element of list here, save highest modularity calculation and cluster
		//		// if mod > 0 add else dont add
		//	}
		//}



		// step 3 agrigate graph. loop back to beginning if graph is not same as preClusterGraph

		//while (true)
		//{
		//	
		//}
		// first int is the cluster key or the ID. second int is the node the cluster belongs to
		// multimap<int, int> clusters;
		// place each part of the graph into its own cluster
		// for (int i=0; i < adjList.numVertices(); i++)
		// {
		// 	clusters.insert(std::pair<int, int>(i, i))
		// }
	}

	//clusterofnodes randlouvain::clusteralgorithm()
	//{
	//	vector<clusterofnodes> subgraphs;
	//	for (int i = 0; i < adjlist->numvertices; i++)
	//	{
	//		clusterofnodes a;
	//		a.nodes = get<0>(adjlist[i]);
	//		subgraphs.push_back(a);
	//	}
	//	int count = 0;
	//	while (count != adjlist->numvertices)
	//	{
	//		clusterofnodes simplecluster;
	//		simplecluster.nodes.push_back(); // push count k of graph into the simple cluster. 
	//	}
	//	count++;
	//}
	RandLouvain::~RandLouvain()
	{
		delete adjList;
		delete preClustgerGraph;
		delete postClusterGraph;
	}
}