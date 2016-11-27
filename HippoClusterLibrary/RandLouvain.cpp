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


	// This function takes a adjacencylist and builds clusters out of the adjacencylist
	//HIPPOCLUSTERLIBRARY_API std::vector<int> buildClusters();

	// Takes a cluster and the adjacency list the cluster is refering to, returns the aggrigated graph
	// This function aggrigates a graph from a cluster of nodes to a node for each cluster 
	// It also needs to assign proper weights from and to each node and also weights within each node

	// change this so its a void function that just rearanges graph
	void RandLouvain::aggregateGraph(AdjacencyList* graph, vector<int> clusters)
	{
		cout << "clusters list: " << endl;
		for (int i = 0; i < clusters.size(); i++)
		{
			cout << clusters[i] << endl;
		}
		AdjacencyList constructedGraph = new AdjacencyList;
		//int edgesNew;
		// counts clusters. 
		vector<int> verticesCounted;
		//cout << "clusters.size() " << clusters.size() << endl;
		verticesCounted.resize(clusters.size(), 0);

		for (int i = 0; i < clusters.size(); i++)
		{
			//cout << "clusters[i] = " << clusters[i] << endl;
			if(clusters[i]>=0)
				verticesCounted[clusters[i]] = 1;//----------------
		}
		// goes from 0 to 9
		//for (int i = 0; i < clusters.size(); i++)
		//{
		//	cout << "clusters " << clusters[i] << endl;
		//}

		// also goes from 0 to 9
		//for (int i = 0; i < clusters.size(); i++)
		//{
		//	cout << "I :" << i << endl;
		//	verticesCounted[clusters[i]] = 1;
		//}

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
			//cout << "neighborsOfI.size() :" << neighborsOfI.size() << endl;
			//for (int m = 0; m < neighborsOfI.size(); m++)
			//{
				//cout << get<0>(neighborsOfI[m]) << endl;
				//cout << get<1>(neighborsOfI[m]) << endl;
				//cout << "vertex name of node :" << graph->getVertexName(get<0>(neighborsOfI[m])) << endl;
			//}
			//cout << "neighborsOfI.size() = " << neighborsOfI.size() << endl;// = 4
			for (int j = 0; j < neighborsOfI.size(); j++)
			{
				// take the weight from the clusters[node] to clusters[node] and add it to the new adjList
				// have to add edge from clusters vector
				//cout << "clusters[get<0>(neighborsOfI[j])] :" << clusters[get<0>(neighborsOfI[j])] << endl;
				//cout << "clusters[i] :" << clusters[i] << endl;
				//
				//
				//if ((clusters[i] >= 0) && (clusters[get<0>(neighborsOfI[j])] >= 0))
				//{
					//cout << "from :" << graph->getVertexName(clusters[i]) << endl;
					//cout << "to :" << graph->getVertexName(get<0>(neighborsOfI[j])) << endl;
					constructedGraph.addEdge(graph->getVertexName(clusters[i]), graph->getVertexName(clusters[get<0>(neighborsOfI[j])]), get<1>(neighborsOfI[j]), false);
				//}
					
			}
		}
		cout << "constructedGraph: " << endl;
		for (int i = 0; i < constructedGraph.numVertices(); i++)
		{
			cout << constructedGraph.getVertexName(i) << endl;
		}
		*graph = constructedGraph;
		//return constructedGraphOrhave it equal graph;
		// assign graph to the new graph
	}

	// cluster is a vector of nodes

	// getModularity returns a number between -1 and 1 indicating the quality of a clustered graph

	// some values are 0 need to fix this
	double RandLouvain::getModularity(int nodeI, int comunity, vector<int> cluster)
	{
		//cout << "nodeI; comunity; cluster.size(); =" << nodeI << ' ' << comunity << ' ' << cluster.size() << endl;
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
		//cout << "nodeI " << nodeI << endl; // nodeI went to 10. cant do that
		vector<std::tuple<int, double, double>> neighborsOfI = adjList->allNeighbors(nodeI);// broken here
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
		// weightiToCluster and weightIntoCluster = 0 at some points 
		//cout << "weightiToCluster; weightOfNetwork; weightIntoCluster; weightIntoNode;" << weightiToCluster << ' '
			//<< weightOfNetwork << ' ' << weightIntoCluster << ' ' << weightIntoNode << endl;
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
		//for (int i = 0; i < clusters.size(); i++)
		//{
		//	clusters[i] = i;
		//}
		// new AdjacencyList* preClusterGraph; // preform clustering algorithm on this graph, place it into post
		//new AdjacencyList* postClusterGraph; // check and see if this graph is equal to prior graph


		AdjacencyList* preClusterGraph(adjList);  
		AdjacencyList* postClusterGraph(preClusterGraph);

		// test the graph
		//cout << "The graph coppied is : ";
		//for (int i = 0; i < preClusterGraph->numVertices(); i++)
		//{
		//	cout << i << " " << preClusterGraph->getVertexName(i) << endl;
		//}

		// step 1 build initial clusters
		for (int i = 0; i < clusters.size(); i++)
		{
			clusters[i] = i;
			cout << i << " " << adjList->getVertexName(i) << endl;
		}
		bool flag = true;
		while (flag)
		{
			/*issue here with i*/
				// ******* phase 1 *******
				//	// step 2 move nodes between clusters until max modularity is reached
				//cout << "preClusterGraph # verticies : " << preClusterGraph->numVertices() << endl;
			for (int i = 0; i < preClusterGraph->numVertices(); i++)
			{
				int nodeICluster = clusters[i];
				// --- remove node i from its comunity
				clusters[i] = -1;
				// --- get random set of neighbors of node i use shuffle
				// can try randomly mixing a list then only taking the first x% of it
				//		*********will have to change this so it produces random set of neighbors*********
				vector<std::tuple<int, double, double>> iNeighbors = preClusterGraph->allNeighbors(i);
				random_shuffle(iNeighbors.begin(), iNeighbors.end());
				int resizeValue = int(iNeighbors.size() * K_constant);
				resizeValue++;
				iNeighbors.resize(resizeValue);
				// then reduce size of stuf
				//	// --- run getModularity on the list.
				int highestModNode, highestModCluster;
				double modNodeValue = 0.0;
				// double getModularity(int nodeI, int comunity, std::vector<int> cluster)
				//cout << "nodeI, comunity, cluster" << i << ' ' << get<0>(iNeighbors[0]) << " not showing cluser"<< endl;

				//cout << "numverticies :" << preClusterGraph->numVertices() << endl;
				//modNodeValue = getModularity(i, get<0>(iNeighbors[0]), clusters); // goes out of bounds
				//cout << "getMod called value :" << modNodeValue << endl;
				highestModNode = 0;
				highestModCluster = -1;
				//cout << "iNeighbors.size() :" << iNeighbors.size() << endl;
				for (int j = 0; j < iNeighbors.size(); j++)
				{
					//cout << "nodeICluster :" << nodeICluster << endl;
					//cout << "i: " << i << endl;
					if (getModularity(nodeICluster, get<0>(iNeighbors[j]), clusters) > modNodeValue)
					{
						// might need to remember the group things added to
						// need to know highest modularity
						modNodeValue = getModularity(nodeICluster, get<0>(iNeighbors[j]), clusters); // might go to 10
						//cout << "getMod called value :" << modNodeValue << endl;
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
				//if(postClusterGraph->numVertices() == preClusterGraph->numVertices())
				//	for (int i = 0; i < postClusterGraph->numVertices(); i++)
				//	{
				//		if (postClusterGraph->getVertexName(i) != preClusterGraph->getVertexName(i))
				//			flag = false; break;
				//	}
				if(preClusterGraph->numVertices() == postClusterGraph->numVertices())
					flag = false;
			}
			// ******* phase 2 *******
			// step 3 agrigate graph. loop back to beginning if graph is not same as preClusterGraph
			// AdjacencyList aggrigateGraph(AdjacencyList* graph, vector<int> clusters)
			//cout << "preClusterGraph size = " << preClusterGraph->numVertices() << endl
				//<< "clusters.size() = " << clusters.size() << endl;

			// have to check the list fed into aggregateGraph with the preClusterGraph
			// 
			aggregateGraph(postClusterGraph, clusters);
		}

	}

	RandLouvain::~RandLouvain()
	{
		delete adjList;
		delete preClustgerGraph;
		delete postClusterGraph;
	}
}