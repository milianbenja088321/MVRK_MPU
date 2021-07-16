// Source: https://github.com/FastLED/FastLED/blob/master/examples/ColorPalette/ColorPalette.ino
#include <FastLED.h>
#define LED_PIN     3
#define NUM_LEDS    116
#define BRIGHTNESS  180
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 60
#define BLINK_SPEED 500

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
uint8_t patternCounter = -1;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
extern const TProgmemPalette16 redStrobe_p PROGMEM;
extern const TProgmemPalette16 orangeStrobe_p PROGMEM;
extern const TProgmemPalette16 yellowStrobe_p PROGMEM;
extern const TProgmemPalette16 greenStrobe_p PROGMEM;
extern const TProgmemPalette16 blueStrobe_p PROGMEM;
extern const TProgmemPalette16 indigoStrobe_p PROGMEM;
extern const TProgmemPalette16 violetStrobe_p PROGMEM;
static uint32_t currentPattern = 0;

static const TProgmemRGBPalette16* strobePatterns[] = {
  &redStrobe_p,
  &orangeStrobe_p,
  &yellowStrobe_p,
  &greenStrobe_p,
  &blueStrobe_p,
  &indigoStrobe_p,
  &violetStrobe_p,
  &myRedWhiteBluePalette_p
};

#define SPACING_16 1
#define SPACING_8 2
#define SPACING_5 3
#define SPACING_4 4
#define SPACING_3 6
#define SPACING_2 8
#define SPACING_1 12
#define FORWARD 1
#define PAUSE 0
#define BACKWARDS -1
#define SPEED_FAST 6
#define SPEED_MEDIUM 3
#define SPEED_SLOW 1

static const int speeds[] = {
  SPEED_SLOW, SPEED_MEDIUM, SPEED_FAST
};
#define BLINK_FAST 200
#define BLINK_MEDIUM 500
#define BLINK_SLOW 1000

static const int blinkRates[] = {
  BLINK_SLOW, BLINK_MEDIUM, BLINK_FAST
};
void setup()
{
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentPalette = *(strobePatterns[currentPattern]);
  currentBlending = NOBLEND;

  Serial.println("Ready");
}


static uint8_t startIndex = 0;
static int direction = FORWARD;
static int speed = 125;

int btn1, btn2, btn3;

bool didBlink = false;

void loop()
{

  btn1 = digitalRead(4);
  btn2 = digitalRead(5);
  btn3 = digitalRead(6);

  if (btn1 == LOW)
  {
    getNextPallet();
  }

  if (btn2 == LOW)
  {
    toggleSpeed();
    Serial.println(speed);
  }

  if (btn3 == LOW)
  {
    patternCounter++;

    if (patternCounter > 4)
    {
      patternCounter = 0;
    }

    Serial.println(patternCounter);

  }

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);


  switch (patternCounter)
  {
    case 0:
      currentBlending = LINEARBLEND;
      break;
    case 1:
      currentBlending = NOBLEND;
      break;
    case 2:
      rainbowBeat();
      Serial.println("case 2");
      break;
    case 3:
      movingDots();
      break;
    case 4:
      blinkStrip();
      Serial.println("case 4");
      break;
  }



  FastLED.show();
  FastLED.delay(speed);
}

void getNextPallet()
{
  const uint8_t numberOfPalettes = sizeof(strobePatterns) / sizeof(strobePatterns[0]);
  static uint8_t whichPalette = currentPattern;
  whichPalette = addmod8( whichPalette, 1, numberOfPalettes);
  currentPattern++;
  currentPalette = *(strobePatterns[whichPalette]);
}
void getPrevPallet() {
  const uint8_t numberOfPalettes = sizeof(strobePatterns) / sizeof(strobePatterns[0]);
  static uint8_t whichPalette = currentPattern;
  whichPalette = addmod8( whichPalette, -1, numberOfPalettes);
  currentPattern--;
  currentPalette = *(strobePatterns[whichPalette]);
}

void toggleDirection() {
  if (direction == PAUSE) {
    direction = FORWARD;
  } else if (direction == FORWARD) {
    direction = BACKWARDS;
  } else if (direction == BACKWARDS) {
    direction = PAUSE;
  }
}
void toggleSpeed() {
  speed -= 50;

  if (speed <= 0) {
    speed = 125;
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += SPACING_16 * direction;
  }
}
void BlinkLEDsFromColor( CRGB color)
{
  uint8_t brightness = 255;
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}




// ***************** PATTERNS ******************* //

void blinkStrip()
{
  if (didBlink == false)
  {
    fill_solid( currentPalette, 16, CRGB::Black);
    didBlink = true;
  } 
  else
  {
    fill_solid( currentPalette, 16, CRGB::Blue);
    didBlink = false;
  }
}

void movingDots() {

  uint16_t posBeat  = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  uint16_t posBeat2 = beatsin16(60, 0, NUM_LEDS - 1, 0, 0);

  uint16_t posBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 32767);
  uint16_t posBeat4 = beatsin16(60, 0, NUM_LEDS - 1, 0, 32767);

  // Wave for LED color
  uint8_t colBeat  = beatsin8(45, 0, 255, 0, 0);

  leds[(posBeat + posBeat2) / 2]  = CHSV(colBeat, 255, 255);
  leds[(posBeat3 + posBeat4) / 2]  = CHSV(colBeat, 255, 255);

  fadeToBlackBy(leds, NUM_LEDS, 10);
}

void rainbowBeat()
{

  uint16_t beatA = beatsin16(30, 0, 255);
  uint16_t beatB = beatsin16(20, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (beatA + beatB) / 2, 8);
}

void redWhiteBlue()
{

  uint16_t sinBeat   = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  uint16_t sinBeat2  = beatsin16(30, 0, NUM_LEDS - 1, 0, 21845);
  uint16_t sinBeat3  = beatsin16(30, 0, NUM_LEDS - 1, 0, 43690);

  leds[sinBeat]   = CRGB::Blue;
  leds[sinBeat2]  = CRGB::Red;
  leds[sinBeat3]  = CRGB::White;

  fadeToBlackBy(leds, NUM_LEDS, 10);
}

void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black,
  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};
const TProgmemPalette16 redStrobe_p PROGMEM =
{
  CRGB::Red,
  CRGB::Red,
  CRGB::Red,
  CRGB::Red,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Red,
  CRGB::Red,
  CRGB::Red,
  CRGB::Red,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black
};
const TProgmemPalette16 orangeStrobe_p PROGMEM =
{
  CRGB::Orange,
  CRGB::Orange,
  CRGB::Orange,
  CRGB::Orange,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Orange,
  CRGB::Orange,
  CRGB::Orange,
  CRGB::Orange,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black
};
const TProgmemPalette16 yellowStrobe_p PROGMEM =
{
  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black
};
const TProgmemPalette16 greenStrobe_p PROGMEM =
{
  CRGB::Green,
  CRGB::Green,
  CRGB::Green,
  CRGB::Green,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Green,
  CRGB::Green,
  CRGB::Green,
  CRGB::Green,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black
};
const TProgmemPalette16 blueStrobe_p PROGMEM =
{
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black
};
const TProgmemPalette16 indigoStrobe_p PROGMEM =
{
  CRGB::Indigo,
  CRGB::Indigo,
  CRGB::Indigo,
  CRGB::Indigo,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Indigo,
  CRGB::Indigo,
  CRGB::Indigo,
  CRGB::Indigo,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black
};
const TProgmemPalette16 violetStrobe_p PROGMEM =
{
  CRGB::Violet,
  CRGB::Violet,
  CRGB::Violet,
  CRGB::Violet,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Violet,
  CRGB::Violet,
  CRGB::Violet,
  CRGB::Violet,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black
};
