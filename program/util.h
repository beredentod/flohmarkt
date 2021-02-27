#pragma once

#ifndef UTIL_H
#define UTIL_H

#include<bits/stdc++.h>
#include"util.h"


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

	int x1, x2;
};

bool sortRecSize(Rec *a, Rec *b);

#endif