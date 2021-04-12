#include"util.h"

//eine Funktion, die das Eingabeformat erkennt und entsprechend
//	zu Stunden oder Minuten umwandelt
int timeToMinutes(string time){
	//falls sich das Zeichen ":" in der Eingabe befindet
	if (time.find(":") != string::npos) {
		string s_min = time.substr(time.size() - 2);
		string s_hours = time.substr(0, time.size() - 3);

		int min = stoi(s_min);
		int hours = stoi(s_hours);

		//Stunden werden zu Minuten umgewandelt
		return hours * 60 + min;
	}
	else {
		//es werden einfache Stunden ausgegeben
		return stoi(time);
	}
}

//Die folgenden Funktionen sind Muster/Kriterien, wie man
//	die Rechtecken in den Streifen sortiert

//kleinere Werte x1 zuerst
bool smallerx1(Rec *a, Rec *b) {
	return (a->x1 < b->x1);
}

//kleinere Werte x2 zuerst
bool smallerx2(Rec *a, Rec *b) {
	return (a->x2 < b->x2);
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



