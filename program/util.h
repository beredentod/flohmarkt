#pragma once

#ifndef UTIL_H
#define UTIL_H

#include<bits/stdc++.h>
#include"util.h"

using namespace std;

//die Klasse mit einem Rechteck (einer Anmeldung)
class Rec {
private:
	//die Laenge eines Rechtecks, der Beginn, das Ende
	int size, b, e;

public:
	Rec(int s, int beg, int end): size(s), b(beg), e(end) {
		//die x-Koordinaten werden dem Rechteck noch nicht zugeordnet
		//das Rechteck liegt ausserhalb des grossen Rechtecks
		x1 = -1;
		x2 = -1;
	}
	~Rec(void){}

	//Zugriff auf die Laenge des Rechtecks
	int getSize(){
		return size;
	}

	//Zugriff auf den Beginn der Anmeldung (y-Koordinate)
	int getBegin(){
		return b;
	}

	//Zugriff auf das Ende der Anmeldung (y-Koordinate)
	int getEnd(){
		return e;
	}

	//der Flaecheninhalt des Rechtecks
	int getArea(){
		return (e-b)*size;
	}

	//die Breite des Rechtecks (Unterschied der y-Koordinaten)
	int getWidth(){
		return (e-b);
	}

	//die x-Koordinaten des Rechtecks
	int x1, x2;
};


//die Klasse fuer eine Luecke
class Hole{
public:
	//die x-Koordinaten einer Luecke;
	//der Streifen, in dem die Luecke sich befindet
	int x1 = -1, x2 = -1, stripe = -1;

	//Konstruktoren fuer eine Luecke
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

//eine Funktion, die das Eingabeformat erkennt und entsprechend
//	zu Stunden oder Minuten umwandelt
int timeToMinutes(string time);


//Die folgenden Funktionen sind Muster/Kriterien, wie man
//	die Rechtecken in den Streifen sortiert

//kleinere Werte x1 zuerst
bool smallerx1(Rec *a, Rec *b);

//kleinere Werte x2 zuerst
bool smallerx2(Rec *a, Rec *b);

//1) groesseres Ende, 2) kleinerer Beginn, groesserer Flaecheninhalt
bool greaterEnd(Rec *a, Rec *b);

//1) kleinere Laenge, 2) kleinerer Beginn
bool smallerSize(Rec *a, Rec *b);

//1) groessere Laenge, 2) kleinerer Beginn
bool greaterSize(Rec *a, Rec *b);

//1) kleinerer Flaecheninhalt, 2) kleinerer Beginn
bool smallerArea(Rec *a, Rec *b);

//1) groesserer Flaecheninhalt, 2) kleinerer Beginn
bool greaterArea(Rec *a, Rec *b);


//Die folgenden Funktionen sind Muster/Kriterien, wie man
//	die Luecken in den Streifen sortiert

//1) groessere Laenge der Luecke, 2) niedrigerer Streifen
bool greaterHolesSize(const Hole &a, const Hole &b);

//1) kleinere Laenge der Luecke, 2) niedrigerer Streifen
bool smallerHolesSize(const Hole &a, const Hole &b);


#endif