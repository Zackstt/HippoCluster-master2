#pragma once

#ifdef HIPPOCLUSTERLIBRARY_EXPORTS
#define HIPPOCLUSTERLIBRARY_API __declspec(dllexport) 
#else
#define HIPPOCLUSTERLIBRARY_API __declspec(dllimport) 
#endif

#include <vector>
#include <map>
#include <string>
#include "AdjacencyList.h"


// General idea: 1 make an initial cluster with a vector (reason for this is because we can use the 
//                  cluster as the value and the node it belongs to as the id of the vector)
//                  we need to do this because changing the nodes cluster is very quick
//               2 maximize modularity of clusters
//               3 compare the initial cluster to final cluster. 
//                  If initial cluster is better make it initial by creating new adjlist. 
//                  else its the same graph is complete

namespace HippoClusterLibrary
{
	class RandLouvain
	{
		/*struct clusterOfNodes {
			std::vector<int> nodes;
			std::vector<clusterOfNodes> subCluster;
		};*/

	private:
		std::vector<int> cluster;
		AdjacencyList* adjList;
		AdjacencyList* preClustgerGraph;
		AdjacencyList* postClusterGraph;

		// cluster contains <nodeID, clusterID> nodeID is the id of the node, clusterID is the cluster its assigned
		// to
		// ***
		// use a vector. the ID is the number of the cluster, the set contains info on the nodes it contains and 
		// the weights. might even be easier with a AdjacencyList structure
		// ***
		// multiset<string, int> cluster;

		// clusterOfNodes cluster;

	public:


		HIPPOCLUSTERLIBRARY_API RandLouvain(AdjacencyList* adjList);

		// ***addEdge***

		// constructs a new cluster *** I need to make sure the new cluster contains both a vector of subClusters and all the nodes in the cluster
		//HIPPOCLUSTERLIBRARY_API void cluster(); // *** may implement variables that can control cluster TEST2

		// This function takes a adjacencylist and builds clusters out of the adjacencylist
		HIPPOCLUSTERLIBRARY_API std::vector<int> buildClusters();

		// Takes a cluster and the adjacency list the cluster is refering to, returns the aggrigated graph
		// This function aggrigates a graph from a cluster of nodes to a node for each cluster 
		// It also needs to assign proper weights from and to each node and also weights within each node
		HIPPOCLUSTERLIBRARY_API AdjacencyList* aggrigateGraph(/*multimap<int, int> cluster, AdjacencyList* adjList*/);

		// getModularity returns a number between -1 and 1 indicating the quality of a clustered graph
		HIPPOCLUSTERLIBRARY_API double getModularity(int nodeI, std::vector<int> cluster);

		// This function is the primary graph clustering code. It 
		// Parameters: numberConsideredNeibours; % of node neighbors considered
		//			   
		// Takes adjacency list and uses functions within the class to preform the algorithm
		HIPPOCLUSTERLIBRARY_API void clusterAlgorithm(AdjacencyList *clusterGraph);

		
		HIPPOCLUSTERLIBRARY_API ~RandLouvain();
	};
}
