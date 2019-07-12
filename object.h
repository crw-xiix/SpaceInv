#ifndef __object_h__
#define __object_h__
#include <stdint.h>
#include <Adafruit_SSD1306.h>

const uint8_t JY_LEFT = 1;
const uint8_t JY_RIGHT = 2;
const uint8_t JY_BUT1 = 4;
const uint8_t JY_BUT2 = 8;


const uint8_t alien_1[] = { 
	0b01111100,
	0b11111110,
	0b11111110,
	0b01111100,
	0b11000110,
	0b11000110,
	0,0 };
const uint8_t alien_2[] = { 
	0b01111100,
	0b11010110,
	0b11111110,
	0b01111100,
	0b11000110,
	0b11000110,
	0,0 };
//It's longer for speed in calculating
const uint8_t shield[] = { 
	0b01111110,
	255,
	255,
	255,
	255,
	255,
	0b10000001,
	0,0,0,0};
const uint8_t ship[] = {
	0b00010000,
	0b00010000,
	0b00111000,
	0b00111000,
	0b10111010,
	0b11111110,
	0b11111110,
	0b01111100,
};

const uint8_t bullet[] = { 0b01000000,0b01000000,0b01000000,0 };

class Object {
public:
	bool alive = true;
	int x, y;
	int vy, vx;
	virtual void draw(Adafruit_SSD1306& display) = 0;
	virtual void move() = 0;
};


#endif
