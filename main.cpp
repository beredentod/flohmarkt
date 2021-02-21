#include<iostream>
#include"solver.h"

int main () {
	int n;
	cout << "Nummer des Beispiels: ";
	cin >> n;
	Solver S("../beispiele/flohmarkt" + to_string(n) + ".txt");
	S.printAllStripes();
	S.printAllBestFit();
}
