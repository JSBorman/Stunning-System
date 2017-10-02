#include "ofApp.h"
#include "midi_button.cpp"

const int grid_size = 4;
float lineXPos = 0;
int left_margin = 100;
int top_margin = 100;

midi_button grid [grid_size][grid_size];
ofColor colorList[] = { ofColor::fromHex(0xff6600), ofColor::fromHex(0x83f52c) };

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetBackgroundAuto(false);

	initialize_board();
	gui.setup();
	gui.add(speed.setup("speed", .5, 0, 1));
	gui.add(frequency.setup("frequency", 0, -300, 300));


/*	//Background Masking
	stayAlivefbo.allocate(ofGetWidth()*2, ofGetHeight()*2);
    goAwayfbo.allocate(ofGetWidth()*2, ofGetHeight()*2);
    
    // Clear the FBO's
    stayAlivefbo.begin();
    ofClear(0,0,0,255);
    stayAlivefbo.end();
    
    goAwayfbo.begin();
    ofClear(0,0,0,255);
    goAwayfbo.end(); */

	//Audio
	frequencyFloat = 440;
	amplitude = 1;
	phase = 0;

	//Background
	for(int i = 0; i < 5; i++){
		timers[i] = 1.0 * i;
		elapsedTimers[i] = &timers[i];

		phases[i] = 5.0 * i;
		backgroundPhases[i] = &phases[i];
	}

	//Leave at end of function
	ofSoundStreamSetup(2, 0, 48000, 512, 4);
}

//Populate grid with buttons
//Each row of buttons should have the same tone
void ofApp::initialize_board(){
	int radius = 50;

	for (int i = 0; i < grid_size; i++){
		for(int j = 0; j < grid_size; j++){
			grid[i][j] = 
				midi_button(left_margin + (i*ofGetWidth()/grid_size),
					top_margin + (j*ofGetHeight()/grid_size),
					radius,
					frequencyFloat + (i*100));
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	//Ensure window size wasn't changed
	ofSetWindowShape(1024, 768);	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::darkMagenta);

	for(int i = 0; i < 5; i++){
		drawBackground(elapsedTimers[i], backgroundPhases[i]);
	}

	updateBoard();

	ofSetLineWidth(5);
	ofDrawLine(lineXPos, 0, lineXPos, 768);			//2 more lines more thickness

	gui.draw();
}

void ofApp::updateBoard(){
	ofSetColor(ofColor::black);
	//Draw all the buttons in grid
	for (int i = 0; i < grid_size; i++){
		for(int j = 0; j < grid_size; j++){

			if (grid[i][j].isLinePassing(lineXPos)&& !grid[i][j].isOn) {
				//if the line is passing through
				ofSetColor(ofColor::blue);
				grid[i][j].isPlaying = false;
			}
			else if (grid[i][j].isLinePassing(lineXPos) && grid[i][j].isOn) {

				grid[i][j].isPlaying = true;
				if (grid[i][j].resetTimeBuffer) {
					grid[i][j].timeBuffer = ofGetFrameNum();
					grid[i][j].resetTimeBuffer = false;
				}

				ofSetColor(ofColor::red);
				int x = grid[i][j].x_pos;
				int y = grid[i][j].y_pos;
				int rad = grid[i][j].radius/2;
				int repeat = 3;
				int frametime = 100 / speed;

				float div = ofGetFrameNum()- grid[i][j].timeBuffer;
				div = (frametime - div) / frametime;

				//draw more circles
				while (repeat > 1) {

					ofSetColor(ofColor::blanchedAlmond);
					ofDrawCircle(x, y - rad*repeat, rad*div);
					ofDrawCircle(x, y + rad*repeat, rad*div);

					ofSetColor(ofColor::darkGrey);
					ofDrawCircle(x + rad*repeat, y, rad*div);
					ofDrawCircle(x + rad*repeat, y + rad*repeat, rad*div);
					ofDrawCircle(x + rad*repeat, y - rad*repeat, rad*div);
					
					ofSetColor(ofColor::darkOrchid);
					ofDrawCircle(x - rad*repeat, y, rad *div);
					ofDrawCircle(x - rad*repeat, y - rad*repeat, rad *div);
					ofDrawCircle(x - rad*repeat, y + rad*repeat, rad *div);

					repeat--;
				} 
			}
			//Draw active buttons as green
			else if (grid[i][j].isOn) {
				ofSetColor(ofColor::green);
				grid[i][j].isPlaying = false;
				grid[i][j].resetTimeBuffer = true;
			}
			else {
				grid[i][j].resetTimeBuffer = true;
			}

			ofDrawCircle(grid[i][j].x_pos,
						 grid[i][j].y_pos,
						 grid[i][j].radius);
			
			ofSetColor(ofColor::black);	//Set back to black when done
			grid[i][j].freq = frequencyFloat + (i * 100);
		}
	}

	lineXPos+=speed;
	frequencyFloat = frequency+440;
	if (lineXPos > 1024)
		lineXPos = 0;
}

//Paints a sin wav across the screen
//How to persist when visual effects of buttons should clear?
void ofApp::drawBackground(float * elapsedTime, float * backgroundPhase){

//	ofLog(OF_LOG_NOTICE, "Pointer: %p", elapsedTime);
//	ofLog(OF_LOG_NOTICE, "Value: %f", *elapsedTime);

	*elapsedTime += 1.0;

	for(int i = 0; i < grid_size; i++){

		if(*elapsedTime > ofGetWidth()){
			*elapsedTime = 0;

			//*backgroundPhase = *backgroundPhase * ofRandom(0,10);
			*backgroundPhase = fmod(*backgroundPhase, TWO_PI);	
		}

		float lower_bound = (grid[i][i].radius * 2 * i) + (top_margin*i);
		float x = ofMap(*backgroundPhase, 0, TWO_PI, 0, ofGetWidth());

		for(int j = 0; j < grid_size - 2; j++){
			float y = ofMap(sin(*backgroundPhase), -1, 1, lower_bound + (j*top_margin/4), lower_bound + (grid[i][i].radius * 2) + (j*top_margin/4));

			if(ofRandom(0,100) > 80)
				ofSetColor( ofColor( ofRandom( 0, 255 ),  ofRandom( 0, 255 ), ofRandom( 0, 255 ) ));
			
			ofSetLineWidth(40); 
			
			//Don't draw lines offscreen
		//	if(x + *elapsedTime + 200 > ofGetWidth())
				ofDrawLine(x + 50, y + 50, x + ofRandom(-20, 100), y + ofRandom(-50, 100));

		//	else
		//		ofDrawLine(x + *elapsedTime, y, x + *elapsedTime + ofRandom(20, 250), y + ofRandom(-250, 250));
		}
	}
}

//check soundPlayer + audioSoundOutput
void ofApp::audioOut(float* buffer, int bufferSize, int nChannels){
	
	//Check each button for noise
	for(int i = 0; i < grid_size; i++)
		for (int j = 0; j < grid_size; j++){

			phaseIncrement = (TWO_PI * grid[i][j].freq)/ (float)48000;

			if (grid[i][j].isPlaying){
				//If there is a button, play it
				for (int i = 0; i < bufferSize; i++){
					float currentSample = 0;

					currentSample = sin(phase) * amplitude;
					phase += phaseIncrement;

					buffer[i*nChannels + 0] = currentSample;	//Left Channel
					buffer[i*nChannels + 1] = currentSample;	//Right Channel
				}
			}
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
