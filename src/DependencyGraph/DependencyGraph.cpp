#include"DependencyGraph.h"

#define NIL -1
int min(int, int);


GraphNode::GraphNode(string name, string dependName, int index, Symbol* nodeSymbol) :name(name), dependName(dependName), index(index)
, nodeSymbol(nodeSymbol){};



DependencyGraph::DependencyGraph(ErrorRecovery* errRecovery){
	this->errRecovery = errRecovery;
	this->strCComp = new list<int>[MAX_NUMBER_OF_SCC];
	this->compoCounter = -1;
	this->totalNodes = 0;
}

void DependencyGraph::insertIntoNodes(char* nodeName, Symbol* nodeSymbol, string dependName){
	nodes.push_back(new GraphNode(nodeName, dependName, this->totalNodes++, nodeSymbol));
}

void DependencyGraph::createGraph(){
	this->adj = new list<int>[this->totalNodes];

	list<GraphNode*>::iterator i;
	for (i = this->nodes.begin(); i != this->nodes.end(); ++i){ // for every node
		GraphNode n = **i;
		list<GraphNode*>::iterator j;
		int depenIndex = -1; // the dependent index
		for (j = this->nodes.begin(); j != this->nodes.end(); ++j){
			GraphNode depNode = **j;
			if (depNode.name == n.dependName){
				depenIndex = depNode.index;
				break;
			}
		}
		if (depenIndex == -1){
			//dependent is undefined
			cout << "undefined dependent" << endl;
		}
		addEdge(n.index, depenIndex); // adding edge between a node and its dependent
	}

}

void DependencyGraph::addEdge(int v, int w){
	adj[v].push_back(w);
}

void DependencyGraph::printGraph(){
	os.open("graph_log.txt", fstream::out);
	os << "TOTAL NODES: " << this->totalNodes << endl;
	list<GraphNode*>::iterator i;
	for (i = this->nodes.begin(); i != this->nodes.end(); ++i){
		GraphNode* n = *i;
		os << "class name :" << n->name << " index:" << n->index << endl;
	}

	list<int>::iterator i2;
	for (int j = 0; j < this->totalNodes; j++){
		os << j << " with ";
		for (i2 = this->adj[j].begin(); i2 != this->adj[j].end(); ++i2){
			int ind = *i2;
			os << ind << " ";
		}
	}
	os << "\nstrongly connected components:\n";
	os << this->compoCounter << endl;
	for (int j = 0; j <= this->compoCounter; j++){
		for (i2 = this->strCComp[j].begin(); i2 != this->strCComp[j].end(); ++i2){
			int ind = *i2;
			os << ind << " ";
		}
		os << endl;
	}


}

void DependencyGraph::checkDependency(){
	this->createGraph();
	this->SCC();
}

// A recursive function that finds and prints strongly connected
// components using DFS traversal
// u --> The vertex to be visited next
// disc[] --> Stores discovery times of visited vertices
// low[] -- >> earliest visited vertex (the vertex with minimum
//             discovery time) that can be reached from subtree
//             rooted with current vertex
// *st -- >> To store all the connected ancestors (could be part
//           of SCC)
// stackMember[] --> bit/index array for faster check whether
//                  a node is in stack
void DependencyGraph::SCCUtil(int u, int disc[], int low[],
	stack<int> *st, bool stackMember[]){
	// A static variable is used for simplicity, we can avoid use
	// of static variable by passing a pointer.
	static int time = 0;

	// Initialize discovery time and low value
	disc[u] = low[u] = ++time;
	st->push(u);
	stackMember[u] = true;

	// Go through all vertices adjacent to this
	list<int>::iterator i;
	for (i = adj[u].begin(); i != adj[u].end(); ++i)
	{
		int v = *i;  // v is current adjacent of 'u'

		// If v is not visited yet, then recur for it
		if (disc[v] == -1)
		{
			SCCUtil(v, disc, low, st, stackMember);

			// Check if the subtree rooted with 'v' has a
			// connection to one of the ancestors of 'u'
			// Case 1 (per above discussion on Disc and Low value)
			low[u] = min(low[u], low[v]);
		}

		// Update low value of 'u' only of 'v' is still in stack
		// (i.e. it's a back edge, not cross edge).
		// Case 2 (per above discussion on Disc and Low value)
		else if (stackMember[v] == true)
			low[u] = min(low[u], disc[v]);
	}

	// head node found, pop the stack and print an SCC
	int w = 0;  // To store stack extracted vertices
	if (low[u] == disc[u])
	{
		this->compoCounter++;//we have a scc
		while (st->top() != u)
		{
			w = (int)st->top();
			this->strCComp[compoCounter].push_back(w);
			cout << w << " |";
			stackMember[w] = false;
			st->pop();
		}
		w = (int)st->top();
		this->strCComp[compoCounter].push_back(w);
		cout << w << "\n";
		stackMember[w] = false;
		st->pop();
	}
}

// The function to do DFS traversal. It uses SCCUtil()
void DependencyGraph::SCC()
{
	int *disc = new int[this->totalNodes];
	int *low = new int[this->totalNodes];
	bool *stackMember = new bool[this->totalNodes];
	stack<int> *st = new stack<int>();

	// Initialize disc and low, and stackMember arrays
	for (int i = 0; i < this->totalNodes; i++)
	{
		disc[i] = NIL;
		low[i] = NIL;
		stackMember[i] = false;
	}

	// Call the recursive helper function to find strongly
	// connected components in DFS tree with vertex 'i'
	for (int i = 0; i < this->totalNodes; i++)
	if (disc[i] == NIL)
		SCCUtil(i, disc, low, st, stackMember);
}

int min(int num1, int num2){
	if (num1 < num2)
		return num1;
	return num2;
}

// report circular dependency using errRecovery.
// we face a circle when we have a strongly connected component size more than one node.
void DependencyGraph::reportCircles(){
	for (int j = 0; j <= this->compoCounter; j++){
		if (this->strCComp[j].size() != 1){ // we have a circle
			//walk the nodes making the circle
			//and fetch thier associated Class Symbol
			//and use it to report the error
			list<int>::iterator i;
			string dependencyPath = ""; // used to construct a string of named circling classes
			for (i = this->strCComp[j].begin(); i != this->strCComp[j].end(); ++i){
				int ind = *i;

				list<GraphNode*>::iterator nodesIter;
				for (nodesIter = this->nodes.begin(); nodesIter != this->nodes.end(); ++nodesIter){
					GraphNode* node = *nodesIter;
					if (node->index == ind){ // the node
						dependencyPath += node->name + " ";
						this->errRecovery->errQ->enqueue(node->nodeSymbol->getLineNo(), node->nodeSymbol->getColNo(), "creates a circular depenedency", "");
					}
				}

			}
			dependencyPath += " creates a dependency circle";
			this->errRecovery->errQ->enqueue(0, 0,dependencyPath.c_str(), "");
		}

	}
}

void DependencyGraph::generate_dot_file(ostream &o) {
	o<<"digraph \"Dependecy Graph\" {"<<endl;
	o<<"fontname = \"Helvetica\"; fontsize = 11; rankdir=BT"
		<<endl
		<<"node [ fontname = \"Helvetica\"; fontsize = 11; ]"<<endl;

	for (auto node : nodes) {
		o<<node->index<<"[label = "<<node->name<<"]"<<endl;
	}
	for (auto v : nodes) {
		for (auto u_index : adj[v->index]) {
			if (u_index == -1) continue;
			o<<v->index<<"->"<<u_index<<";\n";
		}
	}

	o<<"}"<<endl;
}