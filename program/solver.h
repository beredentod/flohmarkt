#pragma once

#ifndef SOLVER_H
#define SOLVER_H

#include<bits/stdc++.h>
#include"util.h"

#define pb push_back
typedef pair<int,int> iPair;

using namespace std;

class Solver{
	int M, N;
	int START;
	vector<Rec*> rectangles;

	vector<vector<Rec*>> rectangles_stripes;
	vector<vector<Rec*>> bestFitRectangles;

	vector<list<Rec*>> placedRectangles;

	vector<list<iPair>> holes;

	void readFile(string path);

	void distributeToStripes();

	void swapRecs(Rec* a, Rec* b);
	bool checkSwap(Rec* a, Rec* b);
	void insertPlace(Rec* r, int p);
	void knapsackStripe(int p);
	void processStripe(int p);

	int findNearestHole(Rec* r, int p);

	void findHolesStripe(int p);
	void findAllHoles();

	Rec* findReplacement(iPair h);

	//void DP();

public:
	Solver(string path, int width = 10, int length = 1000, int start = 8):
		M(width), N(length), START(start) {
		readFile(path);
		distributeToStripes();

		bestFitRectangles = vector<vector<Rec*>> (M);
		placedRectangles = vector<list<Rec*>> (M);
		holes = vector<list<iPair>> (M);

		//for (int i = 0; i < int(rectangles_stripes.size()); i++)
		//	knapsackStripe(i);

		for (int i = 0; i < 8; i++)
			processStripe(i);

		findAllHoles();

		for (int i = 0; i < M; i++){
			cout << "Holes Stripe " << i <<": ";
			for (auto h: holes[i])
				cout << "(" << h.first << ", " << h.second << ") ";
			cout << "\n";
		}
		cout << "\n";

		sort(rectangles.begin(), rectangles.end(), greaterEnd);
	}
	~Solver(){
		for (auto r: rectangles)
			delete r;
	}

	int calculateAreaUsed();
	int calculateTotalArea();

	int getM();
	int getN();
	int getStart();

	void printAllRectangles();
	void printRectangle(Rec *r);
	void printStripe(int p);
	void printAllStripes();
	void printStripeBestFit(int p);
	void printAllBestFit();
	void printPlacedStripe(int p);
	void printAllPlaced();
};

#endif