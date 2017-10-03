#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void initialize_board();	//Draw the base midi board
		void drawBackground(float * elapsedTime);		//Draw that cool sin-y stuff
		void updateBoard();			//Draw the board as line collides, etc.

		void audioOut(float* buffer, int bufferSize, int nChannels);
		void mousePressed(int x, int y, int button);
		

		//Audio Vars
		float frequencyFloat;
		float phase;
		float amplitude;
		float phaseIncrement;

		//Gui Vars
		ofxFloatSlider speed;
		ofxFloatSlider frequency;
		ofxPanel gui;

		//Background Vars
		float timers[8];
		float* elapsedTimers[8];
};
