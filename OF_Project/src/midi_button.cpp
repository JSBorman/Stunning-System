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
		isOn = false;
	}

	//Turn on if off & vice-versa
	void pressButton(){
		isOn = !isOn;
	}

	//Determine if distance between given point & center of circle is smaller than r
	bool isPointInCircle(int mouse_x, int mouse_y){
		int distanceSquared = (x_pos - mouse_x) * (x_pos - mouse_x) + (y_pos - mouse_y) * (y_pos - mouse_y);
		return distanceSquared <= radius*radius;
	}

	//Checks to see if the line is passing through
	bool isLinePassing(int x) {
		return ((x_pos-radius)<x && (x_pos + radius)>x);
	}

	void test(int x, int y, int rad) {
		ofDrawCircle(x, y, rad);
	}


};