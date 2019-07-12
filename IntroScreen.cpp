#include "IntroScreen.h"
#include "WebLog.h"


void GameDisplay::draw(Adafruit_SSD1306 &display) {}
int GameDisplay::process(uint8_t input) { return 0; }



IntroScreen::IntroScreen() {
	//Nothing for now.......
	webLog.println("IntroStart");
	//slow it down some.
	
}

void IntroScreen::draw(Adafruit_SSD1306 &display) {
	//
	
	display.drawBitmap(ax, ay, alien_1, 8, 8, 1);
	
	display.setCursor(20, 20);

	display.setTextColor(WHITE);

	display.setTextSize(1);
	if (flash) display.println("Space Invaders");

	flash = !flash;

	for (int i = 1; i <= 4; i++) {
		int px = (i * 128)/5 - 4;
		display.drawBitmap(px, 40, shield, 8, 8, 1);
	}
	
	const char *stmsg = "Press Button To Start";
	int x = (128 - strlen(stmsg) * 6) >> 1;
	display.setCursor(x, 50);
	display.println("Press Button To Start");
	delay(20);
}


//True means exit menu
int IntroScreen::process(uint8_t input) {

	if (input & JY_LEFT) avx = -1;
	if (input & JY_RIGHT) avx = 1;
	if ((input & 3) == 0) avx = 0;

	if ((ax+avx) > (128 - 5 - 8)) avx = avx * -1;
	if ((ax+avx) < (0+5)) avx = avx * -1;
	ax+=avx;
	if (input & JY_BUT1) return 2;
	return 0;
};
