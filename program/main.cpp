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
	
	cout << "\nArea used: " << used << " [m * h], which is "
	<< used/(S.getM()*S.getN())*100 << "% of the total area and which is "
	<< used/total*100 << "% of all rectangles.\n";
	cout << "Total area of all rectangles: " << total << "\n";
	cout << "Total area of the greater rectangle: " << S.getM()*S.getN() << "\n";

	//das Ergebnis wird unter dieser Adresse gespeichert
	S.saveResult("../results/flohmarkt" + to_string(n) + "_result", true, 3);
}
