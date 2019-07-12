#pragma once
#include "object.h"
#ifndef __Game_h__
#define __Game_h__ 1
#include <Adafruit_SSD1306.h>
#include "IntroScreen.h"

class OBullet : public Object{
public:
	OBullet() = default;
	void draw(Adafruit_SSD1306& display) override;
	void move() override;
	void Init(int x, int y, int vy);
};

class OShield : public Object {
public:
	uint8_t bitmap[8];  //Get degraded
	void draw(Adafruit_SSD1306& display) override;
	void move() override;
	void Init(int ix, int iy, const uint8_t ibitmap[]);
	bool doHit(int x, int y);
};

class OShip : public Object {
public:
	int lives = 3;
	void draw(Adafruit_SSD1306& display) override;
	void move() override;
	void Init(int ix, int iy); 
};

class OAlien : public Object {
public:
	int speed = 1;
private:
	int clk;
public:
	void draw(Adafruit_SSD1306& display) override;
	void move() override;
	void moveDown();
	void Init(int ix, int iy);
};

class GameOver : public GameDisplay {
	int score;
public:
	GameOver() = default;

	GameOver(int sc);
	int process(uint8_t input) override;
	void draw(Adafruit_SSD1306 &display) override;
};


class GSpaceInv : public GameDisplay {
	OShield shields[4];
	OAlien aliens[32];
	OShip   ship;
	static const int numShipBullets = 10;
	static const int numAlienBullets = 20;
	static const int numAliens = 24;
	OBullet alienBullets[numAlienBullets];
	OBullet shipBullets[numShipBullets];
	OAlien mover;
	int frameCount = 0;
public:
	int score = 0;
	GSpaceInv();
	void Init();
	int process(uint8_t input) override;
	void draw(Adafruit_SSD1306 &display) override;
	void ShipShoot(int x, int y);
	void moveAliens();
	void AlienShoot();
public:

};

#endif