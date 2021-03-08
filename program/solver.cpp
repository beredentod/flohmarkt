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

	for (int i = 0; i < M; i++)
		sort(rectangles_stripes[i].begin(), rectangles_stripes[i].end(), greaterEnd);
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

	sort(bestFitRectangles[p].begin(), bestFitRectangles[p].end(), greaterEnd);

	cout << "Stripe " << p << ": " << DP[curr_size][N] << "\n";
}

void Solver::swapRecs(Rec* a, Rec* b){
	if (!checkSwap(a, b)){
		cout << "DEBUG: swap not possible for: ";
		printRectangle(a);
		cout << ", ";
		printRectangle(b);
		cout << "\n";
		return;
	}

	int val1 = a->x1;
	a->x1 = b->x1;
	a->x2 = a->x1 + a->getSize();
	b->x1 = val1;
	b->x2 = b->x1 + b->getSize();
}

bool Solver::checkSwap(Rec* a, Rec* b){
	int val1 = a->x1, val2 = b->x1;
	if (val1 + b->getSize() > N || val2 + a->getSize() > N)
		return false;
	return true;
}

void Solver::insertPlace(Rec* r, int p){
	auto it = upper_bound(placedRectangles[p].begin(), placedRectangles[p].end(), r, smallerx2);
	placedRectangles[p].insert(it, r);
}

int Solver::findNearestHole(Rec* r, int p){
//think about something faster (?)
	list<Rec*>::iterator it = placedRectangles[p].begin();

	if ((*it)->x1 >= r->getSize())
		return 0;

	for (; it != placedRectangles[p].end(); it++){
		list<Rec*>::iterator it2 = it;
		it2++;
		if (it2 == placedRectangles[p].end()) {
			//cout << r->getSize() << " -- " << (*it)->x2 << "\n";
			if (N - (*it)->x2 >= r->getSize()) 
				return (*it)->x2;
			else
				return -1;
		}

		if ((*it2)->x1 - (*it)->x2 >= r->getSize()){
			//cout << r->getSize() << "-- " << (*it)->x2 << "\n";
			return (*it)->x2;
		}
	}

	r->status = 0;
	return -1;	
}

void Solver::processStripe(int p){
	for(auto r: rectangles_stripes[p]){
		if (r->x1 > -1 || r->getBegin() < p)
			continue;

		if (placedRectangles[p].empty())
			r->x1 = 0;
		else {
			//auto it = placedRectangles[p].end();
			//it--;
			int curr = findNearestHole(r, p);
			if (curr > -1)
				r->x1 = curr;
			else {
				continue;
			}
		}
		r->x2 = r->x1 + r->getSize();
		for (int i = r->getBegin(); i < r->getEnd(); i++)
			insertPlace(r, i);
	}

	//auto it = placedRectangles[p].begin();
	//auto it2 = it;
	//it2++;

	//swapRecs(*it2, *it);
}

void Solver::findHolesStripe(int p){
	list<Rec*>::iterator it = placedRectangles[p].begin();
	for (; it != placedRectangles[p].end(); it++){
		auto it2 = it;
		it2++;

		if (it == placedRectangles[p].begin() && (*it)->x1 > 0)
			holes[p].pb({0, (*it)->x1});
		if (it2 == placedRectangles[p].end()){
			if ((*it)->x2 < N)
				holes[p].pb({(*it)->x2, N});
		}
		else if ((*it)->x2 < (*it2)->x1)
			holes[p].pb({(*it)->x1, (*it)->x2});
	}
}

void Solver::findAllHoles(){
	for (int i = 0; i < M; i++)
		findHolesStripe(i);
}


/*void Solver::DP(){
	int num = rectangles.size();
	vector<vector<vector<int>>> DP(num + 1, vector<vector<int>> (N+1, vector<int> (M+1, 0)));

	for (int i = 1; i <= int(rectangles.size()); i++){
		rec *curr = rectangles[i-1];
		int b = curr->getBegin();
		int e = curr->getEnd();
		int a = curr->getArea();
		int s = curr->getSize();

		for (int j = N; j >= s; j--) {
			DP[i]

		}
	}
}*/


int Solver::calculateAreaUsed(){
	int sum = 0;
	for (auto r: rectangles)
		if (r->x1 > -1)
			sum += r->getArea();

	return sum;
}

int Solver::calculateTotalArea(){
	int sum = 0;
	for (auto r: rectangles)
			sum += r->getArea();

	return sum;
}

int Solver::getM(){
	return M;
}
int Solver::getN(){
	return N;
}
int Solver::getStart(){
	return START;
}

// ------------------------- DEBUGGING -------------------------

void Solver::printAllRectangles(){
	cout << "\nDEBUG: All rectangles\n";
	cout << "DEBUG: B\tE\tS\tA\tx1\tx2\n";
	for (auto r: rectangles)
		cout << "DEBUG: " << r->getBegin() << "\t" << r->getEnd() << "\t" << r->getSize()
			 << "\t" << r->getArea() << "\t" << r->x1 << "\t" << r->x2 << "\n";
	cout << "\n"; 	
}

void Solver::printRectangle(Rec *r){
	cout << "(" << r->getBegin() << ", " << r->getEnd() << ", " << r->getSize() << ") ";
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
		cout << "(" << r->getBegin() << ", " << r->getEnd() << ", S: "
			 << r->getSize() << ", A: " << r->getArea() << ") ";
	cout << "\n";
}

void Solver::printAllBestFit(){
	for (int i = 0; i < int(bestFitRectangles.size());i++)
		printStripeBestFit(i);
}

void Solver::printPlacedStripe(int p){
	cout << "DEBUG: Placed stripe: " << p << "\t";
	for (auto r: placedRectangles[p])
		cout << "(" << r->x1 << ", " << r->x2 << ", S: "
			 << r->getSize() << ", A: " << r->getArea() << ") ";
	cout << "\n";
}

void Solver::printAllPlaced(){
	for (int i = 0; i < int(placedRectangles.size()); i++)
		printPlacedStripe(i);
}