#include <ilcplex/ilocplex.h>
#include <vector>
#include <set>
ILOSTLBEGIN

struct Edge {
	int v, capacity;
};

class Graph {
private:
	vector<vector<Edge*>> adjacencia;

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
		int a, b, c;
		while (file >> a >> b >> c) {
			this->addEdge(a, b, c);
		}
	}
	void addEdge(int from, int to, int cap) {
		Edge* e = new Edge();
		e->capacity = cap;
		e->v = to;
		adjacencia[from].push_back(e);
	}
	vector<Edge*> getNeighboringVertices(int vertex) {
		return adjacencia[vertex];
	}
};

int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloModel model(env);
		Graph* g = new Graph("in.txt");
		IloInt n = g->nvertices;
		IloArray<IloNumVarArray> a(env, n);
		for (IloInt i = 0; i < n; i++)
			a[i] = IloNumVarArray(env, n, 0, IloInfinity, ILOINT);
		for (IloInt i = 0; i < n; i++) {
			vector<Edge*> neigh = g->getNeighboringVertices(i);
			for (auto n : neigh)
				model.add(a[i][n->v] <= n->capacity);
		}
		for (IloInt i = 1; i < n - 1; i++) {
			IloExpr inflow(env), outflow(env);
			for (IloInt j = 0; j < n; j++) {
				if (i != j) {
					vector<Edge*> neigh = g->getNeighboringVertices(j);
					for (auto n : neigh)
						if (n->v == i)
							inflow += a[j][i];
				}
			}
			vector<Edge*> neigh = g->getNeighboringVertices(i);
			for (auto n : neigh)
				outflow += a[i][n->v];
			model.add(inflow == outflow);
		}
		IloExpr exp(env);
		for (IloInt i = 0; i < n; i++) {
			vector<Edge*> neigh = g->getNeighboringVertices(i);
			for (auto ne : neigh)
				if (ne->v == n - 1)
					exp += a[i][n - 1];
		}
		model.add(IloMaximize(env, exp));
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