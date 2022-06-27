#include "TextLCD_I2C.h"
#include "functions.h"
#include "mbed.h"
#include <cstdio>

#define SDA D14
#define SCL D15
TextLCD_I2C lcd(SDA, SCL, false); // set to true if LCD backpack has different pinout

InterruptIn klikker(D2);

// info motoren
PwmOut out1(D5);
PwmOut out2(D6);
DigitalOut enA(D7);
DigitalOut enB(D8);
PwmOut out3(D9);
PwmOut out4(D10);

// variabelen voor de switch case voor het rijden
bool entry = true;
enum state { vooruit, botsing };
Timer timer;

//variabele voor indrukken bumper en debounce timer voor de klikker
bool flag_klikker = false;
Timer debounce;

void klikker_ingedrukt() {
  if (debounce.read_ms() > 200) {
    flag_klikker = true;
    debounce.reset();
  }
}

int main() {
  lcd.setBacklight(true);
  lcd.locate(0, 0);
  lcd.printf("ready to go");
  int accu = accupercentage();
  lcd.locate(0, 1);
  lcd.printf("accu: %d%", accu); //accupercentage printen op lcd scherm

  debounce.start();
  klikker.rise(klikker_ingedrukt);

  state state = state::vooruit;

  while (true) {
    switch (state) {
    case (vooruit): // vooruit rijden,en inschakelen van de motoren
      enA = 1;
      out1.write(0);
      out2.write(0.5);
      enB = 1;
      out3.write(0);
      out4.write(0.5);

      if (klikker == 0 ||
          ultrasoon() > 5) { // waarnemen van een botsing of groot hoogte verschil, schiet in case botsing
        flag_klikker = false;
        state = state::botsing;
      }
      break;
    case (botsing):
      if (entry) { // achteruit rijden
        out1.write(0.5);
        out2.write(0);
        out3.write(0.5);
        out4.write(0);

        timer.reset();
        timer.start();
        entry = false;
      }
      if (timer.read() > 0.5) {
        out1.write(0.5); // draaien
        out2.write(0);
        out3.write(0);
        out4.write(0.5);
        ThisThread::sleep_for(1s);
        state = state::vooruit;
        flag_klikker = false;
        entry = true;
      }
      break;
    default:
      break;
    }
  }
}
