#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

const int fact[3] = { 120, 80, 80 }, whouses[3] = { 150, 70, 60 };
const int cost[3][3] = { {8, 5, 6}, {15, 10, 12}, {3, 9, 10} };

int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloModel model(env);
		IloArray<IloNumVarArray> arr(env, 3);
		for (IloInt i = 0; i < 3; i++)
			arr[i] = IloNumVarArray(env, 3);
		for (IloInt i = 0; i < 3; i++)
			for (IloInt j = 0; j < 3; j++)
				arr[i][j] = IloNumVar(env, 0.0, IloInfinity, ILOINT);
		for (IloInt i = 0; i < 3; i++)
			model.add(IloSum(arr[i]) <= fact[i]);
		for (IloInt i = 0; i < 3; i++) {
			IloExpr saida(env);
			for (IloInt j = 0; j < 3; j++)
				saida += arr[j][i];
			model.add(saida == whouses[i]);
		}
		IloExpr sum(env);
		for (IloInt i = 0; i < 3; i++)
			for (IloInt j = 0; j < 3; j++)
				sum += arr[i][j] * cost[i][j];
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