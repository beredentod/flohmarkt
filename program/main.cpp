#include<iostream>
#include"solver.h"

int main () {
	int n;
	cout << "Nummer des Beispiels: ";
	cin >> n;
	//die Eingabedatei
	//Solver S("../beispiele/flohmarkt" + to_string(n) + ".txt");
	//double used = S.calculateAreaUsed();

	vector<int> results;

	for (int i = 0; i < 1000; i++){
		Solver S("../beispiele/flohmarkt" + to_string(n) + ".txt");
		int used = S.calculateAreaUsed();
		results.push_back(used);	
		cout << used << "\n";
	}

	ofstream file;
	file.open("../other/random_smH" + to_string(n) + ".csv");

	for (auto x: results)
		file << x << "\n";
	file.close();

	/*
	//der Gesamtflächeninhalt der platzierten Rechtecke
	double used = S.calculateAreaUsed();
	//der Gesamtflächeninhalt aller Rechtecke
	double total = S.calculateTotalArea();

	//das Eingabeformat
	int format = S.getFormat();
	string unit = "[m * h]";
	if (format)
		unit = "[m * min]";

	//der Flächeninhalt des großen Rechtecks
	int area = S.getM()*S.getN() - S.getRecess()*S.getN();
	
	cout << "\nDer Gesamtflächeninhalt der platzierten Rechtecke: " << used << " " << unit <<
	",\nwas " << (used/area)*100 << "% des Flächeninhalts des umschließenden Rechtecks\nund "
	<< used/total*100 << "% des Gesamtflächeninhalts aller Rechtecke entspricht.\n";
	cout << "Der Gesamtflächeninhalt aller Rechtecke: " << total << "\n";
	cout << "Des Flächeninhalt des umschließenden Rechtecks: " << area << "\n\n";

	//das Ergebnis wird unter dieser Adresse gespeichert
	S.saveResult("../results/flohmarkt" + to_string(n) + "_result", true, 3);*/
}
