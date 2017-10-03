#include "ofApp.h"
#include "midi_button.cpp"

const int grid_size = 4;
float lineXPos = 0;
int left_margin = 100;
int top_margin = 100;
int background_size = 7;

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

	//Audio
	frequencyFloat = 440;
	amplitude = 1;
	phase = 0;

	//Background
	for(int i = 0; i < background_size; i++){
		timers[i] = i;
		elapsedTimers[i] = &timers[i];
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

	for(int i = 0; i < background_size-1; i++){
		drawBackground(elapsedTimers[i]);
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
				ofSetColor(ofColor::mediumPurple);
				grid[i][j].isPlaying = false;
			}
			else if (grid[i][j].isLinePassing(lineXPos) && grid[i][j].isOn) {

				grid[i][j].isPlaying = true;
				if (grid[i][j].resetTimeBuffer) {
					grid[i][j].timeBuffer = ofGetFrameNum();
					grid[i][j].resetTimeBuffer = false;
				}

				int x = grid[i][j].x_pos;
				int y = grid[i][j].y_pos;
				int rad = grid[i][j].radius/2;
				int repeat = 3;
				int frametime = 100 / speed;

				float div = ofGetFrameNum()- grid[i][j].timeBuffer;
				div = (frametime - div) / frametime;

				//draw more circles
				while (repeat > 1) {

					ofSetColor(ofColor::lightBlue);
					ofDrawCircle(x, y - rad*repeat, rad*div);
					ofDrawCircle(x, y + rad*repeat, rad*div);

					ofSetColor(ofColor::lightBlue);
					ofDrawCircle(x + rad*repeat, y, rad*div);
					ofDrawCircle(x + rad*repeat, y + rad*repeat, rad*div);
					ofDrawCircle(x + rad*repeat, y - rad*repeat, rad*div);
					
					ofSetColor(ofColor::lightBlue);
					ofDrawCircle(x - rad*repeat, y, rad *div);
					ofDrawCircle(x - rad*repeat, y - rad*repeat, rad *div);
					ofDrawCircle(x - rad*repeat, y + rad*repeat, rad *div);

					ofSetColor(ofColor::midnightBlue);

					repeat--;
				} 
			}
			//Draw active buttons as green
			else if (grid[i][j].isOn) {
				ofSetColor(ofColor::skyBlue);
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

//Paints sin waves across the screen
void ofApp::drawBackground(float * elapsedTime){

	*elapsedTime += (1+speed) * ofGetLastFrameTime();	//Controls speed, based on framerate

	for(int i = 0; i < background_size; i++){

		if(*elapsedTime > 6.3){
			*elapsedTime = 0;
		}

		float lower_bound = (grid[0][0].radius * 2 * i) + (top_margin*i/2);

		//Base the motion on the change in time
		float new_y_value = fmod(*elapsedTime, TWO_PI);
		float y = ofMap(new_y_value, 0, TWO_PI, 0, ofGetHeight());

		int colFreq = fmod(i, grid_size);
		float grid_Freq = grid[colFreq][0].freq;
		grid_Freq = grid_Freq/TWO_PI/2;
		
		float sin_value = .5 * sin(y / grid_Freq);
		float x = ofMap( sin_value, -1, 1, lower_bound, lower_bound + (grid[0][0].radius * 2) );

		ofSetColor(ofColor::lightSteelBlue);
		ofDrawCircle(x, y, 10);
	}
}

//check soundPlayer + audioSoundOutput
void ofApp::audioOut(float* buffer, int bufferSize, int nChannels){
	
	//Check each button for noise
	for(int i = 0; i < grid_size; i++)
		for (int j = 0; j < grid_size; j++){

			phaseIncrement = (TWO_PI * grid[i][j].freq)/ (float)48000;
			cout << "phase: "<<phaseIncrement << endl;

				//If there is a button, play it
			if (grid[i][j].isPlaying){
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

void ofApp:: exit() {
	ofSoundStreamClose();
}