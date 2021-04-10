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
	vector<vector<Rec*>> unusedRectangles;

	vector<list<Rec*>> placedRectangles;
	vector<list<Hole>> holes;

	vector<Hole> all_holes;

	void readFile(string path);
	void distributeToStripes();

	int findNearestHole(Rec* r, int p);
	void insertPlace(Rec* r, int p);
	void processStripe(int p);
	vector<Rec*> processStripeReturn(int p);
	void determineUnused(int p = -1);
	void findHoles(int p = -1);

	Hole findNextLargestHole(int it);
	pair<Rec*, iPair> findReplacement(Hole hole, int it);
	vector<Rec*> addNew(Rec* rep);
	bool removeCollisions(int area, pair<Rec*, iPair> rep);

	void run();
	void runOptimization();

//DEBUGGING
	bool checkIfOverlaps();

public:
	Solver(string path){
		readFile(path);
		placedRectangles = vector<list<Rec*>> (M);
		unusedRectangles = vector<vector<Rec*>> (M);
		holes = vector<list<Hole>> (M);

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

//DEBUGGING
	void printAllRectangles();
	void printStripes(int p, bool unused = false);
	void printHoles(int p = -1);
	void printPlaced(int from = 0, int p = -1);
};

#endif