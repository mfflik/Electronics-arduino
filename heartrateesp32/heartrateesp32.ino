
#include <DallasTemperature.h>
#include <Wire.h> 
#include <WiFi.h>
#include <WiFiMulti.h>
#include <LiquidCrystal_I2C.h>
 
//LCD
LiquidCrystal_I2C lcd(0x3F,16,2); // set the LCD address to 0x3F for a 16 chars and 2 line display
 
// SENSOR SUHU


WiFiMulti WiFiMulti;

//Inisialsi Thingspek and WIFI
const char* ssid     = "ini"; // Your SSID (Name of your WiFi)
const char* password = "12345678"; //Your Wifi password

const char* host = "api.thingspeak.com";
String api_key = "LRP6M5YPSZ2YG8PC"; // Your API Key provied by thingspeak

//Inisialisasi GSR
const int GSR = 35;
int BPMM;
float sensorValue;
float GSRvalue;

int Tempe;
int sensorPin = 32; 

//  VARIABLES HEARTRATE
int fadePin = 12;                 // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin
int LED_BUILTIN = 13;

// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded!
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

String TingkatStress;


void Intro()
{
  lcd.setCursor(0, 0);
  lcd.print("SANS PROJECT :  ");
  lcd.setCursor(0, 1);
  lcd.print("PERANGKAT     ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("ASISTEN DOKTER");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("UNTUK PENYAKIT"); 
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("STRESS        ");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("151354007     ");
  lcd.setCursor(0, 1);
  lcd.print("DANIEL S G    ");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("151354009     ");
  lcd.setCursor(0, 1);
  lcd.print("DITTO F       ");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("151354010     ");
  lcd.setCursor(0, 1);
  lcd.print("FEBIAN S      ");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("151354016     ");
  lcd.setCursor(0, 1);
  lcd.print("KHANSA F      ");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("151354018     ");
  lcd.setCursor(0, 1);
  lcd.print("M FAUZAN F     ");
  delay(1000);
}


void setup(){
  Serial.begin(9600);             // we agree to talk fast!

  lcd.init();// Print a message to the LCD.
  lcd.backlight();
  Intro(); 
  Connect_to_Wifi();
  Wire.begin(21,22);
  long sum = 0;
  pinMode(LED_BUILTIN,OUTPUT);      // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  pinMode(33, OUTPUT);
  pinMode(25, OUTPUT);
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS

  for (int i = 0; i < 500; i++)
  {
    sensorValue = analogRead(GSR);
    delay(5);
  }
  
}


void loop(){
//  sensors.requestTemperatures();  
//  Tempe = sensors.getTempCByIndex(0);
//  Serial.print("Temperature is: ");
//      Serial.println(Tempe); 
  sensorValue = analogRead(sensorPin);
  int saacan;
  saacan=(sensorValue / 4096.0) * 3300;
  Tempe=saacan/10;
  sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data

  if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
        fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
        sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
        sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
        QS = false;                      // reset the Quantified Self flag for next time   
        Serial.print("Temperature is: ");
        Serial.println(Tempe); 
        sensorValue = analogRead(GSR);
        GSRvalue = sensorValue/400;
        Serial.print("GSRvalue=");
        Serial.println(GSRvalue);
        Serial.print("BPM = "); 
        Serial.println(BPM);
  lcd.setCursor(0, 0);
  lcd.print("GSR:");
  lcd.setCursor(3, 0);
  lcd.print(GSRvalue);
  lcd.setCursor(6, 0);
  lcd.print("HR:");
  lcd.setCursor(9, 0);
  lcd.print(BPM);
  lcd.setCursor(11, 0);
  lcd.print("SH:");
  lcd.setCursor(14, 0);
  lcd.print(Tempe);
//Rileks
 if (GSRvalue<=2 && Tempe>=36 && Tempe<=39 && BPM<=70 ){
  TingkatStress="Rileks/Relaxed";}
 else if(GSRvalue<=2 && Tempe>=36 && Tempe<=39 && BPM>=70 && BPM<=90 ){
  TingkatStress="Rileks/Relaxed";}
 else if(GSRvalue<=4 && GSRvalue>=2 && Tempe>=36 && Tempe<=39 && BPM<=70 ){
  TingkatStress="Rileks/Relaxed";}
 else if(GSRvalue>=2 && GSRvalue<=4 && Tempe>=36 && Tempe<=39 && BPM>=70 && BPM<=90 ){
  TingkatStress="Rileks/Relaxed";}

//Tenang Calm
 else if(GSRvalue<=5 && GSRvalue>=2 && Tempe<=36 && Tempe>=35 && BPM<=90 && BPM>=70){
  TingkatStress="Tenang/Calm";}
 else if(GSRvalue>=3 && GSRvalue<=5 && Tempe<=36 && Tempe>=35 && BPM>=70){
  TingkatStress="Tenang/Calm";}
 else if(GSRvalue<=4 && GSRvalue>=1 && Tempe<=36 && Tempe>=35 && BPM<=100 && BPM>=90){
  TingkatStress="Tenang/Calm";}

//Cemas Tense  
 else if(GSRvalue>=4 && GSRvalue<=6 && Tempe<=35 && Tempe>=33 && BPM>=90 && BPM<=100){
   TingkatStress="Cemas/Tense";
   bukaseled1();
   }
 else if(GSRvalue>=3 && GSRvalue<=5 && Tempe<=35 && Tempe>=33 && BPM>=100){
   TingkatStress="Cemas/Tense";
   bukaseled1();
   }
 else if(GSRvalue>=4 && GSRvalue<=7 && Tempe<=35 && Tempe>=33 && BPM>=70 && BPM<=90){
   TingkatStress="Cemas/Tense";
   bukaseled1();
   }
   
//Tegang Stress   
 else if(GSRvalue>=6 && Tempe<=33 && BPM>=100){
   TingkatStress="Tegang/Stressed";
   bukaseled2();
   }
 else if(GSRvalue>=5 && GSRvalue<=7 && Tempe<=33 && BPM>=90 && BPM<=100){
   TingkatStress="Tegang/Stressed";
   bukaseled2();}
 else if(GSRvalue>=5 && GSRvalue<=6 && Tempe<=33 && BPM>=100){
   TingkatStress="Tegang/Stressed";
   bukaseled2();}     
 else{TingkatStress="Tidak Diketahui";}
      Send_Data();
  lcd.setCursor(0, 1);
  Serial.println(TingkatStress);
  lcd.print(TingkatStress);
     }

}


/*void ledFadeToBeat(){
    fadeRate -= 15;                         //  set LED fade value
    fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
    analogWrite(fadePin,fadeRate);          //  fade LED
  }*/


void sendDataToProcessing(char symbol, int data ){
  //  Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
  //Serial.println(data/10);                // the data to send culminating in a carriage return
  BPMM=data/10;
  }

void Send_Data()
{

  Serial.println("Prepare to send data");

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  {
    String data_to_send = api_key;
    data_to_send += "&field1=";
    data_to_send += String(BPM);
    data_to_send += "&field2=";
    data_to_send += String(Tempe);
    data_to_send += "&field3=";
    data_to_send += String(GSRvalue);
    data_to_send += "&field4=";
    data_to_send += String(TingkatStress);
    data_to_send += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);

    delay(1000);
  }

  client.stop();

}

void Connect_to_Wifi()
{

  // We start by connecting to a WiFi network
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi.......   ");
  lcd.setCursor(0, 1);
  lcd.print("                         ");
  
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0, 0);
  lcd.print("Wifi Connected        ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
}
 void bukaseled1()
 {
  digitalWrite(33, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(33, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
 }
 void bukaseled2()
 {
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
 }
 

