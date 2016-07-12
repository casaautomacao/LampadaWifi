/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>
#include <Crescer.h>
Tempora tempCom;

//const char* ssid = "Crescera automacao";
//const char* password = "casadaautomacao";
const char* ssid = "CasaMN";
const char* password = "caiorafael123";

IPAddress ip(192, 168, 0, 14); 
IPAddress ip1(192, 168, 0, 1);
IPAddress ip2(255, 255, 255, 0);


// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(90);

void setup() {

  tempCom.defiSP(5000);
  
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
    pinMode(0, OUTPUT);
  digitalWrite(0, 1);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);


//   void ESP8266WiFiClass::config(IPAddress local_ip, IPAddress gateway, IPAddress subnet)
  
  WiFi.config(ip, ip1, ip2); 
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {

delay(1);
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);


  if (tempCom.Saida(1))
    {
      Serial.println("Tempo excedeu");
      client.flush();

       String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nNAO COMUNICOU !!! ";
       s += "</html>\n";
    
      // Send the response to the client
      client.print(s);
      delay(1);
      Serial.println("Client disconectado depois da nao comunicacao");

      tempCom.Saida(0);
      return;
    }




    
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  int val2;
  if (req.indexOf("/gpio/0") != -1)
  {  val = 1;
    val2 = 1;}
  else if (req.indexOf("/gpio/1") != -1)
   { val = 1;
       val2 = 0;}
 else if (req.indexOf("/gpio/2") != -1)
{    val = 0;
       val2 = 1;}
        else if (req.indexOf("/gpio/3") != -1)
 {   val = 0;
       val2 = 0;}


       
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(2, val);
    digitalWrite(0, val2);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

