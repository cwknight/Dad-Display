#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
 #include <math.h>
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
 
String errorMessage = "Message too long. Please shorten message to 140 chars or less.";

typedef struct {
    double r;       // ∈ [0, 1]
    double g;       // ∈ [0, 1]
    double b;       // ∈ [0, 1]
} rgb;

typedef struct {
    double h;       // ∈ [0, 360]
    double s;       // ∈ [0, 1]
    double v;       // ∈ [0, 1]
} hsv;

rgb hsv2rgb(hsv HSV)
{
    rgb RGB;
    double H = HSV.h, S = HSV.s, V = HSV.v,
            P, Q, T,
            fract;

    (H == 360.)?(H = 0.):(H /= 60.);
    fract = H - floor(H);

    P = V*(1. - S);
    Q = V*(1. - S*fract);
    T = V*(1. - S*(1. - fract));

    if      (0. <= H && H < 1.)
        RGB = (rgb){.r = V, .g = T, .b = P};
    else if (1. <= H && H < 2.)
        RGB = (rgb){.r = Q, .g = V, .b = P};
    else if (2. <= H && H < 3.)
        RGB = (rgb){.r = P, .g = V, .b = T};
    else if (3. <= H && H < 4.)
        RGB = (rgb){.r = P, .g = Q, .b = V};
    else if (4. <= H && H < 5.)
        RGB = (rgb){.r = T, .g = P, .b = V};
    else if (5. <= H && H < 6.)
        RGB = (rgb){.r = V, .g = P, .b = Q};
    else
        RGB = (rgb){.r = 0., .g = 0., .b = 0.};

    return RGB;
}



Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, 12,      //Put pin number in third space.
  NEO_MATRIX_TOP   + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);
 
const uint16_t colors[] = {
    matrix.Color(0,0,0),
//   matrix.Color(0, 0, 255), 
//   matrix.Color(0, 255, 0), 
//   matrix.Color(255, 255, 0),
//   matrix.Color(255, 51, 204), 
//   matrix.Color(255, 102, 0), 
//   matrix.Color(0, 255, 255), 
//   matrix.Color(255, 255, 255), 
//   matrix.Color(255, 255, 51),
  };

uint16_t colorFromRGB(rgb RGB, Adafruit_NeoMatrix *matrixInternal){
    return matrixInternal->Color(RGB.g*255,RGB.r*255,RGB.b*255);
}

uint16_t colorFromHSV(hsv HSV, Adafruit_NeoMatrix *matrixInternal){
    rgb RGB = hsv2rgb(HSV);
    return colorFromRGB(RGB, matrixInternal);
}


uint16_t randomColorAllColors(Adafruit_NeoMatrix *matrixInternal){ 
    hsv HSV = {.h = random(0,361), .s = random(0,11)*0.1, .v = 0.8};
    return colorFromHSV(HSV, matrixInternal);
    
    // int r = random(0,256);
    // int g = random(0,256);
    // int b = random(0,256);
    // if(r+g+b < 350){
    //     return randomColorAllColors(matrixInternal);
    // // } 
    // uint16_t generatedcolor = matrixInternal->Color(g, r, b);
    // return generatedcolor;
}

// uint16_t randomColorFromList(uint16_t colorArray[]){ 
//     int arrayLength = sizeof(colorArray)/sizeof(uint16_t);
//     int randomIndex = random(0, arrayLength);
//     return colorArray[randomIndex];
// }

// uint16_t randomColorExceptList(uint16_t colorArray[], Adafruit_NeoMatrix *matrixInternal){
//     int arrayLength = sizeof(colorArray)/sizeof(uint16_t);
//     uint16_t generatedcolor = randomColorAllColors(matrixInternal);
//     for(int count = 0; count < arrayLength; count++ ){
//         if(generatedcolor == colorArray[count]){
//             return randomColorExceptList(colorArray, matrixInternal);
//         }
//     }
//     return generatedcolor;
// }

int textLength(String input, int spaces){
    if(spaces < 0){
        spaces = 0;
    }
    int output = input.length() * 6 + spaces * 6;
    
    return output;  
}

int x    = matrix.width();

String text = "*** 3 Days Until Departure Flight ***"; //Put Text here. Cannot exceed 140 characters 
int spacing = 0; // amount of blank time before message repeat
char textChars[141];

uint16_t randomColorHolder[140];

 
void setup() {
 if(text.length() > 140){
    text = errorMessage;
}
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(5);        //Brightness change here.
  matrix.setTextColor(matrix.Color(0,0,0));
   randomSeed(analogRead(0));
   text.toCharArray(textChars,141);
   
for(int count = 0; count < text.length(); count++){
      randomColorHolder[count] = randomColorAllColors(&matrix);
  }

}

void loop() {
    
  matrix.fillScreen(0);
  drawFastHLine(0, 0, matrix.width(), matrix.Color(231, 124, 176));
  drawFastHLine(0, 1, matrix.width(), matrix.Color(232, 54, 54));
  drawFastHLine(0, 2, matrix.width(), matrix.Color(231, 151, 54));
  drawFastHLine(0, 3, matrix.width(), matrix.Color(231, 231, 55));
  drawFastHLine(0, 4, matrix.width(), matrix.Color(56, 152, 56));
  drawFastHLine(0, 5, matrix.width(), matrix.Color(53, 185, 187));
  drawFastHLine(0, 6, matrix.width(), matrix.Color(91, 48, 155));
  drawFastHLine(0, 7, matrix.width(), matrix.Color(143, 46, 143));
  matrix.setCursor(x, 0);
  for(int count = 0; count < text.length(); count++){
      //matrix.setTextColor(randomColorHolder[count]);
      matrix.print(textChars[count]);
  }
//   matrix.print(text.c_str());

   
  if(--x < -textLength(text,spacing)) {
    x = matrix.width();

    //for(int count = 0; count < text.length(); count++){
    //  randomColorHolder[count] = randomColorAllColors(&matrix);
    //}
 
           //use randomColorFromList(colors) for random choice from list colors[].
           //use randomColorExceptList(colors, &matrix) for random choice from all possible colors, except in colors[]
           //use randomColorAllColors(&matrix) for random choice from all possible colors
    // matrix.setTextColor(randomColorAllColors(&matrix));
  }
  matrix.show();
  delay(120);
}