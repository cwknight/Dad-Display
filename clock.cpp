#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
 
#define PIN 12

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
 
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, 12,      //Put pin number in third space.
  NEO_MATRIX_TOP   + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);
 
const uint16_t colors[] = {
  matrix.Color(0, 0, 255), 
  matrix.Color(0, 255, 0), 
  matrix.Color(255, 255, 0),
  matrix.Color(255, 51, 204), 
  matrix.Color(255, 102, 0), 
  matrix.Color(0, 255, 255), 
  matrix.Color(255, 255, 255), 
  matrix.Color(255, 255, 51),
  };

uint16_t randomColorAllColors(Adafruit_NeoMatrix *matrixInternal){ 
    int r = random(0,256);
    int g = random(0,256);
    int b = random(0,256);
    uint16_t generatedcolor = matrixInternal->Color(g, r, b);
    return generatedcolor;
}

uint16_t randomColorFromList(uint16_t colorArray[]){ 
    int arrayLength = sizeof(colorArray)/sizeof(uint16_t);
    int randomIndex = random(0, arrayLength);
    return colorArray[randomIndex];
}

uint16_t randomColorExceptList(uint16_t colorArray[], Adafruit_NeoMatrix *matrixInternal){
    int arrayLength = sizeof(colorArray)/sizeof(uint16_t);
    uint16_t generatedcolor = randomColorAllColors(matrixInternal);
    for(int count = 0; 0 < arrayLength; count++ ){
        if(generatedcolor == colorArray[count]){
            return randomColorExceptList(colorArray, matrixInternal);
        }
    }
    return generatedcolor;
}

 
void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(3);        //Brightness change here.
  matrix.setTextColor(matrix.Color(255,255,255));
   randomSeed(analogRead(0));
}
 
int x    = matrix.width();
int pass = 0;
 
void loop() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("*** 3 Days Until Departure Flight ***"));      //Put Text here.
 
  if(--x < -215          //Change for length of Text this is set for approximately 30 characters and spaces
  ) {
    x = matrix.width();
 
           //use randomColorFromList(colors) for random choice from list colors[].
           //use randomColorExceptList(colors, &matrix) for random choice from all possible colors, except in colors[]
           //use randomColorAllColors(&matrix) for random choice from all possible colors
    matrix.setTextColor(randomColorAllColors(&matrix));
  }
  matrix.show();
  delay(120);
}