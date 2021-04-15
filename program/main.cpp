#include<iostream>
#include"solver.h"

int main () {
	int n;
	cout << "Nummer des Beispiels: ";
	cin >> n;
	//die Eingabedatei
	Solver S("../beispiele/flohmarkt" + to_string(n) + ".txt");

	//der Gesamtflächeninhalt der platzierten Rechtecke
	double used = S.calculateAreaUsed();
	//der Gesamtflächeninhalt aller platzierten Rechtecke
	double total = S.calculateTotalArea();

	int format = S.getFormat();
	string unit = "[m * h]";
	if (format)
		unit = "[m * min]";

	int area = S.getM()*S.getN() - S.getRecess()*S.getN();
	
	cout << "\nArea used: " << used << " " << unit << ", which is "
	<< (used/area)*100 << "% of the total area and which is "
	<< used/total*100 << "% of all rectangles.\n";
	cout << "Total area of all rectangles: " << total << "\n";
	cout << "Total area of the greater rectangle: " << area << "\n";

	//das Ergebnis wird unter dieser Adresse gespeichert
	S.saveResult("../results/flohmarkt" + to_string(n) + "_result", true, 3);
}
