#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloArray<IloNumArray> cost(env);
		IloNumArray fcost(env);
		ifstream file("in.txt");
		if (!file) {
			cerr << "Não foi possível ler o arquivo de entrada." << endl;
			throw(-1);
		}
		file >> fcost >> cost;
		IloInt n = fcost.getSize(), m = cost[0].getSize();
		IloModel model(env);
		IloBoolVarArray y(env, n);
		IloArray<IloBoolVarArray> x(env, n);
		for (IloInt i = 0; i < n; i++) {
			y[i] = IloBoolVar(env);
			x[i] = IloBoolVarArray(env, m);
		}
		for (IloInt j = 0; j < m; j++) {
			IloExpr exp(env);
			for (IloInt i = 0; i < n; i++)
				exp += x[i][j];
			model.add(exp == 1);
		}
		for (IloInt i = 0; i < n; i++)
			model.add(IloSum(x[i]) <= y[i] * m);
		IloExpr exp1(env), exp2(env);
		for (IloInt j = 0; j < m; j++)
			for (IloInt i = 0; i < n; i++)
				exp1 += cost[i][j] * x[i][j];
		for (IloInt i = 0; i < n; i++)
			exp2 += fcost[i] * y[i];
		model.add(IloMinimize(env, exp1 + exp2));
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