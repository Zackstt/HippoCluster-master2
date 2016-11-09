#include "stdafx.h"
#include "RandLouvain.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <ppl.h>

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
	double RandLouvain::getModularity(int nodeI, vector<int> cluster)
	{
		double weightOfNetwork = adjList->sumWeights();

		// weightIntoCluster completed
		double weightIntoCluster = 0, weightIntoNode = 0, weightiToCluster = 0;

		// sum of weights from i to Cj
		//for (int i = 0; i < cluster.nodes.size(); i++)
		//{
		//	// get all neighbors of node i
		//	vector<tuple<int, double, double>> neighbors = adjList->allNeighbors(i);
		//	// go through all neighbors of node i and check if they are in cluster. If not add wieght to weightIntoCluster
		//	for (int j = 0; j < neighbors.size(); j++)
		//	{//***************************************************I think its this == that is the problem************************************
		//		//if (find(cluster.nodes.begin(), cluster.nodes.end(), neighbors[j]) == cluster.nodes.end())
		//		//{
		//		//	weightIntoCluster += get<1>(neighbors[j]);
		//		//}
		//	}
		//}

		// sum of weights into Ci
		//for (int i = 0; i < cluster.nodes.size(); i++)
		//{
		//	// get all neighbors of node i
		//	vector<tuple<int, double, double>> iNeighbors = adjList->allNeighbors(i);
		//	// go through all neighbors of node i and check if they are in cluster. If they are not add wieght to weightIntoCluster
		//	for (int j = 0; j < iNeighbors.size(); i++)
		//	{// ***************************************************I think its this == that is the problem************************************
		//		//if (find(cluster.nodes.begin(), cluster.nodes.end(), iNeighbors[j]) != cluster.nodes.end())
		//		//{
		//		//	weightIntoCluster += get<1>(iNeighbors[j]);
		//		//}
		//	}
		//}

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
		// General idea: 1 make an initial cluster with a vector (reason for this is because we can use the 
		//                  cluster as the value and the node it belongs to as the id of the vector)
		//                  we need to do this because changing the nodes cluster is very quick
		//               2 maximize modularity of clusters
		//               3 compare the initial cluster to final cluster. 
		//                  If initial cluster is better make it initial by creating new adjlist. 
		//                  else its the same graph is complete

		vector<int> clusters;
		// new AdjacencyList* preClusterGraph; // preform clustering algorithm on this graph, place it into post
		//new AdjacencyList* postClusterGraph; // check and see if this graph is equal to prior graph

		AdjacencyList* preClusterGraph = new AdjacencyList;  
		AdjacencyList* postClusterGraph = new AdjacencyList;

		// step 1 build initial clusters
		for (int i = 0; i < adjList->numVertices(); i++)
		{
			cout << i << " " << adjList->getVertexName(i) << endl;
		}

		// step 2 move nodes between clusters until max modularity is reached

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