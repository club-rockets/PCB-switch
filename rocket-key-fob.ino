#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//Adress IP : 192.168.4.1

#define ssid "Secret Wifi Arming" // WiFi SSID
#define password "password"       // WiFi password

struct Relay
{
    String name;
    int pin_on;
    int pin_off;
    bool status;

    Relay(String name, int on, int off)
    {
        Relay::name = name;
        pin_on = on;
        pin_off = off;

        pinMode(pin_on, OUTPUT);
        digitalWrite(pin_on, LOW);
        pinMode(pin_off, OUTPUT);
        digitalWrite(pin_off, HIGH);

        status = false;
    }

    String button()
    {
        String html = "<button class=\"";
        html += (status ? "armed" : "disarmed");
        html += "\" type=\"submit\" name=\"" + name + "\">";
        html += name + (status ? " - ARMED" : " - DISARMED");
        html += "</button>";
        return html;
    }
};

Relay *relays[3] = {nullptr, nullptr, nullptr};
String infolog;
// Création des objets / create Objects
ESP8266WebServer server(80);

String makeButton(Relay *relay)
{
}

String getPage()
{
    String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
    page += "<title>RockETS</title>";
    page += "<style> body { background: black; font-family: Source Code Pro, monospace; Color: white } button { width: 100%; height: 20%; font-size: 3em; border:none; margin-bottom: 1% } button.disarmed { background: green } button.armed { background: red } div#log { height: 20%; overflow: scroll }</style>";
    page += "</head><body><h1>RockETS</h1>";
    page += "<form action='/' method='POST'>";
    page += relays[0]->button() + relays[1]->button() + relays[2]->button();
    page += "</form><div id=\"log\">";
    page += infolog;
    page += "</div></body></html>";
    return page;
}
void handleRoot()
{
    //Channel 1
    for (int i = 0; i < 3; ++i)
    {
        if (server.hasArg(relays[i]->name))
        {
            relays[i]->status = !relays[i]->status;
            digitalWrite(relays[i]->pin_off, LOW);
            digitalWrite(relays[i]->pin_off, LOW);
            digitalWrite(relays[i]->pin_off, !relays[i]->status);
            digitalWrite(relays[i]->pin_on, relays[i]->status);

            String line = "switched " + relays[i]->name + (relays[i]->status ? " ON" : " OFF") + "<br/>";
            infolog = line + infolog;
        }
    }
    server.send(200, "text/html", getPage());
}

void setup()
{
    WiFi.softAP(ssid, password);

    server.on("/", handleRoot);
    server.begin();

    relays[0] = new Relay("VBATT", 14, 16);
    relays[1] = new Relay("DROGUE", 4, 5);
    relays[2] = new Relay("MAIN", 13, 12);
}

void loop()
{
    server.handleClient();
    delay(200);
}
