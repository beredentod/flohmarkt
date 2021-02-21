#pragma once

#ifndef SOLVER_H
#define SOLVER_H

#include<bits/stdc++.h>

#define pb push_back

using namespace std;

struct rec {
	int size, b, e;
};

class Solver{
	int N = 1000, M = 10;
	int START = 8;
	vector<rec> rectangles;

	vector<vector<rec>> rectangles_stripes;
	vector<vector<rec>> bestFitRectangles;

	void readFile(string path);

	void distributeToStripes();

public:
	Solver(string path, int width = 10, int length = 1000, int start = 8):
		M(width), N(length), START(start) {
		readFile(path);
		distributeToStripes();
		bestFitRectangles = vector<vector<rec>> (M);
		for (int i = 0; i < rectangles_stripes.size(); i++)
			knapsackStripe(i);
	}
	~Solver(void){}

	void knapsackStripe(int p);

	void printAllRectangles();
	void printStripe(int p);
	void printAllStripes();
	void printStripeBestFit(int p);
	void printAllBestFit();
};

#endif