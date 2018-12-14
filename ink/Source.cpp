#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

const float cost[4] = { 1.5, 1.0, 4.0, 6.0 };

int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloModel model(env);
		IloArray<IloNumVarArray> comps(env, 4);
		for (IloInt i = 0; i < 4; i++) {
			comps[i] = IloNumVarArray(env, 2);
			for (IloInt j = 0; j < 2; j++)
				comps[i][j] = IloNumVar(env, 0, IloInfinity, ILOINT);
		}
		IloExpr prod1(env), prod2(env);
		for (IloInt i = 0; i < 4; i++) {
			prod1 += comps[i][0];
			prod2 += comps[i][1];
		}
		model.add(prod1 == 1000);
		model.add(prod2 == 250);
		model.add((comps[2][0] + comps[0][0] * 0.3 + comps[1][0] * 0.6) / 1000 >= 0.25);
		model.add((comps[2][1] + comps[0][1] * 0.3 + comps[1][1] * 0.6) / 250 >= 0.2);
		model.add((comps[3][0] + comps[0][0] * 0.7 + comps[1][0] * 0.4) / 1000 >= 0.5);
		model.add((comps[3][1] + comps[0][1] * 0.7 + comps[1][1] * 0.4) / 250 >= 0.5);
		IloExpr sum(env);
		for (IloInt i = 0; i < 4; i++)
			sum += cost[i] * (comps[i][0] + comps[i][1]);
		model.add(IloMinimize(env, sum));
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