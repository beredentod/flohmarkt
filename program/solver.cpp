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
			a -= START;
			b -= START;
			Rec *r = new Rec (c, a, b);
			rectangles.pb(r);
		}
	}
	else {
		cerr << "Error: File could not be opened. Abort.\n";
		exit(0);
	}
}

void Solver::distributeToStripes(){
	rectangles_stripes = vector<vector<Rec*>> (M);

	for (auto r: rectangles) {
		//Rec *rp = &r;
		for (int i = r->getBegin(); i < r->getEnd(); i++) 
			rectangles_stripes[i].pb(r);
	}
}

void Solver::knapsackStripe(int p){
	int curr_size = rectangles_stripes[p].size();
	vector<vector<int>> DP(curr_size+1, vector<int> (N+1));

	for (int i = 1; i <= curr_size; i++){
		Rec *r = rectangles_stripes[p][i-1];

		for (int j = N; j >= r->getSize(); j--)
			DP[i][j] = max(DP[i-1][j], DP[i-1][j - r->getSize()] + r->getSize());

		for (int j = r->getSize()-1; j >= 0; j--)
			DP[i][j] = DP[i-1][j];
	}

	int it1 = curr_size, it2 = N;
	while (it2 > 0 && it1 > 0){
		if (DP[it1][it2] == DP[it1-1][it2])
			it1--;
		else {
			bestFitRectangles[p].pb(rectangles_stripes[p][it1-1]);
			it2 -= rectangles_stripes[p][--it1]->getSize();
		}
	}

	sort(bestFitRectangles[p].begin(), bestFitRectangles[p].end(), greaterRec);

	cout << "Stripe " << p << ": " << DP[curr_size][N] << "\n";
}

void Solver::insertPlace(Rec* r, int p){
	auto it = upper_bound(placedRectangles[p].begin(), placedRectangles[p].end(), r, smallerx2);
	placedRectangles[p].insert(it, r);
}

void Solver::processStripe(int p){
	for(auto r: bestFitRectangles[p]){
		if (placedRectangles[p].empty())
			r->x1 = 0;
		else {
			auto it = placedRectangles[p].end();
			it--;
			int curr = (*it)->x2;
			r->x1 = curr;
		}
		r->x2 = r->x1 + r->getSize();
		for (int i = r->getBegin(); i < r->getEnd(); i++)
			insertPlace(r, i);
	}
}


// ------------------------- DEBUGGING -------------------------

void Solver::printAllRectangles(){
	cout << "\nDEBUG: All rectangles\n";
	for (auto r: rectangles)
		cout << "DEBUG: " << r->getBegin() << "\t" << r->getEnd() << "\t" << r->getSize()
			 << "\t" << r->x1 << "\t" << r->x2 << "\n"; 	
}

void Solver::printStripe(int p){
	cout << "DEBUG: Stripe " << p <<":\t";
	for (auto r: rectangles_stripes[p])
		cout << "(" << r->getBegin() << ", " << r->getEnd() << ", " << r->getSize() << ") ";
	cout << "\n";
}

void Solver::printAllStripes(){
	for (int i = 0; i < int(rectangles_stripes.size());i++)
		printStripe(i);
}

void Solver::printStripeBestFit(int p){
	cout << "DEBUG: Best fit for stripe: " << p << ":\t";
	for (auto r: bestFitRectangles[p])
		cout << "(" << r->getBegin() << ", " << r->getEnd() << ", " << r->getSize() << ") ";
	cout << "\n";
}

void Solver::printAllBestFit(){
	for (int i = 0; i < int(bestFitRectangles.size());i++)
		printStripeBestFit(i);
}