#ifndef INFLUXDB_H
#define INFLUXDB_H

#include <Base64.h>
#include "application.h"

class InfluxDB
{
public:
  InfluxDB(const char *database);
  void setCredentials(const char *username, const char *password);
  void begin(const char *hostname, int port);
  void write(const char *metric, String value);
  void addTag(const char *name, const char *value);
private:
  void _send(const char *metric, String value);
  IPAddress _ip;
  int _port;
  TCPClient _client;
  const char *_hostname;
  const char *_database;
  const char *_username;
  const char *_password;
  String _tags;
};

#endif
