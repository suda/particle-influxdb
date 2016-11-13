#include "InfluxDB.h"

InfluxDB::InfluxDB(const char *database) {
  _database = database;
}

void InfluxDB::begin(const char *hostname, int port) {
  _hostname = hostname;
  _ip = WiFi.resolve(_hostname);
  if (_ip) {
    Serial.print("IP resolved to: ");
    Serial.println(_ip);
  } else {
    Serial.println("Failed to resolve IP");
  }
  _port = port;
}

void InfluxDB::setCredentials(const char *username, const char *password) {
  _username = username;
  _password = password;
}

void InfluxDB::write(const char *metric, String value) {
  InfluxDB::_send(metric, value);
}

void InfluxDB::addTag(const char *name, const char *value) {
  _tags += "," + String(name) + "=" + String(value);
}

void InfluxDB::_send(const char *metric, String value) {
  String data;

  Serial.print("InfluxDB connecting... ");
  if (_client.connect(_ip, _port)) {
    Serial.print("Connected... ");

    data = String(metric) + _tags + " value=" + value;

    _client.print("POST /write?db=");
    _client.print(_database);
    _client.println(" HTTP/1.1");

    _client.print("Host: ");
    _client.println(_hostname);

    String auth = String(_username) + ":" + String(_password);
    int inputLen = auth.length() + 1;
    char input[inputLen];
    auth.toCharArray(input, inputLen);
    int encodedLen = base64_enc_len(inputLen);
    char encoded[encodedLen];
    base64_encode(encoded, input, inputLen);

    _client.print("Authorization: Basic ");
    _client.println(encoded);

    _client.print("Content-Length: ");
    _client.println(data.length());
    _client.println();
    _client.print(data);

    _client.stop();
    Serial.println("Wrote metric!");
  } else {
    Serial.println("Connection failed!");
  }
}
