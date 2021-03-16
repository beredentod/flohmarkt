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
};

class Hole{
public:
	int x1 = -1, x2 = -1, stripe = -1, it = -1;
	Hole(){}
	Hole(int a, int b){
		x1 = a;
		x2 = b;
	}
	Hole(int a, int b, int c){
		x1 = a;
		x2 = b;
		stripe = c;
	}

};

bool smaller(Rec *a, Rec *b);
bool smallerx2(Rec *a, Rec *b);
bool greaterRec(Rec *a, Rec *b);
bool greaterEnd(Rec *a, Rec *b);
bool smallerSize(Rec *a, Rec *b);
bool smallerArea(Rec *a, Rec *b);
bool greaterArea(Rec *a, Rec *b);
bool sortHolesSize(const Hole &a, const Hole &b);


#endif