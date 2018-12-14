#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloModel model(env);
		IloNumArray w(env), v(env);
		IloInt capacity;
		ifstream file("in.txt");
		file >> capacity >> v >> w;
		IloInt n = v.getSize();
		IloBoolVarArray x(env, n);
		IloExpr capSum(env), valSum(env);;
		for (IloInt i = 0; i < n; i++) {
			capSum += w[i] * x[i];
			valSum += v[i] * x[i];
		}
		model.add(capSum <= capacity);
		model.add(IloMaximize(env, valSum));
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