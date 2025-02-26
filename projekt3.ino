#include "WiFi.h"
#include "ESPAsyncWebServer.h"


// Replace with your network credentials
const char* ssid = "ZTE_5A1BF7";
const char* password = "2P78CW22PV";



// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String ocitajTemp() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature(); // prilagodit na nase senzore
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Neuspjesno ocitanje sa senzora temperature!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}


String ocitajVlaz() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Neuspjesno ocitanje sa senzora vlage!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

String ocitajPritisak() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float p = dht.readPressure(); // prilagodit na nase senzore
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(p)) {    
    Serial.println("Neuspjesno ocitanje sa senzora pritiska!");
    return "--";
  }
  else {
    Serial.println(p);
    return String(p);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 sensor Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperatura</span> 
    <span id="temperatura">%TEMPERATURA%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Vlaznost</span>
    <span id="vlaznost">%VLAZNOST%</span>
    <sup class="units">&percnt;</sup>
  </p>
   <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Pritisak</span>
    <span id="pritisak">%PRITISAK%</span>
    <sup class="units">&percnt;</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperatura").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperatura", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("vlaznost").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/vlaznost", true);
  xhttp.send();
}, 10000 ) ;
</script>


setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pritisak").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pritisak", true);
  xhttp.send();
}, 10000 ) ;
</html>)rawliteral";


// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readTemperature();
  }
  else if(var == "HUMIDITY"){
    return readHumidity();
  }
  else {
    return readPressure();
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readHumidity().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPressure().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}