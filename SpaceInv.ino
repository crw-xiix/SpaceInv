#include <Wire.h>
#include <ESP8266WiFi.h>
#include <time.h>

#include <ArduinoOTA.h>

//Led section
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



//Time Day and internet time
#include "nettime.h"
//OTA actions
#include "OTASetup.h"

//On webpage action log
#include "WebLog.h"

//Input/Output pins
#include "output.h"

//Config options
#include "passwords.h"

//Game stuff
#include "IntroScreen.h"

#include "Game.h"

extern "C" {
#include <user_interface.h>
}

IPAddress ip(DEVICE_IP); // where .xxx is the desired IP Address
IPAddress gateway(DEVICE_GATEWAY); // set gateway to match your network
IPAddress subnet(DEVICE_SUBNET); // set subnet mask to match your
IPAddress dns1(DEVICE_DNS1);
IPAddress dns2(DEVICE_DNS2);


WiFiServer server(80);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char asdfasd[40];

//Our Screens
IntroScreen introScreen;
GSpaceInv theGame;
GameOver gameOver;

GameDisplay *disp;




uint8 volatile action = 0;
int volatile aPos = 0;

void ICACHE_RAM_ATTR ISR_Encoder() {
	if (digitalRead(D3)) {
		aPos--;
	}
	else {
		aPos++;
	}
}

void setup()
{
	char buffer[80];
	byte error, address;
	int nDevices;


	// Show initial display buffer contents on the screen --
	// the library initializes this with an Adafruit splash screen.

	pinMode(D4, OUTPUT);
	pinMode(D3, INPUT_PULLUP);
	pinMode(D7, INPUT_PULLUP);
	pinMode(D0, INPUT_PULLUP);

	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3D for 128x64
		webLog.println("SSD1306 allocation failed");
		//for (;;); // Don't proceed, loop forever
	}
	else {
		webLog.println("lcd screen started");
		// Clear the buffer
		display.clearDisplay();
		display.setCursor(0, 0);
		display.setTextSize(1);
		display.setTextColor(1);
		display.println("Loading...");
		display.display();
	}

	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	WiFi.hostname(wifiHostName);

	WiFi.config(ip, gateway, subnet, dns1, dns2);
	WiFi.begin(wifiNetwork, wifiPassword);

	while (WiFi.status() != WL_CONNECTED) {
		digitalWrite(D4, LOW);
		delay(125);
		digitalWrite(D4, HIGH);
		delay(125);
		digitalWrite(D4, LOW);
		delay(125);
		digitalWrite(D4, HIGH);
		delay(125);
	}



	SetupOTA("TempSend-ESP8235");


	// Start the server
	server.begin();
	webLog.println("Server started:");
	webLog.println(WiFi.localIP().toString().c_str());
	netTime.Init(DEVICE_TZ);
//	lastTime = millis() - 5000;
	pinMode(D4, INPUT_PULLUP);
	introScreen = IntroScreen();
	theGame = GSpaceInv();
	display.clearDisplay();
	display.setCursor(0, 0);
	display.setTextColor(1);
	display.setTextSize(1);
	display.println("OTA time...");
	display.display();
	for (int i = 0; i < 20; i++) {
		ArduinoOTA.handle();
		delay(50);
	}

	attachInterrupt(digitalPinToInterrupt(D4), ISR_Encoder, FALLING);
	disp = &introScreen;
}

WiFiClient *tempClient = NULL;


uint8_t lastD3 = 1;
uint8_t lastD7 = 1;
uint8_t lastD0 = 1;
bool first = true;

int lastShot = 5;
void loop() {
	char buffer[80];
	//For reprogramming
	netTime.process();
	ArduinoOTA.handle();
	//Lets us know the program is running.......
//	digitalWrite(D4, !digitalRead(D4));
	uint8_t d3i = digitalRead(D3);
	uint8_t d4i = digitalRead(D4);
	uint8_t d7i = digitalRead(D7);
	uint8_t d0i = digitalRead(D0);

	//Rotary encoder....
	action = 0;
	if (aPos < 0) {
		action = JY_LEFT;
		aPos++;
	}
	if (aPos >0) {
		action = JY_RIGHT;
		aPos--;
		}
	if (d0i != lastD0) {
		if (!d0i) {
			if (lastShot == 0) {
				action |= JY_BUT1;
				lastShot = 5;
			}
		}
		lastD0 = d0i;
	}

	if (d7i != lastD7) {
		if (!d7i) {
			if (lastShot == 0) {
				action |= JY_BUT1;
				lastShot = 5;
			}
		}
		lastD7 = d7i;
	}
	lastShot--;
	if (lastShot < 0) lastShot = 0;
	display.clearDisplay();

	int nextMenu = disp->process(action);
	if (nextMenu != 0) {
		switch (nextMenu) {
		case 1: disp = &introScreen; break;
		case 2: {
			theGame = GSpaceInv();
			disp = &theGame;
			break;
		}
		case 3: {
			gameOver = GameOver(theGame.score);
			disp = &gameOver;
			break;
		}
		}
	}
	disp->draw(display);
	display.display();
	ESP.wdtFeed();
	
	//Loop out if we lose WiFi
	while (WiFi.status() != WL_CONNECTED) {
		delay(50);
	}
	/* Everything else after this is the website, so all fucntions have to be above*/

	WiFiClient client = server.available();
	
	int lps = 0;
	if (client == NULL) return;
	// Wait until the client sends some data

	
	while (client.connected() && !client.available())
	{
		delay(1);
		lps++;
		if (lps > 200) return;  //Skip wifi stuff.
	}


	//Bail out, something failed.
	if (!client.connected()) return;


	// Read the first line of the request
	char request[160];
	size_t bytes = client.readBytesUntil('\r', request, 160); // as readBytes with terminator character
															  //	String request = client.readStringUntil('\r');

	client.flush();
	if (!client.connected()) return;

	// Match the request
	int value = LOW;
	printHeader(client, "text/html");
	client.println("html");
	
	tempClient = &client;
	webLog.PrintReverse(clientPrint);
	
	tempClient = NULL;
	client.flush();
	
}

void website_log(WiFiClient c) {
	tempClient = &c;
	printHeader(c, "text/html");
	webLog.PrintReverse([](const char *d) {
		tempClient->println(d);
	});
	tempClient = NULL;
}

///ctype:  application/json : text/html 
void printHeader(WiFiClient client, const char *ctype) {
	//application / json
	client.println(("HTTP/1.1 200 OK"));
	client.print(("Content-Type: "));
	client.print(ctype);
	client.println(F("; charset = UTF-8"));
	client.println("");
}

void clientPrint(const char *data) {
	tempClient->println(data);
}


