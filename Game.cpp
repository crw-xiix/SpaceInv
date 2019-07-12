#include "Game.h"
#include "WebLog.h"


GSpaceInv::GSpaceInv() {
	Init();
}

void GSpaceInv::Init() {
	int i;
	int score = 0;
	for (i = 0; i < 4; i++) {
		shields[i] = OShield();
		int px = ((i+1) * 128) / 5 - 4;
		shields[i].Init(px, 44, shield);
		}
	// 4 rows of 8 of the lil bastards
	mover.vx = 1;
	for (i = 0; i < numAliens; i++) {
		aliens[i] = OAlien();
		int row = i / 8;
		int col = i % 8;
		int px = 14 + (col * 100) / 9;
		int py = row*8 + 2;
		aliens[i].Init(px, py);
	
	}
	for (i = 0; i < 20; i++) {
		alienBullets[i].alive = false;
	}
	for (i = 0; i < 10; i++) {
		shipBullets[i].alive = false;
	}
	ship.x = 60;
	ship.y = 55;
}



void GSpaceInv::draw(Adafruit_SSD1306 &display) {
	int i;
	for (i = 0; i < 4; i++) {
		if (shields[i].alive) shields[i].draw(display);
	}
	for (i = 0; i < numAliens; i++) {
		if (aliens[i].alive) aliens[i].draw(display);
	}
	for (i = 0; i < numShipBullets; i++) {
		if (shipBullets[i].alive) shipBullets[i].draw(display);
	}
	for (i = 0; i < numAlienBullets; i++) {
		if (alienBullets[i].alive) alienBullets[i].draw(display);
	}

	if (ship.alive) ship.draw(display);
}

void GSpaceInv::moveAliens() {
	bool left = false;
	bool right = false;
	for (int i = 0; i < numAliens; i++) {
		if (!aliens[i].alive) continue;
		aliens[i].x += mover.vx;
		
		if (aliens[i].x > (127 - 7 -1)) right = true;
		if (aliens[i].x < (1)) left = true;
	}

	if (left) mover.vx = 1;
	if (right) mover.vx = -1;
	if (left || right) {
		for (int i = 0; i <numAliens; i++) {
			aliens[i].moveDown();
		}
	}
	

}

int GSpaceInv::process(uint8_t jy) {
	ship.vx = 0;
	frameCount++;
	if (jy&JY_LEFT) ship.vx = -2;
	if (jy&JY_RIGHT) ship.vx = +2;
	if (jy&JY_BUT1) {
		ShipShoot(ship.x + 2, ship.y);
	}
	int aCount = 0;
	for (int j = 0; j < numAliens; j++) {
		if (!aliens[j].alive) continue;
		aCount++;
	}
	if (aCount == 0) return 3;

	ship.move();
	moveAliens();
	for (int i = 0; i < numShipBullets; i++) {
		if (shipBullets[i].alive) {
			shipBullets[i].move();
			//Let see if we hit anything.
			for (int j = 0; j < numAliens; j++) {
				if (!aliens[j].alive) continue;
				if ((shipBullets[i].x > aliens[j].x) &&
					(shipBullets[i].x < (aliens[j].x + 7)) &&
					(shipBullets[i].y > aliens[j].y) &&
					(shipBullets[i].y < (aliens[j].y + 7))) {
					score += 50;
					aliens[j].alive = false;
					shipBullets[i].alive = false;
				}
			}
			for (int j = 0; j < 4; j++) {
				if ((shipBullets[i].x+2 > shields[j].x) &&
					(shipBullets[i].x <= (shields[j].x + 7)) &&
					(shipBullets[i].y+2 > shields[j].y) &&
					(shipBullets[i].y <= (shields[j].y + 7))) {
					if (shields[j].doHit(shipBullets[i].x, shipBullets[i].y)) {
						shipBullets[i].alive = false;
					}
				}

			}
		}
	}
	for (int i = 0; i < numAlienBullets; i++) {
		if (alienBullets[i].alive) {
			alienBullets[i].move();
			//Let see if we hit anything.
			if ((alienBullets[i].x + 1 > ship.x) &&
				(alienBullets[i].x <= (ship.x + 5)) &&
				(alienBullets[i].y + 2 > ship.y) &&
				(alienBullets[i].y <= (ship.y + 6))) {
				return 3;
			}
			for (int j = 0; j < 4; j++) {
				if ((alienBullets[i].x + 2 > shields[j].x) &&
					(alienBullets[i].x <= (shields[j].x + 7)) &&
					(alienBullets[i].y + 2 > shields[j].y) &&
					(alienBullets[i].y <= (shields[j].y + 7))) {
					if (shields[j].doHit(alienBullets[i].x, alienBullets[i].y)) {
						alienBullets[i].alive = false;
					}
				}

			}
		}
	}

	if ((frameCount % 10) == 2) {
		AlienShoot();
	}
	return 0;
}


void GSpaceInv::AlienShoot() {
	int cols[] = { -1,-1,-1,-1,-1,-1,-1,-1 };
	int col;
	//Find the bottom most alien
	for (int i = 0; i < numAliens; i++) {
		col = i % 8;
		if (aliens[i].alive) {
			if (i > cols[col]) cols[col] = i;
		}
	}
	col = rand();
	char buffer[80];
	for (;;) {
		col = col % 8;
		if (cols[col] >= 0) {
			for (int i=0; i<numAlienBullets; i++) {
				if (!alienBullets[i].alive) {
					alienBullets[i].Init(aliens[cols[col]].x+3, aliens[cols[col]].y + 7 , 2);
					sprintf(buffer, "SHoot#%d  col:  %d", cols[col], col);
					webLog.println(buffer);
					return;
				}
			}
		}
		col++;

	}
}


void GSpaceInv::ShipShoot(int x, int y) {
	char buffer[80];
	int i = 0;
	for (i < 0; i < numShipBullets; i++) {
		if (!shipBullets[i].alive) {
			shipBullets[i].Init(x, y, -3);
			sprintf(buffer, "Shot: %d,%d,%d", x, y, -2);
			webLog.println(buffer);
			return;
		}
	}
}



/******************************Game over *********************/
GameOver::GameOver(int sc) {
	score = sc;
}

int GameOver::process(uint8_t input) {
	if (input & JY_BUT1) return 1;
	return 0;
}

void GameOver::draw(Adafruit_SSD1306 &display) {
	char buffer[80];
	display.drawBitmap(5, 5, alien_1, 8, 8, 1);

	display.setCursor(20, 20);
	display.setTextColor(WHITE);

	display.setTextSize(1);
	display.println("Space Invaders");


	sprintf(buffer, "Score: %d", score);
	int px = (128 - strlen(buffer) * 6) >> 1;
	display.setCursor(px, 34);
	display.println(buffer);

	const char *stmsg = "Press Button";
	px = (128 - strlen(stmsg) * 6) >> 1;
	display.setCursor(px, 50);
	display.println(stmsg);

}









/*
*****************************   OBJECTS (((********************************************************
*/


void OShield::Init(int ix, int iy, const uint8_t ibitmap[]) {
	x = ix;
	y = iy;
	memcpy(bitmap, ibitmap, sizeof(bitmap));
}

void OShield::move() {
}

void OShield::draw(Adafruit_SSD1306& display) {
	display.drawBitmap(x, y, bitmap, 8, 8, 1);
}

bool OShield::doHit(int bx, int by) {
	char buffer[40];
	bool hit = false;
	uint8_t area = 0;
	//start blank
	int dx = bx - x;
	int dy = by - y;
	if (dx < 0) return false;
	if (dx >= 7) return false;
	//12 here
	for (int i = 0; i < 3; i++) {
		int idy = i + dy;
		if (idy < 0) continue;
		uint8_t	temp = ((dx == 0) ? 0xc0 : 0xe0) >> dx;

		if (temp & bitmap[idy]) { //we hit
			hit = true;
		}
//		sprintf(buffer, "%02X-%02X", temp, bitmap[idy]);
//		webLog.println(buffer);
		bitmap[idy] &= ~temp;
		
	}
	area = 0;
	for (int i = 0; i < 7; i++) area |= bitmap[i];

	sprintf(buffer, "area:%02X %02X", area,0x80>>dx);
	webLog.println(buffer);
	if (area == 0) alive = false;
	return hit;
}



	
	






///----------------------BUllet


void OBullet::draw(Adafruit_SSD1306& display) {
	display.drawBitmap(x, y, bullet, 4, 4, 1);
}

void OBullet::move() {

	y = y + vy;
	if ((y < 0) || (y >= 128)) alive = false;
}

void OBullet::Init(int ix, int iy, int ivy) {
	x = ix; y = iy; vy = ivy;

	alive = true;
}


///-----------------------  SHIP

void OShip::draw(Adafruit_SSD1306& display) {
	display.drawBitmap(x, y, ship, 8, 8, 1);
}

void OShip::move() {
	x = x + vx;
	if (x < 5) x = 5;
	if (x > (128 - 7 - 5)) x = 128 - 7 - 5;
}

void OShip::Init(int ix, int iy) {
	x = ix; y = iy;
	lives = 3;
}

///---------------------- ALIEN

void OAlien::draw(Adafruit_SSD1306& display) {
	display.drawBitmap(x, y, alien_1, 8, 8, 1);
}

void OAlien::move() {
	x = x + vx;

}

void OAlien::moveDown() {
	y++;
}
void OAlien::Init(int ix, int iy) {
	x = ix; y = iy; 
}


