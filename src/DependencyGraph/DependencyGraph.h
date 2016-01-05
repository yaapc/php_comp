// A class that finds strongly connected components (Circular Dependency) in a given
// directed graph using Tarjan's algorithm (single DFS)
// References : 
//   http://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/
//   https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm


#pragma once

#include "../SymbolTable/Symbol.h"
#include "../ErrorRecovery/ErrorRecovery.h"
#include<list>
#include<stack>
#include<fstream>

using namespace std;


class GraphNode {
public:
	string name;
	string dependName;
	int index;
	Symbol* nodeSymbol;

	GraphNode(string name, string dependName, int index, Symbol* nodeSymbol);
};


class DependencyGraph {
public:
	DependencyGraph(ErrorRecovery* errRecovery);

	void insertIntoNodes(char* nodeName, Symbol* nodeSymbol, string dependName);
	
	void checkDependency();
	void reportCircles(); // report a circular dependency using errRecovery

	void printGraph();
private:
	void createGraph();
	list<int> *adj; // A dynamic array of adjacency lists
	list<int> *strCComp; // A dynamic array of strongly connected components
	static const int MAX_NUMBER_OF_SCC = 100;
	int compoCounter; // strongly connected components counter


	list<GraphNode*>  nodes; // List of nodes
	int totalNodes; // No. of vertices

	// A Recursive DFS based function used by SCC()
	void SCCUtil(int u, int disc[], int low[],
		stack<int> *st, bool stackMember[]);

	void SCC(); // catches Strongly Connected Components

	void addEdge(int v, int w);

	ErrorRecovery* errRecovery;

	
	ofstream os;
};
