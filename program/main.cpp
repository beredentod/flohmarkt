#include<iostream>
#include"solver.h"

int main () {
	int n;
	cout << "Nummer des Beispiels: ";
	cin >> n;
	Solver S("../beispiele/flohmarkt" + to_string(n) + ".txt");
	S.printAllStripes();
	S.printAllRectangles();
	S.printAllPlaced();

	double used = S.calculateAreaUsed();
	double total = S.calculateTotalArea();
	
	cout << "\nArea used: " << used << " [m * h], which is "
	<< used/(S.getM()*S.getN())*100 << "% of the total area and which is "
	<< used/total*100 << "% of all rectanlges.\n";
	cout << "Total area of all rectangles: " << total << "\n";
}
