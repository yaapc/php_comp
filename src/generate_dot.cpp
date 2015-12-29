#include "generate_dot.hpp"
#include <vector>
#include <string>
#include <algorithm>

using namespace	std;

namespace {
	class DotUtility {
	public:
		DotUtility(Scope*, ostream&);
		void generate_dot();
	private:
		void generate_initial_metadata();
		void generate_scope_data(Scope*, int rank);
		void generate_edges();
		void generate_ranks();

		Scope *root;
		vector< pair<int, int> > ranked_nodes;
		vector< pair<int, int> > edges;
		ostream &os;
	};

	DotUtility::DotUtility(Scope *root, ostream &os) : root(root), os(os) {}

	void DotUtility::generate_dot() {
		os<<"digraph \"Symbol table\" {"<<endl;
		generate_initial_metadata();
		generate_scope_data(root, 0);
		generate_edges();
		generate_ranks();
		os<<"}"<<endl;
	}

	void DotUtility::generate_initial_metadata() {
		os<<"fontname = \"Arial\"; fontsize = 8;"
			<<endl
			<<"node [ shape = \"record\"; ]"
			<<endl;
	}

	void DotUtility::generate_scope_data(Scope *scope, int rank) {
		string node_name = (scope->getOwnerSymbol() ? scope->getOwnerSymbol()->getName() : "None");
		os<<int(scope)
			<<"[ label = \"{"<<node_name<<"|}\"]"
			<<endl;
		ranked_nodes.push_back(make_pair(rank, int(scope)));
		auto it_scope = scope->getInnerScope();
		while (it_scope != nullptr) {
			edges.push_back(make_pair(int(scope), int(it_scope)));
			generate_scope_data(it_scope, rank + 1);
			it_scope = it_scope->getNextScope();
		}
	}

	void DotUtility::generate_edges() {
		for (auto e : edges) {
			os<<e.first<<"->"<<e.second<<";"<<endl;
		}
	}

	void DotUtility::generate_ranks() {
		sort(ranked_nodes.begin(), ranked_nodes.end());
		auto node = ranked_nodes.begin();
		while (node != ranked_nodes.end()) {
			int rank = node->first;
			os<<"{rank=same;";
			while (true) {
				os<<node->second;
				++node;
				if (node == ranked_nodes.end() || node->first > rank) break;
				os<<",";
			}
			os<<"}"<<endl;
		}
	}
}

void generate_dot(Scope *root, ostream &os) {
	DotUtility utility(root, os);
	utility.generate_dot();
}