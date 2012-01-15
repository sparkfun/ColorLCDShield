//*************************************************************************
//					Nokia Shield O-Scope
//*************************************************************************
//* Written by Peter Davenport on December 23, 2010.
//* http://peterdavenport.blogspot.com/
//* Uses the Sparkfun Color LCD Shield
//* Adapted for Arduino 1.0 IDE & ColorLCDShield Library by VERHILLE Arnaud
//*************************************************************************

#include <ColorLCDShield.h>

LCDShield lcd;
int buttonPins[3] = {3, 4, 5};

int val1 = 0; // The current value taken on the analog pin
int lastval = 1; // The last value taken on the analog pin
int drawSpeed = 50; // Delay on sample rate.
int osctime = 1; // Variable for timing/X axis position

//************************************************************************
//					Startup code
//************************************************************************
void	setup()
{
	// This should only be init(EPSON) or init(PHILLIPS)
	lcd.init(PHILLIPS);	    //Initialize the LCD
        lcd.contrast(40);   // Sets LCD contrast, adjust this if your contrast is odd looking.
        lcd.clear(WHITE);    // Clear LCD to solid white
        lcd.setStr("Ardu-scope", 0, 4, BLACK, WHITE); //Intro and instructions on screen
        lcd.setStr("code by Peter", 16, 4, BLACK, WHITE);
        lcd.setStr("Davenport!", 32, 4, BLACK, WHITE);
        lcd.setStr("The program is", 48, 4, RED, WHITE);
        lcd.setStr("reading analog", 64, 4, RED, WHITE);
        lcd.setStr("pin #2.", 80, 4, RED, WHITE);
        lcd.setStr("Starting in:", 96, 20, GREEN, BROWN); // Startup Timer
        lcd.setStr("5", 112, 58, GREEN, BROWN);
        delay(1000);
        lcd.setStr("4", 112, 58, GREEN, BROWN);
        delay(1000);
        lcd.setStr("3", 112, 58, GREEN, BROWN);
        delay(1000);
        lcd.setStr("2", 112, 58, GREEN, BROWN);
        delay(1000);
        lcd.setStr("1", 112, 58, GREEN, BROWN);
        delay(1000);
        lcd.setStr("0", 112, 58, GREEN, BROWN);
        lcd.clear(WHITE);    // Clear LCD to solid White
    //Draw voltage indicator lines
    lcd.setLine(128, 0, 128, 136, BLACK);
    lcd.setLine(44, 0, 44, 136, ORANGE);
    lcd.setLine(2, 0, 2, 136, RED);
    //Write indicator volatges
    lcd.setStr("3.3", 44, 102, ORANGE, WHITE);
    lcd.setStr("5v", 2, 112, RED, WHITE);
    //Write the speed
    if(drawSpeed == 1){
    lcd.setStr("Fast", 2, 4, BLUE, WHITE);
    } else if (drawSpeed == 50) {
    lcd.setStr("Medium", 2, 4, BLUE, WHITE);
    } else {
    lcd.setStr("Slow", 2, 4, BLUE, WHITE);
    }
    //End of startup and begenning the o-scope loop that takes readings, displays them, and checks for button presses.
}
//************************************************************************
//					Loop
//************************************************************************
void	loop()
{
  
  if(osctime <= 130){ // if the X position is still on the screen then...
  
      val1 = analogRead(2);    // read the value from the sensor for line 2
      val1 = 1024 - val1; // Invert it to make it display corectly.
      val1 = val1 / 8; // Divide it by eight to allow it to fit on the screen properly.
      lcd.setLine(lastval, osctime, val1, osctime + 1, BLUE); // Draw it as a linf from last value to current value.
      lastval = val1;//Update last value
      delay(drawSpeed); // wait the prescribed time
      osctime++;//add one to the X position
    
  }else{ // if the X position is not on the screen then...
      osctime = 0;//Clear the timer
      lcd.clear(WHITE);    //Clear the LCD
      //Re-draw indicator lines
      lcd.setLine(128, 0, 128, 136, BLACK);
      lcd.setLine(44, 0, 44, 136, ORANGE);
      lcd.setLine(2, 0, 2, 136, RED);
      //Re-write indicator volatges
      lcd.setStr("3.3", 44, 102, ORANGE, WHITE);
      lcd.setStr("5v", 2, 112, RED, WHITE);
      //Re-write the speed
        if(drawSpeed == 1){
          lcd.setStr("Fast  ", 2, 4, BLUE, WHITE);
        } else if (drawSpeed == 50) {
          lcd.setStr("Medium", 2, 4, BLUE, WHITE);
        } else {
          lcd.setStr("Slow  ", 2, 4, BLUE, WHITE);
        }
  }
  //Now it's time to check for button presses
int	s1, s2, s3; // Make intergers for all the swiches
	s1	=	!digitalRead(buttonPins[0]); //apply the button values to the intergers
	s2	=	!digitalRead(buttonPins[1]);
	s3	=	!digitalRead(buttonPins[2]);

if (s1){ // if button 1 is pressed then ....
   drawSpeed = 100; // Set the draw speed to 100
   lcd.setStr("Slow  ", 2, 4, BLUE, WHITE); // write on the screen that the speed is Slow
	}
else if (s2){
   drawSpeed = 50; // Set the draw speed to 50
   lcd.setStr("Medium", 2, 4, BLUE, WHITE); // Write on the screen that the speed is Medium
	}
else if (s3){
     drawSpeed = 1; // Set the draw speed to 1
     lcd.setStr("Fast  ", 2, 4, BLUE, WHITE); // Write on the screen that the speed is Fast
	}
  s1 = 0; // set the button values to 0 
  s2 = 0;
  s3 = 0;
}
