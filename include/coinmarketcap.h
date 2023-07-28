#ifndef __COINMARKETCAP_H
#define __COINMARKETCAP_H

#include <Arduino.h>
#include <ArduinoJson.h>

extern const char *COINMARKETCAP_SYMBOL;
extern const char *COINMARKETCAP_CONVERT_TO;
extern const String COINMARKETCAP_API_KEY;
extern const char *COINMARKETCAP_HOST;
extern const uint16_t COINMARKETCAP_PORT;
extern const String CoinMarketCap_RequestURL;

boolean CoinMarketCap_FindLastPrice_FromJson(String *Json, float *Price, uint8_t Percision);

#endif