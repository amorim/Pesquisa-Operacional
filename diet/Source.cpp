#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloModel model(env);
		IloNumVarArray x(env, 6, 0, IloInfinity, ILOINT);
		model.add(x[0] + 2 * x[2] + 2 * x[3] + x[4] + 2 * x[5] >= 9);
		model.add(x[1] + 3 * x[2] + x[3] + 3 * x[4] + 2 * x[5] >= 19);
		model.add(IloMinimize(env, 35 * x[0] + 30 * x[1] + 60 * x[2] + 50 * x[3] + 27 * x[4] + 22 * x[5]));
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