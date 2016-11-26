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
		double K_constant;

		// cluster contains <nodeID, clusterID> nodeID is the id of the node, clusterID is the cluster its assigned
		// to
		// ***
		// use a vector. the ID is the number of the cluster, the set contains info on the nodes it contains and 
		// the weights. might even be easier with a AdjacencyList structure
		// ***
		// multiset<string, int> cluster;

		// clusterOfNodes cluster;

	public:

		HIPPOCLUSTERLIBRARY_API RandLouvain(AdjacencyList* adjList, double k_constant);

		HIPPOCLUSTERLIBRARY_API void aggregateGraph(AdjacencyList* graph, std::vector<int> clusters);
		HIPPOCLUSTERLIBRARY_API double getModularity(int nodeI, int comunity, std::vector<int> cluster);
		HIPPOCLUSTERLIBRARY_API void clusterAlgorithm();

		HIPPOCLUSTERLIBRARY_API ~RandLouvain();
	};
}
