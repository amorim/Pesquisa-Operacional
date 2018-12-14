#include <ilcplex/ilocplex.h>
#include <vector>
#include <set>

ILOSTLBEGIN

const int schoolBuildCost[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

class Graph {
private:
	vector<set<int>> adjacencia;
	int nvertices;

public:
	Graph(int nvertices) {
		this->nvertices = nvertices;
		adjacencia.resize(nvertices);
	}
	void addEdge(int from, int to) {
		adjacencia[from].insert(to);
		adjacencia[to].insert(from);
	}
	set<int> getNeighboringVertices(int vertex) {
		return adjacencia[vertex];
	}
};


int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloModel model(env);
		Graph *g = new Graph(9);
		g->addEdge(0, 1);
		g->addEdge(1, 2);
		g->addEdge(2, 3);
		g->addEdge(1, 4);
		g->addEdge(4, 7);
		g->addEdge(4, 6);
		g->addEdge(6, 8);
		g->addEdge(8, 5);
		IloBoolVarArray x(env, 9);
		for (IloInt i = 0; i < 9; i++) {
			IloExpr exp(env);
			set<int> neigh = g->getNeighboringVertices(i);
			for (auto n : neigh)
				exp += x[n];
			exp += x[i];
			model.add(exp >= 1);
		}
		IloExpr sum(env);
		for (IloInt i = 0; i < 9; i++)
			sum += schoolBuildCost[i] * x[i];
		model.add(IloMinimize(env, sum));
		IloCplex cplex(model);
		cplex.solve();
		env.out() << "Solution status = " << cplex.getStatus() << endl;
		env.out() << "Solution value = " << cplex.getObjValue() << endl;
	}
	catch (IloException &ex) {
		cerr << "Concert exception caught: " << ex << endl;
	}
	env.end();
	return 0;
}