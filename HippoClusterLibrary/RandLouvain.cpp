#include "stdafx.h"
#include "RandLouvain.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <ppl.h>
#include <algorithm>
#include <assert.h>

using namespace std;

namespace HippoClusterLibrary
{

	RandLouvain::RandLouvain(AdjacencyList* AdjList, double k_constant)
	{
		adjList = AdjList;
		clusteredGraph = new AdjacencyList(adjList);
		//preClustgerGraph = new AdjacencyList;
		//postClusterGraph = new AdjacencyList;
		K_constant = k_constant;

	}


	// This function takes a adjacencylist and builds clusters out of the adjacencylist
	//HIPPOCLUSTERLIBRARY_API std::vector<int> buildClusters();

	// Takes a cluster and the adjacency list the cluster is refering to, returns the aggrigated graph
	// This function aggrigates a graph from a cluster of nodes to a node for each cluster 
	// It also needs to assign proper weights from and to each node and also weights within each node

	// change this so its a void function that just rearanges graph
	void RandLouvain::aggregateGraph(AdjacencyList* graph, vector<int> clusters)
	{
		AdjacencyList constructedGraph = new AdjacencyList;
		//int edgesNew;
		// counts clusters. 
		vector<int> verticesCounted;
		verticesCounted.resize(clusters.size(), 0);

		for (int i = 0; i < clusters.size(); i++)
		{
			if(clusters[i]>=0)
				verticesCounted[clusters[i]] = 1;//----------------
		}

		int numVert = 0;
		for (int i = 0; i < verticesCounted.size(); i++)
		{
			if (verticesCounted[i] == 1)
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
				// take the weight from the clusters[node] to clusters[node] and add it to the new adjList
				// have to add edge from clusters vector
					constructedGraph.addEdge(graph->getVertexName(clusters[i]), graph->getVertexName(clusters[get<0>(neighborsOfI[j])]), get<1>(neighborsOfI[j]), false);
					
			}
		}
		*graph = constructedGraph;
		// assign graph to the new graph
	}

	// cluster is a vector of nodes

	// getModularity returns a number between -1 and 1 indicating the quality of a clustered graph

	// some values are 0 need to fix this
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

		double weightOfNetwork = clusteredGraph->sumWeights();// changed adjList to ClusteredGraph
		// weightIntoCluster completed
		double weightIntoCluster = 0, weightIntoNode = 0, weightiToCluster = 0;

		// sum of weights from i to Cj
		// --- get all neighbors of i
		//cout << "test point 1" << endl;
		vector<std::tuple<int, double, double>> neighborsOfI = clusteredGraph->allNeighbors(nodeI);// changed adjList to ClusteredGraph
		//cout << "test point 2" << endl;
		// --- get sumation of weights
		for (int i = 0; i < neighborsOfI.size(); i++)
		{
			if(binary_search(nodesInComunity.begin(), nodesInComunity.end(),comunity))
				weightiToCluster += get<1>(neighborsOfI[i]);
		}
		// sum of weights into ci
		// is the sum of the weights of the edges into nodes in Cj
		
		// need to go through cluster list, find all neighbors of those nodes and 
		vector<int> neighbors;
		vector<std::tuple<int, double, double>> neighborList;
		for (int i = 0; i < nodesInComunity.size(); i++)
		{
			vector<tuple<int, double, double>> tempHolder = clusteredGraph->allNeighbors(i);// changed adjList to ClusteredGraph
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
		vector<tuple<int, double, double>> iNeighbors = clusteredGraph->allNeighbors(nodeI);// changed adjList to ClusteredGraph
		for (int i = 0; i < iNeighbors.size(); i++)
		{
			weightIntoNode += get<1>(iNeighbors[i]);
		}

		// final equation
		return((weightiToCluster / (2 * weightOfNetwork)) -
			((weightIntoCluster*weightIntoNode)/(2 * weightOfNetwork*weightOfNetwork)) );
	}
	
	// This function is the primary graph clustering code. It 
	// Parameters: numberConsideredNeibours; % of node neighbors considered
	//			   
	// Takes adjacency list and uses functions within the class to preform the algorithm
	void RandLouvain::clusterAlgorithm()
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

		AdjacencyList* preClusterGraph(adjList);  



		// step 1 build initial clusters
		for (int i = 0; i < clusters.size(); i++)
		{
			clusters[i] = i;
		}
		int count = 0;
		int verticies = 0;
		bool flag = true;
		vector<std::tuple<int, double, double>> iNeighbors;
		while (flag)
		{
			AdjacencyList* postClusterGraph(preClusterGraph);
			cout << count << endl;
			count++;
			
			cout << "number of verticies :" << preClusterGraph->numVertices() << endl;
			/*issue here with i*/
				// ******* phase 1 *******
				//	// step 2 move nodes between clusters until max modularity is reached
			verticies = preClusterGraph->numVertices();
			//clusters.resize(verticies);
			for (int i = 0; i < verticies; i++)
			{

				//if (preClusterGraph->numVertices() % 100 == 0)
				//	cout << preClusterGraph->numVertices() << endl;
				int nodeICluster = clusters[i];
				// --- remove node i from its comunity
				clusters[i] = -1;
				// --- get random set of neighbors of node i use shuffle
				iNeighbors.clear();
				iNeighbors = preClusterGraph->allNeighbors(i);
				//random_shuffle(iNeighbors.begin(), iNeighbors.end());
				//int resizeValue = int(iNeighbors.size() * K_constant);
				//resizeValue++;
				//iNeighbors.resize(resizeValue);
				//sort(iNeighbors.begin(),iNeighbors.end());
				// then reduce size of stuf
				//	// --- run getModularity on the list.
				int highestModNode, highestModCluster;
				double modNodeValue = 0.0;
				highestModNode = 0;
				highestModCluster = -1;
				cout << "Getting highest modularity" << endl;
				cout << "At" << i << endl;
				srand(time(NULL));
				for (int j = 0; j < iNeighbors.size(); j++)
				{
					// if(rand%100 < 100*numberOfNeibhorsgoingto)
					if(rand()%100 < K_constant*100)
						if (getModularity(nodeICluster, get<0>(iNeighbors[j]), clusters) > modNodeValue)
						{
							// might need to remember the group things added to
							// need to know highest modularity
							modNodeValue = getModularity(nodeICluster, get<0>(iNeighbors[j]), clusters); // might go to 10
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
					clusters[i] = i;

			}
			// ******* phase 2 *******
			// step 3 agrigate graph. loop back to beginning if graph is not same as preClusterGraph

			cout << "Reducing graph" << endl;
			// have to check the list fed into aggregateGraph with the preClusterGraph
			aggregateGraph(postClusterGraph, clusters);
			cout << "PreCluster; postCluster; " << preClusterGraph->numVertices() << ' ' << postClusterGraph->numVertices() << endl;
			if (verticies == postClusterGraph->numVertices()+1)
				flag = false;
		}
		clusteredGraph = new AdjacencyList(preClusterGraph);

	}

	RandLouvain::~RandLouvain()
	{
		delete adjList;
		delete clusteredGraph;
		//delete preClustgerGraph;
		//delete postClusterGraph;
	}
}