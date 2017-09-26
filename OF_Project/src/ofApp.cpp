#include "ofApp.h"
#include "midi_button.cpp"

const int grid_size = 4;
midi_button grid [grid_size][grid_size];
//--------------------------------------------------------------
void ofApp::setup(){
	//size of convas or window
	// proportional size of square
	//outline only, no fill

	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetBackgroundAuto(false);
	initialize_board();
}

//Populate grid with buttons
void ofApp::initialize_board(){
	int radius = 50;
	int left_margin = 100;
	int top_margin = 100;
	for (int i = 0; i < grid_size; i++){
		for(int j = 0; j < grid_size; j++){
			grid[i][j] = midi_button(left_margin + (i*ofGetWidth()/grid_size),
									top_margin + (j*ofGetHeight()/grid_size),
									radius);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(ofColor::white);
	ofSetColor(ofColor::green);

	//Draw all the buttons in grid
	for (int i = 0; i < grid_size; i++){
		for(int j = 0; j < grid_size; j++){
			ofDrawCircle(grid[i][j].x_pos,
						 grid[i][j].y_pos,
						 grid[i][j].radius);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
