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

bool smaller(Rec *a, Rec *b) {
	//if (a->getSize() == b->getSize())
	//	return a->getBegin() < b->getBegin();

	return (a->x2 < b->x1);
}


bool greaterEnd(Rec *a, Rec *b) {
	if(a->getEnd() == b->getEnd()){
		if (a->getBegin() == b->getBegin())
			return (a->getArea() > b->getArea());

		return (a->getBegin() < b->getBegin());
	}

	return (a->getEnd() > b->getEnd());
}

bool smallerSize(Rec *a, Rec *b) {
	if (a->getSize() == b->getSize())
		return a->getBegin() < b->getBegin();

	return a->getSize() < b->getSize();
}

bool smallerArea(Rec *a, Rec *b) {
	if (a->getArea() == b->getArea())
		return a->getBegin() < b->getBegin();

	return a->getArea() < b->getArea();
}

bool greaterArea(Rec *a, Rec *b) {
	if (a->getArea() == b->getArea())
		return a->getBegin() < b->getBegin();

	return a->getArea() > b->getArea();
}

bool sortHolesSize(const pair<pair<int, int>, int> &a, const pair<pair<int, int>, int> &b){
	if (a.first.second - a.first.first == b.first.second - b.first.first)
		return a.second < b.second;

	return (a.first.second - a.first.first > b.first.second - b.first.first);
}