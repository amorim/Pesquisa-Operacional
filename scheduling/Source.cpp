#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

const int d[7] = { 1, 2, 3, 4, 5, 6, 7 };

IloInt wrapAround(IloInt i) {
	if (i >= 0)
		return i;
	return i + 7;
}

int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloModel model(env);
		IloNumVarArray x(env, 7, 0, IloInfinity, ILOINT);
		for (IloInt i = 0; i < 7; i++) {
			IloExpr sum(env);
			for (IloInt j = i; j > i - 5; j--) {
				IloInt valid = wrapAround(j);
				sum += x[valid];
			}
			model.add(sum >= d[i]);
		}
		model.add(IloMinimize(env, IloSum(x)));
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