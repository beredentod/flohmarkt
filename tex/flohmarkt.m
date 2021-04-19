//der Lauf des Programms
void Solver::run(){
	//die Rechtecke werden auf die Streifen verteilt, 
	// durch die sie verlaufen
	distributeToStripes();

	//jeder Streifen wird verarbeitet, indem der erste
	//	Greedy-Algorithmus angewendet wird
	for (int i = 0; i < M; i++)
		processStripe(i);

	//Indikator dafuer, ob alle Rechtecke platziert wurden
	bool all = true;
	for (auto r: rectangles)
		if (r->x1 == -1)
			all = false;

	//der Gesamtflaecheninhalt der platzierten Rechtecke wird berechnet
	int area = calculateAreaUsed();
	if (all || getM()*getN() == area)
		return;

	//wenn nicht alle Rechtecke platziert wurden
	//	oder das grosse Rechteck nicht vollstaendig mit Rechtecken 
	//	bedeckt ist, laesst man das heuristische Verbesserungsverfahren laufen
	runOptimization();
}

//der Lauf des heuristischen Verbesserungsverfahrens
void Solver::runOptimization(){
	//der Gesamtflaecheninhalt aller platzierten Rechtecke
	int area;

	//der Indikator dafuer, ob eine Platzierung angenommen wurde
	bool result = true;

	//die Luecke
	Hole hole;

	//das Rechteck mit den Kooridnaten der Luecke, in die 
	//	es eingefuegt werden soll
	pair<Rec*, iPair> rep;

	//die Iteratoren zur Liste unusedRectangles und zur
	//	Liste all_holes
	int itR = 0, itH = 0;
	do {
		//falls die vorherige Platzierung vom Algorithmus
		//	angenommen wurde
		if (result){
			//der Gesamtflaecheninhalt aller platzierten Rechteck
			//	wird bestimmt
			area = calculateAreaUsed();

			//alle nicht gelegten Rechtecke werden bestimmt
			determineUnused();
			//alle Luecken werden bestimmt
			findHoles();

			//eine Luecke wird gewaehlt
			hole = findNextHole(itH = 0);
			//ein Rechteck wird anhand der Luecke gewaehlt
			rep = findReplacement(hole, itR = 0);

			//wenn es Probleme mit dem Rechteck oder mit
			//	der Luecke gibt
			while (rep.second.second < 0){
				//es gibt keine Luecken mehr
				if (rep.second.second == -1)
					return;

				//itR ist ans Ande der Liste unusedRectangles gelangt
				if (rep.second.second == -2){
					hole = findNextHole(++itH);
					rep = findReplacement(hole, itR = 0);
				}

				//das Rechteck kann nicht in diese Luecke 
				//	eingefuegt werden
				if (rep.second.second == -3)
					rep = findReplacement(hole, ++itR);
			}
			//das Rechteck wird in die gewaehlte Luecke platziert, 
			//	Kollisionen werden behoben, neue Rechtecke werden
			//	eingefuegt, eine Platzierung wird angenommen oder abgelehnt
			result = removeCollisions(area, rep);
		}
		else {
			//ein Rechteck wird anhand der Luecke gewaehlt
			rep = findReplacement(hole, ++itR);
			//wenn es Probleme mit dem Rechteck oder mit
			//	der Luecke gibt
			while (rep.second.second < 0){
				//es gibt keine Luecken mehr
				if (rep.second.second == -1)
					return;

				//itR ist ans Ande der Liste unusedRectangles gelangt
				if (rep.second.second == -2){
					hole = findNextHole(++itH);
					rep = findReplacement(hole, itR = 0);
				}

				//das Rechteck kann nicht in diese Luecke 
				//	eingefuegt werden
				if (rep.second.second == -3)
					rep = findReplacement(hole, ++itR);
			}
			//das Rechteck wird in die gewaehlte Luecke platziert, 
			//	Kollisionen werden behoben, neue Rechtecke werden
			//	eingefuegt, eine Platzierung wird angenommen oder abgelehnt
			result = removeCollisions(area, rep);
		}

	} while(rep.second.second > -1);

	return;	
}

//diese Methode verteilt jedes Rechteck auf die Streifen,
//	ueber die es verlaeuft
void Solver::distributeToStripes(){
	//die Listen mit den Listen S_j 
	rectangles_stripes = vector<vector<Rec*>> (M);

	//jedes Rechteck wird zu jeder Liste rectangles_stripes
	//	eingefuegt, zu der es gehoert
	for (auto r: rectangles) {
		for (int i = r->getBegin(); i < r->getEnd(); i++) 
			rectangles_stripes[i].pb(r);
	}

	//jede der Lsite wird sortiert
	for (int i = 0; i < M; i++)
		sort(rectangles_stripes[i].begin(), rectangles_stripes[i].end(), greaterEnd);
}

//diese Methode findet alle nicht platzierten Rechtecke in einem
//	Streifen p; p = -1 steht fuer alle Streifen
void Solver::determineUnused(int p){
	if (p == -1){
		for (int i = 0; i < M; i++)
			unusedRectangles[i].clear();
		for (int i = 0; i < M; i++)
			determineUnused(i);
			
		//der Zufallszahlengenerator
		mt19937 g(rd());

		//jede der Liste wird sortiert
		for (int i = 0; i < M; i++)
			sort(unusedRectangles[i].begin(), unusedRectangles[i].end(), smallerSize);
			//shuffle(unusedRectangles[i].begin(), unusedRectangles[i].end(), smallerSize);
	}
	else {
		//in jedem Streifen wird jedes Rechteck
		//	geprueft, ob es nicht platziert wurde
		for (auto r: rectangles_stripes[p])
			if (r->x1 == -1)
				unusedRectangles[p].pb(r);
	}
}

//diese Methode findet alle Luecken im Streifen p;
//	p = -1 steht fuer alle Streifen
void Solver::findHoles(int p){
	if (p == -1){ 
		//alle urspruenglichen Luecken werden entfernt
		for (int i = 0; i < M; i++)
			holes[i].clear();
		all_holes.clear();

		for (int i = 0; i < M; i++)
			findHoles(i);
			
		//der Zufallszahlengenerator
		mt19937 g(rd());

		//die Luecken werden sortiert
		sort(all_holes.begin(), all_holes.end(), greaterHolesSize);
		//shuffle(all_holes.begin(), all_holes.end(), g);
	}
	else {
		//es wird geprueft, ob es Luecken zwischen
		//zwei nebeneinander stehenden Rechtecken gibt

		//1. Rechteck
		auto it = placed[p].begin();
		for (; it != placed[p].end(); it++){
			//2. Rechteck
			auto it2 = it;
			it2++;

			//Gibt es eine Luecke zwischen der Seite des grossen Rechtecks
			//	und dem ersten Rechteck im Streifen?
			if (it == placed[p].begin() && (*it)->x1 > 0)
				holes[p].emplace_back(0, (*it)->x1, p);

			//Gibt es eine Luecke zwischen der Seite des grossen Rechtecks
			//	und dem letzten Rechteck im Streifen?
			if (it2 == placed[p].end()){
				if ((*it)->x2 < N)
					holes[p].emplace_back((*it)->x2, N, p);
			}
			//Gibt es eine Luecke zwischen den Rechtecken it und it2?
			else if ((*it)->x2 < (*it2)->x1)
				holes[p].emplace_back((*it)->x2, (*it2)->x1, p);
		}
		//alle Luecken werden in all_holes hinzugefuegt
		for (auto h: holes[p])
			all_holes.pb(h);
	}
}

//diese Methode findet die naechste leere Luecke im Streifen p
//	fuer ein Rechteck r
int Solver::findNearestHole(Rec* r, int p){
	auto it = placed[p].begin();

	//falls es in einem Streifen p noch keine Rechtecke gibt,
	//	kann r am Anfang platziert werden
	if (placed[p].empty())
		return 0;

	//falls es eine genug grosse Luecke am Anfang des Streifens gibt,
	//	kann r dort platziert werden
	if ((*it)->x1 >= r->getSize())
		return 0;

	//es wird ueber die Rechtecke im Streifen p iteriert
	for (; it != placed[p].end(); it++){
		auto it2 = it;
		it2++;

		//wenn it das letzte Rechteck im Streifen ist
		if (it2 == placed[p].end()) {
			//es wird gepruegt, ob es eine genug Luecke zur Wand 
			//	des grossen Rechtecks gibt
			if (N - (*it)->x2 >= r->getSize()) 
				return (*it)->x2;
			//falls es unmoeglich ist, das Rechteck zu platzieren,
			//	wird -1 ausgegeben
			else
				return -1;
		}

		//falls es eine genug grosse Luecke zwischen zwei Rechtecken
		//	im Streifen gibt
		if ((*it2)->x1 - (*it)->x2 >= r->getSize()){
			return (*it)->x2;
		}
	}

	//falls es unmoeglich ist, das Rechteck zu platzieren,
	//	wird -1 ausgegeben
	return -1;
}

//diese Methode verarbeitet den Streifen p beim Lauf
//	des Greedy-Algorithmus am Anfang
void Solver::processStripe(int p){
	//es wird durch die Rechtecke im Streifen p iteriert
	for(auto r: rectangles_stripes[p]){
		//falls das Rechteck bereits platziert wurde oder
		//	nicht mit dem Streifen beginnt,
		//	wird zum naechten Rechteck uebergegangen
		if (r->x1 > -1 || r->getBegin() < p)
			continue;

		//falls es noch keine Rechtecke im Streifen gibt,
		//	wird das Rechteck ans Anfang platziert
		if (placed[p].empty())
			r->x1 = 0;
		else {
			//es wird die naechste Luecke im Streifen p fuer
			//	dieses Rechteck gesucht
			int curr = findNearestHole(r, p);

			//falls so eine Luecke gefunden wurde, wird
			//	die x1 Koordinate des Rechtecks r gesetzt
			if (curr > -1)
				r->x1 = curr;
			//falls so eine Luecke nicht gefunden wurde,
			//	wird zum naechsten Rechteck uebergegangen
			else
				continue;
		}

		//die Koordinate x2 zum Rechteck r wird gesetzt
		r->x2 = r->x1 + r->getSize();

		//in alle Streifen, zu denen r gehoert, wird
		//	r an der richtigen Stelle eingefuegt
		for (int i = r->getBegin(); i < r->getEnd(); i++)
			placed[i].insert(r);
	}
}

//diese Methode verarbeitet den Streifen p beim Lauf
//	des Verbesserungsalgorithmus
vector<Rec*> Solver::processStripeReturn(int p){
	//ein vector mit allen neu eingefuegten Rechtecken
	vector<Rec*> added;

	//es wird durch die Rechtecke im Streifen p iteriert
	for(auto r: rectangles_stripes[p]){
		//falls das Rechteck bereits platziert wurde oder
		//	nicht mit dem Streifen beginnt,
		//	wird zum naechten Rechteck uebergegangen
		if (r->x1 > -1 || r->getBegin() < p)
			continue;

		//die naechste Luecke wird im Streifen p fuer
		//	dieses Rechteck gesucht
		int curr = findNearestHole(r, p);

		//das iterierte Rechteck wird kopiert
		Rec rr(r->getSize(), r->getBegin(), r->getEnd());
		rr.x1 = curr;
		rr.x2 = curr + rr.getSize();
		Rec *rr_p = &rr;

		//falls so eine Luecke gefunden wurde, wird geprueft, ob es genug
		//	Platz in anderen Streifen gibt
		if (curr > -1){
			for (int i = r->getBegin()+1; i < r->getEnd(); i++){
				//die potenzielle Stelle fuer das Rechteck rr wird im Streifen
				//	gefunden
				auto it = upper_bound(placed[i].begin(), placed[i].end(), rr_p, smallerx2);

				//die Koordinate x1 des Rechtecks unter dem Iterator it
				int curr_x1;
				//die Koordinate x2 des davor stehenden Rechtecks
				int prev_x2;

				//falls it sich am Ende der Liste befindet,
				//	wird curr_x1 zu N gesetzt
				if (it != placed[i].end())
					curr_x1 = (*it)->x1;
				else
					curr_x1 = N;

				//falls it sich am Anfang der Liste befindet,
				//	wird prev_x2 zu 0 gesetzt
				if (it != placed[i].begin())
					prev_x2 = (*(--it))->x2;
				else
					prev_x2 = 0;

				//es wird geprueft, ob das Rechteck platziert
				//werden kann
				if (curr_x1 - prev_x2 < rr.getSize() ||
					curr_x1 < curr + rr.getSize()
					|| prev_x2 > rr.x1) {
					goto next;
				}
			}
			//die Koordinate x1 wird dem Rechteck r gesetzt
			r->x1 = curr;
		}
		else {
			next:
			continue;
		}

		//das Rechteck wird in alle Streifen eingefuegt,
		//	zu denen es gehoert
		r->x2 = r->x1 + r->getSize();
		for (int i = r->getBegin(); i < r->getEnd(); i++)
			placed[i].insert(r);
		added.pb(r);
	}
	return added;
}

//diese Methode findet die naechste groesste Luecke im ganzen
//	grossen Rechtek; itH ist ein Iterator fuer die Liste all_holes
Hole Solver::findNextHole(int itH){
	for (; itH < int(all_holes.size()); itH++)
		//es wird geprueft, ob die Luecke nicht in einem Streifen
		//	liegt, in dem alle Rechtecke platziert sind
		if (!unusedRectangles[all_holes[itH].stripe].empty()){
			return all_holes[itH];
		}


	//falls es keine Luecken mehr gibt
	Hole h(-1, -1);
	return h;
}

//diese Methode findet ein noch nicht platziertes Rechteck 
// fuer einer Luecke hole; itR ist ein Iterator fuer unusedRectangles
pair<Rec*, iPair> Solver::findReplacement(Hole hole, int itR){
	//der Streifen, zu dem die Luecke gehoert
	int stripe = hole.stripe;

	Rec *rep;
	Rec a(-1,-1,-1);
	Rec *a_p = &a;

	//falls es keine Luecken mehr gibt
	if (stripe == -1)
		return {a_p, {-1, -1}};

	//falls es keine Rechtecke mehr gibt,
	//weil der Iterator da Ende der Liste erreicht
	if (unusedRectangles[stripe].empty() || 
		itR > int(unusedRectangles[stripe].size() -1)){
		return {a_p, {-2, -2}};		
	}

	//das Rechteck wird aus der Lsite unusedRectangles abgelesen
	rep = unusedRectangles[stripe][itR];

	//falls das Rechteck ueber die Grenzen des grossen Rechtecks
	//	hinausreichen wuerde
	if (rep->getSize() > hole.x2)
		return {a_p, {-3, -3}};

	//zurueckgegeben wird das Rechteck und die Koordinaten der Luecke
	return make_pair(rep, make_pair(hole.x1, hole.x2));
}

//die Methode versucht im Verbesserungsverfahren, neue Rechtecke
//	ins grosse Rechteck zu legen
vector<Rec*> Solver::addNew(Rec* rep){
	//ein vector mit allen neu eingefuegten Rechtecke
	vector<Rec*> added;

	//das Rechteck wird in alle Streifen eingefuegt 
	for (int i = rep->getBegin(); i < rep->getEnd(); i++)
		placed[i].insert(rep);
	added.pb(rep);

	//alle Streifen werden verarbeitet, um potenziell neue 
	//	Rechtecke einzufuegen
	for (int i = 0; i < M; i++){
		auto v = processStripeReturn(i);
		added.insert(added.end(), v.begin(), v.end());
	}

	return added;
}

//diese Methode entfernt alle Rechtecke, die mit einem im Verbesserungsverfahren
//	gelegten Rechteck kollidieren und aktualisiert die Platzierung,
//	falls es sich ein besserer Gesamtflaecheninhalt ergibt
bool Solver::removeCollisions(int area, pair<Rec*, iPair> rep){
	//alle Streifen mit allen platzierten Rechtecken werden kopiert
	auto placedOld = placed; 

	//alle kollidierenden Rechtecke 
	vector<pair<set<Rec*, setRecSort>::iterator, Rec>> collisions;

	//alle Rechtecke, die man entfernen muss
	vector<pair<int, set<Rec*, setRecSort>::iterator>> to_remove;
	auto [rr, hole] = rep;
	
	//die Koordinaten des einzufuegenden Recktecks werden
	//	festgelegt
	rr->x2 = hole.second;
	rr->x1 = rr->x2 - rr->getSize();

	Rec rep_c = *rr;
	rep_c.x2 = rep_c.x1;
	Rec *rr_p = &rep_c;

	//alle Kollisionen werden gesucht
	for (int i = rr->getBegin(); i < rr->getEnd(); i++){
		//die Stelle in jedem Streifn fuer die Platzierung wird gesucht
		auto it = upper_bound(placedOld[i].begin(), placedOld[i].end(), rr_p, smallerx2);

		//alle Kollisionen werden in collisions gespeichert
		for (; it != placedOld[i].end() && ((*it)->x1 <= rr->x2); it++){
			Rec &r_copy = *(*it);
			collisions.pb({it, r_copy});
		}
	}

	//die Koordinaten aller kollidierenden Rechtecke werden auf -1 gesetzt
	for (auto [it, r_copy]: collisions){
		(*it)->x1 = -1;
		(*it)->x2 = -1;
	}

	//alle Rechtecke mit Koordinaten -1 werden aus der Platzierung entfernt
	for (int i = 0; i < M; i++){
		for (auto it = placed[i].begin(); it != placed[i].end(); it++)
			if ((*it)->x1 == -1)
				to_remove.pb({i, it});
	}
	for (auto [stripe, it]: to_remove)
		placed[stripe].erase(it);
	
	//neue Rechtecke werden in die Platzierung eingefuegt
	vector<Rec*> added = addNew(rr);

	//der Gesamtflaecheninhalt aller neu platzierten Rechtecke
	//	wird berechnet
	int new_area = calculateAreaUsed();
	double diff = new_area - area;

	//die neue Platzierung wird angenommen
	if (diff > 0) {
		return true;
	}	
	//die neue Platzierung wird abgelehnt
	else {
		//alle neu eingefuegten Rechtecke werden entfernt
		for (auto r: added){
			r->x1 = -1;
			r->x2 = -1;
		}

		//die Koordinaten aller urspruenglich kollidierenden
		//	Rechtecke werden zurueckgesetzt
		for (auto [it, r_copy]: collisions){
			(*it)->x1 = r_copy.x1;
			(*it)->x2 = r_copy.x2;
		}

		//die Platzierung wird zurueckgesetzt
		placed = placedOld;
		return false;
	}
}


//diese Methode gibt den Gesamtflaecheninhalt aller Rechtecke,
//	die platziert wurden
int Solver::calculateAreaUsed(){
	int sum = 0;
	for (auto r: rectangles)
		if (r->x1 > -1)
			sum += r->getArea();
	return sum;
}


