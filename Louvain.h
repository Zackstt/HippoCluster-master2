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
	class Louvain
	{
	private:

		/*
		may need to change some of the private functions
		*/

		std::vector<int> clusterAssignments;
		AdjacencyList* adjList;
		int numVertices;

	public:

		/*
		may need to change some of these functions also
		*/
		HIPPOCLUSTERLIBRARY_API Louvain(AdjacencyList* AdjList);

		HIPPOCLUSTERLIBRARY_API void step();
		HIPPOCLUSTERLIBRARY_API int getCluster(std::string vertexName);
		// HIPPOCLUSTERLIBRARY_API void getAllClusterAssignments(std::vector<int>& assignments);

		// HIPPOCLUSTERLIBRARY_API void exportClusterAssignments(std::string filename);

		HIPPOCLUSTERLIBRARY_API ~Louvain();
	};
}
