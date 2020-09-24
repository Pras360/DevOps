#include "HTMLForm.h"
#include <Arduino.h>

//Public
String HTMLForm::login(String target, String message){
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membuat kode HTML halaman login.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>Lampu Jalan</title>";
  html += "</head>";
  html += "<body>";
  html += "<form action=\"/login\" method=\"post\">";
  html += "<table>";
  html += "<tr>";
  html += "<td>Username : </td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_USERNAME) + "\" placeholder=\"Username\" maxlength=\"100\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Password : </td>";
  html += "<td><input type=\"password\" name=\"" + getArg(ARG_PASSWORD) + "\" placeholder=\"Password\" maxlength=\"100\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td><button type=\"submit\">Login</button></td>";
  html += "<td></td>";
  html += "</tr>";
  html += "</table>";
  html += "</form>";

  if(message != ""){
    html += "<script type=\"text/javascript\">alert(\"";
    html += message;
    html += "\");</script>";
  }
  html += "</body>";
  html += "</html>";
  return html;
}

String HTMLForm::pengaturanKoneksi(String target, String message, String link_pengaturan_koneksi, String link_pengaturan_pengguna, String link_informasi_firmware, String link_logout, String wifi_ssid, String wifi_password, String mqtt_broker, String device_number, String device_total){
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membuat kode HTML halaman pengaturan koneksi.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>Lampu Jalan</title>";
  html += "</head>";
  html += "<body>";
  html += "<a href=\"" + link_pengaturan_koneksi + "\">Pengaturan Koneksi</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_pengaturan_pengguna + "\">Pengaturan Pengguna</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_informasi_firmware + "\">Informasi Firmware</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_logout + "\">Logout</a>";
  html += "<br>";
  html += "<br>";
  html += "<form action=\"" + link_pengaturan_koneksi + "\" method=\"post\">";
  html += "Pengaturan Koneksi";
  html += "<table>";
  html += "<tr>";
  html += "<td>Wi-FI SSID </td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_WIFI_SSID) + "\" placeholder=\"Wi-FI SSID\" value=\"" + wifi_ssid + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Wi-FI Password </td>";
  html += "<td>:</td>";
  html += "<td><input type=\"password\" name=\"" + getArg(ARG_WIFI_PASSWORD) + "\" placeholder=\"Wi-FI Password\" value=\"" + wifi_password + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>MQTT Broker</td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_MQTT_BROKER) + "\" Placeholder=\"MQTT Broker\" value=\"" + mqtt_broker + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Device Number</td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_DEVICE_NUMBER) + "\" Placeholder=\"Device Number\" value=\"" + device_number + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Total Device</td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_DEVICE_TOTAL) + "\" Placeholder=\"Total Device\" value=\"" + device_total + "\"></td>";
  html += "</tr>";
  html += "</table>";
  html += "<button type=\"submit\">Simpan</button>";
  html += "</form>";
  if(message != ""){
    html += "<script type=\"text/javascript\">alert(\"";
    html += message;
    html += "\");</script>";
  }
  html += "</body>";
  html += "</html>";
  return html;
}

String HTMLForm::pengaturanPengguna(String target, String message, String link_pengaturan_koneksi, String link_pengaturan_pengguna, String link_informasi_firmware, String link_logout, String username, String password, String konfirmasi_password){
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membuat kode HTML halaman pengaturan pengguna.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>Lampu Jalan</title>";
  html += "</head>";
  html += "<body>";
  html += "<a href=\"" + link_pengaturan_koneksi + "\">Pengaturan Koneksi</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_pengaturan_pengguna + "\">Pengaturan Pengguna</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_informasi_firmware + "\">Informasi Firmware</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_logout + "\">Logout</a>";
  html += "<br>";
  html += "<br>";
  html += "<form action=\"" + link_pengaturan_pengguna + "\" method=\"post\">";
  html += "Pengaturan Pengguna";
  html += "<table>";
  html += "<tr>";
  html += "<td>Username </td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_USERNAME) + "\" placeholder=\"Wi-FI SSID\" value=\"" + username + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Password </td>";
  html += "<td>:</td>";
  html += "<td><input type=\"password\" name=\"" + getArg(ARG_PASSWORD) + "\" placeholder=\"Wi-FI Password\" value=\"" + password + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Konfirmasi Password </td>";
  html += "<td>:</td>";
  html += "<td><input type=\"password\" name=\"" + getArg(ARG_KONFIRMASI_PASSWORD) + "\" placeholder=\"Wi-FI Password\" value=\"" + password + "\"></td>";
  html += "</tr>";
  html += "</table>";
  html += "<button type=\"submit\">Simpan</button>";
  html += "</form>";
  if(message != ""){
    html += "<script type=\"text/javascript\">alert(\"";
    html += message;
    html += "\");</script>";
  }
  html += "</body>";
  html += "</html>";
  return html;
}

String HTMLForm::informasiFirmware(FIRMWARE_IN informasi_firmware, String link_pengaturan_koneksi, String link_pengaturan_pengguna, String link_informasi_firmware, String link_logout){
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membuat kode HTML halaman informasi firmware.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>Lampu Jalan</title>";
  html += "</head>";
  html += "<body>";
  html += "<a href=\"" + link_pengaturan_koneksi + "\">Pengaturan Koneksi</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_pengaturan_pengguna + "\">Pengaturan Pengguna</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_informasi_firmware + "\">Informasi Firmware</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_logout + "\">Logout</a>";
  html += "<br>";
  html += "<br>";
  html += "Informasi Firmware<br>";
  html += "<br>Firmware Name : ";
  html += informasi_firmware.firmwareName;
  html += "<br>Firmware Description : ";
  html += informasi_firmware.firmwareDescription;
  html += "<br>Firmware Version : ";
  html += informasi_firmware.firmwareVersion;
  html += "<br>Legal Copyright : ";
  html += informasi_firmware.legalCopyright;
  html += "<br>Company Name : ";
  html += informasi_firmware.companyName;
  html += "<br>Firmware Type : ";
  html += informasi_firmware.firmwareType;
  html += "</body>";
  html += "</html>";
  return html; 
}

String HTMLForm::getArg(HTML_FORM_ARG arg){
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk mendapatkan arg pada Form HTML.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return htmlFormArg[arg];
}
//------------------------------------------------------------------------------
