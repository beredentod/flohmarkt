#include "solver.h"

void Solver::readFile(string path){
	ifstream file;
	file.open(path, ios::in);
	if (file.is_open()) {
		int n;
		file >> n;

		for (int i = 0; i < n; i++) {
			int a, b, c;
			file >> a >> b >> c;
			rec r;
			a -= START;
			b -= START;
			r.size = c;
			r.b = a;
			r.e = b;
			rectangles.pb(r);
		}
	}
	else {
		cerr << "Error: File could not be opened. Abort.\n";
		exit(0);
	}
}

void Solver::distributeToStripes(){
	rectangles_stripes = vector<vector<rec>> (M);

	for (auto r: rectangles) {
		for (int i = r.b; i < r.e; i++) 
			rectangles_stripes[i].pb(r);
	}
}

void Solver::knapsackStripe(int p){
	int curr_size = rectangles_stripes[p].size();
	vector<vector<int>> DP(curr_size+1, vector<int> (N+1));

	for (int i = 1; i <= curr_size; i++){
		rec r = rectangles_stripes[p][i-1];
		for (int j = N; j >= r.size; j--)
			DP[i][j] = max(DP[i-1][j], DP[i-1][j - r.size] + r.size);
		for (int j = r.size-1; j >= 0; j--)
			DP[i][j] = DP[i-1][j];
	}

	int it1 = curr_size, it2 = N;
	while (it2 > 0 && it1 > 0){
		if (DP[it1][it2] == DP[it1-1][it2])
			it1--;
		else {
			bestFitRectangles[p].pb(rectangles_stripes[p][it1-1]);
			it2 -= rectangles_stripes[p][--it1].size;
		}
	}

	cout << "Stripe " << p << ": " << DP[curr_size][N] << "\n";
}

void Solver::printAllRectangles(){
	cout << "DEBUG: All rectangles\n";
	for (auto r: rectangles)
		cout << "DEBUG: " << r.b << "\t" << r.e << "\t" << r.size << "\n"; 	
}

void Solver::printStripe(int p){
	cout << "DEBUG: Stripe " << p <<":\t";
	for (auto r: rectangles_stripes[p])
		cout << "(" << r.b << ", " << r.e << ", " << r.size << ") ";
	cout << "\n";
}

void Solver::printAllStripes(){
	for (int i = 0; i < rectangles_stripes.size();i++)
		printStripe(i);
}

void Solver::printStripeBestFit(int p){
	cout << "DEBUG: Best fit for stripe: " << p << ":\t";
	for (auto r: bestFitRectangles[p])
		cout << "(" << r.b << ", " << r.e << ", " << r.size << ") ";
	cout << "\n";
}

void Solver::printAllBestFit(){
	for (int i = 0; i < bestFitRectangles.size();i++)
		printStripeBestFit(i);
}