#include "ofMain.h"
#include "ofApp.h"

class midi_button{

public: 
	int x_pos;
	int y_pos;
	int radius;
	bool isOn = false;
	//Add audio file

	midi_button(){}
	midi_button(int x, int y, int r){
		x_pos  = x;
		y_pos  = y;
		radius = r;
	}

	//Turn on if off & vice-versa
	void pressButton(){
		isOn = !isOn;
	}

};