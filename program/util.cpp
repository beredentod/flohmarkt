#include"util.h"

//eine Funktion, die das Eingabeformat erkennt und entsprechend
//	zu Stunden oder Minuten umwandelt
pair<int, int> timeToMinutes(string time){
	//falls sich das Zeichen ":" in der Eingabe befindet
	if (time.find(":") != string::npos) {
		string s_min = time.substr(time.size() - 2);
		string s_hours = time.substr(0, time.size() - 3);

		int min = stoi(s_min);
		int hours = stoi(s_hours);

		//Stunden werden zu Minuten umgewandelt
		return {hours * 60 + min, 1};
	}
	else {
		//es werden einfache Stunden ausgegeben
		return {stoi(time), 0};
	}
}

//eine Funktion, die aus der Reihenfolgen von OEffnungs-
//	und Schliessungszeiten die Dauer der Pause des
//	Flohmarkts ausrechnet
int calculateRecess(vector<int> &v){
	int recess = 0;

	for (int i = 1; i < v.size() - 1; i+=2)
		recess += (v[i+1] - v[i]);

	return recess;
}

//eine Funkttion, die einen String mit der
//	der Reihenfolge von OEffnungs- und Schliessungszeiten
//	in einzelne Uhrzeiten aufsplittet
pair<vector<int>, int> processInput(string line){
	vector<string> data;

	//der vector mit den verarbeiteten Zeiten
	vector<int> result;

	//der String line wird auf kleine Strings aufgeteilt
	size_t pos = 0;
	string token;
	while ((pos = line.find(" ")) != std::string::npos) {
	    token = line.substr(0, pos);
	    data.pb(token);
	    line.erase(0, pos + string(" ").size());
	}
	data.pb(line);

	//das Eingabeformat 
	//	0 -- Stunden
	//	1 -- Minuten
	int mode = -1;

	for (auto x: data){
		auto res = timeToMinutes(x);
		mode = res.second;
		result.pb(res.first);
	}

	//der vector mit den Uhrzeiten und das Eingabeformat
	return {result, mode};
}

//Die folgenden Funktionen sind Muster/Kriterien, wie man
//	die Rechtecken in den Streifen sortiert

//dies wird fuer die Funktion upper_bound verwendet
bool smallerx2(Rec* a, Rec *b){
	return a->x2 < b->x2;
}

//1) groesseres Ende, 2) kleinerer Beginn, groesserer Flaecheninhalt
bool greaterEnd(Rec *a, Rec *b) {
	if(a->getEnd() == b->getEnd()){
		if (a->getBegin() == b->getBegin())
			return (a->getArea() > b->getArea());

		return (a->getBegin() < b->getBegin());
	}

	return (a->getEnd() > b->getEnd());
}

//1) kleinere Laenge, 2) kleinerer Beginn
bool smallerSize(Rec *a, Rec *b) {
	if (a->getSize() == b->getSize())
		return a->getBegin() < b->getBegin();

	return a->getSize() < b->getSize();
}

//1) groessere Laenge, 2) kleinerer Beginn
bool greaterSize(Rec *a, Rec *b) {
	if (a->getSize() == b->getSize())
		return a->getBegin() < b->getBegin();

	return a->getSize() > b->getSize();
}

//1) kleinerer Flaecheninhalt, 2) kleinerer Beginn
bool smallerArea(Rec *a, Rec *b) {
	if (a->getArea() == b->getArea())
		return a->getBegin() < b->getBegin();

	return a->getArea() < b->getArea();
}

//1) groesserer Flaecheninhalt, 2) kleinerer Beginn
bool greaterArea(Rec *a, Rec *b) {
	if (a->getArea() == b->getArea())
		return a->getBegin() < b->getBegin();

	return a->getArea() > b->getArea();
}

//Die folgenden Funktionen sind Muster/Kriterien, wie man
//	die Luecken in den Streifen sortiert

//1) groessere Laenge der Luecke, 2) niedrigerer Streifen
bool greaterHolesSize(const Hole &a, const Hole &b){
	if (a.x2 - a.x1 == b.x2 - b.x1)
		return a.stripe < b.stripe;

	return (a.x2 - a.x1 > b.x2 - b.x1);
}

//1) kleinere Laenge der Luecke, 2) niedrigerer Streifen
bool smallerHolesSize(const Hole &a, const Hole &b){
	if (a.x2 - a.x1 == b.x2 - b.x1)
		return a.stripe < b.stripe;

	return (a.x2 - a.x1 < b.x2 - b.x1);
}



