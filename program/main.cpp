#include<iostream>
#include"solver.h"

int main () {
	int n;
	cout << "Nummer des Beispiels: ";
	cin >> n;
	Solver S("../beispiele/flohmarkt" + to_string(n) + ".txt");
	S.printAllStripes();
	S.printAllBestFit();
	S.printAllRectangles();
	S.printAllPlaced();

	double used = S.calculateAreaUsed();
	double total = S.calculateTotalArea();
	
	cout << "Area used: " << used << " [m * h], which is "<< used/(10*1000)*100 << "%\n";
	cout << "Total area of all rectangles: " << total << "\n";
}
