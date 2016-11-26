// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../HippoClusterLibrary/AdjacencyList.h"
#include "../HippoClusterLibrary/HippoCluster.h"
#include "../HippoClusterLibrary/RandLouvain.h"
#include <iostream>
#include <vector>
using namespace std;
using namespace HippoClusterLibrary;

int main()
{
	// read in the test graph
	AdjacencyList adjacencyList;
	adjacencyList.fromTSV("testGraph.tsv");

	// another way to create the graph is to add edge info one-by-one
	//AdjacencyList adjacencyList;
	//adjacencyList.addEdge("A", "B", 1);
	//adjacencyList.addEdge("B", "A", 1);
	//adjacencyList.addEdge("D", "A", 0.1);
	//etc...

	// list all vertex names and assigned id numbers
	cout << "vertex name	id" << endl;
	for (int i = 0; i < adjacencyList.numVertices(); i++)
	{
		cout << adjacencyList.getVertexName(i) << "\t" << i << endl;
	}

	// show some info about the graph
	cout << "number of vertices: " << adjacencyList.numVertices() << endl;
	cout << "number of edges:    " << adjacencyList.numEdges() << endl;

	// find strongly connected components
	vector<vector<int>*> sccs;
	int maxIndex = adjacencyList.findSCCs(sccs);

	// keep only the biggest SCC
	cout << "reducing to biggest SCC" << endl;
	AdjacencyList mainSCC = adjacencyList.subgraph(sccs[maxIndex]);

	// list all vertex names and assigned id numbers
	cout << "vertex name	id" << endl;
	for (int i = 0; i < mainSCC.numVertices(); i++)
	{
		cout << mainSCC.getVertexName(i) << "\t" << i << endl;
	}

	// RandLouvain stuff

	cout << "*******randLouvain output: " << endl;
	RandLouvain rl(&adjacencyList, 0.75); 

	rl.clusterAlgorithm();

	vector<int> clusterlist;
	cout << "point 1" << endl;
	clusterlist.resize(10);
	clusterlist[0]=0;
	clusterlist[1]=0;
	clusterlist[2]=0;
	clusterlist[3]=1;
	clusterlist[4]=1;
	clusterlist[5]=1;
	clusterlist[6]=2;
	clusterlist[7]=2;
	clusterlist[8]=2;
	clusterlist[9]=2;

	AdjacencyList* ranLouv = new AdjacencyList;
	
	//rl.clusterAlgorithm();

	

	rl.aggregateGraph(&adjacencyList, clusterlist);

	cout << "adjacencyList size :" << adjacencyList.numVertices() << endl;
	
	for (int i = 0; i < adjacencyList.numVertices(); i++)
	{
		cout << adjacencyList.getVertexName(i) << "     " << i << endl;
	}

	//rl.aggregateGraph(&adjacencyList, clusterlist);

	cout << "ranLouv elements :" << endl;

	

	cout << "point 2" << endl;

	//rl.clusterAlgorithm(&adjacencyList);
	cout << "point 3" << endl;
	// double getModularity(int nodeI, int comunity, std::vector<int> cluster)
	cout << "get modularity returns: " << rl.getModularity(2, 0, clusterlist) << endl;

	cout << "point 4" << endl;
	// run a clustering algorithm on the full graph
	

	cout << "*******end of randLouvain" << endl << endl;
	//cout << "running the new clustering algorith for 1000 iterations" << endl;
	//HippoCluster hc(&adjacencyList);
	//hc.setNumNodes(10);
	//hc.setTrajectoryLength(adjacencyList.numVertices());
	//for (int i = 0; i < 1000; i++)
	//{
	//	hc.step();
	//	hc.dropUnusedNodes();
	//}
	//cout << "vertex name	cluster assignment" << endl;
	//vector<int> assignments;
	//hc.getAllClusterAssignments(assignments);
	//for (int i = 0; i < assignments.size(); i++)
	//{
	//	cout << adjacencyList.getVertexName(i) << "\t" << assignments[i] << endl;
	//}

	//// display some stats for the clustering
	//ClusterStats stats = adjacencyList.getClusterStats(assignments);
	//cout << "mean cluster size: " << stats.getClusterSizeMean() << " +/- " << stats.getClusterSizeSD() << endl;
	//cout << "weighted average relative density: " << stats.getWeightedAvgRelativeDensity() << endl;


	getchar();
    return 0;
}

