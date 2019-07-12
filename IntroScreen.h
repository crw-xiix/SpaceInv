#pragma once
#ifndef __IntroScreen_h__
#define __IntroScreen_h__ 1

#include "object.h"
#include <Adafruit_SSD1306.h>


class GameDisplay {
public:
	virtual void draw(Adafruit_SSD1306 &display);
	virtual int process(uint8_t input); //Returns next menu........
};


class IntroScreen : public GameDisplay {
	int ax = 5;
	int ay = 5;
	int avx = 1;
	bool flash = false;
public:
	void draw(Adafruit_SSD1306 &display) override;
	int process(uint8_t input) override;
	IntroScreen();
};


#endif