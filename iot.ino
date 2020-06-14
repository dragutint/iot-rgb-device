#include <Kelvin2RGB.h>
#include <ESP8266WiFi.h>

#define REDPIN 5
#define GREENPIN 4
#define BLUEPIN 0
#define FADESPEED 2

const char* ssid     = "komsija";
const char* password = "dragutin1234";
WiFiServer server(80);
String header;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

int mode = 0;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

float brightnessLED = 0.7;

boolean rgbType = true;
int colorMode = 0; 
int pattern = 0;
int prevFadeVal = 0;
int currentFadeVal = 0;
boolean increasing = true;

int fadeVal = 5; 
int fadeMAX = 1024; 
int fadeMIN = 0;  
int fadeDelay = 50;

int blinkVal = 0;
boolean blinkON = false;
int counter = 10; 
int blinkRate = 1000; 
int rainbowRedVal = 0;
int rainbowGreenVal = 0;
int rainbowBlueVal = 0;
int rainbowTransitionVal = 0;
int rainbowDelay = 5;

void setup() {
  Serial.begin(115200);
  
  pinMode(REDPIN, OUTPUT);  
  pinMode(GREENPIN, OUTPUT);  
  pinMode(BLUEPIN, OUTPUT);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  sequenceTest();
  allOFF(); 
  rgbCalc();
  rgbShow(); 
}

void loop(){
  checkClient();
  
  switch (pattern) {
    case 1:
      patternON();
      break;
    case 2:
      patternFade();
      break;
    case 3:
      patternBlink();
      break;
    case 4:
      patternRainbow();
      break;
    case 5:
      temperatureMode();
      break;
    default:
      allOFF();
      rgbCalc();
      rgbShow();
      break;
  }
}

void checkClient(){
  WiFiClient client = server.available();   

  if (client) {                            
    handleRequest(client);
  }
}

void handleRequest(WiFiClient client){
  Serial.println("New Client.");          
    String currentLine = "";               
    currentTime = millis();
    previousTime = currentTime;
    
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();         
      if (client.available()) {            
        char c = client.read();           
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /color/red") >= 0) {
              Serial.println("red");
              colorMode = 1;
            }else if (header.indexOf("GET /color/blue") >= 0) {
              Serial.println("blue");
              colorMode = 9;
            }else if (header.indexOf("GET /color/green") >= 0) {
              Serial.println("green");
              colorMode = 5;
            }else if (header.indexOf("GET /color/orange") >= 0) {
              Serial.println("orange");
              colorMode = 2;
            }else if (header.indexOf("GET /color/yellow") >= 0) {
              Serial.println("yellow");
              colorMode = 3;
            }else if (header.indexOf("GET /color/chartruese") >= 0) {
              Serial.println("chartruese");
              colorMode = 4;
            }else if (header.indexOf("GET /color/springGreen") >= 0) {
              Serial.println("spring green");
              colorMode = 6;
            }else if (header.indexOf("GET /color/cyan") >= 0) {
              Serial.println("cyan");
              colorMode = 7;
            }else if (header.indexOf("GET /color/azure") >= 0) {
              Serial.println("azure");
              colorMode = 8;
            }else if (header.indexOf("GET /color/magenta") >= 0) {
              Serial.println("magenta");
              colorMode = 11;
            }else if (header.indexOf("GET /color/violet") >= 0) {
              Serial.println("violet");
              colorMode = 10;
            }else if (header.indexOf("GET /color/rose") >= 0) {
              Serial.println("rose");
              colorMode = 12;
            }else if (header.indexOf("GET /pattern/on") >= 0) {
              Serial.println("pattern 1");
              pattern = 1;
            } else if (header.indexOf("GET /pattern/fade") >= 0) {
              Serial.println("pattern 2");
              pattern = 2;
            } else if (header.indexOf("GET /pattern/rainbow") >= 0) {
              Serial.println("pattern 3");
              pattern = 4;
            } else if (header.indexOf("GET /pattern/blink") >= 0) {
              Serial.println("pattern 4");
              pattern = 3;
            } else if (header.indexOf("GET /mode/temperature") >= 0) {
              Serial.println("temperature");
              pattern = 5;
            } else if(header.indexOf("GET /off") >= 0) {
              Serial.println("OFF");
              pattern = 0;
            } else if(header.indexOf("GET /color/white") >= 0) {
              pattern = 1;
              redValue = 1024;
              greenValue = 1024;
              blueValue = 1024;
              rgbShow();
              delay(5000);
            }
          } else {
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c; 
        }
      }
    }
    
 
    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
}


void allOFF() {
  redValue = 0;
  greenValue =  0;
  blueValue = 0;
}

void redON() {
  redValue = 1024;
  greenValue =  0;
  blueValue = 0;
}

void orangeON() {
  redValue = 1024;
  greenValue = 512;
  blueValue = 0;
}

void yellowON() {
  redValue = 1024;
  greenValue = 1024;
  blueValue = 0;
}

void chartrueseON() {
  redValue = 512;
  greenValue = 1024;
  blueValue = 0;
}

void greenON() {
  redValue = 0;
  greenValue = 1024;
  blueValue = 0;
}

void springGreenON() {
  redValue = 0;
  greenValue = 1024;
  blueValue = 512;
}

void cyanON() {
  redValue = 0;
  greenValue = 1024;
  blueValue = 1024;
}

void azureON() {
  redValue = 0;
  greenValue = 512;
  blueValue = 1024;
}

void blueON() {
  redValue = 0;
  greenValue = 0;
  blueValue = 1024;
}

void violetON() {
  redValue = 512;
  greenValue = 0;
  blueValue = 1024;
}

void magentaON() {
  redValue = 1024;
  greenValue = 0;
  blueValue = 1024;
}

void roseON() {
  redValue = 1024;
  greenValue = 0;
  blueValue = 512;
}

void whiteON() {
  redValue = 1024;
  greenValue = 1024;
  blueValue = 1024;
}

void sequenceTest() {
  redON();
  rgbCalc();
  rgbShow();
  delay(50);

  orangeON();
  rgbCalc();
  rgbShow();
  delay(50);

  allOFF();
  rgbCalc();
  rgbShow();
  delay(50);
}

void rgbCalc() {
  redValue = int(redValue * brightnessLED);
  greenValue = int(greenValue * brightnessLED);
  blueValue = int(blueValue * brightnessLED);
}

void rgbShow() {
  analogWrite(REDPIN, redValue);
  analogWrite(GREENPIN, greenValue);
  analogWrite(BLUEPIN, blueValue);
}

void patternON() {
  switch (colorMode){
    case 1:
      redON();
      break;
    case 2:
      orangeON();
      break;
    case 3:
      yellowON();
      break;
    case 4:
      chartrueseON();
      break;
    case 5:
      greenON();
      break;
    case 6:
      springGreenON();
      break;
    case 7:
      cyanON();
      break;
    case 8:
      azureON();
      break;
    case 9:
      blueON();
      break;
    case 10:
      violetON();
      break;
    case 11:
      magentaON();
      break;
    case 12:
      roseON();
      break;
    case 13:
      whiteON();
      break;
    default:
      allOFF();
      break;
  }
  rgbCalc();
  rgbShow();
}

void patternFade() {

  switch (colorMode) {
    case 1:
      redValue = currentFadeVal;
      greenValue = 0;
      blueValue = 0;

      rgbCalc();
      break;

    case 2:
      redValue = currentFadeVal;
      greenValue = currentFadeVal * 0.498;
      blueValue = 0;

      rgbCalc();

      if (redValue > 0 && greenValue == 0) {
        redValue = 0;
      }
      break;

    case 3:
      redValue = currentFadeVal;
      greenValue = currentFadeVal;
      blueValue = 0;

      rgbCalc();
      break;

    case 4:
      redValue = currentFadeVal * 0.498;
      greenValue = currentFadeVal;
      blueValue = 0;

      rgbCalc();

      if (greenValue > 0 && redValue == 0) {
        greenValue = 0;
      }
      break;

    case 5:
      redValue = 0;
      greenValue = currentFadeVal;
      blueValue = 0;

      rgbCalc();
      break;

    case 6:
      redValue = 0;
      greenValue = currentFadeVal;
      blueValue = currentFadeVal * 0.498;

      rgbCalc();

      if (greenValue > 0 && blueValue == 0) {
        greenValue = 0;
      }
      break;

    case 7:
      redValue = 0;
      greenValue = currentFadeVal;
      blueValue = currentFadeVal;

      rgbCalc();
      break;

    case 8:
      redValue = 0;
      greenValue = currentFadeVal * 0.498;
      blueValue = currentFadeVal;

      rgbCalc();
      if (blueValue > 0 && greenValue == 0) {
        blueValue = 0;
      }
      break;

    case 9:
      redValue = 0;
      greenValue = 0;
      blueValue = currentFadeVal;

      rgbCalc();
      break;

    case 10:
      redValue = currentFadeVal * 0.498;
      greenValue = 0;
      blueValue = currentFadeVal;

      rgbCalc();

      if (blueValue > 0 && redValue == 0) {
        blueValue = 0;
      }
      break;

    case 11:
      redValue = currentFadeVal;
      greenValue = 0;
      blueValue = currentFadeVal;

      rgbCalc();
      break;

    case 12:
      redValue = currentFadeVal;
      greenValue = 0;
      blueValue = currentFadeVal * 0.498;

      rgbCalc();

      if (redValue > 0 && blueValue == 0) {
        redValue = 0;
      }
      break;

    case 13:
      redValue = currentFadeVal;
      greenValue = currentFadeVal;
      blueValue = currentFadeVal;

      rgbCalc();
      break;

    default:
      allOFF();
      rgbCalc();
      break;
  }

  rgbShow();
  delay(fadeDelay);


  if (increasing == true) {
    currentFadeVal += fadeVal;
  }
  else { //decreasing
    currentFadeVal -= fadeVal;
  }

  if (currentFadeVal > fadeMAX) {
    increasing = false;
    prevFadeVal -= fadeVal;//undo addition

    currentFadeVal = prevFadeVal;

  }
  else if (currentFadeVal < fadeMIN) {
    increasing = true;
    prevFadeVal += fadeVal;//unto subtraction

    currentFadeVal = prevFadeVal;
  }

  prevFadeVal = currentFadeVal;
}

void patternBlink() {

  switch (colorMode) {
    case 1://RED
      redValue = blinkVal;
      greenValue = 0;
      blueValue = 0;

      rgbCalc();
      break;

    case 2://ORANGE
      redValue = blinkVal;
      greenValue = blinkVal * 0.498;
      blueValue = 0;

      rgbCalc();
      break;

    case 3://YELLOW
      redValue = blinkVal;
      greenValue = blinkVal;
      blueValue = 0;

      rgbCalc();
      break;

    case 4://CHARTREUSE
      redValue = blinkVal * 0.498;
      greenValue = blinkVal;
      blueValue = 0;

      rgbCalc();
      break;

    case 5://GREEN
      redValue = 0;
      greenValue = blinkVal;
      blueValue = 0;

      rgbCalc();
      break;

    case 6://SRING GREEN
      redValue = 0;
      greenValue = blinkVal;
      blueValue = blinkVal * 0.498;

      rgbCalc();
      break;

    case 7://CYAN
      redValue = 0;
      greenValue = blinkVal;
      blueValue = blinkVal;

      rgbCalc();
      break;

    case 8://AZURE
      redValue = 0;
      greenValue = blinkVal * 0.498;
      blueValue = blinkVal;

      rgbCalc();
      break;

    case 9://BLUE
      redValue = 0;
      greenValue = 0;
      blueValue = blinkVal;

      rgbCalc();
      break;

    case 10://VIOLET
      redValue = blinkVal * 0.498;
      greenValue = 0;
      blueValue = blinkVal;

      rgbCalc();
      break;

    case 11://MAGENTA
      redValue = blinkVal;
      greenValue = 0;
      blueValue = blinkVal;

      rgbCalc();
      break;

    case 12://ROSE
      redValue = blinkVal;
      greenValue = 0;
      blueValue = blinkVal * 0.498;

      rgbCalc();
      break;
    case 13://WHITE
      redValue = blinkVal;
      greenValue = blinkVal;
      blueValue = blinkVal;

      rgbCalc();
      break;

    default:
      allOFF();
      rgbCalc();
      break;
  }

  rgbShow();

  if (counter == blinkRate) {
    if (blinkON == true) {
      blinkVal = 0;
      blinkON = false;
    }
    else { //it was on, so turn off
      blinkVal = 255;
      blinkON = true;
    }
    counter = 0;
  }
  else {
    counter = counter + 1;
  }
}

void patternRainbow() {
  if (rainbowTransitionVal == 0) {
    //RED
    rainbowRedVal += 5;
    if (rainbowRedVal >= 255) {
      rainbowTransitionVal = 1;
    }
  }
  else if (rainbowTransitionVal == 1) {
    //RED TO ORANGE TO YELLOW
    rainbowGreenVal += 5;

    if (rainbowGreenVal >= 255) {
      rainbowTransitionVal = 2;
    }
  }
  else if (rainbowTransitionVal == 2) {
    //YELLOW to CHARTREUSE to GREEN
    rainbowRedVal -= 5;

    if (rainbowRedVal <= 0) {
      rainbowTransitionVal = 3;
    }
  }
  else if (rainbowTransitionVal == 3) {
    //GREEN to SPRING GREEN to CYAN
    rainbowBlueVal += 5;

    if (rainbowBlueVal >= 255) {
      rainbowTransitionVal = 4;
    }
  }
  else if (rainbowTransitionVal == 4) {
    //CYAN to AZURE to BLUE
    rainbowGreenVal -= 5;

    if (rainbowGreenVal <= 0) {
      rainbowTransitionVal = 5;
    }
  }
  else if (rainbowTransitionVal == 5) {
    //BLUE to VIOLET to MAGENTA
    rainbowRedVal += 5;

    if (rainbowRedVal >= 255) {
      rainbowTransitionVal = 6;
    }
  }
  else if (rainbowTransitionVal == 6) {
    //MAGENTA to ROSE to RED
    rainbowBlueVal -= 5;

    if (rainbowBlueVal <= 0) {
      rainbowTransitionVal = 1;
    }
  }

  redValue = int(rainbowRedVal * brightnessLED);
  greenValue = int(rainbowGreenVal * brightnessLED);
  blueValue = int(rainbowBlueVal * brightnessLED);
  
  rgbShow();

  delay(rainbowDelay);
}

void calculateTemperatureColors(int temperature){  
  Kelvin2RGB kelvin(temperature, 100);
  redValue = map(kelvin.Red, 0, 255, 0, 1023);
  blueValue = map(kelvin.Blue, 0, 255, 0, 1023);
  greenValue = map(kelvin.Green, 0, 255, 0, 1023);
}

void temperatureMode(){  
  int sensorValue = analogRead(A0);
  int temperature = map(sensorValue, 0, 1023, 1000, 5000);
  
  calculateTemperatureColors(temperature);
  rgbCalc();
  rgbShow();
}
