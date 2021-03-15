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
	vector<vector<Rec*>> rec_stripes_area;

	vector<list<Rec*>> placedRectangles;
	vector<list<iPair>> holes;

	vector<pair<iPair, int>> all_holes;

	void readFile(string path);
	void distributeToStripes();

	int findNearestHole(Rec* r, int p);
	void insertPlace(Rec* r, int p);
	void processStripe(int p);
	vector<Rec*> processStripeReturn(int p);

	void findHoles(int p = -1);
	pair<iPair, int> findLargestHole();
	pair<Rec*, iPair> findReplacement(int epoch);
	vector<Rec*> addNew(Rec* rep);
	void removeCollisions(int area, pair<Rec*, iPair> rep);

	bool checkIfOverlaps();

	void run();

public:
	Solver(string path, int width = 10, int length = 1000, int start = 8):
		M(width), N(length), START(start) {

		placedRectangles = vector<list<Rec*>> (M);
		holes = vector<list<iPair>> (M);

		readFile(path);
		run();
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

	void saveResult(string path, bool all = true, int mode = 1);

	void printAllRectangles();
	void printStripes(int p, bool unused = false);
	void printHoles(int p = -1);
	void printPlaced(int p = -1);
};

#endif