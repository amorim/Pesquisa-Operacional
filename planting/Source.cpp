#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

const float a[3] = { 5.5, 4, 3.5 };
const int ac[3] = { 400, 650, 350 };

int main(int argc, char **argv) {
	IloEnv env;
	try {
		IloModel model(env);
		IloArray<IloNumVarArray> q(env, 3);
		for (IloInt i = 0; i < 3; i++) {
			q[i] = IloNumVarArray(env, 3);
			for (IloInt j = 0; j < 3; j++)
				q[i][j] = IloNumVar(env, 0, IloInfinity, ILOFLOAT);
		}
		IloExpr exp1(env), exp2(env), exp3(env), exp4(env), exp5(env), exp6(env), exp7(env), exp8(env), exp9(env);
		for (IloInt i = 0; i < 3; i++) {
			exp1 += q[i][0];
			exp2 += q[i][1];
			exp3 += q[i][2];
			exp4 += q[0][i] * a[i];
			exp5 += q[1][i] * a[i];
			exp6 += q[2][i] * a[i];
			exp7 += q[0][i];
			exp8 += q[1][i];
			exp9 += q[2][i];
		}
		model.add(exp1 <= 660);
		model.add(exp2 <= 880);
		model.add(exp3 <= 400);
		model.add(exp4 <= 1800);
		model.add(exp5 <= 2200);
		model.add(exp6 <= 950);
		model.add(exp7 <= 400);
		model.add(exp8 <= 650);
		model.add(exp9 <= 350);
		model.add(exp7 / ac[0] == exp8 / ac[1]);
		model.add(exp8 / ac[1] == exp9 / ac[2]);
		model.add(IloMaximize(env, exp1 * 5000 + exp2 * 4000 + exp3 * 1800));
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