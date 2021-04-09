#include "solver.h"

void Solver::readFile(string path){
	ifstream file;
	file.open(path, ios::in); 
	if (file.is_open()) {
		file >> N;
		int a, b;
		file >> a >> b;
		M = b - a;
		START = a;
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

	printPlaced();

	//DEBUG
	cout << "DEBUG: Overlaps: " << boolalpha << checkIfOverlaps() << "\n\n";

	bool all = true;
	for (auto r: rectangles)
		if (r->x1 == -1)
			all = false;
	int area = calculateAreaUsed();
	int total = calculateTotalArea();
	if (all || total == area)
		return;
	runOptimization();
}

void Solver::runOptimization(){
	int area = calculateAreaUsed();
	bool result = true;
	Hole hole;
	pair<Rec*, iPair> rep;
	int itR = 0, itH = 0;
	int temp = 10000;
	do {

		//DEBUG
		//cout << "\t[New try!]\n\n";
		//printPlaced(950, 4);
		//printPlaced(950, 5);
		//printPlaced(950, 6);
		//printPlaced(950, 7);

		if (result){
			area = calculateAreaUsed();
			determineUnused();
			findHoles();

			//DEBUG
			//printHoles();

			hole = findNextLargestHole(itH = 0);
			rep = findReplacement(hole, itR = 0);
			while (rep.second.second < 0){
				if (rep.second.second == -1)
					return;
				if (rep.second.second == -2){
					hole = findNextLargestHole(++itH);
					rep = findReplacement(hole, itR = 0);
				}
				if (rep.second.second == -3)
					rep = findReplacement(hole, ++itR);
				temp--;
			}
			result = removeCollisions(area, rep, temp);
			temp--;

			//DEBUG
			bool overlap = checkIfOverlaps();
			cout << "\nDEBUG: Overlaps: " << boolalpha << overlap << "\n";
			if (overlap)
				return;
		}
		else {
			rep = findReplacement(hole, ++itR);
			while (rep.second.second < 0){
				if (rep.second.second == -1)
					return;
				if (rep.second.second == -2){
					hole = findNextLargestHole(++itH);
					rep = findReplacement(hole, itR = 0);
				}
				if (rep.second.second == -3)
					rep = findReplacement(hole, ++itR);
				temp--;
			}
			result = removeCollisions(area, rep, temp);
			temp--;

			//DEBUG
			bool overlap = checkIfOverlaps();
			cout << "\nDEBUG: Overlaps: " << boolalpha << overlap << "\n";
			if (overlap)
				return;
		}

	} while(rep.second.second > -1);
	
	return;	
}

void Solver::distributeToStripes(){
	rectangles_stripes = vector<vector<Rec*>> (M);

	for (auto r: rectangles) {
		for (int i = r->getBegin(); i < r->getEnd(); i++) 
			rectangles_stripes[i].pb(r);
	}

	for (int i = 0; i < M; i++)
		sort(rectangles_stripes[i].begin(), rectangles_stripes[i].end(), greaterEnd);
}

void Solver::determineUnused(int p){
	if (p == -1){
		for (int i = 0; i < M; i++)
			unusedRectangles[i].clear();
		for (int i = 0; i < M; i++)
			determineUnused(i);

		//unsigned seed = 785;
		//auto rng = default_random_engine {seed};
		for (int i = 0; i < M; i++)
			sort(unusedRectangles[i].begin(), unusedRectangles[i].end(), smallerSize);
			//shuffle(unusedRectangles[i].begin(), unusedRectangles[i].end(), rng);
	}
	else {
		for (auto r: rectangles_stripes[p])
			if (r->x1 == -1)
				unusedRectangles[p].pb(r);
	}
}

void Solver::findHoles(int p){
	if (p == -1){ 
		for (int i = 0; i < M; i++)
			holes[i].clear();
		all_holes.clear();

		for (int i = 0; i < M; i++)
			findHoles(i);
		sort(all_holes.begin(), all_holes.end(), greaterHolesSize);
	}
	else {
		list<Rec*>::iterator it = placedRectangles[p].begin();
		for (; it != placedRectangles[p].end(); it++){
			auto it2 = it;
			it2++;

			if (it == placedRectangles[p].begin() && (*it)->x1 > 0)
				holes[p].emplace_back(0, (*it)->x1, p);

			if (it2 == placedRectangles[p].end()){
				if ((*it)->x2 < N)
					holes[p].emplace_back((*it)->x2, N, p);
			}
			else if ((*it)->x2 < (*it2)->x1)
				holes[p].emplace_back((*it)->x2, (*it2)->x1, p);
		}
		for (auto h: holes[p])
			all_holes.pb(h);
	}
}

void Solver::insertPlace(Rec* r, int p){
	auto it = upper_bound(placedRectangles[p].begin(), placedRectangles[p].end(), r, smallerx2);
	placedRectangles[p].insert(it, r);
}

int Solver::findNearestHole(Rec* r, int p){
	list<Rec*>::iterator it = placedRectangles[p].begin();

	//think about something faster (?)

	if (placedRectangles[p].empty())
		return 0;

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
			else
				continue;
		}

		r->x2 = r->x1 + r->getSize();
		for (int i = r->getBegin(); i < r->getEnd(); i++)
			insertPlace(r, i);
	}
}

vector<Rec*> Solver::processStripeReturn(int p){
	vector<Rec*> added;
	for(auto r: rectangles_stripes[p]){
		if (r->x1 > -1 || r->getBegin() < p)
			continue;

		int curr = findNearestHole(r, p);
		Rec rr(r->getSize(), r->getBegin(), r->getEnd());
		rr.x1 = curr;
		rr.x2 = curr + rr.getSize();
		Rec *rr_p = &rr;
		if (curr > -1){
			//cout << "(S: " << rr.getSize() << ", A: " << rr.getArea() << ") -> "
			// << rr.x1 << ",  " << rr.x2 << "\n";
			for (int i = r->getBegin()+1; i < r->getEnd(); i++){
				auto it = upper_bound(placedRectangles[i].begin(),
					placedRectangles[i].end(), rr_p, smallerx2);
				int curr_x1, prev_x2;
				if (it != placedRectangles[i].end())
					curr_x1 = (*it)->x1;
				else
					curr_x1 = N;
				if (it != placedRectangles[i].begin())
					prev_x2 = (*(--it))->x2;
				else
					prev_x2 = 0;

				//cout << "s: " << i << "; x2: " << prev_x2 << ", x1: " << curr_x1 << "\n";
				if (curr_x1 - prev_x2 < rr.getSize() || curr_x1 < curr + rr.getSize()
					|| prev_x2 > rr.x1){
					//cout << " -- IMPOSSIBLE\n";
					goto next;
				}
			}
			//cout << " -- possible\n";
			r->x1 = curr;
		}
		else {
			next:
			continue;
		}

		r->x2 = r->x1 + r->getSize();
		for (int i = r->getBegin(); i < r->getEnd(); i++)
			insertPlace(r, i);
		added.pb(r);
	}
	return added;
}


Hole Solver::findNextLargestHole(int it){
	for (; it < int(all_holes.size()); it++)
		if (!unusedRectangles[all_holes[it].stripe].empty()){
			//cout << "Largest hole: (S: " << all_holes[it].stripe << ", " << all_holes[it].x1 << ", " << all_holes[it].x2 << ")\n";
			return all_holes[it];
		}

	Hole h(-1, -1);
	return h;
}


pair<Rec*, iPair> Solver::findReplacement(Hole hole, int it){
	int stripe = hole.stripe;

	if (stripe == -1) {
		cout << "=== No holes applicable. ===\n";
		Rec a(-1,-1,-1);
		Rec *a_p = &a;
		return {a_p, {-1, -1}};
	}

	Rec *rep;
	Rec a(-1,-1,-1);
	Rec *a_p = &a;

	if (unusedRectangles[stripe].empty()){
		cout << " => No rectangle for replacement.\n";
		return {a_p, {-2, -2}};		
	}

	/*for (auto r: unusedRectangles[stripe])
		cout << "(" << r->x1 << ", " << r->x2 << ", S: "
			 << r->getSize() << ", A: " << r->getArea() << ") ";*/

	//cout << "it: " << it << ",  size: " << unusedRectangles[stripe].size() << "\n";
	if (it > int(unusedRectangles[stripe].size() -1)){
		return {a_p, {-2, -2}};	
	}

	rep = unusedRectangles[stripe][it];

	if (rep->getSize() > hole.x2)
		return {a_p, {-3, -3}};

	return make_pair(rep, make_pair(hole.x1, hole.x2));
}

vector<Rec*> Solver::addNew(Rec* rep){
	vector<Rec*> added;
	for (int i = rep->getBegin(); i < rep->getEnd(); i++)
		insertPlace(rep, i);
	added.pb(rep);

	/*cout << "\n\n";
	printPlaced(950, 3);
	printPlaced(950, 4);
	printPlaced(950, 5);
	printPlaced(950, 6);
	printPlaced(950, 7);
	cout << "\n\n";*/

	for (int i = 0; i < M; i++){
		auto v = processStripeReturn(i);
		added.insert(added.end(), v.begin(), v.end());
	}

	return added;
}

bool Solver::removeCollisions(int area, pair<Rec*, iPair> rep, int temp){
	auto placedOld = placedRectangles; 
	vector<int> remove(M);
	vector<pair<list<Rec*>::iterator, Rec>> rec_remove;

	Rec *rep_rec = rep.first;
	iPair hole = rep.second;
	
	rep_rec->x2 = hole.second;
	rep_rec->x1 = rep_rec->x2 - rep_rec->getSize();

	/*cout << "Replacement: (B: " << rep_rec->getBegin() << ", E: " << rep_rec->getEnd() << ", S: "
			 << rep_rec->getSize() << ", A: " << rep_rec->getArea() << ")  ->  ("
			 << rep_rec->x1 << ", " << rep_rec->x2 << ")\n";*/

	Rec rep_c = *rep_rec;
	rep_c.x2 = rep_c.x1;
	Rec *rec_p = &rep_c;

	for (int i = rep_rec->getBegin(); i < rep_rec->getEnd(); i++){
		auto it = upper_bound(placedOld[i].begin(), placedOld[i].end(), rec_p, smaller);
		//cout << "Removing (" << i << "): ";
		for (; it != placedOld[i].end(); it++){
			if ((*it)->x1 > rep_rec->x2)
				break;
			//cout << "(" << (*it)->getSize() << " -> " << (*it)->getArea() << ") ";
			Rec &r_copy = *(*it);
			rec_remove.pb({it, r_copy});
			for (int j = (*it)->getBegin(); j < (*it)->getEnd(); j++)
				remove[j] = 1;
		}
		//cout << "\n";
	}

	for (auto r: rec_remove){
		//cout << "-1: " << (*r.first)->getSize() << "\n";
		(*r.first)->x1 = -1;
		(*r.first)->x2 = -1;
	}

	vector<pair<int, list<Rec*>::iterator>> to_remove;

	for (int i = 0; i < M; i++){
		if (remove[i]) {
			for (list<Rec*>::iterator it = placedRectangles[i].begin(); it != 
				placedRectangles[i].end(); it++)
				if ((*it)->x1 == -1)
					to_remove.pb({i, it});
		}
	}

	for (auto r: to_remove)
		placedRectangles[r.first].erase(r.second);

	//TODO: check this funtion
	/*for (int i = 0; i < M; i++){
		if (remove[i]) {
			list<Rec*>::iterator it2 = placedRectangles[i].begin(), it1 = placedRectangles[i].end();
			for (; it2 != placedRectangles[i].end(); it2++) {
				if ((*it2)->x1 > rep_rec->x2)
					break;
				if (it1 == placedRectangles[i].end() && (*it2)->x1 == -1)
					it1 = it2;
			}
			placedRectangles[i].erase(it1, it2);
		}
	}*/
	
	vector<Rec*> added = addNew(rep_rec);

	/*cout << "\n\n << New additional rectangles >>\n";
	for (auto r: added) {
		cout << "Added\t(B: " << r->getBegin() << ", E: " << r->getEnd() << ", S: "
		<< r->getSize() << ", A: " << r->getArea() << ", x: " << r->x1 << ", " << r->x2 << ")\n"; 
	}
	cout << "\n";*/

	//printPlaced();

	/*for (int i = 0; i < M; i++) {
		cout << "Placed old stripe: " << i << "  ";
		for (auto r: placedOld[i])
			cout << "(" << r->x1 << ", " << r->x2 << ", S: "
				 << r->getSize() << ", A: " << r->getArea() << ") ";
		cout << "\n";
	}*/

	cout << "Previous area: " << area << ", new area: " << calculateAreaUsed() << "\n";
	int new_area = calculateAreaUsed();
	double diff = new_area - area;
	uniform_real_distribution<double> dist(0.997899,1.0);
	if (temp <= 1)
		temp = 2;
	cout << "DEBUG: diff: " << diff << "\ttemp: " << temp << "\n";
	double ex = diff / (double)temp;
	cout << "DEBUG: ex: " << ex << "\n";
	cout << "DEBUG: exp: " <<(double)pow((double)EulerConstant, ex) << "\n";
	cout << "DEBUG: rand: " << dist(gen) << "\n";
	if (diff > 0 || pow(EulerConstant, ex) > dist(gen)) {
		cout << "\033[1;32mBETTER!\n\033[0m";
		placedOld.clear();
		vector<pair<int, list<Rec*>::iterator>> rem;
		for (int i = 0; i < M; i++)
			for (auto it = placedRectangles[i].begin(); it != placedRectangles[i].end(); it++)
				if ((*it)->x1 == -1 || (*it)->x2 == -1)
					rem.pb({i, it});

		for (auto x: rem)
			placedRectangles[x.first].erase(x.second);

		return true;
	}	
	else {
		cout << "\033[1;33mRevert!\n\033[0m";

		for (auto r: added){
			r->x1 = -1;
			r->x2 = -1;
		}

		for (auto r: rec_remove){
			(*r.first)->x1 = r.second.x1;
			(*r.first)->x2 = r.second.x2;
		}

		placedRectangles.clear();
		placedRectangles = placedOld;

		vector<pair<int, list<Rec*>::iterator>> rem;
		for (int i = 0; i < M; i++)
			for (auto it = placedRectangles[i].begin(); it != placedRectangles[i].end(); it++)
				if ((*it)->x1 == -1 || (*it)->x2 == -1)
					rem.pb({i, it});

		for (auto x: rem)
			placedRectangles[x.first].erase(x.second);

		return false;
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
	bool overlap = false;
	for (int i = 0; i < M; i++){
		list<Rec*>::iterator it = placedRectangles[i].begin();
		for (; it != placedRectangles[i].end(); it++){
			auto it2 = it;
			it2++;

			if (it == placedRectangles[i].begin() && (*it)->x1 < 0){
				cout << "\033[1;31m1OVERLAP\033[0m: Stripe " << i << ": (" << (*it)->x1 << ", " << 0 << ")\n";
				overlap = true;
				//return true;
			}

			if (it2 == placedRectangles[i].end()){
				if ((*it)->x2 > N){
					cout << "\033[1;31m2OVERLAP\033[0m: Stripe " << i << ": (" << (*it)->x2 << ", " << N << ")\n";
					overlap = true;
					//return true;
				}
			}
			else if ((*it)->x2 > (*it2)->x1){
				cout << "\033[1;31m3OVERLAP\033[0m: Stripe " << i << ": ("
				<< (*it)->x2 << ", " << (*it2)->x1 << ", S1: " << (*it)->getSize() << ", S2: " 
				<< ((*it2)->getSize()) << ")\n";
				cout << " --- > " << "(" << (*it)->x1 << ", " << (*it)->x2 << ", S: "
					 << (*it)->getSize() << ", A: " << (*it)->getArea() << ") ";
				cout << "(" << (*it2)->x1 << ", " << (*it2)->x2 << ", S: "
					 << (*it2)->getSize() << ", A: " << (*it2)->getArea() << ")\n";					
				overlap = true;
				//return true;
			}
		}		
	}
	return overlap;
	//return false;
}


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
		cout << "\033[1;32mStripe " << p <<":   \033[0m";
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
			cout << "(" << h.x1 << ", " << h.x2 <<", S: " << h.x2 - h.x1<< ") ";
		cout << "\n";	
	}
}

void Solver::printPlaced(int beg, int p){
	if (p == -1) {
		for (int i = 0; i < int(placedRectangles.size()); i++)
			printPlaced(beg, i);
		cout << "\n";		
	}
	else {
		//cout << "DEBUG: Placed stripe: " << p << "\t";
		cout << "\033[1;34mPlaced stripe: " << p << "  \033[0m";
		for (auto r: placedRectangles[p]){
			if (r->x1 >= beg || r->x1 == -1) {
				//if (r->x1 == -1 || r->x2 == -1){
				//	cout << "\033[1;31m(" << r->x1 << ", " << r->x2 << ", S: "
				//		 << r->getSize() << ", A: " << r->getArea() << ") \033[0m";			
				//}

				cout << "(" << r->x1 << ", " << r->x2 << ", S: "
					 << r->getSize() << ", A: " << r->getArea() << ") ";
			}
		}
		cout << "\n";
	}
}



