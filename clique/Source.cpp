#include <ilcplex/ilocplex.h>
#include <vector>
#include <set>
ILOSTLBEGIN

class Graph {
private:
	vector<set<int>> adjacencia;

public:
	int nvertices;

	Graph(int nvertices) {
		this->nvertices = nvertices;
		adjacencia.resize(nvertices);
	}
	Graph(string filename) {
		ifstream file(filename);
		int vertices;
		file >> vertices;
		this->nvertices = vertices;
		adjacencia.resize(vertices);
		int a, b;
		while (file >> a >> b) {
			this->addEdge(a, b);
		}
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
		Graph* g = new Graph("in.txt");
		IloInt v = g->nvertices;
		IloBoolVarArray x(env, v);
		for (IloInt i = 0; i < v; i++) {
			for (IloInt j = 0; j < v; j++) {
				set<int> neigh = g->getNeighboringVertices(i);
				if (!neigh.count(j) && i != j)
					model.add(x[i] + x[j] <= 1);
			}
		}
		model.add(IloMaximize(env, IloSum(x)));
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