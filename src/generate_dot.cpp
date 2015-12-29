#include "generate_dot.hpp"
#include "SymbolTable/SymbolTable.h"
#include "definitions.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace	std;

namespace {
	struct SymbolsCategories {
		vector<Variable*> variables;
		vector<Function*> functions;
		vector<Class*> classes;
		vector<DataMember*> data_members;
		vector<Method*> methods;
		vector<Parameter*> parameters;
	};

	SymbolsCategories classify_symbols(vector<Symbol*> symbols_list) {
		SymbolsCategories res;
		for (auto symbol : symbols_list) {
			if (auto p = dynamic_cast<Class*>(symbol)) res.classes.push_back(p);
			else if (auto p = dynamic_cast<Method*>(symbol)) res.methods.push_back(p);
			else if (auto p = dynamic_cast<Function*>(symbol)) res.functions.push_back(p);
			else if (auto p = dynamic_cast<DataMember*>(symbol)) res.data_members.push_back(p);
			else if (auto p = dynamic_cast<Parameter*>(symbol)) res.parameters.push_back(p);
			else if (auto p = dynamic_cast<Variable*>(symbol)) res.variables.push_back(p);

		}
		return res;
	}

	class DotUtility {
	public:
		DotUtility(Scope*, ostream&);
		void generate_dot();
	private:
		void generate_initial_metadata();
		void generate_scope_data(Scope*, int rank);
		void process_node(Scope*);
		void process_symbols(vector<Symbol*>);
		void process_functions(vector<Function*>);
		void process_methods(vector<Method*>);
		void process_variables(vector<Variable*>);
		void process_data_members(vector<DataMember*>);
		void process_parameters(vector<Parameter*>);
		void process_classes(vector<Class*>);
		void print_parameters(vector<Parameter*>);
		void generate_edges();
		void generate_ranks();
		char visibility_representation(int);

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
		os<<"fontname = \"Helvetica\"; fontsize = 11;"
			<<endl
			<<"node [ shape = \"record\"; fontname = \"Helvetica\"; fontsize = 11; ]"
			<<endl;
	}

	void DotUtility::generate_scope_data(Scope *scope, int rank) {
		process_node(scope);
		ranked_nodes.push_back(make_pair(rank, int(scope)));
		auto it_scope = scope->getInnerScope();
		while (it_scope != nullptr) {
			edges.push_back(make_pair(int(scope), int(it_scope)));
			generate_scope_data(it_scope, rank + 1);
			it_scope = it_scope->getNextScope();
		}
	}

	void DotUtility::process_node(Scope* scope) {
		auto symbols_list = scope->getSymbolTable()->symbols();
		string node_name = (scope->getOwnerSymbol() ? scope->getOwnerSymbol()->getName() : "None");
		os<<int(scope)
			<<"[ label = \"{"<<node_name;
		process_symbols(symbols_list);
		os<<"}\"]"
			<<endl;
	}

	void DotUtility::process_symbols(vector<Symbol*> symbols_list) {
		auto categories = classify_symbols(symbols_list);
		process_functions(categories.functions);
		process_methods(categories.methods);
		process_variables(categories.variables);
		process_data_members(categories.data_members);
		process_parameters(categories.parameters);
		process_classes(categories.classes);
	}

	void DotUtility::process_functions(vector<Function*> functions) {
		if (functions.empty()) return;
		os<<"|";
		for (auto f : functions) {
			os<<f->getName();
			print_parameters(f->parameters());
			os<<" : "<<f->getReturnType()<<"\\l";
		}
	}

	void DotUtility::process_methods(vector<Method*> methods) {
		if (methods.empty()) return;
		os<<"|";
		for (auto m: methods) {
			os<<visibility_representation(m->getAccessModifier())<<" "<<m->getName();
			print_parameters(m->parameters());
			os<<" : "<<m->getReturnType()<<"\\l";
		}

	}

	void DotUtility::process_variables(vector<Variable*> variables) {
		if (variables.empty()) return;
		os<<"|";
		for (auto var : variables)
			os<<var->getName()<<" : "<<var->getVariableType()<<"\\l";
	}

	void DotUtility::process_data_members(vector<DataMember*> data_members) {
		if (data_members.empty()) return;
		os<<"|";
		for (auto d : data_members)
			os<<visibility_representation(d->getAccessModifier())<<" "<<d->getName()<<" : "<<d->getVariableType()<<"\\l";
	}

	void DotUtility::process_parameters(vector<Parameter*> parameters) {
		// Do nothing. Parametrs were printed in the function/method header
	}

	void DotUtility::process_classes(vector<Class*> classes) {
		if (classes.empty()) return;
		os<<"|";
		for (auto c : classes)
			os<<"Class: "<<c->getName()<<"\\l";
	}

	void DotUtility::print_parameters(vector<Parameter*> parameters) {
		if (parameters.empty()) {
			os<<"()";
			return;
		}
		os<<"(";
		auto pit = parameters.begin();
		while (true) {
			auto p = *pit;
			os<<p->getName()<<": "<<p->getVariableType();
			++pit;
			if (pit == parameters.end()) break;
			os<<", ";
		}
		os<<")";
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

	char DotUtility::visibility_representation(int visibility) {
		char res = '-';
		if (visibility == PUBLIC_ACCESS) res = '+';
		else if (visibility == PROTECTED_ACCESS) res = '#';
		return res;
	}
}

void generate_dot(Scope *root, ostream &os) {
	DotUtility utility(root, os);
	utility.generate_dot();
}