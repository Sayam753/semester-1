/*****************************************************************************
   Project: CEDAR Logic Simulator
   Copyright 2006 Cedarville University, Benjamin Sprague,
                     Matt Lewellyn, and David Knierim
   All rights reserved.
   For license information see license.txt included with distribution.   
*****************************************************************************/

// logic_circuit.h: interface for the Circuit class.
//
//////////////////////////////////////////////////////////////////////

#ifndef LOGIC_CIRCUIT_H
#define LOGIC_CIRCUIT_H

#include "logic_defaults.h"
#include "logic_event.h"
#include "logic_wire.h"
#include "logic_gate.h"
#include "logic_junction.h"

#include<queue>
#include<vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// A struct to hold parameters that need to be updated:
struct changedParam {
	IDType gateID;
	string paramName;
	changedParam( IDType nGateID, string nParamName ) : gateID( nGateID ), paramName( nParamName ) {};
};

class Circuit  
{
friend class Junction;
friend class Wire;
public:

// **************** The visible interface of the circuit ********************

// ********** Circuit input methods *************
	// Step the simulation forward by one timestep:
	// If a pointer to a set is passed, then it will
	// return a set of all the changed wires to the calling function.
	void step(  ID_SET< IDType > *changedWires = NULL );
	
	// Create a new gate, and return its ID:
	// NOTE: There should also be some way to pass
	// parameters back and forth to gates that use them.
	IDType newGate( string type, IDType gateID = ID_NONE  );
	
	// Create a new wire and return its ID:
	IDType newWire( IDType wireID = ID_NONE  );

	// Create a new junction and return its ID:
	IDType newJunction( IDType juncID = ID_NONE  );
	
	// Delete a gate, removing its connections to wires first:
	void deleteGate( IDType theGate );
	
	// Delete a wire, removing its connections from gates first:
	// (The implementation of this may require that wires keep track
	// of their input gates.)
	void deleteWire( IDType theWire );

	// Delete a junction, removing all its connections from wires first:
	void deleteJunction( IDType theJunc );

/*OBSOLETE
	// Connect an external event output to this wire, and return a new wire input ID
	// which the output can connect to:
	IDType connectExternalWireInput( IDType theWire );
*/

	// Connect a gate input to the output of a wire:
	IDType connectGateInput( IDType gateID, string gateInputID, IDType wireID );
	
	// Connect a gate output to the input of a wire:
	// (The wire will create one unique input for each unique gateID/gateOutputID combination.)
	IDType connectGateOutput( IDType gateID, string gateOutputID, IDType wireID);
	
	// Disconnect a gate input from the output of a wire:
	void disconnectGateInput( IDType gateID, string gateInputID );
	
	// Disconnect a gate output from the input of a wire:
	void disconnectGateOutput( IDType gateID, string gateOutputID );

	// Connect a junction to a wire:
	void connectJunction( IDType juncID, IDType wireID );
	
	// Unhook a junction from a wire:
	void disconnectJunction( IDType juncID, IDType wireID );

	// Create an event and put it in the event queue:
	void createEvent( TimeType eventTime, IDType wireID, IDType gateID, string gateOutputID, StateType newState );
	
	// Create an event that occurs at systemTime + delay:
	TimeType createDelayedEvent( TimeType delay, IDType wireID, IDType gateID, string gateOutputID, StateType newState );

	// Create Junction Event and put it in the event queue:
	void createJunctionEvent( TimeType eventTime, IDType juncID, bool newState );
	
	// Clear out the event queue, destroying all events,
	// and also erase all events in the gateUpdateList and wireUpdateList.
	// This is used if we wanted a simulation where all of the wires
	// start with "UNKNOWN" state and don't update until a signal
	// from the outside world reaches them.
	void destroyAllEvents( void );

	// Set a gate parameter:
	// (If the gate's parameter change requires the gate to be
	// re-evaluated during the next cycle, then add it to the update list.)
	void setGateParameter( IDType gateID, string paramName, string value );
	void setGateInputParameter( IDType gateID, string inputID, string paramName, string value );
	void setGateOutputParameter( IDType gateID, string outputID, string paramName, string value );

	// Methods and data for handling parameter updates to be
	// sent to the GUI from the logic core:
	void addUpdateParam( IDType gateID, string paramName ) {
		paramUpdateList.push_back( changedParam( gateID, paramName ) );
	};

	vector < changedParam > getParamUpdateList( void ) {
		return paramUpdateList;
	};

	void clearParamUpdateList( void ) {
		paramUpdateList.clear();
	};

protected:
	vector < changedParam > paramUpdateList;
public:

// ************ Circuit inspection methods **************

	// Get the IDs of all gates in the circuit:
	ID_SET< IDType > getGateIDs( void ) {
		ID_SET< IDType > idList;
		ID_MAP< IDType, GATE_PTR >::iterator thisGate = gateList.begin();
		while( thisGate != gateList.end() ) {
			idList.insert( thisGate->first );
			thisGate++;
		}
		return idList;
	};

	// Get the value of a gate parameter:
	string getGateParameter( IDType gateID, string paramName );

	// Get a wire state by ID:
	StateType getWireState( IDType wireID );

	// Get and set a junction's on/off toggle state:
	void setJunctionState( IDType juncID, bool newState );
	bool getJunctionState( IDType juncID );

	// Return the current simulation time:
	TimeType getSystemTime( void );

	// Returns a list of all wires that are connected to this
	// wire via junctions:
	set< WIRE_PTR > getJunctionGroup( IDType wireID );
	set< IDType > getJunctionGroupIDs( IDType wireID );
	set< WIRE_PTR > getJunctionGroup( set< IDType >* wireGroupIDs );

	// Returns maps of junction items for use in gate functions
	ID_MAP< string, IDType >* getJunctionIDs( void ) { return &junctionIDs; };
	ID_MAP< string, unsigned long >* getJunctionUseCounter( void ) { return &junctionUseCounter; };
	
// ************* End of the visible interface of the circuit ****************

	Circuit();
	virtual ~Circuit();

protected:
// For use by the Junction and Wire classes only:
	WIRE_PTR getWire( IDType theWire ) { return wireList[theWire]; };
	JUNC_PTR getJunction( IDType theJunc ) { return juncList[theJunc]; };

private:
	// All the gates in the circuit, and the ID counter:
	ID_MAP< IDType, GATE_PTR > gateList;
	IDType gateIDCount;

	// All the wires in the circuit, and the ID counter:
	ID_MAP< IDType, WIRE_PTR > wireList;
	IDType wireIDCount;

	// All the junctions in the circuit, and its ID counter:
	ID_MAP< IDType, JUNC_PTR > juncList;
	IDType juncIDCount;
	
	// This is the mapping of junction states, and how often each is used (# of gates):
	ID_MAP< string, IDType > junctionIDs;
	ID_MAP< string, unsigned long > junctionUseCounter;

	// The gates that must be polled at each time step
	// to allow them to create events:
	// This is just a list of gate IDs to access from the gateList.
	ID_SET< IDType > polledGates;

	// This is the set of gates that needs to be forced into updating
	// at the next call to step() due to one of their inputs being
	// either connected or disconnected:
	ID_SET< IDType > gateUpdateList;

	// This is the set of wires that needs to be forced into updating
	// at the next call to step() due to one of their inputs being
	// either connected or disconnected:
	ID_SET< IDType > wireUpdateList;

	
	// This is the event queue for the Circuit:
	priority_queue< Event, vector< Event >, greater< Event > > eventQueue;
	
	// This is the current system time:
	TimeType systemTime;
};


// An operator to sort events by their event time. Used by the eventQueue;
bool operator > (const Event &left, const Event &right);

#endif // LOGIC_CIRCUIT_H
