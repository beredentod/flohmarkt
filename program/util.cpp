#include"util.h"

bool greaterRec(Rec *a, Rec *b) {
	//if (a->getSize() == b->getSize())
	//	return a->getBegin() < b->getBegin();

	return (a->getSize() > b->getSize());
}

bool smallerx2(Rec *a, Rec *b) {
	//if (a->getSize() == b->getSize())
	//	return a->getBegin() < b->getBegin();

	return (a->x2 < b->x2);
}