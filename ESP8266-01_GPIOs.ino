/*Como utilizar so dois GPIOs acessiveis no ESP8266-01
* um deles liga um rele energizando sua bobina de 5V com um transistor como driver
* devido a diferença de potencial entre o rele 5V e a tensão do ESP8266 3V3, utilizei um artificio simples e eficaz
* colocando um LED em serie com a base do transistor.
*/ 


 
#include <ESP8266WiFi.h>
 
const char* ssid = "REDE";
const char* password = "SENHA";
 
WiFiServer server(80);
 
void setup() {
    //Serial.begin(115200),SERIAL_8N1,SERIAL_TX_ONLY);
  Serial.begin(115200);
  delay(10);
 
  // prepare GPIO0
  pinMode(0, OUTPUT);
  digitalWrite(0, 0);
 
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
 
  String buf = "";
 
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n";
  buf += "<html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>\r\n";
  buf += "<title>ESP8266 Web Server</title>";
  buf += "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:80%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;}</style>";
  buf += "</head>";
  buf += "<h3>ESP8266 Web Server - ESP8266-01</h3>";
  buf += "<p>GPIO0 <a href=\"?function=led0_off\"><button>OFF</button></a> <a href=\"?function=led0_on\"><button>ON</button></a></p>";
  buf += "<p>GPIO2 <a href=\"?function=led2_on\"><button>ON</button></a> <a href=\"?function=led2_off\"><button>OFF</button></a></p>";
  buf += "<h4>CK Eletronica</h4>";
  buf += "</html>\n";
 
  client.print(buf);
  client.flush();
 
  if (req.indexOf("led2_on") != -1)
    digitalWrite(2, 1);
  else if (req.indexOf("led2_off") != -1)
    digitalWrite(2, 0);
  else if (req.indexOf("led0_off") != -1)
    digitalWrite(0, 0);
  else if (req.indexOf("led0_on") != -1)
    digitalWrite(0, 1);
  else {
    Serial.println("invalid request");
    client.stop();
  }
  Serial.println("Client disonnected");
}