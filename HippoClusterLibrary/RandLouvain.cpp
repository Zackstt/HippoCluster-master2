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
		clusterOfNodes cluster;
		for(int i=0; i < adjList->numVertices(); i++)
			cluster.nodes.push_back(i);
	}


	// cluster is a vector of nodes
	double RandLouvain::getModularity(int nodeI, clusterOfNodes cluster)
	{
		double weightOfNetwork = adjList->sumWeights();

		// weightIntoCluster completed
		double weightIntoCluster = 0, weightIntoNode = 0, weightiToCluster = 0;

		// sum of weights from i to Cj
		for (int i = 0; i < cluster.nodes.size(); i++)
		{
			// get all neighbors of node i
			vector<tuple<int, double, double>> neighbors = adjList->allNeighbors(i);
			// go through all neighbors of node i and check if they are in cluster. If not add wieght to weightIntoCluster
			for (int j = 0; j < neighbors.size(); j++)
			{//***************************************************I think its this == that is the problem************************************
				if (find(cluster.nodes.begin(), cluster.nodes.end(), neighbors[j]) == cluster.nodes.end())
				{
					weightIntoCluster += get<1>(neighbors[j]);
				}
			}
		}

		// sum of weights into Ci
		for (int i = 0; i < cluster.nodes.size(); i++)
		{
			// get all neighbors of node i
			vector<tuple<int, double, double>> iNeighbors = adjList->allNeighbors(i);
			// go through all neighbors of node i and check if they are in cluster. If they are not add wieght to weightIntoCluster
			for (int j = 0; j < iNeighbors.size(); i++)
			{// ***************************************************I think its this == that is the problem************************************
				if (find(cluster.nodes.begin(), cluster.nodes.end(), iNeighbors[j]) != cluster.nodes.end())
				{
					weightIntoCluster += get<1>(iNeighbors[j]);
				}
			}
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
	
	

	clusterOfNodes RandLouvain::clusterAlgorithm()
	{
		vector<clusterOfNodes> subGraphs;
		for (int i = 0; i < adjList->numVertices; i++)
		{
			clusterOfNodes a;
			a.nodes = get<0>(adjList[i]);
			subGraphs.push_back(a);
		}
		int count = 0;
		while (count != adjList->numVertices)
		{
			clusterOfNodes simpleCluster;
			simpleCluster.nodes.push_back(); // push count k of graph into the simple cluster. 
		}
		count++;
	}

}