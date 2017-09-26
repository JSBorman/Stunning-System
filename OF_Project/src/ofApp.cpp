#include "ofApp.h"
#include "midi_button.cpp"

const int grid_size = 4;
float lineXPos = 0;
float lineSpeed = .4;
midi_button grid [grid_size][grid_size];
ofColor colorList[] = { ofColor::fromHex(0xff6600), ofColor::fromHex(0x83f52c) };
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetBackgroundAuto(false);

	initialize_board();

	//Needs to be at end of function
	ofSoundStreamSetup(2, 0, 48000, 512, 4);
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
	ofSetColor(ofColor::black);

	//Draw all the buttons in grid
	for (int i = 0; i < grid_size; i++){
		for(int j = 0; j < grid_size; j++){

			if (grid[i][j].isLinePassing(lineXPos)&& !grid[i][j].isOn) {
				//if the line is passing through
				ofSetColor(ofColor::blue);
			}
			else if (grid[i][j].isLinePassing(lineXPos) && grid[i][j].isOn) {

				ofSetColor(ofColor::red);
				int x = grid[i][j].x_pos;
				int y = grid[i][j].y_pos;
				int rad = grid[i][j].radius/2;
				int repeat = 3;
				//draw more circles
				while (repeat > 1) {
					grid[i][j].test(x + rad*repeat, y, rad / repeat);
					grid[i][j].test(x - rad*repeat, y, rad / repeat);
					grid[i][j].test(x , y - rad*repeat, rad / repeat);
					grid[i][j].test(x, y + rad*repeat, rad / repeat);
					grid[i][j].test(x + rad*repeat, y + rad*repeat, rad / repeat);
					grid[i][j].test(x - rad*repeat, y + rad*repeat, rad / repeat);
					grid[i][j].test(x + rad*repeat, y - rad*repeat, rad / repeat);
					grid[i][j].test(x + rad*repeat, y + rad*repeat, rad / repeat);
					grid[i][j].test(x + rad*repeat, y - rad*repeat, rad / repeat);
					grid[i][j].test(x - rad*repeat, y - rad*repeat, rad / repeat);
					grid[i][j].test(x - rad*repeat, y - rad*repeat, rad / repeat);
					grid[i][j].test(x - rad*repeat, y + rad*repeat, rad / repeat);

					repeat--;
				}

			}else if (grid[i][j].isOn) {
					//Draw active buttons as green
				ofSetColor(ofColor::green);
			}
			ofDrawCircle(grid[i][j].x_pos,
						 grid[i][j].y_pos,
						 grid[i][j].radius);
			ofSetColor(ofColor::black);
		}
	}
	lineXPos+=lineSpeed;
	if (lineXPos > 1024)
		lineXPos = 0;

}
//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawLine(lineXPos + 1, 0, lineXPos + 1, 768);//initial line
	ofDrawLine(lineXPos, 0, lineXPos, 768);			//2 more lines more thickness
	ofDrawLine(lineXPos - 1, 0, lineXPos - 1, 768);

}

void ofApp::audioOut(float* buffer, int bufferSize, int nChannels){
	for (int i = 0; i < bufferSize; i++){
		float currentSample = 0;

		buffer[i] = currentSample;
	}
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

	//Check if the user clicked inside a circle
	for(int i = 0; i < grid_size; i++){
		for (int j = 0; j < grid_size; j++){
			if (grid[i][j].isPointInCircle(x, y) ){
				grid[i][j].pressButton();
				return;
			}
		}
	}
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
