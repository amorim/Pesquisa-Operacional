#include <ilcplex/ilocplex.h>
#include <vector>
#include <set>

ILOSTLBEGIN

const int F[4] = { 1, 2, 3, 4 };

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
		Graph *g = new Graph(4);
		g->addEdge(0, 1);
		g->addEdge(0, 2);
		g->addEdge(1, 2);
		g->addEdge(1, 3);
		g->addEdge(2, 3);
		IloArray<IloBoolVarArray> x(env, 4);
		IloBoolVarArray y(env, 4);
		for (IloInt i = 0; i < 4; i++)
			x[i] = IloBoolVarArray(env, 4);
		for (IloInt i = 0; i < 4; i++)
			model.add(IloSum(x[i]) == 1);
		for (IloInt i = 0; i < 4; i++) {
			for (IloInt j = 0; j < 4; j++) {
				set<int> neigh = g->getNeighboringVertices(j);
				for (auto n : neigh)
					model.add(x[j][i] + x[n][i] <= 1);
				model.add(x[j][i] <= y[i]);
			}
		}
		model.add(IloMinimize(env, IloSum(y)));
		IloCplex cplex(model);
		cplex.solve();
		env.out() << "Solution status = " << cplex.getStatus() << endl;
		env.out() << "Solution value = " << cplex.getObjValue() << endl;
	}
	catch (IloException &ex) {
		cerr << "Concert exception caught: " << ex << endl;
	}
	return 0;
}