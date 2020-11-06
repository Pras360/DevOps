#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

//Include Sensor
#include "Sensor/DigitalSensor/DigitalSensor.h"
#include "Sensor/IRDistance/IRDistance.h"
//------------------------------------------------------------------------------

//Include Aktuator
#include "Aktuator/Lamp/Lamp.h"
#include "Aktuator/Led/Led.h"
//------------------------------------------------------------------------------

//Include Pengaturan Perangkat
#include "PengaturanPerangkat/EEPROMData/EEPROMData.h"
#include "PengaturanPerangkat/HTMLForm/HTMLForm.h"
//------------------------------------------------------------------------------

#include "FirmwareInformation/FirmwareInformation.h"
#include "OTAUpdate/OTAUpdate.h"

#include "../lib/MQTT/PubSubClient.h"
#include "../ESP8266Scheduler/Scheduler.h"

#include "MeshCom/MeshCom.h"

const char *ap_ssid = "Lampu_Jalan_Device";
const char *ap_password = "";
ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient MQTT(espClient);

HTMLForm form;
EEPROMData pengaturan;

DigitalSensor jumper(5);
IRDistance jarak(A0);
Lamp lampu(4);
Led led(LED_BUILTIN);

String session;
OTAUpdate update;

MeshCom node;
int device_number;

//Web Server
String generateSession(){
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membuat nama session secara random dengan panjang 100 digit.
  - Keluaran fungsi berupa objek kelas String.
  */

  String char_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
  String str = "HOME_AUTOMATION_SESSION=";
  for(int i = 0; i < 10; i++){

    str += char_set[random(char_set.length())];
  }
  return str;
}

bool is_authentified() {
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk mengecek apakah pengguna telah login atau telah logout.
  - Keluaran fungsi berupa tipe data boolean.
  - true = apabila user telah login.
  - false = apabila user telah logout.
  */

  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf(session) != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

void handleLogin() {
  /*
  Spesifikasi :
  - Apabila pengguna belum login server akan memanggil prosedur ini, sehingga pengguna
    diminta untuk login terlebih dahulu sebelum masuk ke halaman pengaturan.
  */

  String message = "";
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("disconnect")) {
    session = generateSession();
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  if (server.hasArg(form.getArg(ARG_USERNAME)) && server.hasArg(form.getArg(ARG_PASSWORD))) {
    if ((server.arg(form.getArg(ARG_USERNAME)) == pengaturan.readUsername()) && (server.arg(form.getArg(ARG_PASSWORD)) == pengaturan.readPassword())) {
      session = generateSession();
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", session);
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }
    message = "Username atau password salah.";
    Serial.println("Log in Failed");
  }
  server.send(200, "text/html", form.login("/login", message));
}

void handleKoneksi() {
  /*
  Spesifikasi :
  - Apabila pengguna telah login server membuka halaman / server akan memanggil prosedur ini,
    sehingga pengguna dapat masuk ke halaman pengaturan koneksi.
  - Apabila pengguna belum login server akan memanggil prosedur handleLogin(),
    sehingga pengguna diminta untuk login terlebih dahulu sebelum masuk ke halaman ini.
  */

  String message = "";
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }

  if(server.hasArg(form.getArg(ARG_WIFI_SSID)) && server.hasArg(form.getArg(ARG_WIFI_PASSWORD)) && server.hasArg(form.getArg(ARG_MQTT_BROKER)) && server.hasArg(form.getArg(ARG_DEVICE_NUMBER)) && server.hasArg(form.getArg(ARG_DEVICE_TOTAL))){
    String error_message = "";
    if(server.arg(form.getArg(ARG_WIFI_SSID)) == ""){
      error_message += "Wi-Fi SSID tidak boleh kosong.\\n";
    }

    if(server.arg(form.getArg(ARG_WIFI_PASSWORD)) == ""){
      error_message += "Wi-Fi Password tidak boleh kosong.\\n";
    }

    if(server.arg(form.getArg(ARG_MQTT_BROKER)) == ""){
      error_message += "MQTT Broker tidak boleh kosong.\\n";
    }

    if(server.arg(form.getArg(ARG_DEVICE_NUMBER)) == ""){
      error_message += "Nomor perangkat tidak boleh kosong.\\n";
    }

    if(server.arg(form.getArg(ARG_DEVICE_TOTAL)) == ""){
      error_message += "Jumlah perangkat tidak boleh kosong.\\n";
    }

    if(error_message == ""){
      if(pengaturan.readWifiSSID() != server.arg(form.getArg(ARG_WIFI_SSID))){
        pengaturan.writeWifiSSID(server.arg(form.getArg(ARG_WIFI_SSID)));
      }

      if(pengaturan.readWifiPassword() != server.arg(form.getArg(ARG_WIFI_PASSWORD))){
        pengaturan.writeWifiPassword(server.arg(form.getArg(ARG_WIFI_PASSWORD)));
      }

      if(pengaturan.readMQTTBroker() != server.arg(form.getArg(ARG_MQTT_BROKER))){
        pengaturan.writeMQTTBroker(server.arg(form.getArg(ARG_MQTT_BROKER)));
      }

      if(pengaturan.readDeviceNumber() != server.arg(form.getArg(ARG_DEVICE_NUMBER))){
        pengaturan.writeDeviceNumber(server.arg(form.getArg(ARG_DEVICE_NUMBER)));
      }

      if(pengaturan.readDeviceTotal() != server.arg(form.getArg(ARG_DEVICE_TOTAL))){
        pengaturan.writeDeviceTotal(server.arg(form.getArg(ARG_DEVICE_TOTAL)));
      }

      message = "Pengaturan koneksi telah tersimpan.";
    }else{
      message = error_message;
    }
  }
  server.send(200, "text/html", form.pengaturanKoneksi("/", message, "/", "/pengguna", "/firmware", "/login?disconnect=1", pengaturan.readWifiSSID(), pengaturan.readWifiPassword(), pengaturan.readMQTTBroker(), pengaturan.readDeviceNumber(), pengaturan.readDeviceTotal()));
}

void handlePengguna() {
  /*
  Spesifikasi :
  - Apabila pengguna telah login server membuka halaman /pengguna server akan memanggil prosedur ini,
    sehingga pengguna dapat masuk ke halaman pengaturan pengguna.
  - Apabila pengguna belum login server akan memanggil prosedur handleLogin(),
    sehingga pengguna diminta untuk login terlebih dahulu sebelum masuk ke halaman ini.
  */

  String message = "";
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }

  if (server.hasArg(form.getArg(ARG_USERNAME)) && server.hasArg(form.getArg(ARG_PASSWORD)) && server.hasArg(form.getArg(ARG_KONFIRMASI_PASSWORD))) {
    String error_message = "";
    if(server.arg(form.getArg(ARG_USERNAME)) == ""){
      error_message += "Username tidak boleh kosong.\\n";
    }

    if(server.arg(form.getArg(ARG_PASSWORD)) == ""){
      error_message += "Password tidak boleh kosong.\\n";
    }

    if(server.arg(form.getArg(ARG_KONFIRMASI_PASSWORD)) == ""){
      error_message += "Konfirmasi password tidak boleh kosong.\\n";
    }

    if(server.arg(form.getArg(ARG_PASSWORD)) != server.arg(form.getArg(ARG_KONFIRMASI_PASSWORD))){
      error_message += "Konfirmasi password tidak sesuai.\\n";
    }

    if(error_message == ""){
      if(pengaturan.readUsername() != server.arg(form.getArg(ARG_USERNAME))){
        pengaturan.writeUsername(server.arg(form.getArg(ARG_USERNAME)));
      }

      if(pengaturan.readPassword() != server.arg(form.getArg(ARG_PASSWORD))){
        pengaturan.writePassword(server.arg(form.getArg(ARG_PASSWORD)));
      }

      message = "Pengaturan pengguna telah tersimpan.";
    }else{
      message = error_message;
    }
  }
  server.send(200, "text/html", form.pengaturanPengguna("/pengguna", message, "/", "/pengguna", "/firmware", "/login?disconnect=1", pengaturan.readUsername(), pengaturan.readPassword(), pengaturan.readPassword()));
}

void handleFirmware() {
  /*
  Spesifikasi :
  - Apabila pengguna telah login server membuka halaman /pengguna server akan memanggil prosedur ini,
    sehingga pengguna dapat masuk ke halaman informasi firmware.
  - Apabila pengguna belum login server akan memanggil prosedur handleLogin(),
    sehingga pengguna diminta untuk login terlebih dahulu sebelum masuk ke halaman ini.
  */

  String message = "";
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }  

  FIRMWARE_IN informasi_firmware;
  informasi_firmware.firmwareName = firmware_name;
  informasi_firmware.firmwareDescription = firmware_description;
  informasi_firmware.firmwareVersion = firmware_version;
  informasi_firmware.legalCopyright = legal_copyright;
  informasi_firmware.companyName = company_name;
  informasi_firmware.firmwareType = firmware_type;
  server.send(200, "text/html", form.informasiFirmware(informasi_firmware, "/", "/pengguna", "/firmware", "/login?disconnect=1"));
}

void handleNotFound() {
  /*
  Spesifikasi :
  - Apabila pengguna membuka halaman selain /login, /, dan /pengaturan server akan menjalankan prosedur ini,
    sehingga akan muncul status "Halaman tida ditemukan." di web browser.
  */

  String message = "Halaman tidak ditemukan.\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
//------------------------------------------------------------------------------




//MQTT
void setup_wifi() {
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk keneksi perangkat ke Wi-Fi.
  */

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(pengaturan.readWifiSSID());
  WiFi.begin(pengaturan.readWifiSSID().c_str(), pengaturan.readWifiPassword().c_str());

  while (WiFi.status() != WL_CONNECTED) {
    if(jumper.read() == LOW){
        ESP.restart();
    }
    Serial.print(".");
    delay(100);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  /*
  Spesifikasi :
  - Prosedur ini akan terpanggil apabila ada perubahan topic yang telah di subscribe perangkat.
  */

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String _topic = topic;
  
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(_topic.indexOf(node.getTopicNext()) != -1){
    if((char)payload[0] == '1'){
      node.setNode(device_number + 1, VEHICLE_DETECT);
    }else{
      node.setNode(device_number + 1, VEHICLE_CLEAR);
    }
  }

  if(_topic.indexOf(node.getTopicPrev()) != -1){
    if((char)payload[0] == '1'){
      node.setNode(device_number - 1, VEHICLE_DETECT);
    }else{
      node.setNode(device_number - 1, VEHICLE_CLEAR);
    }
  }
}

void reconnect() {
  /*
  Spesifikasi :
  - Apabila koneksi MQTT Broker terputus perangkat akan memanggil prosedur ini. 
  */

  while (!MQTT.connected()) {
    if(jumper.read() == LOW){
        ESP.restart();
    }
    
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (MQTT.connect(clientId.c_str())) {
      Serial.println("connected");

      if(node.getTopicNext() != ""){
        MQTT.subscribe(node.getTopicNext().c_str());
      }

      if(node.getTopicPrev() != ""){
        MQTT.subscribe(node.getTopicPrev().c_str());
      }

    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTT.state());
      Serial.println(" try again in 1 seconds");
    }

    led.setOn();
    delay(500);
    led.setOff();
    delay(500);
  }
}
//------------------------------------------------------------------------------

class Pengaturan : public Task {
public:
    void loop() {
        server.handleClient();
        if(jumper.read() == HIGH){
            ESP.restart();
        }
    }
} t_pengaturan;

int counter = 0;
class MainProgram : public Task {
public:
    void loop() {
        if(jumper.read() == LOW){
            ESP.restart();
        }
        if (!MQTT.connected()) {
          reconnect();
        }
        //Serial.println(jarak.read());
        if(counter > 100){
          counter = 0;
          if(jarak.read() < 550){
            MQTT.publish(node.getTopic().c_str(), "1");
            node.setNode(device_number, VEHICLE_DETECT);
          }else{
            MQTT.publish(node.getTopic().c_str(), "0");
            node.setNode(device_number, VEHICLE_CLEAR);
          }
        }else{
          counter++;
        }
        MQTT.loop();

        node.loop();
        
        if(node.getStatusLamp() == HIGH){
          //Serial.println("TERANG");
          lampu.setBright();
        }else{
          //Serial.println("REDUP");
          lampu.setDim();
        }

        delay(1);
    }
} t_main_program;

class OTAUpdateTask : public Task {
public:
    void loop() {
        Serial.println("OTA");
        update.loop();
        delay(10000); //delay update 10 detik sekali
    }
} t_ota_update;

void setup(){
    randomSeed(micros()); //Inisialisasi random
    update.begin("http://devops-silaju.firmandev.com");
    pengaturan.begin();
    pengaturan.writeUsername("admin");
    pengaturan.writePassword("admin");

    Serial.begin(9600);
    if(jumper.read() == LOW){
        WiFi.softAP(ap_ssid, ap_password);
        IPAddress myIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(myIP);
        session = generateSession();
        server.on("/", handleKoneksi);
        server.on("/pengguna", handlePengguna);
        server.on("/firmware", handleFirmware);
        server.on("/login", handleLogin);
        server.onNotFound(handleNotFound);

        const char * headerkeys[] = {"User-Agent", "Cookie"} ;
        size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
        server.collectHeaders(headerkeys, headerkeyssize);
        server.begin();

        Scheduler.start(&t_pengaturan);
    }else{
        device_number = pengaturan.readDeviceNumber().toInt();
        if(device_number > 0){
          device_number = device_number - 1;
        }

        node.begin(device_number, pengaturan.readDeviceTotal().toInt());
        setup_wifi();
        String broker = pengaturan.readMQTTBroker();
        Serial.println(broker.c_str());
        MQTT.setServer(strdup(broker.c_str()), 1883);
        MQTT.setCallback(callback);
        Scheduler.start(&t_main_program);
        Scheduler.start(&t_ota_update);
    }
    Scheduler.begin();
}

void loop(){}