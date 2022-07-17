#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>

//320*240

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

void setup()
{
	Serial.begin(115200);
	tft.init();
	tft.setRotation(3);
	tft.fillScreen(TFT_BLACK);
	tft.setTextColor(TFT_RED);
	tft.drawString("HELLO WORLD", 25, 25);
	sprite.createSprite(tft.width(), tft.height());
}

void loop()
{
	sprite.setTextColor(TFT_WHITE);
	sprite.setTextSize(20);
	sprite.drawString("HELLO WORLD", 25, 25);

	sprite.pushSprite(0,0);
	sprite.fillSprite(TFT_BLACK);
}