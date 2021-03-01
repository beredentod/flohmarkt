#pragma once

#ifndef SOLVER_H
#define SOLVER_H

#include<bits/stdc++.h>
#include"util.h"

#define pb push_back

using namespace std;

class Solver{
	int M, N;
	int START;
	vector<Rec*> rectangles;

	vector<vector<Rec*>> rectangles_stripes;
	vector<vector<Rec*>> bestFitRectangles;

	vector<list<Rec*>> placedRectangles;

	void readFile(string path);

	void distributeToStripes();

	void insertPlace(Rec* r, int p);
	void knapsackStripe(int p);
	void processStripe(int p);

public:
	Solver(string path, int width = 10, int length = 1000, int start = 8):
		M(width), N(length), START(start) {
		readFile(path);
		distributeToStripes();

		bestFitRectangles = vector<vector<Rec*>> (M);
		placedRectangles = vector<list<Rec*>> (M);

		for (int i = 0; i < int(rectangles_stripes.size()); i++)
			knapsackStripe(i);

		processStripe(0);
	}
	~Solver(){
		for (auto r: rectangles)
				delete r;
	}

	void printAllRectangles();
	void printStripe(int p);
	void printAllStripes();
	void printStripeBestFit(int p);
	void printAllBestFit();
};

#endif