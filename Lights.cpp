#include "Arduino.h"

#define red 9
#define green 11
#define blue 10

class Lights {
  private:
    int redAt = 1;
    int greenAt = 1;
    int blueAt = 1;
  
  public:

    Lights () {
//      redWrite(redAt);
//      greenWrite(greenAt);
//      blueWrite(blueAt);
    }

    void redWrite(int val) {
      if (val < 256 && val > 0) {
        analogWrite( red, 255-val);
      } else if ( val > 255 ) {
        analogWrite( red, 0);
      } else {
        analogWrite( red, 255);
      }
    }

    void greenWrite(int val) {
      if (val < 256 && val > 0) {
        analogWrite( green, 255-val);
      } else if ( val > 255 ) {
        analogWrite( green, 0);
      } else {
        analogWrite( green, 255);
      }
    }

    void blueWrite(int val) {
      if (val < 256 && val > 0) {
        analogWrite( blue, 255-val);
      } else if ( val > 255 ) {
        analogWrite( blue, 0);
      } else {
        analogWrite( blue, 255);
      }
    }

    void fadeInto( int r, int g, int b, int rate) {
      bool rUp = false;
      bool gUp = false;
      bool bUp = false;
      
      if (r > redAt)
        rUp = true;

      if (g > greenAt)
        gUp = true;

      if (b > blueAt)
        bUp = true;

      if (rUp && gUp && bUp) {
        while (redAt < r || greenAt < g || blueAt < b) {
          if (redAt != r)
            redWrite(++redAt);
          if (greenAt != g)
            greenWrite(++greenAt);
          if (blueAt != b)
            blueWrite(++blueAt);

          delay(rate);
        }
      } else if (rUp && gUp && !bUp) {
        while (redAt < r || greenAt < g || blueAt > b) {
          if (redAt != r)
            redWrite(++redAt);
          if (greenAt != g)
            greenWrite(++greenAt);
          if (blueAt != b)
            blueWrite(--blueAt);

          delay(rate);
        }
      } else if (rUp && !gUp && bUp) {
        while (redAt < r || greenAt > g || blueAt < b) {
          if (redAt != r)
            redWrite(++redAt);
          if (greenAt != g)
            greenWrite(--greenAt);
          if (blueAt != b)
            blueWrite(++blueAt);

          delay(rate);
        }
      } else if (rUp && !gUp && !bUp) {
        while (redAt < r || greenAt > g || blueAt > b) {

          if (redAt != r)
            redWrite(++redAt);
          if (greenAt != g)
            greenWrite(--greenAt);
          if (blueAt != b)
            blueWrite(--blueAt);

          delay(rate);
        }
      } else if (!rUp && gUp && bUp) {
        while (redAt > r || greenAt < g || blueAt < b) {

          if (redAt != r)
            redWrite(--redAt);
          if (greenAt != g)
            greenWrite(++greenAt);
          if (blueAt != b)
            blueWrite(++blueAt);

          delay(rate);
        }
      } else if (!rUp && gUp && !bUp) {
        while (redAt > r || greenAt < g || blueAt > b) {

          if (redAt != r)
            redWrite(--redAt);
          if (greenAt != g)
            greenWrite(++greenAt);
          if (blueAt != b)
            blueWrite(--blueAt);

          delay(rate);
        }
      } else if (!rUp && !gUp && bUp) {
        while (redAt > r || greenAt > g || blueAt < b) {

          if (redAt != r)
            redWrite(--redAt);
          if (greenAt != g)
            greenWrite(--greenAt);
          if (blueAt != b)
            blueWrite(++blueAt);

          delay(rate);
        }
      } else {
        while (redAt > r || greenAt > g || blueAt > b) {

        if (redAt != r)
          redWrite(--redAt);
        if (greenAt != g)
          greenWrite(--greenAt);
        if (blueAt != b)
          blueWrite(--blueAt);

          delay(rate);
        }
      }
    }

    void sunrise() {
      redAt = 5;
      greenAt = 1;
      blueAt = 0;

      redWrite(redAt);
      greenWrite(greenAt);
      blueWrite(blueAt);

      delay(1000);

      while (redAt < 190) {
        redWrite( redAt++ );
        if (redAt > 60 && greenAt < 125) {
          greenWrite( greenAt++ );
        }

        delay(100);
      }
    }

    void on(int redV, int greenV, int blueV) {

      redV = 255 - redV;
      greenV = 255 - greenV;
      blueV = 255 - blueV;
      
      analogWrite(red,redV);
      analogWrite(green,greenV);
      analogWrite(blue,blueV);
    }
    
    void off() {
      analogWrite(red,255);
      analogWrite(green,255);
      analogWrite(blue,255);
    }

    void flash(int r, int g, int b) {
      redWrite(r);
      greenWrite(g);
      blueWrite(b);

      delay(50);

      redWrite(redAt);
      greenWrite(greenAt);
      blueWrite(blueAt);
    }

    void set(byte r, byte g, byte b) {
      redWrite(redAt = r);
      greenWrite(greenAt = g);
      blueWrite(blueAt = b);
    }
  
};
