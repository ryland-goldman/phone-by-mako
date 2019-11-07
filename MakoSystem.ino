/*
 * 
 * Phone by Mako
 * System 1.2
 * 
 * For Installation Instructions, See Page 28 of Instruction Manual
 * Download Arduino IDE from https://arduino.cc/
 * (Arduino IDE does not work on MacOS Catalina)
 * 
 * Copyright (c) 2019 Phone by Mako
 * 
 */
#include <Keypad.h>
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include <U8glib.h>

U8GLIB_SH1106_128X64 u8g(5, 3, 11, 6, 10);

#define FONA_RX  9
#define FONA_TX  8 
#define FONA_RST 4
#define FONA_RI  7

int yPos = -16;

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {0, A5, A4, A3};
byte colPins[COLS] = {A2, A1, A0};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

char replybuffer[255];

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

uint8_t type;

unsigned long lastDisplay = millis();

String dispTime;
String dispDate;
String callStatus;
String stopMode = "Start";

int8_t i_8;
int8_t dbi;
int16_t i_16;
int16_t vbat;

char favorites[][20]={"1111111111","0000000000","0000000000","0000000000","0000000000"};

void setup() {
  // clear the screen
  // left from last "on"
    u8g.firstPage();  
  do {
  } while( u8g.nextPage() );
    //delay(1000);
 
  Serial.begin(9600);
  
  u8g.firstPage();  
  do {
    msgConnect();
  } while( u8g.nextPage() );
  
   //delay(2000);
  fonaSS.begin(9600);
 
  if (! fona.begin(fonaSS)) {
    Serial.println(F("Couldn't find FONA :("));
    
    while (1);
  }
  
  while (fona.getNetworkStatus() != 1) {
    u8g.firstPage();  
  do {
    msgNetService();
  } while( u8g.nextPage() );
   
  }
  
    u8g.firstPage();  
  do {
    msgNetConnect();
  } while( u8g.nextPage() );
    delay(2000);
 
  fona.setAudio(FONA_EXTAUDIO);

  char imei[15] = {0};
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print(F("Module IMEI: ")); 
    Serial.println(imei);
  }
}

void answerPhone()
{
  if (! fona.pickUp()) {
     Serial.println(F("Failed"));
  }
}

void msgConnect()
{
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr(26, 32,"Phone by Mako");
}

void msgNetService()
{
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr(42, 32,"Welcome");
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr(yPos, 40,"...");

  if(yPos < 127){
    yPos++;  }
  else{
    yPos = -16;
  }  
}

void msgNetConnect() {
   //u8g.setFont(u8g_font_lucasfont_alternater);
   u8g.setFont(u8g_font_6x12);
   u8g.drawStr(42, 32,"Welcome");
}

int8_t getRSSI (int8_t i) {
  uint8_t n = fona.getRSSI();
  int8_t r;
  
  if (n == 0) r = -115;
  if (n == 1) r = -111;
  if (n == 31) r = -52;
  if ((n >= 2) && (n <= 30)) {
    r = map(n, 2, 30, -110, -54);
  }
  return(r);
}

uint16_t getBattery(uint16_t i) {
  uint16_t vbat; 
  if (! fona.getBattPercent(&vbat)) {
  } else {
    return(vbat);
  }
}

void cellDate() {
 char buffer[23];
 fona.getTime(buffer, 23);  // make sure replybuffer is at least 23 bytes!
 
 String Str1 = buffer;
 dispDate = Str1.substring(4,9);
 dispDate.concat("/");
 dispDate.concat(Str1.substring(1,3));
}

void cellTime() {
  char buffer[23]; 
  
  fona.getTime(buffer, 23);
  String Str1 = buffer;
  
  String Str2;
  
  int rptHour = (Str1.substring(10,12).toInt());
  int rptMinutes = (Str1.substring(13,15).toInt());
  int i = rptHour;
  
  if ( rptHour > 12 ) {
    rptHour = rptHour-12;
  }
  
  Str2.concat(rptHour);
  Str2.concat(":");
  if (rptMinutes < 10)
   { Str2.concat(0);}
   Str2.concat(rptMinutes);
  if ( i >= 12 )
    { Str2.concat(" PM");}
  else
    { Str2.concat(" AM");}
  
  dispTime = Str2;

}

void callEntry() {
  char phoneNumber[20]= " ";
  char key = keypad.getKey();
  int i = 0;
  callStatus = "Dial a Number";
  //Display the page!
  u8g.firstPage();
  do { 
        callEntryDisplay(phoneNumber);
      } while( u8g.nextPage() );
      
  while ((key != '*') and (key != '#')) {
    key = keypad.getKey();
   
    if (key) {
      phoneNumber[i] = key;
      i++;
      u8g.firstPage();  
      do {
    
        callEntryDisplay(phoneNumber); 
      } while( u8g.nextPage() );
    }
    
  }
  
  int length = strlen(phoneNumber);
  phoneNumber[length-1] = '\0';   
  
  if (key == '#') {
    return;
  }
  
  if (key == '*') { 
  callStatus = "Call in Progress";
  
  u8g.firstPage();  
      do {
    
        callEntryDisplay(phoneNumber); 
      } while( u8g.nextPage() ); 
      
  makeCall(phoneNumber);}
}

void makeCall(char phoneNumber[20]) { 
  char key = keypad.getKey();
  if (!fona.callPhone(phoneNumber)) 
      {
        callStatus = " Call Failure";
      }
  while (key != '#')
  {
  key = keypad.getKey();
  
  if (key)
   {
    
    u8g.firstPage();  
    do 
    {
      callEntryDisplay(phoneNumber); 
    } while( u8g.nextPage() );
   }
   int callstatus = fona.getCallStatus();
   if (callstatus == 2)   // phone is currently ringing
   {
    // use ringing tone AT+STONE 8
   }
  }
  if (! fona.hangUp()) {
    Serial.println(F("Failed"));
  }
}

void callEntryDisplay(char inNumber[20]) { 
  //u8g.setFont(u8g_font_lucasfont_alternater);
  u8g.setFont(u8g_font_6x12);
  
  u8g.setPrintPos(0,7);
  u8g.print(callStatus);
  u8g.setPrintPos(30,32);
  u8g.print(inNumber);
}

void favsEntryDisplay(){ 
  
  u8g.setFont(u8g_font_6x12);
  
  u8g.setPrintPos(0,7);
  u8g.print(F("Contacts"));
  u8g.setPrintPos(0, 20);
  u8g.print(F("1 Collin Wentzien"));
  u8g.setPrintPos(0, 30);
  u8g.print(F("2 Ryland Goldman"));
  u8g.setPrintPos(0, 40);
  u8g.print(F("3 Jonhnny Appleseed"));
  u8g.setPrintPos(0, 50);
  u8g.print(F("4 Miranda Smith"));
  u8g.setPrintPos(0, 60);
  u8g.print(F("5 Mary Green"));
}


void callFavorites() {
 char phoneNumber[20] = " ";
 char key = keypad.getKey();
 int i = 0;
 //Display the page!
 u8g.firstPage();  
 do { 
        favsEntryDisplay(); 
      } while( u8g.nextPage() );
      
while ((key != '*') and (key != '#')) {
    key = keypad.getKey();
   
    if (key)
      if ((key != '*') and (key != '#'))
    { // convert a single character to int
      int ikey = key - '0';
     strcpy(phoneNumber, favorites[ikey-1]);
      u8g.firstPage();  
      do {
    
        callEntryDisplay(phoneNumber); 
      } while( u8g.nextPage() );
     }
   }  
   
// remove "*" from string
int length = sizeof(phoneNumber);
phoneNumber[length-1] = '\0';   

if (key == '#')
{ return;}

if (key == '*')
{ callStatus = " Calling ";

  u8g.firstPage();  
      do {
    
        callEntryDisplay(phoneNumber); 
      } while( u8g.nextPage() ); 
      
  makeCall(phoneNumber);}
}


void keyPress()
{
  char key = keypad.getKey();

    // Answer an incoming call
  if (key == '*')
  { answerPhone();
    
    return;
    }


// convert a single character to int
  int ikey = key - '0';
  
  switch (ikey) {
    case 1:
      callEntry();
      break;
    case 2:
      callFavorites();
      break;
    case 3:
      startPong();
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }
  
}

void dispMenu(int8_t dbi, uint16_t vbat) //HOME
{   // set font 
    //u8g.setFont(u8g_font_lucasfont_alternater);
      u8g.setFont(u8g_font_6x12);
      
    // draw variable battery level and per cent battery remaining
    u8g.drawFrame(111,0,14,7);
    u8g.drawBox(126,2,2,3);
    
    u8g.drawBox(113,2,(10 * (vbat *.01)),3);     
    u8g.setPrintPos(86,7);
    u8g.print(vbat);
    u8g.print("%");

    // draw text menu
    u8g.drawStr(0, 26,"1 Dial");
    u8g.drawStr(0, 40,"2 Contacts");
    u8g.drawStr(0, 54,"3 Pong");

    // display the time
    u8g.setPrintPos(0, 7);
    u8g.print(dispTime);
}

void loop() {
  // don't hog the processor!
  if (millis() - lastDisplay > 10000) {
    dbi = getRSSI(i_8);
    vbat = getBattery(i_16);
    cellDate();
    cellTime();
    lastDisplay = millis();
  }
  u8g.firstPage();  
  do {
    dispMenu(dbi, vbat);
    keyPress(); 
  } while( u8g.nextPage() );
}



/***************************** PONG *****************************/

int player1y = 25;
int player2y = 25;
int p1s = 0;
int p2s = 0;
int lineLength = 15;

long ballx = 64.0;
long bally = 32.0;
long directionx = 1.0;
long directiony = 2.0;
int ballspeed = 0;
int prev = millis();

void displayPong(){
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos(53, 10);
  u8g.print(p1s);
  u8g.setPrintPos(72, 10);
  u8g.print(p2s);
  u8g.drawLine(64, 64, 64, 0); 
  u8g.drawLine(2, player1y, 2, player1y+lineLength);
  u8g.drawLine(3, player1y, 3, player1y+lineLength);
  u8g.drawLine(126, player2y, 126, player2y+lineLength);
  u8g.drawLine(125, player2y, 125, player2y+lineLength);
  u8g.drawCircle(ballx,bally,2);
}
void movePlayer1Up(){
  if(player1y > 4){
    player1y = player1y-3;
  }
}
void movePlayer1Down(){
  if(player1y < 64-lineLength){
    player1y = player1y+3;
  }
}
void movePlayer2Up(){
  if(player2y > 4){
    player2y = player2y-3;
  }
}
void movePlayer2Down(){
  if(player2y < 64-lineLength){
    player2y = player2y+3;
  }
}

void startPong(){
  p1s = 0;
  p2s = 0;
  player1y = 25;
  player2y = 25;
  ballx = random(60,70);
  bally = random(20,40);
  directiony = random(1,2);
  directionx = random(1,2);
  ballspeed = random(7,13);
  char key = keypad.getKey();
  while (key != '#')
  {
    key = keypad.getKey();
    if(key == '#'){
      break;
    }


    //if(millis() >= prev+10){
      ballx = ballx+directionx;
      bally = bally+directiony;
      if(bally <= 1 || bally >= 63){
        directiony = 0-directiony;
      }
      if(ballx <= 3){
        if(player1y == bally || player1y == bally-1 || player1y == bally-2 || player1y == bally-3 || player1y == bally-4 || player1y == bally-5 || player1y == bally-6 || player1y == bally-7 || player1y == bally-8 || player1y == bally-9 || player1y == bally-10 || player1y == bally-11 || player1y == bally-12 || player1y == bally-13 || player1y == bally-14){
          directionx = 0-directionx;
        } else {
          p2s = p2s + 1;
          ballx = random(60,70);
          bally = random(20,40);
          directiony = random(1,2);
          directionx = random(1,2);
          ballspeed = random(7,13);
        }
      }
      if(ballx >= 125){
        if(player2y == bally || player2y == bally-1 || player2y == bally-2 || player2y == bally-3 || player2y == bally-4 || player2y == bally-5 || player2y == bally-6 || player2y == bally-7 || player2y == bally-8 || player2y == bally-9 || player2y == bally-10 || player2y == bally-11 || player2y == bally-12 || player2y == bally-13 || player2y == bally-14){
          directionx = 0-directionx;
        } else {
          p1s = p1s + 1;
          ballx = random(60,70);
          bally = random(20,40);
          directiony = random(1,2);
          directionx = random(1,2);
          ballspeed = random(7,13);
        }
      //}
      delay(10);
    }

    if(key == '1'){
      movePlayer1Up();
    }
    if(key == '4'){
      movePlayer1Down();
    }
    if(key == '3'){
      movePlayer2Up();
    }
    if(key == '6'){
      movePlayer2Down();
    }
    u8g.firstPage();  
    do 
    {
      displayPong();
    } while( u8g.nextPage() );
}}
