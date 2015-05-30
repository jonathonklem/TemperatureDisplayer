/*
  Temperature Reporter LCD Version 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
// 10, 11, 12, 13 is used by Ethernet shield
LiquidCrystal lcd(8, 9, 4, 3, 2, 1);

// edit this to the analog pin that your temperature transistor is connected to
const int temperaturePin = 3;

// Enter a MAC address for your controller below.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Your website's domain
char server[] = "64.90.61.80";    

// Set the static IP address to use if the DHCP fails to assign
// be sure to set this to a valid ip in your subnet
IPAddress ip(192,168,0,100);

// This will be used to interact with ethernet shield
EthernetClient client;

// This keeps track of whether or not we have sent out a GET request or are waiting for a response
boolean sentRequest = false;

// keeps track of "|" delimeters for printing LCD
boolean inSection = false;

int lcdLine = 0; // which line of LCD are we on?
int lcdColumn = 0; // which column are we on?

void setup() {
  // start the Ethernet connection:
  Ethernet.begin(mac, ip);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  
    // Print a message to the LCD.
  lcd.print("Initializing...");
  
  // give the Ethernet shield a second to initialize:
  delay(150);
}

void loop()
{
  if (!sentRequest) { 
    httpRequest();
    sentRequest = true;
  } else {
    // if there are incoming bytes available 
    // from the server, read them and print them:
    if (client.available()) {
      char c = client.read();
      
      if (c == '|') {
        if (lcdLine > 2) {
           lcdLine = 0;
           lcd.clear();
        }
        
        // were we previously not printing?
        if (!inSection) {
          lcd.clear();
        }
        
        lcdColumn = 0; 
        
        lcd.setCursor(lcdColumn, lcdLine);
        
        inSection = true;
        lcdLine = lcdLine + 1;
       
      } else {
        if (inSection) {
          lcd.print(c);
          lcdColumn = lcdColumn +1;
          lcd.setCursor(lcdColumn, lcdLine-1);
        }
      }
    }
  
    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      client.stop();
      
      delay(15000); // repeat once every quarter minute
      sentRequest = false;
    }
  }
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    // send the HTTP PUT request:
    client.println("GET /weather.php HTTP/1.1");
    client.println("Host: www.jonathonklem.com");
    client.println("Connection: close");
    client.println();
  } 
  else {
    // if you couldn't make a connection:
    client.stop();
  }
  
}
