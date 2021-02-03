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

	void readFile(string path);

public:
	Solver(string path, int width = 10, int length = 1000, int start = 8):
		M(width), N(length), START(start) {
		readFile(path);
	}
	~Solver(void){}

};

#endif