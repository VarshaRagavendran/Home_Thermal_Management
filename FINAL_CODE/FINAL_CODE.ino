// Include the ESP8266 WiFi library. (Works a lot like the
// Arduino WiFi library.)
#include <ESP8266WiFi.h>
// Include the SparkFun Phant library.
#include <Phant.h>
#include <ER_MicroOLED.h> // Include the ER_MicroOLED library
#define PIN_RESET 5 // Connect RST to pin 9 (req. for SPI and I2C)
//#define PIN_DC 8 // Connect DC to pin 8 (required for SPI)
//#define PIN_CS 10 // Connect CS to pin 10 (required for SPI)
#define DC_JUMPER 1
// Also connect pin 13 to SCK and pin 11 to MOSI
// Declare a MicroOLED object. The parameters include:
// 1 – Reset pin: Any digital pin
// 2 – D/C pin: Any digital pin (SPI mode only)
// 3 – CS pin: Any digital pin (SPI mode only, 10 recommended)
//MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS);
MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C declaration
#include <ArduinoJson.h>


//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiSSID[] = "The Better S7";
const char WiFiPSK[] = "6476695657";

/////////////////////
// Pin Definitions //
/////////////////////
/*const int LED_PIN = 5; // Thing's onboard, green LED
const int ANALOG_PIN = A0; // The only analog pin on the Thing
const int DIGITAL_PIN = 12; // Digital pin to be read */

////////////////
// Phant Keys //
////////////////
const char PhantHost[] = "data.sparkfun.com";
const char PublicKey[] = "NJWV7M7xEWCGZdnorqg9";
const char PrivateKey[] = "5dPvwjwE7PIVKJGkNR6M";

/////////////////
// Post Timing //
/////////////////
const unsigned long postRate = 30000;
unsigned long lastPost = 0;
int speakerPin = 4;


int length = 2; // the number of notes
char notes[] = "ab"; // a space represents a rest
int beats[] = { 1, 1, };
int tempo = 300;
void setup() 
{
  initHardware();
  connectWiFi();
  //digitalWrite(LED_PIN, HIGH);
  pinMode(speakerPin, OUTPUT);
}

void loop() 
{
  if (lastPost + postRate <= millis())
  {
    if (postToPhant())
      lastPost = millis();
    else
      delay(100);    
  }
}

void connectWiFi()
{
 // byte ledStatus = LOW;

  // Set WiFi mode to station (as opposed to AP or AP_STA)
  WiFi.mode(WIFI_STA);

  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
  // to the stated [ssid], using the [passkey] as a WPA, WPA2,
  // or WEP passphrase.
  WiFi.begin(WiFiSSID, WiFiPSK);

  // Use the WiFi.status() function to check if the ESP8266
  // is connected to a WiFi network.
  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink the LED
   // digitalWrite(LED_PIN, ledStatus); // Write LED high/low
    //ledStatus = (ledStatus == HIGH) ? LOW : HIGH;

    // Delays allow the ESP8266 to perform critical tasks
    // defined outside of the sketch. These tasks include
    // setting up, and maintaining, a WiFi connection.
    delay(100);
    // Potentially infinite loops are generally dangerous.
    // Add delays -- allowing the processor to perform other
    // tasks -- wherever possible.
  }
}

void initHardware()
{
  Serial.begin(9600);
  //pinMode(DIGITAL_PIN, INPUT_PULLUP);
  //pinMode(LED_PIN, OUTPUT);
  //digitalWrite(LED_PIN, LOW);
  // Don't need to set ANALOG_PIN as input, 
  // that's all it can be.
}

int postToPhant()
{
  // LED turns on when we enter, it'll go off when we 
  // successfully post.
  //digitalWrite(LED_PIN, HIGH);

  // Declare an object from the Phant library - phant
  Phant phant(PhantHost, PublicKey, PrivateKey);


  // Add the four field/value pairs defined by our stream:
  //phant.add("temp", 85);
  //phant.add("sensor",1);

    // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(PhantHost, httpPort)) {
    Serial.println("connection failed");
    return 0;
  }

  // If we successfully connected, print our Phant post:
  //client.print(phant.post());
  
  // We now create a URI for the request
  String url = "/output/";
  url += PublicKey;
  url += ".json?limit=4";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + PhantHost + "\r\n" +
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial
  // Response is shown between the lines
  Serial.println("_____________________________");
  String line = "";
 // const char *b[2];
  while(client.available()){
    line = client.readString();
    Serial.print(line);
   // b[0] = line.c_str();
  }
  Serial.println("PRINTING LINES OF ARRAY");
 /* int i = 0;
  for(i=0; i < 4 ; i++){
    Serial.println(b[i]);
  }*/
 // Serial.println(b[0]);
if(line.length() == 0){
  return 0;
}
int firstbracket = line.indexOf('[');
  int secondbracket = line.indexOf(']', firstbracket + 1);
  line.remove(secondbracket+1);
  line.remove(0,firstbracket);
 /* char json[512];
//  json = line;

//-------------------------------
  StaticJsonBuffer<200> jsonBuffer; //JSON
  // Root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
    Serial.println(line);

  JsonObject& root = jsonBuffer.parseObject(line);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return 0;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();
  const char* sensor = root["sensor"].asString();
const char* temp = root["temp"].asString();
  // Print values.
  Serial.println(sensor);
  Serial.println(temp); */

    int firstOpenBracket = line.indexOf('{');
    int secondOpenBracket = line.indexOf('{', firstOpenBracket+1);
    int thirdOpenBracket = line.indexOf('{', secondOpenBracket+1);
    int fourthOpenBracket = line.indexOf('{', thirdOpenBracket+1);
      
    String firstValue = line.substring(firstOpenBracket, secondOpenBracket-5);
    String secondValue = line.substring(secondOpenBracket, thirdOpenBracket-5);
    String thirdValue = line.substring(thirdOpenBracket, fourthOpenBracket-5);
    String fourthValue = line.substring(fourthOpenBracket, line.length()-4);

    //{"sensor":"15","temp":"5","timestamp":"2017-03-26T09:02:22.702Z"}


    /** Get time of first sensor **/
    int firstSCharacter = firstValue.indexOf('s');

    String timeOfValue = line.substring(firstSCharacter + 52, firstSCharacter + 57);
    Serial.print("Time stamp: ");
    Serial.println(timeOfValue);  

    /** FIRST SENSOR & TEMPERATURE VALUE **/
    
    String valueOfFirstSensor ="";
    if(!firstValue.charAt(firstSCharacter +10) == '"'){
      
        valueOfFirstSensor = firstValue.substring(firstSCharacter + 9, firstSCharacter + 10);
      
    }
    else{
      
        valueOfFirstSensor = firstValue.substring(firstSCharacter + 9,firstSCharacter + 10);
      
    }
    Serial.print("VALUE OF 1st SENSOR: ");
    Serial.println(valueOfFirstSensor);
    int firstTCharacter = firstValue.indexOf('t');
    String valueOfFirstTemp ="";
    if(!firstValue.charAt(firstTCharacter + 9) == '"'){
      
        valueOfFirstTemp = firstValue.substring(firstTCharacter + 7, firstTCharacter + 12);
      
    }
    else{
      
        valueOfFirstTemp = firstValue.substring(firstTCharacter + 7,firstTCharacter + 12);
      
    }
    Serial.print("VALUE OF 1st TEMP: ");
    Serial.println(valueOfFirstTemp);

    /** SECOND SENSOR & TEMPERATURE VALUE **/
    int secondSCharacter = secondValue.indexOf('s');
    String valueOfSecondSensor ="";
    if(!secondValue.charAt(secondSCharacter +10) == '"'){
      valueOfSecondSensor = secondValue.substring(secondSCharacter + 9, secondSCharacter + 10);
    }
    else{
      valueOfSecondSensor = secondValue.substring(secondSCharacter + 9,secondSCharacter + 10);
    }    
    Serial.print("VALUE OF 2nd SENSOR: ");
    Serial.println(valueOfSecondSensor);
    int secondTCharacter = secondValue.indexOf('t');
    String valueOfSecondTemp ="";
    if(!secondValue.charAt(firstTCharacter + 9) == '"'){
      valueOfSecondTemp = secondValue.substring(secondTCharacter + 7, secondTCharacter + 12);
    }
    else{
      valueOfSecondTemp = secondValue.substring(secondTCharacter + 7,secondTCharacter + 12);
    }
    Serial.print("VALUE OF 2nd TEMP: ");
    Serial.println(valueOfSecondTemp);

     /** THIRD SENSOR & TEMPERATURE VALUE **/
    int thirdSCharacter = thirdValue.indexOf('s');
    String valueOfThirdSensor ="";
    if(!thirdValue.charAt(thirdSCharacter +10) == '"'){
      valueOfThirdSensor = thirdValue.substring(thirdSCharacter + 9, thirdSCharacter + 10);
    }
    else{
      valueOfThirdSensor = thirdValue.substring(thirdSCharacter + 9,thirdSCharacter + 10);
    } 
    Serial.print("VALUE OF 3rd SENSOR: ");
    Serial.println(valueOfThirdSensor);
    int thirdTCharacter = thirdValue.indexOf('t');
     String valueOfThirdTemp ="";
    if(!thirdValue.charAt(firstTCharacter + 9) == '"'){
      valueOfThirdTemp = thirdValue.substring(thirdTCharacter + 7, thirdTCharacter + 12);
    }
    else{
      valueOfThirdTemp = thirdValue.substring(thirdTCharacter + 7,thirdTCharacter + 12);
    }
    Serial.print("VALUE OF 3rd TEMP: ");
    Serial.println(valueOfThirdTemp);

    /** FOURTH SENSOR & TEMPERATURE VALUE **/
    int fourthSCharacter = fourthValue.indexOf('s');
    String valueOfFourthSensor ="";
    if(!fourthValue.charAt(fourthSCharacter +10) == '"'){
      
        valueOfFourthSensor = fourthValue.substring(fourthSCharacter + 9, fourthSCharacter + 10);
      
    }
    else{
      valueOfFourthSensor = fourthValue.substring(fourthSCharacter + 9,fourthSCharacter + 10);
    }     
    Serial.print("VALUE OF 4th SENSOR: ");
    Serial.println(valueOfFourthSensor);
    int fourthTCharacter = fourthValue.indexOf('t');
    String valueOfFourthTemp ="";
    if(!fourthValue.charAt(firstTCharacter + 9) == '"'){
      valueOfFourthTemp = fourthValue.substring(fourthTCharacter + 7, fourthTCharacter + 12);
    }
    else{
      valueOfFourthTemp = fourthValue.substring(fourthTCharacter + 7,fourthTCharacter + 12);
    }    
    Serial.print("VALUE OF 4th TEMP: ");
    Serial.println(valueOfFourthTemp);

    /*****************Temperature Difference***************************/

    float roomOne = valueOfThirdTemp.toFloat() - valueOfFirstTemp.toFloat();
    float roomTwo = valueOfFourthTemp.toFloat() - valueOfSecondTemp.toFloat();

  if( roomOne < 5 && roomOne > -5 || roomTwo < 5 && roomTwo > -5  ) {
 
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    
    // pause between notes
    delay(tempo / 1);
}
  }


    
    /** For now OLED to display just 1 reading **/
    oled.begin();
    oled.clear(PAGE);
    oled.clear(ALL);
    oled.setCursor(0,0);
    oled.setTextColor(WHITE);
    oled.setTextSize(0);
    /********* Room 1*****************/
    oled.print("Room: ");
    oled.println(valueOfFirstSensor);
    oled.println("Change: ");
    oled.print(roomOne);
    oled.print(char(248));
    oled.println("C");
    /********* Room 2****************/
    oled.print("Room: ");
    oled.println(valueOfSecondSensor);
    oled.println("Change: ");
    oled.print(roomTwo);
    oled.print(char(248));
    oled.println("C");
    oled.display(); 
  
  Serial.println("_____________________________");

  Serial.println();
  Serial.println("closing connection");

  return 1; // Return success
}
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}


void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
