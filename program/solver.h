#pragma once

#ifndef SOLVER_H
#define SOLVER_H

#include<bits/stdc++.h>
#include"util.h"

#define pb push_back
typedef pair<int,int> iPair;

using namespace std;

class Solver{
	//die Breite und die Laenge des grossen Rechtecks
	int M, N;

	//die Dauer der Schließungszeit des Flohmarkt, 
	//	wenn sein Zeitraum unterbochen ist
	int recess;

	//der Zeitpunkt, ab dem der Flohmarkt beginnt, also B in der Doku
	int START;

	//die Liste Z mit allen Rechtecken
	vector<Rec*> rectangles;

	//jede i-te Liste enthaelt alle Rechtecken, die durch den Streifen
	// i verlaufen
	vector<vector<Rec*>> rectangles_stripes;

	//jede i-te Liste enthaelt alle Rechtecken, die durch den Streifen
	// i verlaufen und nicht ins grosse Rechteck platziert wurden
	vector<vector<Rec*>> unusedRectangles;

	//jede i-te Menge enthaelt alle Rechtecken, die durch den Streifen
	// i verlaufen und die ins grosse Rechteck platziert wurden
	vector<set<Rec*, setRecSort>> placed;

	//jede i-te Liste enthaelt alle Luecken, die im Streifen i auftreten
	vector<vector<Hole>> holes;

	//eine Liste mit allen Luecken
	vector<Hole> all_holes;

	//das Format der Eingabe: Stunden -- 0, Minuten -- 1
	int format;

	//eine Methode zum Einlesen der Textdatei und zur Konversion der Eingabe
	void readFile(string path);

	//diese Methode verteilt jedes Rechteck auf die Streifen,
	//	über die es verlaeuft
	void distributeToStripes();

	//diese Methode findet die naechste leere Luecke im Streifen p
	//	fuer ein Rechteck r
	int findNearestHole(Rec* r, int p);

	//diese Methode verarbeitet den Streifen p beim Lauf
	//	des Greedy-Algorithmus am Anfang
	void processStripe(int p);

	//diese Methode verarbeitet den Streifen p beim Lauf
	//	des Verbesserungsalgorithmus
	vector<Rec*> processStripeReturn(int p);

	//diese Methode findet alle nicht platzierten Rechtecke in einem
	//	Streifen p; p = -1 steht fuer alle Streifen
	void determineUnused(int p = -1);

	//diese Methode findet alle Luecken im Streifen p;
	//	p = -1 steht fuer alle Streifen
	void findHoles(int p = -1);

	//diese Methode findet die naechste Luecke im ganzen
	//	grossen Rechtek; itH ist ein Iterator fuer die Liste all_holes
	Hole findNextHole(int itH);

	//diese Methode findet ein noch nicht platziertes Rechteck 
	// fuer einer Luecke hole; itR ist ein Iterator fuer unusedRectangles
	pair<Rec*, iPair> findReplacement(Hole hole, int itR);

	//die Methode versucht im Verbesserungsverfahren, neue Rechtecke
	//	ins grosse Rechteck zu legen
	vector<Rec*> addNew(Rec* rep);

	//diese Methode entfernt alle Rechtecke, die mit einem im Verbesserungsverfahren
	//	gelegten Rechteck kollidieren und aktualisiert die Platzierung,
	//	falls es sich ein besserer Gesamtflaecheninhalt ergibt
	bool removeCollisions(int area, pair<Rec*, iPair> rep);


	//der Lauf des Programms
	void run();

	//der Lauf des heuristischen Verbesserungsverfahrens
	void runOptimization();

	random_device rd;
	
public:
	Solver(string path){
		//die Textdatei wird eingelesen
		readFile(path);
		//die vector werden initialisiert
		placed = vector<set<Rec*, setRecSort>> (M);
		unusedRectangles = vector<vector<Rec*>> (M);
		holes = vector<vector<Hole>> (M);

		//Lauf des Programms
		run();
	}
	~Solver(){
		//Entfernung aller Zeiger
		for (auto r: rectangles)
			delete r;
	}

	//diese Methode gibt den Gesamtflaecheninhalt aller Rechtecke,
	//	die platziert wurden
	int calculateAreaUsed();

// ---------------------- SEMI-DEBUGGING -------------------------

	//diese Methode gibt den Gesamtflaecheninhalt aller Rechtecke
	int calculateTotalArea();

	//diese Methode gibt den Wert M zurück
	int getM();
	//diese Methode gibt den Wert N zurück
	int getN();
	//diese Methode gibt das Format der Eingabe zurück
	int getFormat();
	//diese Methode gibt die Laenge der Pause zurück
	int getRecess();

	//diese Methode speichert die Ergebnisse in einer Textdatei
	void saveResult(string path, bool all = true, int mode = 1);


// ------------------------- DEBUGGING -------------------------

private:
	//diese Methode prueft, ob die Rechtecke sich nicht ueberdecken
	bool checkIfOverlaps();

public:

	//diese Methode gibt alle Rechtecken aus
	void printAllRectangles();
	//diese Methode gibt alle Rechtecken im Streifen p aus;
	//	p = -1 steht fuer alle Striefen
	void printStripes(int p, bool unused = false);
	//diese Methode gibt alle Luecken im Streifen p aus;
	//	p = -1 steht fuer alle Striefen
	void printHoles(int p = -1);
	//diese Methode gibt alle Rechtecken im Streifen p ab dem x1-Wert "from" aus,
	//	die platziert wurden;
	//	p = -1 steht fuer alle Striefen
	void printPlaced(int from = 0, int p = -1);
};

#endif