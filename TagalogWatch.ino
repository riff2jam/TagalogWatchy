///////////////////////////
//
// Adapted from Brendan Sleight's original code at https://github.com/bmsleight/Exactly-Words/blob/main/Exactly-Words.ino
// 
///////////////////////////
#include <Watchy.h> //include the Watchy library

#include "Fonts/Roboto_Medium14pt7b.h" //include any fonts you want to use
#include "Fonts/Roboto_Medium7pt7b.h" //include any fonts you want to use

RTC_DATA_ATTR int stored_oldsteps[7] ;

class WatchFace: public Watchy { //inherit and extend Watchy class
	public:
    	void drawRightAligned(String text, byte x, byte y) {
      		int16_t x1, y1;
      		uint16_t w1, h1;
      		display.getTextBounds(text, 0, 0, & x1, & y1, & w1, & h1);
      		display.setCursor(x - w1, y);
      		display.print(text);
    	}

  		void drawWrapCentred(String text, byte centerx, byte centery, byte bx, byte yspacing) {
    		// Only one space - no verylongwordthatdoesnotrwap - no carriage returns
    		const byte maxlines = 20;
    		byte line = 0;
    		int startnewlinepos[maxlines + 1];
    		int stringpos = 0;
    		int nextspace = 0;
    		int lastspace = 0;
    		String linetext;

    		int16_t x1, y1, cx, cy;
    		uint16_t w1, h1;

    		display.setTextWrap(false); // Needs to test if long - so we do not want auto wrapping

    		startnewlinepos[line] = 0;
    		while (stringpos < text.length()) {
      			nextspace = text.indexOf(' ', stringpos);
      			if (nextspace == -1) nextspace = text.length();
      			// Careful will wrap on letter so say 171 will be max if next char is
      			display.getTextBounds(text.substring(startnewlinepos[line], nextspace), 0, 0, & x1, & y1, & w1, & h1);
      			if (w1 > bx) {
        			// wider at next space
        			line++;
        			startnewlinepos[line] = lastspace;
        			stringpos = lastspace + 1;
      			} else {
        			lastspace = nextspace;
        			stringpos = nextspace + 1;
      			}
    		}
    		startnewlinepos[line + 1] = text.length();

    		for (int i = 0; i <= line; i++) {
      			linetext = text.substring(startnewlinepos[i], startnewlinepos[i + 1]);
      			display.getTextBounds(linetext, 0, 0, & x1, & y1, & w1, & h1);
      			cx = centerx - w1 / 2;
      			cy = centery - (line * (yspacing / 2)) + (i * yspacing) + (yspacing / 3);
      			display.setCursor(cx, cy);
      			display.print(linetext);
    		}
  		}

  		// Used https://github.com/peerdavid/Watchy/blob/master/examples/WatchFaces/David/Watchy_Base.cpp
  		uint8_t getBattery() {
			float voltage = getBatteryVoltage() + 0.25;
			uint8_t percentage = 2808.3808 * pow(voltage, 4) -
		  		43560.9157 * pow(voltage, 3) +
		  		252848.5888 * pow(voltage, 2) -
		  		650767.4615 * voltage +
		  		626532.5703;
			percentage = min((uint8_t) 100, percentage);
			percentage = max((uint8_t) 0, percentage);
			percentage = (uint8_t)floor(percentage/10);
			return percentage;
  		}

    	int getMinuteType(int currentTimeMinute) {
        	if (currentTimeMinute>=45)
            	return 2;
			    if (currentTimeMinute > 0)
            	return 1;
        	return 0;
    	}

    	int getPartOfDay(int currentTimeHour) {
			  // 0am - Hatinggabi
			  // 1am<=t<=4am - Madaling araw
			  // 5am<=t<=11am - Umaga
			  // 12pm - Tanghali
			  // 1pm<=t<=5pm - Hapon
			  // 6pm<=t<=11pm - Gabi

			  if (currentTimeHour==0)
				  return 0;
			  else if (currentTimeHour >= 1 && currentTimeHour <= 4)
				  return 1;
			  else if (currentTimeHour >= 5 && currentTimeHour <= 11)
				  return 2;
			  else if (currentTimeHour==12)
				  return 3;
			  else if (currentTimeHour >= 13 && currentTimeHour <= 17)
				  return 4;

			  return 5;
		  }

  		void drawWatchFace() { //override this method to customize how the watch face looks
    		// Using Logic in https://github.com/bmsleight/wasp-os/blob/master/wasp/apps/word_clock.py
 
			String araw[7] = {
					"Linggo","Lunes","Martes","Miyerkoles","Huwebes","Biyernes","Sabado"};
			String buwan[12] = {
					"Enero","Pebrero","Marso","Abril",
					"Mayo","Hunyo","Hulyo","Agosto",
					"Setyembre","Oktubre","Nobyembre","Disyembre"
			};
			String ampm[6] = {
					"","ng madaling araw","ng umaga","ng tanghali","ng hapon","ng gabi"
			};
			String oras[13] = {
					"Hatinggabi",
					"Ala una","Alas dos","Alas tres","Alas kwatro",
					"Alas singko","Alas sais","Alas siete","Alas otso",
					"Alas nueve","Alas dies","Alas onse","Alas dose"
			};
			String bilang[61] = {
					"Isang","Isang","Dalawang","Tatlong","Apat na","Limang", "Anim na","Pitong","Walong","Siyam na","Sampung",
					"Labing isang", "Labing dalawang", "Labing tatlong", "Labing apat na", "Labing limang",
					"Labing anim na","Labing pitong","Labing walong","Labing siyam na",
					"Dalawampung","Dalawampu't isang","Dalawampu't dalawang","Dalawampu't tatlong","Dalawampu't apat na",
					"Dalawampu't limang","Dalawampu't anim na","Dalawampu't pitong","Dalawampu't walong","Dalawampu't siyam na",
					"Tatlumpung","Tatlumpu't isang","Tatlumpu't dalawang","Tatlumpu't tatlong","Tatlumpu't apat na",
					"Tatlumpu't limang","Tatlumpu't anim na","Tatlumpu't pitong","Tatlumpu't walong","Tatlumpu't siyam na",
					"Apatnapung","Apatnapu't isang","Apatnapu't dalawang","Apatnapu't tatlong","Apatnapu't apat na",
					"Apatnapu't limang","Apatnapu't anim na","Apatnapu't pitong","Apatnapu't walong","Apatnapu't siyam na",
					"Limampung","Limampu't isang","Limampu't dalawang","Limampu't tatlong","Limampu't apat na",
					"Limampu't limang","Limampu't anim na","Limampu't pitong","Limampu't walong","Limampu't siyam na"
			};
			
			String datetext = "";
			String batterytext = "";

			byte currentHour = currentTime.Hour;
			byte currentMinute = currentTime.Minute;

			byte h = currentHour % 12;
			String tagalogTime = oras[h];

			switch (getMinuteType(currentMinute)) {
				case 0:
					if (currentHour==12) tagalogTime = oras[currentHour];
					break;
				case 1:
					tagalogTime = bilang[currentMinute] +" minuto makalipas ang "+ ((currentHour==12) ? oras[currentHour] : oras[h]);
					break;
				case 2:
					tagalogTime = bilang[60-currentMinute] +" minuto bago mag "+oras[h+1];
					currentHour += 1;
					if (currentHour > 23) {
						currentHour = 0;
						tagalogTime = bilang[60-currentMinute] +" minuto bago mag " + oras[currentHour];
					}
					break;
			}

			tagalogTime = tagalogTime + " " + ampm[getPartOfDay(currentHour)];

			// Fix casing
			tagalogTime.toLowerCase();
			String tagalogTime2 = tagalogTime.substring(0,1);
			tagalogTime2.toUpperCase();
			tagalogTime = tagalogTime2 + tagalogTime.substring(1);

			// Special treat for Halloween!
			if (currentTime.Month==10 && currentTime.Day==31 
			  && currentHour==0 && currentMinute==0) {
			  tagalogTime += " **Gabi ng Lagim! Bwahaha!**";
			}
			if (currentTime.Month==10 && currentTime.Day==31 
			  && currentHour==12 && currentMinute==0) {
			  tagalogTime += " **Gabi ng lagim mamaya! Bwahaha!**";
			}

			// Clear background
			display.fillScreen(GxEPD_WHITE);
			display.setTextColor(GxEPD_BLACK);
			display.setTextWrap(true);

			display.setFont( & Roboto_Medium14pt7b);
			drawWrapCentred(tagalogTime, 100, 100 - 12, 200, 32);
			
			display.setFont( & Roboto_Medium7pt7b);

			// Draw Battery
			batterytext = "B" + String(getBattery());
			drawRightAligned(batterytext, 198, 196);

			// Draw date
			datetext = araw[currentTime.Wday-1]; 
			datetext += " " + String(currentTime.Day) + "/" + buwan[currentTime.Month-1] + "/" + String(currentTime.Year + 1970 - 2000);
			display.setCursor(0, 196);
			display.print(datetext);
  		}
};

WatchFace m; //instantiate your watchface

void setup() {
	//Serial.begin(115200); 
  	//while (!Serial) {
  	//  ; // wait for serial port to connect. Needed for native USB port only
  	//}
  	m.init(); //call init in setup
}

void loop() {
  	// this should never run, Watchy deep sleeps after init();
}
