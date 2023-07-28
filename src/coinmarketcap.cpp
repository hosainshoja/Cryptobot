#include <coinmarketcap.h>

const char *COINMARKETCAP_SYMBOL = "BTC";
const char *COINMARKETCAP_CONVERT_TO = "USD";
const String COINMARKETCAP_API_KEY = "PUT_YOUR_COINMARKETCAP_API_HERE";
const char *COINMARKETCAP_HOST = "pro-api.coinmarketcap.com";
const uint16_t COINMARKETCAP_PORT = 443;
const String CoinMarketCap_RequestURL = "/v1/cryptocurrency/quotes/latest?CMC_PRO_API_KEY=" + COINMARKETCAP_API_KEY + "&symbol=" + COINMARKETCAP_SYMBOL + "&convert=" + COINMARKETCAP_CONVERT_TO;

boolean CoinMarketCap_FindLastPrice_FromJson(String *Json, float *Price, uint8_t Percision)
{
    DynamicJsonDocument jsonDocument(4096);
    DeserializationError error = deserializeJson(jsonDocument, *Json);
    if (error)
    {
        Serial.println("deserializeJson() failed with code:");
        Serial.println(error.c_str());
        return false;
    }
    JsonObject root = jsonDocument.as<JsonObject>();
    // check API status
    JsonObject status = root["status"];
    int statusErrorCode = status["error_code"];
    if (statusErrorCode != 0)
    {
        String statusErrorMessage = status["error_message"];
        Serial.print("Error: ");
        Serial.println(statusErrorMessage);
    }

    JsonObject coin = root["data"][COINMARKETCAP_SYMBOL];
    String name = coin["name"];
    String lastUpdated = coin["last_updated"];
    Serial.println("Name: " + name);
    Serial.println("Last updated: " + lastUpdated);

    JsonObject quote = coin["quote"][COINMARKETCAP_CONVERT_TO];
    *Price = quote["price"];
    Serial.print("Price: $");
    Serial.println(*Price, Percision);
    return true;
}