#pragma once

#ifndef UTIL_H
#define UTIL_H

#include<bits/stdc++.h>
#include"util.h"

using namespace std;

class Rec {
private:
	int size, b, e;

public:
	Rec(int s, int beg, int end): size(s), b(beg), e(end) {
		x1 = -1;
		x2 = -1;
	}
	~Rec(void){}

	int getSize(){
		return size;
	}

	int getBegin(){
		return b;
	}

	int getEnd(){
		return e;
	}

	int getArea(){
		return (e-b)*size;
	}

	int x1, x2;
	bool status = 1;
};

bool greaterRec(Rec *a, Rec *b);
bool greaterEnd(Rec *a, Rec *b);
bool smallerx2(Rec *a, Rec *b);

struct hole{
	int x1, x2, b, e;
};

#endif