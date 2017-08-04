#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
// size of buffer used to capture HTTP requests
#define REQ_BUF_SZ   60

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 15, 104);    // IP address, may need to change depending on network
EthernetServer server(80);        // create a server at port 80
File webFile;                     // the web page file on the SD card
char HTTP_req[REQ_BUF_SZ] = {0};  // buffered HTTP request stored as null terminated string
char req_index = 0;               // index into HTTP_req buffer
unsigned char LED_state[70] = {0}; // stores the states of the LEDs, 1 bit per LED
char led[] = {0,2,3,5,6,7,8,9,14,15,16,17,18,19,20,21,22,23,24,25,
26,27,28,29,30,31,32,33,34,35,36,37,38,39,
40,41,42,43,44,45,46,47,48,49,50,51,52,53};

void setup()
{
    int i;
    
    // disable Ethernet chip
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    
    Serial.begin(9600);       // for debugging
    
    // initialize SD card
    Serial.println("Initializing SD card...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;    // init failed
    }
    Serial.println("SUCCESS - SD card initialized.");
    // check for index.htm file
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - Can't find index.htm file!");
        return;  // can't find index file
    }
    Serial.println("SUCCESS - Found index.htm file.");
    
    // pins 26 to 49 are outputs
    for (i = 0; i <= 60; i++) {
        pinMode(led[i], OUTPUT);    // set pins as outputs
        digitalWrite(led[i], LOW);  // switch the output pins off
    }
    
    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
}

void loop()
{
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // limit the size of the stored received HTTP request
                // buffer first part of HTTP request in HTTP_req array (string)
                // leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    // remainder of header follows below, depending on if
                    // web page or XML page is requested
                    // Ajax request - send XML file
                    if (StrContains(HTTP_req, "ajax_inputs")) {
                        // send rest of HTTP header
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();
                        SetLEDs();
                        // send XML file containing input states
                        XML_response(client);
                    }
                    else {  // web page request
                        // send rest of HTTP header
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");
                        client.println();
                        // send web page
                        webFile = SD.open("index.htm");        // open web page file
                        if (webFile) {
                            while(webFile.available()) {
                                client.write(webFile.read()); // send web page to client
                            }
                            webFile.close();
                        }
                    }
                    // display received HTTP request on serial port
                    //Serial.print(HTTP_req);
                    // reset buffer index and all buffer elements to 0
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}

// checks if received HTTP request is switching on/off LEDs
// also saves the state of the LEDs
void SetLEDs(void)
{
    char str_on[12] = {0};
    char str_off[12] = {0};
    unsigned char i;
    char led_ka;
    for (i = 1; i < 65; i++) {
            led_ka = led[i];
            sprintf(str_on,  "LED%d=%d", i, 1);
            sprintf(str_off, "LED%d=%d", i, 0);
            if (StrContains(HTTP_req, str_on)) {
              switch(i){
                  case 48:
                  pinMode(A0, OUTPUT);
                  digitalWrite(A0, HIGH);
                  break;
                  case 49:
                  pinMode(A1, OUTPUT);
                  digitalWrite(A1, HIGH);
                  break;
                  case 50:
                  pinMode(A2, OUTPUT);
                  digitalWrite(A2, HIGH);
                  break;
                  case 51:
                  pinMode(A3, OUTPUT);
                  digitalWrite(A3, HIGH);
                  break;
                  case 52:
                  pinMode(A4, OUTPUT);
                  digitalWrite(A4, HIGH);
                  break;
                  case 53:
                  pinMode(A5, OUTPUT);
                  digitalWrite(A5, HIGH);
                  break;
                  case 54:
                  pinMode(A6, OUTPUT);
                  digitalWrite(A6, HIGH);
                  break;
                  case 55:
                  pinMode(A7, OUTPUT);
                  digitalWrite(A7, HIGH);
                  break;
                  case 56:
                  pinMode(A8, OUTPUT);
                  digitalWrite(A8, HIGH);
                  break;
                  case 57:
                  pinMode(A9, OUTPUT);
                  digitalWrite(A9, HIGH);
                  break;
                  case 58:
                  pinMode(A10, OUTPUT);
                  digitalWrite(A10, HIGH);
                  break;
                  case 59:
                  pinMode(A11, OUTPUT);
                  digitalWrite(A11, HIGH);
                  break;
                  case 60:
                  pinMode(A12, OUTPUT);
                  digitalWrite(A12, HIGH);
                  break;
                  case 61:
                  pinMode(A13, OUTPUT);
                  digitalWrite(A13, HIGH);
                  break;
                  case 62:
                  pinMode(A14, OUTPUT);
                  digitalWrite(A14, HIGH);
                  break;
                  case 63:
                  pinMode(A15, OUTPUT);
                  digitalWrite(A15, HIGH);
                  break;
                  default:
                  digitalWrite(led_ka, HIGH);
                  break;
                  }
                   
                LED_state[i] = 1;  // save LED state
            }
            else if (StrContains(HTTP_req, str_off)) {
                LED_state[i] = 0;  // save LED state
                switch(i){
                  case 48:
                  pinMode(A0, OUTPUT);
                  digitalWrite(A0, LOW);
                  break;
                  case 49:
                  pinMode(A1, OUTPUT);
                  digitalWrite(A1, LOW);
                  break;
                  case 50:
                  pinMode(A2, OUTPUT);
                  digitalWrite(A2, LOW);
                  break;
                  case 51:
                  pinMode(A3, OUTPUT);
                  digitalWrite(A3, LOW);
                  break;
                  case 52:
                  pinMode(A4, OUTPUT);
                  digitalWrite(A4, LOW);
                  break;
                  case 53:
                  pinMode(A5, OUTPUT);
                  digitalWrite(A5,  LOW);
                  break;
                  case 54:
                  pinMode(A6, OUTPUT);
                  digitalWrite(A6, LOW);
                  break;
                  case 55:
                  pinMode(A7, OUTPUT);
                  digitalWrite(A7, LOW);
                  break;
                  case 56:
                  pinMode(A8, OUTPUT);
                  digitalWrite(A8,  LOW);
                  break;
                  case 57:
                  pinMode(A9, OUTPUT);
                  digitalWrite(A9,  LOW);
                  break;
                  case 58:
                  pinMode(A10, OUTPUT);
                  digitalWrite(A10, LOW);
                  break;
                  case 59:
                  pinMode(A11, OUTPUT);
                  digitalWrite(A11, LOW);
                  break;
                  case 60:
                  pinMode(A12, OUTPUT);
                  digitalWrite(A12, LOW);
                  break;
                  case 61:
                  pinMode(A13, OUTPUT);
                  digitalWrite(A13, LOW);
                  break;
                  case 62:
                  pinMode(A14, OUTPUT);
                  digitalWrite(A14, LOW);
                  break;
                  case 63:
                  pinMode(A15, OUTPUT);
                  digitalWrite(A15,  LOW);
                  break;
                  default:
                  digitalWrite(led_ka, LOW);
                  break;
                  }
            }
        }

}

// send the XML file with analog values, switch status
//  and LED status
void XML_response(EthernetClient cl)
{
    unsigned char i;
    
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
    for (i = 1; i < 64; i++) {
            cl.print("<LED>");
            if (LED_state[i]) {
                cl.print("checked");
               //* Serial.println("ON");
            }
            else {
                cl.print("unchecked");
            }
            cl.println("</LED>");
        }
    cl.print("</inputs>");
}

// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}
