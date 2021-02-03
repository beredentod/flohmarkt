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

		for (auto r: rectangles)
			cout << r.b << "\t" << r.e << "\t" << r.size << "\n"; 
	}
	else {
		cerr << "Error: File could not be opened. Abort.\n";
		exit(0);
	}
}