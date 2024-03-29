/*****************************************************************************
   Project: CEDAR Logic Simulator
   Copyright 2006 Cedarville University, Benjamin Sprague,
                     Matt Lewellyn, and David Knierim
   All rights reserved.
   For license information see license.txt included with distribution.   

   CircuitParse: uses XMLParser to load and save user circuit files.
*****************************************************************************/

#ifndef CIRCUITPARSE_H_
#define CIRCUITPARSE_H_


#include "XMLParser.h"
#include "guiGate.h"
#include "GUICircuit.h"
#include "GUICanvas.h"
#include "guiWire.h"
#include <string>
#include <fstream>
#include <map>
#include <hash_map.h>

using namespace std;

// used for parsing inputs and outputs
class gateConnector {
public:
	string connectionID;
	long wireID;
};

// holds a parameter, whether gui or logic
class parameter {
public:
	parameter(string x, string y, bool nIsGUI) { paramName = x; paramValue = y; isGUI = nIsGUI; };
	string paramName;
	string paramValue;
	bool isGUI;
};

// Class CircuitParse:
//	Uses XMLParser to read and write user circuit files
class CircuitParse {
public:
	CircuitParse(string, vector< GUICanvas* >);
	CircuitParse(GUICanvas*);
	~CircuitParse();
	
	void loadFile(string);
	void parseFile();
	void saveCircuit(string, vector< GUICanvas* >, unsigned int currPage = 0);
	
private:
	XMLParser* mParse;
	string fileName;

	vector< GUICanvas* > gCanvases;
	GUICanvas* gCanvas;
	
	// Takes the pieces of gate info found in parseFile and implements them
	void parseGateToSend(string type, string ID, string position, vector < gateConnector > &inputs, vector < gateConnector > &outputs, vector < parameter > &params);
	// Parses, builds, and sets a wire's information (shape, id, etc)
	void parseWireToSend( void );
};

#endif /*CIRCUITPARSE_H_*/
