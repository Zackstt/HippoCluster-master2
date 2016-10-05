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

		AdjacencyList* adjList;
		clusterOfNodes cluster;

	public:


		HIPPOCLUSTERLIBRARY_API RandLouvain(AdjacencyList* adjList);

		// constructs a new cluster *** I need to make sure the new cluster contains both a vector of subClusters and all the nodes in the cluster
		//HIPPOCLUSTERLIBRARY_API void cluster(); // *** may implement variables that can control cluster 

		// getModularity returns a number between -1 and 1 indicating the quality of a clustered graph
		HIPPOCLUSTERLIBRARY_API double getModularity(int nodeI, clusterOfNodes cluster);
		HIPPOCLUSTERLIBRARY_API clusterOfNodes clusterAlgorithm();

		HIPPOCLUSTERLIBRARY_API ~RandLouvain();
	};
}
