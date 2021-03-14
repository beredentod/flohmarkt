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

void Solver::run(){
	distributeToStripes();
	for (int i = 0; i < M; i++)
		processStripe(i);
	int area = calculateAreaUsed();
	int total = calculateTotalArea();
	printAllRectangles();
	if (int(rectangles.size()) < 30) {
		printPlaced();
	}

	cout << "DEBUG: Overlaps: " << boolalpha << checkIfOverlaps() << "\n\n";

	bool all = true;
	for (auto r: rectangles) if (r->x1 == -1) all = false;
	if (all || total == area) return;
	
	//if (int(rectangles.size()) < 30) {
		printStripes(-1, true);
	//}

	findHoles();
	printHoles();

	Rec* rep = findReplacement();

	printPlaced();

	removeCollisions(area, rep);

}

void Solver::distributeToStripes(){
	rectangles_stripes = vector<vector<Rec*>> (M);

	for (auto r: rectangles) {
		for (int i = r->getBegin(); i < r->getEnd(); i++) 
			rectangles_stripes[i].pb(r);
	}

	for (int i = 0; i < M; i++)
		sort(rectangles_stripes[i].begin(), rectangles_stripes[i].end(), greaterEnd);

	rec_stripes_size = rectangles_stripes;
	for (int i = 0; i < M; i++)
		sort(rec_stripes_size[i].begin(), rec_stripes_size[i].end(), smallerSize);
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
			if (N - (*it)->x2 >= r->getSize()) 
				return (*it)->x2;
			else
				return -1;
		}

		if ((*it2)->x1 - (*it)->x2 >= r->getSize()){
			return (*it)->x2;
		}
	}

	return -1;	
}

vector<Rec*> Solver::processStripeReturn(int p){
	vector<Rec*> added;
	for(auto r: rectangles_stripes[p]){
		if (r->x1 > -1 || r->getBegin() < p)
			continue;

		if (placedRectangles[p].empty())
			r->x1 = 0;
		else {
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
		added.pb(r);
	}
	return added;
}

void Solver::processStripe(int p){
	for(auto r: rectangles_stripes[p]){
		if (r->x1 > -1 || r->getBegin() < p)
			continue;

		if (placedRectangles[p].empty())
			r->x1 = 0;
		else {
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
}

void Solver::findHoles(int p){
	if (p == -1) 
		for (int i = 0; i < M; i++)
			findHoles(i);
	else {
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
				holes[p].pb({(*it)->x2, (*it2)->x1});
		}
		for (auto h: holes[p])
			all_holes.pb(make_pair(h, p));
	}
}

pair<iPair, int> Solver::findLargestHole(){
	sort(all_holes.begin(), all_holes.end(), sortHolesSize);
	vector<int> unused(M);

	for (auto r: rectangles)
		if (r->x1 == -1)
			for (int i = r->getBegin(); i < r->getEnd(); i++)
				unused[i]++;

	for (int i = 0; i < int(all_holes.size()); i++)
		if (unused[all_holes[i].second])
			return all_holes[i];

	return {{-1, -1},-1};
}


Rec* Solver::findReplacement(){
	pair<iPair, int> hole = findLargestHole();
	int stripe = hole.second;
	cout << "Largest hole: (" << hole.first.first << ", " << hole.first.second << ")\n";

	Rec *rep = *(rec_stripes_size[stripe].begin());
	rep->x2 = hole.first.second;
	rep->x1 = rep->x2 - rep->getSize();

	cout << "Replacement: (B:" << rep->getBegin() << ", E:" << rep->getEnd() << ", S: "
			 << rep->getSize() << ", A: " << rep->getArea() << ")  ->  ("
			 << rep->x1 << ", " << rep->x2 << ")\n";

	return rep;
}

vector<Rec*> Solver::addNew(Rec* rep){
	vector<Rec*> added;
	for (int i = rep->getBegin(); i < rep->getEnd(); i++)
		insertPlace(rep, i);
	added.pb(rep);

	for (int i = rep->getBegin(); i < rep->getEnd(); i++){
		auto v = processStripeReturn(i);
		added.insert(added.end(), v.begin(), v.end());
	}
	return added;
}

void Solver::removeCollisions(int area, Rec* rep){
	auto placedOld = placedRectangles; 
	vector<int> remove(M);
	vector<pair<list<Rec*>::iterator, Rec>> rec_remove;

	for (int i = rep->getBegin(); i < rep->getEnd(); i++){
		auto it = lower_bound(placedOld[i].begin(), placedOld[i].end(), rep, smaller);
		cout << "Removing: ";
		for (; it != placedOld[i].end(); it++){
			if ((*it)->x1 > rep->x2)
				break;
			cout << "(" << (*it)->getSize() << ") ";
			Rec &r_copy = *(*it);
			rec_remove.pb({it, r_copy});
			for (int j = (*it)->getBegin(); j < (*it)->getEnd(); j++)
				remove[j] = 1;
		}
		cout << "\n";
	}

	for (auto r: rec_remove){
		(*r.first)->x1 = -1;
		(*r.first)->x2 = -1;
	}

	//TODO: check this funtion
	for (int i = 0; i < M; i++){
		if (remove[i]) {
			list<Rec*>::iterator it2 = placedRectangles[i].begin(), it1 = placedRectangles[i].end();
			for (; it2 != placedRectangles[i].end(); it2++) {
				if ((*it2)->x1 > rep->x2)
					break;
				if (it1 == placedRectangles[i].end() && (*it2)->x1 == -1)
					it1 = it2;
			}
			placedRectangles[i].erase(it1, it2);
		}
	}

	printPlaced();

	for (int i = 0; i < M; i++) {
		cout << "Placed old stripe: " << i << "  ";
		for (auto r: placedOld[i])
			cout << "(" << r->x1 << ", " << r->x2 << ", S: "
				 << r->getSize() << ", A: " << r->getArea() << ") ";
		cout << "\n";
	}

	cout << "\nPrevious area: " << area << ", new area: " << calculateAreaUsed() << "\n";
	if (area > calculateAreaUsed()) {
		for (auto r: rec_remove){
			(*r.first)->x1 = r.second.x1;
			(*r.first)->x2 = r.second.x2;
		}
		placedRectangles = placedOld;

		for (auto r: addNew(rep)){
			r->x1 = -1;
			r->x2 = -1;
		}
	}	

	/*for (int i = 0; i < M; i++) {
		cout << "Placed old stripe: " << i << "  ";
		for (auto r: placedOld[i])
			cout << "(" << r->x1 << ", " << r->x2 << ", S: "
				 << r->getSize() << ", A: " << r->getArea() << ") ";
		cout << "\n";
	}

	printPlaced();*/
}


bool Solver::checkIfOverlaps(){
	for (int i = 0; i < M; i++){
		list<Rec*>::iterator it = placedRectangles[i].begin();
		for (; it != placedRectangles[i].end(); it++){
			auto it2 = it;
			it2++;

			if (it == placedRectangles[i].begin() && (*it)->x1 < 0)
				return true;

			if (it2 == placedRectangles[i].end()){
				if ((*it)->x2 > N)
					return true;
			}
			else if ((*it)->x2 > (*it2)->x1)
				return true;
		}		
	}
	return false;
}


/*void Solver::swapRecs(Rec* a, Rec* b){
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
}*/



// ------------------- SEMI-DEBUGGING ----------------------------------

int Solver::calculateAreaUsed(){
	int sum = 0;
	for (auto r: rectangles) if (r->x1 > -1) sum += r->getArea();
	return sum;
}

int Solver::calculateTotalArea(){
	int sum = 0;
	for (auto r: rectangles) sum += r->getArea();
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

//modes:
//1 - plain txt
//2 - TeX tabular
void Solver::saveResult(string path, bool all, int mode){
	ofstream file;
	if (mode == 1)
		path += ".txt";
	else if (mode == 2)
		path += ".tex";
	file.open(path, ios::out);
	if (mode == 1) {
		if (all)
			file << "All rectangles\n";
		else 
			file << "Placed rectangles only\n";
		file << "begin\tend\tlength\tx1\tx2\n";
	}
	for (auto r: rectangles) {
		if (mode == 1)
			file << r->getBegin() + START << "\t" << r->getEnd() + START << "\t" 
			  << r->getSize() << "\t" << r->x1 << "\t" << r->x2 << "\n";
		else if (mode == 2)
			file << r->getBegin() + START << "\t&\t" << r->getEnd() + START << "\t&\t" 
			  << r->getSize() << "\t&\t" << r->x1 << "\t&\t" << r->x2 << "\\\\ \n"; 
	}
	file.close();
}

// ------------------------- DEBUGGING -------------------------

void Solver::printAllRectangles(){
	//cout << "\nDEBUG: All rectangles\n";
	//cout << "DEBUG: B\tE\tS\tA\tx1\tx2\n";
	cout << "\nAll rectangles\n";
	cout << "B\tE\tS\tA\tx1\tx2\n";
	for (auto r: rectangles)
	//	cout << "DEBUG: " << r->getBegin() << "\t" << r->getEnd() << "\t" << r->getSize()
	//		 << "\t" << r->getArea() << "\t" << r->x1 << "\t" << r->x2 << "\n";
		cout << r->getBegin() << "\t" << r->getEnd() << "\t" << r->getSize()
			 << "\t" << r->getArea() << "\t" << r->x1 << "\t" << r->x2 << "\n";
	cout << "\n"; 	
}

void Solver::printStripes(int p, bool unused){
	if (p == -1) {
		for (int i = 0; i < int(rectangles_stripes.size());i++)
			printStripes(i, unused);
		cout << "\n";
	}
	else {
		//cout << "DEBUG: Stripe " << p <<":\t";
		cout << "Stripe " << p <<":   ";
		for (auto r: rectangles_stripes[p]){
			if ((unused && r->x1 == -1) || !unused)
				cout << "(" << r->getBegin() << ", " << r->getEnd() << ", "
					 << r->getSize() << ", " << r->getArea() << ") ";
		}
		cout << "\n";
	}
}

void Solver::printHoles(int p){
	if (p == -1){
		for (int i = 0; i < M; i++) 
			printHoles(i);
		cout << "\n";
	}
	else {
		//cout << "DEBUG: Holes Stripe " << p <<": ";
		cout << "Holes Stripe " << p <<":   ";
		for (auto h: holes[p])
			cout << "(" << h.first << ", " << h.second <<", S: " << h.second - h.first<< ") ";
		cout << "\n";	
	}
}

void Solver::printPlaced(int p){
	if (p == -1) {
		for (int i = 0; i < int(placedRectangles.size()); i++)
			printPlaced(i);
		cout << "\n";		
	}
	else {
	//cout << "DEBUG: Placed stripe: " << p << "\t";
	cout << "Placed stripe: " << p << "  ";
	for (auto r: placedRectangles[p])
		cout << "(" << r->x1 << ", " << r->x2 << ", S: "
			 << r->getSize() << ", A: " << r->getArea() << ") ";
	cout << "\n";
	}
}



