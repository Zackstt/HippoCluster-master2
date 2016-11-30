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
	// read in the .graph
	AdjacencyList adjacencyList1;
	adjacencyList1.fromGRAPH("PGPgiantcompo.graph");
	RandLouvain test1(&adjacencyList1, 0.25);
	test1.clusterAlgorithm();
	cout << "PGPgiantcompo cluster graph :" << endl;
	cout << "vertex name	id" << endl;
	for (int i = 0; i < adjacencyList1.numVertices(); i++)
	{
		cout << adjacencyList1.getVertexName(i) << "\t" << i << endl;
	}

	// another way to create the graph is to add edge info one-by-one
	AdjacencyList adjacencyList2;
	adjacencyList2.addEdge("A", "B", 1);
	adjacencyList2.addEdge("B", "A", 1);
	adjacencyList2.addEdge("B", "C", 1);
	adjacencyList2.addEdge("C", "B", 1);
	adjacencyList2.addEdge("C", "D", 1);
	adjacencyList2.addEdge("D", "C", 1);
	adjacencyList2.addEdge("D", "A", 1);
	adjacencyList2.addEdge("A", "D", 1);
	adjacencyList2.addEdge("C", "D", 1);
	adjacencyList2.addEdge("D", "C", 1);
	adjacencyList2.addEdge("A", "C", 1);
	adjacencyList2.addEdge("C", "A", 1);

	adjacencyList2.addEdge("1A", "1B", 1);
	adjacencyList2.addEdge("1B", "1A", 1);
	adjacencyList2.addEdge("1B", "1C", 1);
	adjacencyList2.addEdge("1C", "1B", 1);
	adjacencyList2.addEdge("1C", "1D", 1);
	adjacencyList2.addEdge("1D", "1C", 1);
	adjacencyList2.addEdge("1D", "1A", 1);
	adjacencyList2.addEdge("1A", "1D", 1);
	adjacencyList2.addEdge("1C", "1D", 1);
	adjacencyList2.addEdge("1D", "1C", 1);
	adjacencyList2.addEdge("1A", "1C", 1);
	adjacencyList2.addEdge("1C", "1A", 1);

	adjacencyList2.addEdge("2A", "2B", 1);
	adjacencyList2.addEdge("2B", "2A", 1);
	adjacencyList2.addEdge("2B", "2C", 1);
	adjacencyList2.addEdge("2C", "2B", 1);
	adjacencyList2.addEdge("2C", "2D", 1);
	adjacencyList2.addEdge("2D", "2C", 1);
	adjacencyList2.addEdge("2D", "2A", 1);
	adjacencyList2.addEdge("2A", "2D", 1);
	adjacencyList2.addEdge("2C", "2D", 1);
	adjacencyList2.addEdge("2D", "2C", 1);
	adjacencyList2.addEdge("2A", "2C", 1);
	adjacencyList2.addEdge("2C", "2A", 1);

	adjacencyList2.addEdge("A", "1C", 1);
	adjacencyList2.addEdge("1C", "A", 1);
	adjacencyList2.addEdge("1B", "2A", 1);
	adjacencyList2.addEdge("2A", "1B", 1);
	adjacencyList2.addEdge("2D", "D", 1);
	adjacencyList2.addEdge("D", "2D", 1);

	cout << "adjacencyList2 before cluster:" << endl;
	cout << "vertex name	    id" << endl;
	cout << "adjList2.numvert :" << adjacencyList2.numVertices() << endl;
	for (int i = 0; i < adjacencyList2.numVertices(); i++)
	{
		cout << adjacencyList2.getVertexName(i) << "\t" << i << endl;
	}

	RandLouvain randL(&adjacencyList2, 0.75);
	randL.clusterAlgorithm();

	cout << "adjacencyList2 after cluster:" << endl;
	cout << "vertex name	    id" << endl;
	for (int i = 0; i < adjacencyList2.numVertices(); i++)
	{
		cout << adjacencyList2.getVertexName(i) << "\t" << i << endl;
	}

	//adjacencyList1.addEdge("D", "A", 0.1);
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

	for (int i = 0; i < adjacencyList.numVertices(); i++)
	{
		cout << adjacencyList.getVertexName(i) << "\t" << i << endl;
	}
	

	AdjacencyList* ranLouv = new AdjacencyList;
	


	

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

