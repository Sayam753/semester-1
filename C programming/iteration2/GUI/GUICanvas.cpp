/*****************************************************************************
   Project: CEDAR Logic Simulator
   Copyright 2006 Cedarville University, Benjamin Sprague,
                     Matt Lewellyn, and David Knierim
   All rights reserved.
   For license information see license.txt included with distribution.   

   GUICanvas: Contains rendering and input functions for a page
*****************************************************************************/

#include "GUICanvas.h"
#include "MainApp.h"
#include "paramDialog.h"
#include "klsClipboard.h"

// Included to use the min() and max() templates:
#include <algorithm>
using namespace std;

// Enable access to objects in the main application
DECLARE_APP(MainApp)

// GUICanvas constructor - defaults grid size to 1 unit square
GUICanvas::GUICanvas(wxWindow *parent, GUICircuit* gCircuit, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : klsGLCanvas(parent, name, id, pos, size, style|wxSUNKEN_BORDER ) {

	this->gCircuit = gCircuit;
	isWithinPaste = false;
	currentDragState = DRAG_NONE;
	
	hotspotHighlight = "";
	
	drawWireHover = false;
	
	setHorizGrid(0.5);
	setVertGrid(0.5);
	
	// Add mouse object to collision checker
	mouse = new klsCollisionObject( COLL_MOUSEBOX );
	snapMouse = new klsCollisionObject( COLL_MOUSEBOX );
	collisionChecker.addObject( mouse );
	
	// Add drag selection box to collision checker
	dragselectbox = new klsCollisionObject( COLL_SELBOX );
	collisionChecker.addObject( dragselectbox );
}

GUICanvas::~GUICanvas() {
	return;
}

// Clears the circuit by selecting all gates and wires and then running a delete command
void GUICanvas::clearCircuit() {
	selectedGates.clear();
	selectedWires.clear();
	preMove.clear();
	preMoveWire.clear();

	collisionChecker.clear();
	gateList.clear();
	wireList.clear();

	// Add mouse object to collision checker
	collisionChecker.addObject( mouse );
	
	// Add drag selection box to collision checker
	collisionChecker.addObject( dragselectbox );
	
	hotspotHighlight = "";
	potentialConnectionHotspots.clear();
	drawWireHover = false;
	isWithinPaste = false;
	saveMove = false;
}

// Inserts an existing gate onto the canvas at a particular x,y position
void GUICanvas::insertGate(unsigned long id, guiGate* gt, float x, float y) {
	if (gt == NULL) return;
	gt->setGLcoords(x, y);
	gateList[id] = gt;
	
	// Add the gate to the collision checker:
	collisionChecker.addObject( gt );
}

// Inserts an existing wire onto the canvas
void GUICanvas::insertWire(unsigned long id, guiWire* wire) {
	if (wire == NULL) return;
	wireList[id] = wire;

	// Add the wire to the collision checker:
	collisionChecker.addObject( wire );
}

// If the gate exists on this page, then remove it from the page
void GUICanvas::removeGate(unsigned long gid) {
	hash_map < unsigned long, guiGate* >::iterator thisGate = gateList.find(gid);
	if (thisGate != gateList.end()) {
		// Clear a hotspot we're holding if we need to
		if (hotspotGate == gid) hotspotHighlight = "";
		
		// Take the gate out of the collision checker:
		collisionChecker.removeObject( thisGate->second );
		collisionChecker.update();

		gateList.erase(thisGate);
	}
}

// If the wire exists on this page, then remove it from the page
void GUICanvas::removeWire(unsigned long wid) {
	hash_map < unsigned long, guiWire* >::iterator thisWire = wireList.find(wid);
	if (thisWire != wireList.end()) {
		// Take the wire out of the collision checker:
		collisionChecker.removeObject( thisWire->second );
		collisionChecker.update();

		wireList.erase(thisWire);
	}
}

// Render the page
void GUICanvas::OnRender() {
	
	glColor4f( 0.0, 0.0, 0.0, 1.0 );
	
	// Draw the wires:
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	// Draw the wires:
	hash_map< unsigned long, guiWire* >::iterator thisWire = wireList.begin();
	while( thisWire != wireList.end() ) {
		(thisWire->second)->draw();
		thisWire++;
	}

	// Draw the gates:
	hash_map< unsigned long, guiGate* >::iterator thisGate = gateList.begin();
	while( thisGate != gateList.end() ) {
		(thisGate->second)->draw();
		thisGate++;
	}
	
	
	// Draw the basic view objects:
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	// Mouse-over hotspot
	if( hotspotHighlight.size() > 0 ) {
		// Outline the hotspots:
		GLfloat oldColor[4];
		glGetFloatv( GL_CURRENT_COLOR, oldColor );
		glColor4f( 1.0, 0.0, 0.0, 1.0 );
		glDisable( GL_LINE_STIPPLE );
	
		float diff = HOTSPOT_SCREEN_RADIUS * getZoom();
		float x, y;
		gateList[hotspotGate]->getHotspotCoords(hotspotHighlight, x, y);
		glBegin(GL_LINE_LOOP);
			glVertex2f( x - diff, y + diff );
			glVertex2f( x + diff, y + diff );
			glVertex2f( x + diff, y - diff );
			glVertex2f( x - diff, y - diff );
		glEnd();

		// Set the color back to the old color:
		glColor4f( oldColor[0], oldColor[1], oldColor[2], oldColor[3] );
	}

	// Mouse-over wire
	if( drawWireHover ) {
		// Outline the hotspot:
		GLfloat oldColor[4];
		glGetFloatv( GL_CURRENT_COLOR, oldColor );
		glColor4f( 1.0, 0.0, 0.0, 1.0 );
		glDisable( GL_LINE_STIPPLE );
	
		float diff = HOTSPOT_SCREEN_RADIUS * getZoom();
		GLPoint2f m = getMouseCoords();
		glBegin(GL_LINES);
			glVertex2f( m.x - diff, m.y + diff );
			glVertex2f( m.x + diff, m.y - diff );
		glEnd();
		glBegin(GL_LINES);
			glVertex2f( m.x + diff, m.y + diff );
			glVertex2f( m.x - diff, m.y - diff );
		glEnd();

		// Set the color back to the old color:
		glColor4f( oldColor[0], oldColor[1], oldColor[2], oldColor[3] );
	}

	// Collisions
	bool drawOverlaps = true;
	if( drawOverlaps ) {
		// Draw the alpha-blended selection box over top of the overlap:
		glColor4f( 0.4, 0.1, 0.0, 0.3 );
		
		map< klsCollisionObjectType, CollisionGroup >::iterator ovrLists = collisionChecker.overlaps.begin();
		while( ovrLists != collisionChecker.overlaps.end() ) {
			if (ovrLists->first != COLL_GATE) { ovrLists++; continue; };
			CollisionGroup::iterator obj = (ovrLists->second).begin();
			while( obj != (ovrLists->second).end() ) {
				if ((*obj)->getType() != COLL_GATE) { obj++; continue; };
				CollisionGroup hitThings = (*obj)->getOverlaps();
				CollisionGroup::iterator hit = hitThings.begin();
				while( hit != hitThings.end() ) {
					if ((*hit)->getType() != COLL_GATE) { hit++; continue; };
					klsBBox hitBox = (*obj)->getBBox();
					hitBox = hitBox.intersect((*hit)->getBBox());
					if( !hitBox.empty() ) {
						glRectf(hitBox.getLeft(), hitBox.getBottom(), hitBox.getRight(), hitBox.getTop());
					}
					hit++;
				}
				obj++;
			}
			ovrLists++;
		}

		// Reset the color back to black:
		glColor4f( 0.0, 0.0, 0.0, 1.0 );
	}

	// Drag select box
	if (currentDragState == DRAG_SELECT) {
		// If we're in drag-select, draw the sel box

		glColor4f( 0.0, 0.4, 1.0, 1.0 );

		// Draw the solid outline box:
		GLPoint2f start = getDragStartCoords();
		GLPoint2f end = getMouseCoords();
		glBegin(GL_LINE_LOOP);
			glVertex2f(start.x, start.y);
			glVertex2f(start.x, end.y);
			glVertex2f(end.x, end.y);
			glVertex2f(end.x, start.y);
		glEnd();


		// Draw the alpha-blended selection box over top of the gates:
		glColor4f( 0.0, 0.4, 1.0, 0.3 );
		
		glRectf(start.x, start.y, end.x, end.y);

		// Reset the color back to black:
		glColor4f( 0.0, 0.0, 0.0, 1.0 );
	}
	// Drag-connect line 
	else if (currentDragState == DRAG_CONNECT) {
		GLPoint2f start = getDragStartCoords();
		GLPoint2f end = getMouseCoords();
		glColor4f( 0.0, 1.0, 0.0, 1.0 );
		glBegin(GL_LINES);
			glVertex2f(start.x, start.y);
			glVertex2f(end.x, end.y);
		glEnd();
		glColor4f( 0.0, 0.0, 0.0, 1.0 );
	}
	// Draw the new gate for Drag_Newgate 
	else if (currentDragState == DRAG_NEWGATE) {
		newDragGate->draw();
	}
	
	// Draw potential connection hotspots
	glLoadIdentity ();
	glColor4f( 0.0, 0.0, 1.0, 1.0 );
	for (unsigned int i = 0; i < potentialConnectionHotspots.size(); i++) {
		float diff = HOTSPOT_SCREEN_RADIUS * getZoom();
		float x = potentialConnectionHotspots[i].x, y = potentialConnectionHotspots[i].y;
		glBegin(GL_LINE_LOOP);
			glVertex2f( x - diff, y + diff );
			glVertex2f( x + diff, y + diff );
			glVertex2f( x + diff, y - diff );
			glVertex2f( x - diff, y - diff );
		glEnd();
	}			
	glColor4f( 0.0, 0.0, 0.0, 1.0 );
}

void GUICanvas::mouseLeftDown(wxMouseEvent& event) {
	GLPoint2f m = getMouseCoords();
	bool handled = false;
	// If I am in a paste operation then mouse-up is all I am concerned with
	if (isWithinPaste) return;
	
	// Do a collision detection on all first-level objects.
	// The map collisionChecker.overlaps now contains
	// all of the objects involved in any collisions.
	//collisionChecker.update();

	// Loop through all objects hit by the mouse
	//	Favor wires over gates
	CollisionGroup hitThings = mouse->getOverlaps();
	CollisionGroup::iterator hit = hitThings.begin();
	while( hit != hitThings.end() && !handled ) {
		if ((*hit)->getType() == COLL_WIRE) {
			guiWire* hitWire = ((guiWire*)(*hit));
			bool wasSelected = hitWire->isSelected();
			hitWire->unselect();
			if ( hitWire->hover( m.x, m.y, WIRE_HOVER_SCREEN_DELTA * getZoom() )) {
				hitWire->select();
				if (event.ShiftDown() && wasSelected) hitWire->unselect();
				if (!(event.ShiftDown())) {
					unselectAllWires();
					unselectAllGates();
					hitWire->select();
				}
				if (event.ControlDown() && !(this->isLocked())) {
					currentConnectionSource.isGate = false;
					currentConnectionSource.objectID = hitWire->getID();
					currentDragState = DRAG_CONNECT;
				}
				else if (!(event.ShiftDown())) {
					wireHoverID = hitWire->getID();
					if (wireList[wireHoverID]->startSegDrag(snapMouse) && !(this->isLocked())) currentDragState = DRAG_WIRESEG;
					hitWire->unselect();
				}
				handled = true;	
			}
		}
		hit++;
	}

	// do we have a highlighted hotspot (which means we're on it now)
	if (hotspotHighlight.size() > 0 && currentDragState == DRAG_NONE && !(this->isLocked())) {
		// Start dragging a new wire:
		//gateList[hotspotGate]->select();
		unselectAllGates();
		unselectAllWires();
		handled = true; // Don't worry about checking other events in this proc
		currentDragState = DRAG_CONNECT;
		currentConnectionSource.isGate = true;
		currentConnectionSource.objectID = hotspotGate;
		currentConnectionSource.connection = hotspotHighlight;
	}

	// Now check gate collisions
	hit = hitThings.begin();
	while( hit != hitThings.end() && !handled ) {
		if ((*hit)->getType() == COLL_GATE) {
			guiGate* hitGate = ((guiGate*)(*hit));
			bool wasSelected = hitGate->isSelected();
			if (event.ShiftDown() && wasSelected) hitGate->unselect(); // Remove gate from selection
			else if (event.ShiftDown() && !wasSelected) hitGate->select(); // Add gate to selection
			else if (!(event.ShiftDown()) && !wasSelected) { // Begin new selection group
				unselectAllGates();
				unselectAllWires();
				hitGate->select();
			}
			if (!(event.ShiftDown()) && !(this->isLocked())) currentDragState = DRAG_SELECTION; // Start dragging
			handled = true;
		}
		hit++;
	}

	// If I am not in a selection group and I haven't handled a selection then unselect everything
	if (!handled && !(event.ShiftDown())) {
		unselectAllGates();
		unselectAllWires();
	}
	
	if (!handled) { // Otherwise initialize drag select
		currentDragState = DRAG_SELECT;
	}
	
	// Show the updates
	Refresh();

	// clean up the selected gates vector and saved premove state
	selectedGates.clear();
	preMove.clear();
	saveMove = false;
	hash_map < unsigned long, guiGate* >::iterator thisGate = gateList.begin();
	while (thisGate != gateList.end()) {
		if ((thisGate->second)->isSelected()) {
			// Push back the gate's id, xy pos, angle, and select flag
			preMove.push_back(GateState((thisGate->first), 0, 0, (thisGate->second)->isSelected()));
			(thisGate->second)->getGLcoords(preMove[preMove.size()-1].x, preMove[preMove.size()-1].y);
			selectedGates.push_back((thisGate->first));
		}
		thisGate++;
	}
	// clean up the selected wires vector
	selectedWires.clear();
	preMoveWire.clear();
	hash_map < unsigned long, guiWire* >::iterator thisWire = wireList.begin();
	while (thisWire != wireList.end()) {
		if ((thisWire->second)->isSelected()) {
			// Push back the wire's id
			preMoveWire.push_back(WireState((thisWire->first), (thisWire->second)->getCenter(), (thisWire->second)->getSegmentMap()));
			selectedWires.push_back((thisWire->first));
		}
		thisWire++;
	}
}

void GUICanvas::mouseRightDown(wxMouseEvent& event) {
	GLPoint2f m = getMouseCoords();
	bool angleHandled = false; // flag to ensure only one gate is rotated (if any)
	vector < unsigned long >::iterator sGate;

	if (isWithinPaste) return; // Left mouse up is the next event we are looking for

	// Do a collision detection on all first-level objects.
	// The map collisionChecker.overlaps now contains
	// all of the objects involved in any collisions.
	//collisionChecker.update();

	// Go ahead and remove all selection since this is the right mouse button
	unselectAllGates();
	unselectAllWires();

	// If locked then we have nothing else to do
	if ( this->isLocked() ) return;

	// do we have a highlighted hotspot (which means we're on it now)
	if (hotspotHighlight.size() > 0) {
		// If the hotspot is connected then we disconnect it and generate a command
		if (gateList[hotspotGate]->isConnected(hotspotHighlight)) {
			// disconnect this wire
			if (gateList[hotspotGate]->getConnection(hotspotHighlight)->numConnections() > 2)
				gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdDisconnectWire( gCircuit, gateList[hotspotGate]->getConnection(hotspotHighlight)->getID(), hotspotGate, hotspotHighlight )) );
			else gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdDeleteWire( gCircuit, this, gateList[hotspotGate]->getConnection(hotspotHighlight)->getID() )) );
		}
		currentDragState = DRAG_NONE;
	} else if (currentDragState == DRAG_NONE) {
		// Not on a hotspot, so check if it's on a gate:
		// Loop through all objects hit by the mouse
		CollisionGroup hitThings = mouse->getOverlaps();
		CollisionGroup::iterator hit = hitThings.begin();
		unselectAllGates();
		unselectAllWires();
		while( hit != hitThings.end() && !angleHandled ) {
			if ((*hit)->getType() == COLL_GATE) {
				guiGate* hitGate = ((guiGate*)(*hit));
				if (!angleHandled) {
					// BEGIN WORKAROUND
					//	Gates that have connections cannot be rotated without sacrificing wire sanity
					map < string, GLPoint2f > gateHotspots = hitGate->getHotspotList();
					map < string, GLPoint2f >::iterator ghsWalk = gateHotspots.begin();
					bool gateConnected = false;
					while ( ghsWalk !=  gateHotspots.end() ) {
						if ( hitGate->isConnected( ghsWalk->first ) ) {
							gateConnected = true;
							break;
						}
						ghsWalk++;
					}
					if ( gateConnected ) { hit++; continue; }
					// END WORKAROUND
					map < string, string > newParams(*(hitGate->getAllGUIParams()));
					istringstream issAngle(newParams["angle"]);
					GLfloat angle;
					issAngle >> angle;
					angle += 90.0;
					if (angle >= 360.0) angle -= 360.0;
					ostringstream ossAngle;
					ossAngle << angle;
					newParams["angle"] = ossAngle.str();
					gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdSetParams(gCircuit, hitGate->getID(), paramSet(&newParams, NULL) )) );
				}
			}
			hit++;
		}				
	}
	Refresh();
}

void GUICanvas::OnMouseMove( GLdouble glX, GLdouble glY, bool ShiftDown, bool CtrlDown ) {
	// Keep a flag for whether things have changed.  If nothing changes, then no render is necessary.
	bool shouldRender = false;

	if (wxGetApp().appSystemTime->Time() > wxGetApp().appSettings.refreshRate) {
		wxGetApp().appSystemTime->Pause();
		if (gCircuit->panic) return;
		wxCriticalSectionLocker locker(wxGetApp().m_critsect);
		while (wxGetApp().mexMessages.TryLock() == wxMUTEX_BUSY) wxYield();
		while (wxGetApp().dLOGICtoGUI.size() > 0) {
			gCircuit->parseMessage(wxGetApp().dLOGICtoGUI.front());
			wxGetApp().dLOGICtoGUI.pop_front();
		}
		wxGetApp().mexMessages.Unlock();
wxGetApp().logfile << "stepping after " << wxGetApp().appSystemTime->Time() << "ms" << endl;
		if (gCircuit->panic) return;
		// Do function of number of milliseconds that passed since last step
		ostringstream oss;
		oss << "STEPSIM " << wxGetApp().appSystemTime->Time() / wxGetApp().timeStepMod;
		gCircuit->lastTime = wxGetApp().appSystemTime->Time();
		gCircuit->lastTimeMod = wxGetApp().timeStepMod;
		gCircuit->lastNumSteps = wxGetApp().appSystemTime->Time() / wxGetApp().timeStepMod;
		gCircuit->sendMessageToCore(oss.str());
		gCircuit->simulate = false;
		wxGetApp().appSystemTime->Start(wxGetApp().appSystemTime->Time() % wxGetApp().timeStepMod);
		shouldRender = true;
	}

	GLPoint2f m = getMouseCoords();
	GLPoint2f dStart = getDragStartCoords(BUTTON_LEFT);
	GLPoint2f diff( m.x - dStart.x, m.y - dStart.y ); // What is the difference between start and now

	GLPoint2f mSnap = getSnappedPoint( m ); // Work with a snapped mouse coord
	GLPoint2f dStartSnap = getSnappedPoint( dStart );
	GLPoint2f diffSnap( mSnap.x - dStartSnap.x, mSnap.y - dStartSnap.y );

	// Update the mouse as a collision object:
	klsBBox mBox;
	float delta = MOUSE_HOVER_DELTA * getZoom();
	mBox.addPoint( m );
	mBox.extendTop( delta );
	mBox.extendBottom( delta );
	mBox.extendLeft( delta );
	mBox.extendRight( delta );
	mouse->setBBox( mBox );

	klsBBox smBox;
	smBox.addPoint( mSnap );
	snapMouse->setBBox( smBox );

	// Update the drag select box coordinates:
	klsBBox dBox;
	dBox.addPoint( dStart );
	dBox.addPoint( m );
	dragselectbox->setBBox( dBox );
	
	if ( this->isLocked() ) return;
	
wxStopWatch collCheckWatch;
	// Do a collision detection on all first-level objects.
	// The map collisionChecker.overlaps now contains
	// all of the objects involved in any collisions.
	collisionChecker.update();
wxGetApp().logfile << "profile time a: " << collCheckWatch.Time() << endl;
	// Update a newly-dragged gate's position
	if (currentDragState == DRAG_NEWGATE) {
		shouldRender = true;
		newDragGate->setGLcoords(mSnap.x, mSnap.y);
		//selectedGates.push_back(newDragGate->getID());
	}
	
	// If the hotspot hover is on, make it clear
	if (hotspotHighlight.size() > 0) shouldRender = true;
	hotspotHighlight = "";

	// If necessary, save the move being done.
	if (preMove.size() > 0 && currentDragState != DRAG_CONNECT && (diffSnap.x != 0 || diffSnap.y != 0)) saveMove = true;

	if (currentDragState == DRAG_SELECTION) {
		// Move all gates that are selected in the preMove vector:
		for (unsigned int i = 0; i < preMoveWire.size(); i++) wireList[preMoveWire[i].id]->move(preMoveWire[i].point, diffSnap);
		for (unsigned int i = 0; i < preMove.size(); i++) gateList[preMove[i].id]->setGLcoords(preMove[i].x+diffSnap.x, preMove[i].y+diffSnap.y);
	} else if (currentDragState == DRAG_WIRESEG) {
		wireList[wireHoverID]->updateSegDrag(snapMouse);
	}

	// Generate a new list of potential connections
	potentialConnectionHotspots.clear();
	// Reset wire hover flag
	if (drawWireHover) shouldRender = true;
	drawWireHover = false;

	CollisionGroup hitThings = mouse->getOverlaps();
	CollisionGroup::iterator hit = hitThings.begin();
	while( hit != hitThings.end()) {
		if ((*hit)->getType() == COLL_GATE) {
			guiGate* hitGate = ((guiGate*)(*hit));
			
			// Update the hotspot hover variables:
			if( hotspotHighlight.size() == 0 ) {
				if (currentDragState != DRAG_NEWGATE || hitGate->getID() != newDragGate->getID()) hotspotHighlight = hitGate->checkHotspots( m.x, m.y, HOTSPOT_SCREEN_DELTA * getZoom() );
				if( hotspotHighlight.size() > 0 ) {
					if (currentDragState != DRAG_NEWGATE || hitGate->getID() != newDragGate->getID()) hotspotGate = hitGate->getID();
					shouldRender = true;
				}
			}
			
		}
		if ((*hit)->getType() == COLL_WIRE && !drawWireHover && currentDragState != DRAG_WIRESEG) { // Check for wire hover
			guiWire* hitWire = ((guiWire*)(*hit));
			drawWireHover = hitWire->hover( m.x, m.y, WIRE_HOVER_SCREEN_DELTA * getZoom() );
			wireHoverID = hitWire->getID();
			if (drawWireHover) shouldRender = true;
		}
		hit++;
	}

	if (currentDragState == DRAG_SELECT) { // Check for items within drag select box
		CollisionGroup selThings = dragselectbox->getOverlaps();
		hit = selThings.begin();
		while( hit != selThings.end()) {
			if ((*hit)->getType() == COLL_GATE) {
				guiGate* hitGate = ((guiGate*)(*hit));
				bool found = false;
				for (unsigned int i = 0; i < preMove.size() && !found; i++) {
					if (preMove[i].id == hitGate->getID()) found = true;
				}
				if (!found) {
					if (dBox.contains((*hit)->getBBox())) hitGate->select();
					else hitGate->unselect();
				}
			}
			if ((*hit)->getType() == COLL_WIRE) {
				guiWire* hitWire = ((guiWire*)(*hit));
				bool found = false;
				for (unsigned int i = 0; i < preMoveWire.size() && !found; i++) {
					if (preMoveWire[i].id == hitWire->getID()) found = true;
				}
				if (!found) {
					if (dBox.contains((*hit)->getBBox())) hitWire->select();
					else hitWire->unselect();
				}
			}
			hit++;
		}
	}
	
	// Check potential hotspot connections (on gate/gate collisions)
	CollisionGroup ovrList = collisionChecker.overlaps[COLL_GATE];
	CollisionGroup::iterator obj = ovrList.begin();
	while( obj != ovrList.end() ) {
		CollisionGroup hitThings = (*obj)->getOverlaps();
		CollisionGroup::iterator hit = hitThings.begin();
		while( hit != hitThings.end() ) {
			// Only check gate collisions
			if ((*hit)->getType() != COLL_GATE) { hit++; continue; };
			// obj and hit are two overlapping gates
			//  get overlapping hotspots of obj in another group
			CollisionGroup hotspotOverlaps = (*obj)->checkSubsToSubs(*hit);
			CollisionGroup::iterator hotspotCollide = hotspotOverlaps.begin();
			while (hotspotCollide != hotspotOverlaps.end()) {
				// hotspotCollide is in obj; hsWalk is in hit
				CollisionGroup hshits = (*hotspotCollide)->getOverlaps();
				CollisionGroup::iterator hsWalk = hshits.begin();
				while (hsWalk != hshits.end()) {
					if ( !(((guiGate*)(*obj))->isConnected(((gateHotspot*)(*hotspotCollide))->name)) && !(((guiGate*)(*hit))->isConnected(((gateHotspot*)(*hsWalk))->name)))
						potentialConnectionHotspots.push_back( ((gateHotspot*)(*hotspotCollide))->getLocation() );
					hsWalk++;
				}
				hotspotCollide++;
			}
			hit++;
		}
		obj++;
	}
wxGetApp().logfile << "profile time b: " << collCheckWatch.Time() << endl;

	if (currentDragState == DRAG_SELECTION || currentDragState == DRAG_SELECT || currentDragState == DRAG_CONNECT || currentDragState == DRAG_WIRESEG) shouldRender = true;
	
	// Only render if necessary
	//	REFRESH DOESN'T SEEM TO UPDATE IN TIME FOR MOUSE MOVE
	if (shouldRender) {
		klsGLCanvasRender();
		// Show the new buffer:
		glFlush();
		SwapBuffers();
	}
	
	// clean up the selected gates vector
	selectedGates.clear();
	hash_map < unsigned long, guiGate* >::iterator thisGate = gateList.begin();
	while (thisGate != gateList.end()) {
		if ((thisGate->second)->isSelected()) selectedGates.push_back((thisGate->first));
		thisGate++;
	}
	// clean up the selected wires vector
	selectedWires.clear();
	hash_map < unsigned long, guiWire* >::iterator thisWire = wireList.begin();
	while (thisWire != wireList.end()) {
		if ((thisWire->second)->isSelected()) selectedWires.push_back((thisWire->first));
		thisWire++;
	}
}

void GUICanvas::OnMouseUp(wxMouseEvent& event) {
	GLPoint2f m = getMouseCoords();

	bool handled = false;
	SetCursor(wxCursor(wxCURSOR_ARROW));
	hash_map < unsigned long, guiGate* >::iterator thisGate;
	cmdMoveSelection* movecommand = NULL;
	cmdCreateGate* creategatecommand = NULL;

	// Update the drag select box coordinates for wire source detection:
	klsBBox dBox;
	float delta = HOTSPOT_SCREEN_DELTA * getZoom();
	dBox.addPoint( getDragStartCoords( BUTTON_LEFT ) );
	dBox.extendTop( delta );
	dBox.extendBottom( delta );
	dBox.extendLeft( delta );
	dBox.extendRight( delta );
	dragselectbox->setBBox( dBox );

	// If moving a selection then save the move as a command
	if (saveMove && currentDragState == DRAG_SELECTION) {
		float gX, gY;
		if (preMove.size() > 0) {
			gateList[preMove[0].id]->getGLcoords(gX, gY);
			movecommand = new cmdMoveSelection( gCircuit, preMove, preMoveWire, preMove[0].x, preMove[0].y, gX, gY );
			for (unsigned int i = 0; i < preMove.size(); i++) gateList[preMove[i].id]->updateConnectionMerges();
			if (!isWithinPaste) gCircuit->GetCommandProcessor()->Submit( (wxCommand*)movecommand );
			if (!isWithinPaste) movecommand->Undo();
		}
		if (preMove.size() > 1) preMove.clear();
	}

	// Check for single selection out of group
	if (preMove.size() > 0) {
		float gX, gY;
		gateList[preMove[0].id]->getGLcoords(gX, gY);
wxGetApp().logfile << "MOUSEUP" << endl;
wxGetApp().logfile << gX << "," << gY << " " << preMove[0].x << "," << preMove[0].y << " " << saveMove << endl;
		if (gX == preMove[0].x && gY == preMove[0].y && !(event.ShiftDown())) { // no move
			CollisionGroup hitThings = mouse->getOverlaps();
			CollisionGroup::iterator hit = hitThings.begin();
			while( hit != hitThings.end() ) {
				if ((*hit)->getType() == COLL_GATE) {
					guiGate* hitGate = ((guiGate*)(*hit));
					unselectAllGates();
					unselectAllWires();
					preMove.clear();
					preMove.push_back(GateState(hitGate->getID(), 0, 0, true));
					hitGate->select();
					saveMove = false;
					break;
				}
				hit++;
			}		
		}
	}

	if (currentDragState == DRAG_WIRESEG) {
		wireList[wireHoverID]->endSegDrag();
		wireList[wireHoverID]->select();
		gCircuit->GetCommandProcessor()->Submit( new cmdWireSegDrag( gCircuit, this, wireHoverID ) );
	}

	// If dragging a new gate then 
	if (currentDragState == DRAG_NEWGATE) {
		int newGID = gCircuit->getNextAvailableGateID();
		float nx, ny;
		newDragGate->getGLcoords(nx, ny);
		gCircuit->getGates()->erase(newDragGate->getID());
		creategatecommand = new cmdCreateGate( this, gCircuit, newGID, newDragGate->getLibraryGateName(), nx, ny );
		gCircuit->GetCommandProcessor()->Submit( (wxCommand*)creategatecommand );
		collisionChecker.removeObject( newDragGate );
		// Only now do a collision detection on all first-level objects since the new gate is in.
		// The map collisionChecker.overlaps now contains
		// all of the objects involved in any collisions.
		collisionChecker.update();
		cmdSetParams setgateparams( gCircuit, newGID, paramSet((*(gCircuit->getGates()))[newGID]->getAllGUIParams(), (*(gCircuit->getGates()))[newGID]->getAllLogicParams()));
		setgateparams.Do();
		delete newDragGate;
		gateList[newGID]->select();
		selectedGates.push_back(newGID);
	}
	else {
		// Do a collision detection on all first-level objects.
		// The map collisionChecker.overlaps now contains
		// all of the objects involved in any collisions.
		collisionChecker.update();
		
		if ((currentDragState == DRAG_NONE || currentDragState == DRAG_SELECTION) && preMove.size() == 1) {
			// Loop through all objects hit by the mouse
			//	Favor wires over gates
//			CollisionGroup hitThings = mouse->getOverlaps();
//			CollisionGroup::iterator hit = hitThings.begin();
//			while( hit != hitThings.end() && !handled ) {
//				if ((*hit)->getType() == COLL_GATE) {
//					guiGate* hitGate = ((guiGate*)(*hit));
					guiGate* hitGate = gateList[preMove[0].id];
					if (!(event.ShiftDown()) && ((event.LeftUp() && currentDragState == DRAG_SELECTION) || event.LeftDClick())) {
						// Check for toggle switch
						float x, y;
						hitGate->getGLcoords(x,y);
						if (!saveMove) {
							string clickHandleGate = hitGate->checkClick( m.x, m.y );
							if (clickHandleGate.size() > 0) {
								gCircuit->sendMessageToCore(clickHandleGate);
								handled = true;
							}
						}
						if (event.LeftDClick() && !handled) {
							hitGate->doParamsDialog( gCircuit, gCircuit->GetCommandProcessor() );
							currentDragState = DRAG_NONE;
							// setparams command will handle oscope update
//							if (hitGate->getGUIType() == "TO") gCircuit->myOscope->UpdateMenu();
							handled = true;
						}
					}
//				}
//				hit++;
//			}
		}
		
		// if in drag_connect, check for a highlighted hotspot to hook
		unsigned long targetGate = 0;
		string targetConnection;
		if (currentDragState == DRAG_CONNECT && hotspotHighlight.size()) { 
			// Set our target gate and hotspot, these are the objects currently pointed to by
			//	the cursor
			targetGate = hotspotGate;
			targetConnection = hotspotHighlight;
		}

		// if we have a target gate, search for the source
		if (targetConnection.size() > 0 || (drawWireHover && currentDragState == DRAG_CONNECT)) {
			// Check my drag start coordinate collisions
//			CollisionGroup potentialSources = dragselectbox->getOverlaps();
//			CollisionGroup::iterator hit = potentialSources.begin();
//			while( hit != potentialSources.end() && !handled) {
				// Source is hotspot and target is hotspot:
				//		if source is connected and target is connected, merge
				//		if source is connected or target is connected, connect
				//		if neither is connected then create a wire
				// Source is hotspot and target is wire:
				//		if source is connected then merge
				//		if source is not connected then connect
				// Source is wire and target is hotspot:
				//		if target is connected then merge
				//		if target is not connected then connect
				// Source is wire and target is wire:
				//		merge
//				if ((*hit)->getType() == COLL_GATE) { // Source is hotspot
				if ( currentConnectionSource.isGate ) {
					guiGate* hitGate = gateList[currentConnectionSource.objectID]; // ((guiGate*)(*hit));
					string startHS = currentConnectionSource.connection; //hitGate->checkHotspots( getDragStartCoords(BUTTON_LEFT).x, getDragStartCoords(BUTTON_LEFT).y, HOTSPOT_SCREEN_DELTA * getZoom() );
					//if (startHS == "") // Didn't find a source with this gate
					//	{ hit++; continue; }
					if (drawWireHover) { // Target is wire
						if (!(hitGate->isConnected(startHS))) // source is not connected, so connect
							gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdConnectWire ( gCircuit, wireHoverID, hitGate->getID(), startHS )) );
						else 								  // source is connected, so merge
							if (hitGate->getConnection(startHS)->getID() != wireHoverID) gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdMergeWire ( gCircuit, this, hitGate->getConnection(startHS)->getID(), wireHoverID, getSnappedPoint( m ) )) );
						handled = true;
					}
					else if (!drawWireHover && startHS.size() > 0 && !(startHS == targetConnection && hitGate->getID() == targetGate)) {	 // Target is hotspot
						if (!(hitGate->isConnected(startHS)) && !(gateList[targetGate]->isConnected(targetConnection))) { // Neither connected, so create wire
							long newWID = gCircuit->getNextAvailableWireID();
							cmdConnectWire* connectwire = new cmdConnectWire( gCircuit, newWID, hitGate->getID(), startHS );
							cmdConnectWire* connectwire2 = new cmdConnectWire( gCircuit, newWID, targetGate, targetConnection );
							gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdCreateWire ( this, gCircuit, newWID, connectwire, connectwire2 )) );
						}
						else if ((hitGate->isConnected(startHS)) && (gateList[targetGate]->isConnected(targetConnection))) { // Both connected, merge the wires
							if (hitGate->getConnection(startHS)->getID() != gateList[targetGate]->getConnection(targetConnection)->getID()) gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdMergeWire ( gCircuit, this, hitGate->getConnection(startHS)->getID(), gateList[targetGate]->getConnection(targetConnection)->getID(), getSnappedPoint( m ) )) );
						} else if ((hitGate->isConnected(startHS))) { // source is connected, so connect target to source's wire
							gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdConnectWire ( gCircuit, hitGate->getConnection(startHS)->getID(), targetGate, targetConnection )) );
						} else if ((gateList[targetGate]->isConnected(targetConnection))) {
							gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdConnectWire ( gCircuit, gateList[targetGate]->getConnection(targetConnection)->getID(), hitGate->getID(), startHS )) );						
						}
						handled = true;
					}
				}
//				else if ((*hit)->getType() == COLL_WIRE) {
				else {
					guiWire* hitWire = wireList[currentConnectionSource.objectID]; //((guiWire*)(*hit));
					if (hitWire->hover(getDragStartCoords(BUTTON_LEFT).x, getDragStartCoords(BUTTON_LEFT).y, WIRE_HOVER_SCREEN_DELTA * getZoom())) {
						if (!drawWireHover) { // Source is wire, target is hotspot
							// Source is gate but we're targeting a wire
							if (!(gateList[targetGate]->isConnected(targetConnection))) // target is not connected, so connect
								gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdConnectWire(gCircuit, hitWire->getID(), targetGate, targetConnection)) );
							else 														// target is connected, so merge
								if (hitWire->getID() != gateList[targetGate]->getConnection(targetConnection)->getID()) gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdMergeWire ( gCircuit, this, hitWire->getID(), gateList[targetGate]->getConnection(targetConnection)->getID(), getSnappedPoint( m ) )) );							
						} else { // Source is wire, target is wire, so merge
							if (hitWire->getID() != wireHoverID) gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdMergeWire(gCircuit, this, hitWire->getID(), wireHoverID, getSnappedPoint( m ))) );
						}
						handled = true;
					}
				}
//				hit++;
//			}
		}
		collisionChecker.update();
	}

	if ((currentDragState == DRAG_NEWGATE || currentDragState == DRAG_SELECTION) && (potentialConnectionHotspots.size() > 0)) {
		// Check potential hotspot connections (on gate/gate collisions)
		CollisionGroup ovrList = collisionChecker.overlaps[COLL_GATE];
		CollisionGroup::iterator obj = ovrList.begin();
		while( obj != ovrList.end() ) {
			CollisionGroup hitThings = (*obj)->getOverlaps();
			CollisionGroup::iterator hit = hitThings.begin();
			while( hit != hitThings.end() ) {
				// Only check gate collisions
				if ((*hit)->getType() != COLL_GATE) { hit++; continue; };
				// obj and hit are two overlapping gates
				//  get overlapping hotspots of obj in another group
				CollisionGroup hotspotOverlaps = (*obj)->checkSubsToSubs(*hit);
				CollisionGroup::iterator hotspotCollide = hotspotOverlaps.begin();
				while (hotspotCollide != hotspotOverlaps.end()) {
					// hotspotCollide is in obj; hsWalk is in hit
					CollisionGroup hshits = (*hotspotCollide)->getOverlaps();
					CollisionGroup::iterator hsWalk = hshits.begin();
					while (hsWalk != hshits.end()) {
						if ( !(((guiGate*)(*obj))->isConnected(((gateHotspot*)(*hotspotCollide))->name)) && !(((guiGate*)(*hit))->isConnected(((gateHotspot*)(*hsWalk))->name)) &&
							(((guiGate*)(*obj))->isSelected() || ((guiGate*)(*hit))->isSelected())) {
							// potential connection found, so hook it up
							long newWID = gCircuit->getNextAvailableWireID();
							cmdConnectWire* connectwire = new cmdConnectWire( gCircuit, newWID, ((guiGate*)(*obj))->getID(), ((gateHotspot*)(*hotspotCollide))->name );
							cmdConnectWire* connectwire2 = new cmdConnectWire(gCircuit, newWID, ((guiGate*)(*hit))->getID(), ((gateHotspot*)(*hsWalk))->name);
							cmdCreateWire* createwire = new cmdCreateWire( this, gCircuit, newWID, connectwire, connectwire2 );
							createwire->Do();
							//collisionChecker.update();
							if (currentDragState == DRAG_SELECTION) {
								if (movecommand == NULL) {
									movecommand = new cmdMoveSelection( gCircuit, preMove, preMoveWire, 0, 0, 0, 0 );
									if (!isWithinPaste) gCircuit->GetCommandProcessor()->Submit( (wxCommand*)movecommand );
								}
								movecommand->proxconnects.push_back(createwire);
							}
							else if (currentDragState == DRAG_NEWGATE) creategatecommand->proxconnects.push_back(createwire);
							else delete createwire;
						}
						hsWalk++;
					}
					hotspotCollide++;
				}
				hit++;
			}
			obj++;
		}
	}
	
	// Drop a paste block with the proper move coords
	if (isWithinPaste) {
		pasteCommand->addCommand( movecommand );
		gCircuit->GetCommandProcessor()->Submit( pasteCommand );
		isWithinPaste = false;
		autoScrollEnable(); // Re-enable auto scrolling
	}
	
	currentDragState = DRAG_NONE;
	
	Update();
}

void GUICanvas::OnMouseEnter(wxMouseEvent& event) {
	GLPoint2f m = getMouseCoords();

	// Do a collision detection on all first-level objects.
	// The map collisionChecker.overlaps now contains
	// all of the objects involved in any collisions.
	//collisionChecker.update();

	wxGetApp().showDragImage = false;
	if (event.LeftIsDown() && wxGetApp().newGateToDrag.size() > 0 && currentDragState == DRAG_NONE && !(this->isLocked())) {
		newDragGate = gCircuit->createGate(wxGetApp().newGateToDrag, -1);
		if (newDragGate == NULL) { wxGetApp().newGateToDrag = ""; return; }
		newDragGate->setGLcoords(m.x, m.y);
		//wxGetApp().logfile << m.x << " " << (panY-(y*viewZoom)) << endl << flush;
		currentDragState = DRAG_NEWGATE;
		wxGetApp().newGateToDrag = "";
		beginDrag( BUTTON_LEFT );
		unselectAllGates();
		newDragGate->select();
		collisionChecker.addObject( newDragGate );
	} else wxGetApp().newGateToDrag = "";
}


void GUICanvas::OnKeyDown(wxKeyEvent& event) {
	switch (event.GetKeyCode()) {
	case WXK_DELETE:
		if (currentDragState == DRAG_NONE && !(this->isLocked())) deleteSelection();
		break;
	case WXK_ESCAPE:
		unselectAllGates();
		unselectAllWires();
		if (preMove.size() > 0) {
			saveMove = false;
			for (unsigned int i = 0; i < preMove.size(); i++) {
				if (gateList.find(preMove[i].id) == gateList.end()) continue;
				gateList[preMove[i].id]->setGLcoords(preMove[i].x, preMove[i].y);
				if (preMove[i].selected) gateList[preMove[i].id]->select();
			}
			preMove.clear();
		}
		if (preMoveWire.size() > 0) {
			for (unsigned int i = 0; i < preMoveWire.size(); i++) {
				if (wireList.find(preMoveWire[i].id) == wireList.end()) continue;
				wireList[preMoveWire[i].id]->setSegmentMap(preMoveWire[i].oldWireTree);
				wireList[preMoveWire[i].id]->select();
			}
		}
		if (currentDragState == DRAG_NEWGATE) {
			gCircuit->getGates()->erase(newDragGate->getID());
			collisionChecker.removeObject( newDragGate );
			delete newDragGate;
			Update();
		}
		currentDragState = DRAG_NONE;
		endDrag(BUTTON_LEFT);
		Refresh();
		break;
	case WXK_LEFT:
	case WXK_NUMPAD_LEFT:
		translatePan(-PAN_STEP * getZoom(), 0.0);
		break;
	case WXK_RIGHT:
	case WXK_NUMPAD_RIGHT:
		translatePan(+PAN_STEP * getZoom(), 0.0);
		break;
	case WXK_UP:
	case WXK_NUMPAD_UP:
		translatePan(0.0, PAN_STEP * getZoom());
		break;
	case WXK_DOWN:
	case WXK_NUMPAD_DOWN:
		translatePan(0.0, -PAN_STEP * getZoom());
		break;
	case 43: // + key on top row
	case WXK_NUMPAD_ADD:
		zoomInOrOut( true );
		break;
	case 45: // - key on top row
	case WXK_NUMPAD_SUBTRACT:
		zoomInOrOut( false );
		break;
	case WXK_SPACE:
		setZoomAll();
		break;
	}
}

void GUICanvas::deleteSelection() {
	// whatever is in the selected vectors goes
	if (selectedWires.size() > 0 || selectedGates.size() > 0) gCircuit->GetCommandProcessor()->Submit( (wxCommand*)(new cmdDeleteSelection( gCircuit, this, selectedGates, selectedWires )) );
	selectedWires.clear();
	selectedGates.clear();
	preMove.clear();
	saveMove = false;

	// Do a collision detection on all first-level objects.
	// The map collisionChecker.overlaps now contains
	// all of the objects involved in any collisions.
	Update();
}

void GUICanvas::unselectAllGates() {
	hash_map < unsigned long, guiGate* >::iterator thisGate = gateList.begin();
	while (thisGate != gateList.end()) {
		(thisGate->second)->unselect();
		thisGate++;
	}
}

void GUICanvas::unselectAllWires() {
	hash_map < unsigned long, guiWire* >::iterator thisWire = wireList.begin();
	while (thisWire != wireList.end()) {
		(thisWire->second)->unselect();
		thisWire++;
	}
}	

void GUICanvas::copyBlockToClipboard () {
	klsClipboard myClipboard;
	// Ship the selected gates and wires out to the clipboard
	myClipboard.copyBlock( gCircuit, this, selectedGates, selectedWires );
}

void GUICanvas::pasteBlockFromClipboard () {
	if (this->isLocked()) return;
	
	klsClipboard myClipboard;
	pasteCommand = myClipboard.pasteBlock( gCircuit, this );
	if (pasteCommand == NULL) return;
	currentDragState = DRAG_SELECTION; // drag until dropped
	isWithinPaste = true;
	saveMove = true;
	
	// clean up the selected gates vector
	selectedGates.clear();
	preMove.clear();
	hash_map< unsigned long, guiGate* >::iterator thisGate = gateList.begin();
	unsigned long snapToGateID = 0;
	GLPoint2f gatecoord;
	// paste only to snapped point
	GLPoint2f mc = getSnappedPoint(getMouseCoords());
	GLPoint2f minPoint;
	bool ref = false;
	// Find top-left-most point
	while (thisGate != gateList.end()) {
		GLPoint2f temp;
		if ((thisGate->second)->isSelected()) {
			(thisGate->second)->getGLcoords(temp.x, temp.y);
			if (temp.x < minPoint.x || !ref) minPoint.x = temp.x;
			if (temp.y > minPoint.y || !ref) minPoint.y = temp.y;
			ref = true;
		}
		thisGate++;
	}
	ref = false;
	// Try to drag by the top-left-most gate
	double minMagnitude = 0.0;
	thisGate = gateList.begin();
	while (thisGate != gateList.end()) {
		GLPoint2f temp;
		if ((thisGate->second)->isSelected()) {
			if (ref) {
				(thisGate->second)->getGLcoords(temp.x, temp.y);
				float diffx = gatecoord.x - minPoint.x, diffy = gatecoord.y - minPoint.y;
				double newMag = (diffx * diffx) + (diffy * diffy);
				if (newMag < minMagnitude) {
					minMagnitude = newMag;
					gatecoord = temp;
					snapToGateID = (thisGate->first);
				}
			} else {
				(thisGate->second)->getGLcoords(gatecoord.x, gatecoord.y);
				float diffx = gatecoord.x - minPoint.x, diffy = gatecoord.y - minPoint.y;
				minMagnitude = (diffx * diffx) + (diffy * diffy);
				snapToGateID = (thisGate->first);
				ref = true;
			}
		}
		thisGate++;
	}

	// What is the difference between that gate and the mouse coords
	GLPoint2f diff( mc.x-gatecoord.x, mc.y-gatecoord.y );
	// Shift all the gates and track their differences by command
	thisGate = gateList.begin();
	while (thisGate != gateList.end()) {
		if ((thisGate->second)->isSelected()) {
			preMove.push_back(GateState((thisGate->first), 0, 0, (thisGate->second)->isSelected()));
//			(thisGate->second)->translateGLcoords(diff.x, diff.y);
			(thisGate->second)->getGLcoords(preMove[preMove.size()-1].x, preMove[preMove.size()-1].y);
			preMove[preMove.size()-1].x += diff.x; preMove[preMove.size()-1].y += diff.y;
			cmdMoveGate* mgcmd = new cmdMoveGate(gCircuit, (thisGate->first), preMove[preMove.size()-1].x-diff.x, preMove[preMove.size()-1].y-diff.y, preMove[preMove.size()-1].x, preMove[preMove.size()-1].y, true);
			mgcmd->Do();
			pasteCommand->addCommand( mgcmd );
			selectedGates.push_back((thisGate->first));
		}
		thisGate++;
	}
	// clean up the selected wires vector
	selectedWires.clear();
	preMoveWire.clear();
	hash_map< unsigned long, guiWire* >::iterator thisWire = wireList.begin();
	while (thisWire != wireList.end()) {
		if ((thisWire->second)->isSelected()) {
			// Push back the wire's id and set up a premove state
			cmdMoveWire* movewire = new cmdMoveWire(gCircuit, (thisWire->first), (thisWire->second)->getSegmentMap(), diff);
			movewire->Do();
			pasteCommand->addCommand(movewire);
			preMoveWire.push_back(WireState((thisWire->first), (thisWire->second)->getCenter(), (thisWire->second)->getSegmentMap()));
			selectedWires.push_back((thisWire->first));
		}
		thisWire++;
	} 

	autoScrollDisable();
	beginDrag(BUTTON_LEFT);
	
	Update();
}


// Zoom the canvas to fit all items within it:
void GUICanvas::setZoomAll( void ) {
// TODO: BUG this function sometimes hangs the program.
	klsBBox zoomBox;

	// Add all the gates into the zoom all box:
	hash_map< unsigned long, guiGate* >::iterator gateWalk = gateList.begin();
	while( gateWalk != gateList.end() ) {
		zoomBox.addBBox( (gateWalk->second)->getBBox() );
		gateWalk++;
	}

	// Add all the wires into the zoom all box:
	hash_map< unsigned long, guiWire* >::iterator wireWalk = wireList.begin();
	while( wireWalk != wireList.end() ) {
		zoomBox.addBBox( (wireWalk->second)->getBBox() );
		wireWalk++;
	}
	
	// Make sure to not have a dumb zoom factor on an empty canvas:
	if( gateList.empty() ) {
		zoomBox.addPoint(GLPoint2f(0, 0));
	}
	
	// Put some margin around the zoom box:
	zoomBox.extendTop( ZOOM_ALL_MARGIN );
	zoomBox.extendBottom( ZOOM_ALL_MARGIN );
	zoomBox.extendLeft( ZOOM_ALL_MARGIN );
	zoomBox.extendRight( ZOOM_ALL_MARGIN );

	// Zoom to the zoom-all box:
	setViewport( zoomBox.getTopLeft(), zoomBox.getBottomRight() );
}


// print page contents
void GUICanvas::printLists() {
	wxGetApp().logfile << "printing page lists" << endl << flush;
	hash_map< unsigned long, guiGate* >::iterator thisGate = gateList.begin();
	while (thisGate != gateList.end()) {
		float x, y;
		(thisGate->second)->getGLcoords(x, y);
		wxGetApp().logfile << " gate " << thisGate->first << " type " << (thisGate->second)->getLibraryGateName() << " at " << x << "," << y << endl << flush;
		thisGate++;
	}
	hash_map< unsigned long, guiWire* >::iterator thisWire = wireList.begin();
	while (thisWire != wireList.end()) {
		wxGetApp().logfile << " wire " << thisWire->first << endl << flush;
		thisWire++;
	}
}	

// Update the collision checker and refresh
void GUICanvas::Update() {
	if (minimap == NULL) return;

	// In case of resize, we should update every so often
	if (wxGetApp().appSystemTime->Time() > wxGetApp().appSettings.refreshRate) {
		wxGetApp().appSystemTime->Pause();
		if (gCircuit->panic) return;
		wxCriticalSectionLocker locker(wxGetApp().m_critsect);
		while (wxGetApp().mexMessages.TryLock() == wxMUTEX_BUSY) wxYield();
		while (wxGetApp().dLOGICtoGUI.size() > 0) {
			gCircuit->parseMessage(wxGetApp().dLOGICtoGUI.front());
			wxGetApp().dLOGICtoGUI.pop_front();
		}
		wxGetApp().mexMessages.Unlock();
wxGetApp().logfile << "stepping after " << wxGetApp().appSystemTime->Time() << "ms" << endl;
		if (gCircuit->panic) return;
		// Do function of number of milliseconds that passed since last step
		ostringstream oss;
		oss << "STEPSIM " << wxGetApp().appSystemTime->Time() / wxGetApp().timeStepMod;
		gCircuit->lastTime = wxGetApp().appSystemTime->Time();
		gCircuit->lastTimeMod = wxGetApp().timeStepMod;
		gCircuit->lastNumSteps = wxGetApp().appSystemTime->Time() / wxGetApp().timeStepMod;
		gCircuit->sendMessageToCore(oss.str());
		gCircuit->simulate = false;
		wxGetApp().appSystemTime->Start(wxGetApp().appSystemTime->Time() % wxGetApp().timeStepMod);
	}

	//collisionChecker.update();
	minimap->setLists( &gateList, &wireList );
	minimap->setCanvas(this);
	updateMiniMap();
	Refresh();
	wxWindow::Update();
}
