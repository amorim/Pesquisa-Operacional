#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

const int body[4] = { 1, 2, 0, 4 };
const int lid[4] = { 7, 3, 9, 4 };
const int t[4] = { 2, 3, 2, 1 };

int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloModel model(env);
		IloNumVarArray x(env, (IloInt)4, 0, IloInfinity, ILOINT);
		IloNumVar y(env, 0, IloInfinity, ILOINT);
		IloExpr timeSum(env), lidSum(env), bodySum(env);
		for (IloInt i = 0; i < 4; i++) {
			timeSum += x[i] * t[i];
			lidSum += x[i] * lid[i];
			bodySum += x[i] * body[i];
		}
		model.add(timeSum <= 400);
		model.add(x[0] + x[2] + x[3] <= 200);
		model.add(x[1] <= 90);
		model.add(y <= lidSum / 2);
		model.add(y <= bodySum);
		model.add(IloMaximize(env, 50 * y - 5 * (bodySum - y) - 3 * (lidSum - 2 * y)));
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