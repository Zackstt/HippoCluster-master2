#pragma once

#ifdef HIPPOCLUSTERLIBRARY_EXPORTS
#define HIPPOCLUSTERLIBRARY_API __declspec(dllexport) 
#else
#define HIPPOCLUSTERLIBRARY_API __declspec(dllimport) 
#endif

#include <vector>
#include "AdjacencyList.h"

namespace HippoClusterLibrary
{
	class RandLouvain
	{
		struct clusterOfNodes {
			std::vector<int> nodes;
			std::vector<clusterOfNodes> subCluster;
		};

	private:
		// cluster contains <nodeID, clusterID> nodeID is the id of the node, clusterID is the cluster its assigned
		// to
		map<int, int> cluster;
		AdjacencyList* adjList;
		clusterOfNodes cluster;

	public:


		HIPPOCLUSTERLIBRARY_API RandLouvain(AdjacencyList* adjList);

		// ***addEdge***

		// constructs a new cluster *** I need to make sure the new cluster contains both a vector of subClusters and all the nodes in the cluster
		//HIPPOCLUSTERLIBRARY_API void cluster(); // *** may implement variables that can control cluster TEST2

		// Takes a cluster and the adjacency list the cluster is refering to, returns the aggrigated graph
		// This function aggrigates a graph from a cluster of nodes to a node for each cluster 
		// It also needs to assign proper weights from and to each node and also weights within each node
		HIPPOCLUSTERLIBRARY_API AdjacencyList* aggrigateGraph(map<int, int> cluster, AdjacencyList* adjList);

		// getModularity returns a number between -1 and 1 indicating the quality of a clustered graph
		HIPPOCLUSTERLIBRARY_API double getModularity(int nodeI, clusterOfNodes cluster);

		// This function is the primary graph clustering code. It 
		// Parameters: numberConsideredNeibours; % of node neighbors considered
		//			   
		// Takes adjacency list and uses functions within the class to preform the algorithm
		HIPPOCLUSTERLIBRARY_API std::map<int, int> clusterAlgorithm(const AdjacencyList* adjList);

		
		HIPPOCLUSTERLIBRARY_API ~RandLouvain();
	};
}
