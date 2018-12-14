#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloModel model(env);
		IloNumVarArray x(env, 2);
		for (IloInt i = 0; i < 2; i++)
			x[i] = IloNumVar(env, 0.0, IloInfinity, ILOINT);
		model.add(x[0] + 4 * x[1] <= 10000);
		model.add(5 * x[0] + 2 * x[1] <= 30000);
		model.add(IloMaximize(env, 11 * x[0] + 12 * x[1]));
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