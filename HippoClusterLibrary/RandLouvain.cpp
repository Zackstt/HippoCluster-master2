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

	RandLouvain::RandLouvain(AdjacencyList* AdjList, double k_constant)
	{
		adjList = AdjList;
		preClustgerGraph = new AdjacencyList;
		postClusterGraph = new AdjacencyList;
		K_constant = k_constant;
		//for (int i = 0; i < adjList->numVertices(); i++)
		//{
			// vector ID represents node ID and the number stored in the vector represents the cluster id
			//RandLouvain::cluster.push_back(i);
			// cluster.nodes.push_back(i);
			// might have to use a vector insted
		//}

		// need to also create the initial cluster here
	}

	AdjacencyList RandLouvain::aggregateGraph(AdjacencyList* graph, vector<int> clusters)
	{
		AdjacencyList constructedGraph = new AdjacencyList;
		int edgesNew;

		// counts clusters. builds 2 dimension vector from number of clusters
		vector<bool> verticies;
		verticies.resize(false, clusters.size());
		for (int i = 0; i < clusters.size(); i++)
		{
			verticies[clusters[i]] = true;
		}

		int numVert = 0;
		for (int i = 0; i < verticies.size(); i++)
		{
			if (verticies[i] == true)
				numVert++;
		}
		// ***addEdge*** function will add weight if there is already a conection
		// find weights within node and between them
		// - go to each neighbor of node i. neighbor will either be in cluster or out of cluster.
		for (int i = 0; i < graph->numVertices(); i++)
		{
			vector<std::tuple<int, double, double>> neighborsOfI = graph->allNeighbors(i);
			for (int j = 0; j < neighborsOfI.size(); j++)
			{
				constructedGraph.addEdge(graph->getVertexName(clusters[i]), graph->getVertexName(get<0>(neighborsOfI[j])), get<1>(neighborsOfI[j]), false);
			}
		}

		return constructedGraph;
		// assign graph to the new graph
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

		AdjacencyList* preClusterGraph(clusterGraph);  
		AdjacencyList* postClusterGraph = new AdjacencyList;

		// test the graph
		//cout << "The graph coppied is : ";
		//for (int i = 0; i < preClusterGraph->numVertices(); i++)
		//{
		//	cout << i << " " << preClusterGraph->getVertexName(i) << endl;
		//}

		// step 1 build initial clusters
		for (int i = 0; i < adjList->numVertices(); i++)
		{
			clusters[i] = i;
			cout << i << " " << adjList->getVertexName(i) << endl;
		}

		while (true)
		{
			// ******* phase 1 *******
			//	// step 2 move nodes between clusters until max modularity is reached
			for (int i = 0; i < preClusterGraph->numVertices(); i++)
			{
				int nodeICluster = cluster[i];
				// --- remove node i from its comunity
				cluster[i] = -1;
				// --- get random set of neighbors of node i use shuffle
				// can try randomly mixing a list then only taking the first x% of it
				//		*********will have to change this so it produces random set of neighbors*********
				vector<std::tuple<int, double, double>> iNeighbors = preClusterGraph->allNeighbors(i);
				random_shuffle(iNeighbors.begin(), iNeighbors.end());
				int resizeValue = int(iNeighbors.size() * K_constant);
				iNeighbors.resize(resizeValue);
				// then reduce size of stuf
				//	// --- run getModularity on the list.
				int highestModNode, highestModCluster;
				double modNodeValue;
				modNodeValue = getModularity(i, get<0>(iNeighbors[0]), cluster);
				highestModNode = 0;
				for (int j = 1; j < iNeighbors.size(); i++)
				{
					if (getModularity(i, get<0>(iNeighbors[j]), cluster) > modNodeValue)
					{
						// might need to remember the group things added to
						// need to know highest modularity
						modNodeValue = getModularity(nodeICluster, get<0>(iNeighbors[j]), cluster);
						// need to know what node is responsable for it
						highestModNode = i;
						// need to know what cluster the node is being added to
						highestModCluster = get<0>(iNeighbors[j]);
					}
				}
				// --- if max mod > 0, add to that community, if not return node to its own community
				if (modNodeValue > 0)
					clusters[i] = highestModCluster;
				else
					clusters[i] = nodeICluster;
			}
			// ******* phase 2 *******
			// step 3 agrigate graph. loop back to beginning if graph is not same as preClusterGraph
			// AdjacencyList aggrigateGraph(AdjacencyList* graph, vector<int> clusters)
			//postClusterGraph = aggrigateGraph(preClusterGraph,clusters)
		}





	}

	RandLouvain::~RandLouvain()
	{
		delete adjList;
		delete preClustgerGraph;
		delete postClusterGraph;
	}
}