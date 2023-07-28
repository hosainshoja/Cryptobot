#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <coinmarketcap.h>
#include <U8g2lib.h>
#include <candle.h>
#include <chart.h>
#include <SPI.h>
#include <SD.h>

#ifndef STASSID
#define STASSID "PUT_YOUR_WIFI_SSID_HERE"
#define STAPSK "PUT_YOUR_WIFI_PASS_HERE"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

const int SDchipSelect = D8;
File root;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
//const char fingerprint[] PROGMEM = "335b4fc7b2e210256ed6f2f900744ea21f436195";

// Use WiFiClientSecure class to create TLS connection
WiFiClientSecure client;

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// void u8g2_line(uint8_t a)
// {
//   u8g2.drawStr(0, 0, "drawLine");
//   u8g2.drawLine(7 + a, 10, 40, 55);
//   u8g2.drawLine(7 + a * 2, 10, 60, 55);
//   u8g2.drawLine(7 + a * 3, 10, 80, 55);
//   u8g2.drawLine(7 + a * 4, 10, 100, 55);
// }

void u8g2_show_chart(Chart *Schart, Candle *Dcandle)
{
  char priceStr[20];
  sprintf(priceStr, "Price: $%.2f", Dcandle->closePrice);
  u8g2.clearBuffer();                 // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0, 10, priceStr);      // write something to the internal memory
  float highYAxis = Schart->getMaxPrice();
  float lowYAxis = Schart->getMinPrice();
  Serial.print("highYAxis: ");
  Serial.println(highYAxis);
  Serial.print("lowYAxis: ");
  Serial.println(lowYAxis);
  highYAxis = max(Dcandle->closePrice, highYAxis);
  lowYAxis = min(Dcandle->closePrice, lowYAxis);
  Serial.print("highYAxis: ");
  Serial.println(highYAxis);
  Serial.print("lowYAxis: ");
  Serial.println(lowYAxis);
  float diffYAxis = highYAxis - lowYAxis;
  Serial.print("diffYAxis: ");
  Serial.println(diffYAxis);
  uint32_t candleIndex = Schart->getCandleIndex();
  if (candleIndex > 0)
  {
    for (size_t candleCount = 1; candleCount <= candleIndex; candleCount++)
    {
      if (Schart->getCandleClosePrice(candleCount) > Schart->getCandleOpenPrice(candleCount))
      {
        float candleYposition = ((highYAxis - Schart->getCandleClosePrice(candleCount)) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
        Serial.print("Y: ");
        Serial.println(candleYposition);
        float candleHeight = ((Schart->getCandleClosePrice(candleCount) - Schart->getCandleOpenPrice(candleCount)) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
        Serial.print("H: ");
        Serial.println(candleHeight);
        if (candleHeight < 1)
          candleHeight = 1;
        u8g2.drawFrame(Schart->candleWidth * (candleCount - 1), candleYposition + CHART_TEXT_HEIGHT_PX, Schart->candleWidth, candleHeight);
        if (Schart->getCandleHighPrice(candleCount) > Schart->getCandleClosePrice(candleCount))
        {
          float upShadowYposition = ((highYAxis - Schart->getCandleHighPrice(candleCount)) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
          float upShadowHeight = ((Schart->getCandleHighPrice(candleCount) - Schart->getCandleClosePrice(candleCount)) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
          Serial.print("upShadowYposition: ");
          Serial.println(upShadowYposition);
          Serial.print("upShadowHeight: ");
          Serial.println(upShadowHeight);
          u8g2.drawVLine(((Schart->candleWidth * (candleCount - 1)) + (Schart->candleWidth / 2)), upShadowYposition + CHART_TEXT_HEIGHT_PX, upShadowHeight);
        }
        if (Schart->getCandleLowPrice(candleCount) < Schart->getCandleOpenPrice(candleCount))
        {
          float DownShadowYposition = ((highYAxis - Schart->getCandleOpenPrice(candleCount)) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
          float DownShadowHeight = ((Schart->getCandleOpenPrice(candleCount) - Schart->getCandleLowPrice(candleCount)) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
          Serial.print("DownShadowYposition: ");
          Serial.println(DownShadowYposition);
          Serial.print("DownShadowHeight: ");
          Serial.println(DownShadowHeight);
          u8g2.drawVLine(((Schart->candleWidth * (candleCount - 1)) + (Schart->candleWidth / 2)), DownShadowYposition + CHART_TEXT_HEIGHT_PX, DownShadowHeight);
        }
      }
      else
      {
        float candleYposition = ((highYAxis - Schart->getCandleOpenPrice(candleCount)) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
        Serial.print("Y: ");
        Serial.println(candleYposition);
        float candleHeight = ((Schart->getCandleOpenPrice(candleCount) - Schart->getCandleClosePrice(candleCount)) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
        Serial.print("H: ");
        Serial.println(candleHeight);
        if (candleHeight < 1)
          candleHeight = 1;
        u8g2.drawBox(Schart->candleWidth * (candleCount - 1), candleYposition + CHART_TEXT_HEIGHT_PX, Schart->candleWidth, candleHeight);
        float ShadowYposition = ((highYAxis - Schart->getCandleHighPrice(candleCount)) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
        float ShadowHeight = ((Schart->getCandleHighPrice(candleCount) - Schart->getCandleLowPrice(candleCount)) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
        Serial.print("ShadowYposition: ");
        Serial.println(ShadowYposition);
        Serial.print("ShadowHeight: ");
        Serial.println(ShadowHeight);
        u8g2.drawVLine(((Schart->candleWidth * (candleCount - 1)) + (Schart->candleWidth / 2)), ShadowYposition + CHART_TEXT_HEIGHT_PX, ShadowHeight);
      }
    }
  }
  if (candleIndex < CHART_SAVE_CANDLE_NUM)
  {
    if (Dcandle->closePrice > Dcandle->openPrice)
    {
      float candleYposition = ((highYAxis - Dcandle->closePrice) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
      Serial.print("Y: ");
      Serial.println(candleYposition);
      float candleHeight = ((Dcandle->closePrice - Dcandle->openPrice) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
      Serial.print("H: ");
      Serial.println(candleHeight);
      if (candleHeight < 1)
        candleHeight = 1;
      u8g2.drawFrame(Schart->candleWidth * candleIndex, candleYposition + CHART_TEXT_HEIGHT_PX, Schart->candleWidth, candleHeight);
      if (Dcandle->highPrice > Dcandle->closePrice)
      {
        float upShadowYposition = ((highYAxis - Dcandle->highPrice) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
        float upShadowHeight = ((Dcandle->highPrice - Dcandle->closePrice) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
        Serial.print("upShadowYposition: ");
        Serial.println(upShadowYposition);
        Serial.print("upShadowHeight: ");
        Serial.println(upShadowHeight);
        u8g2.drawVLine(((Schart->candleWidth * candleIndex) + (Schart->candleWidth / 2)), upShadowYposition + CHART_TEXT_HEIGHT_PX, upShadowHeight);
      }
      if (Dcandle->lowPrice < Dcandle->openPrice)
      {
        float DownShadowYposition = ((highYAxis - Dcandle->openPrice) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
        float DownShadowHeight = ((Dcandle->openPrice - Dcandle->lowPrice) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
        Serial.print("DownShadowYposition: ");
        Serial.println(DownShadowYposition);
        Serial.print("DownShadowHeight: ");
        Serial.println(DownShadowHeight);
        u8g2.drawVLine(((Schart->candleWidth * candleIndex) + (Schart->candleWidth / 2)), DownShadowYposition + CHART_TEXT_HEIGHT_PX, DownShadowHeight);
      }
    }
    else
    {
      float candleYposition = ((highYAxis - Dcandle->openPrice) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
      Serial.print("Y: ");
      Serial.println(candleYposition);
      float candleHeight = ((Dcandle->openPrice - Dcandle->closePrice) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
      Serial.print("H: ");
      Serial.println(candleHeight);
      if (candleHeight < 1)
        candleHeight = 1;
      u8g2.drawBox(Schart->candleWidth * candleIndex, candleYposition + CHART_TEXT_HEIGHT_PX, Schart->candleWidth, candleHeight);
      float ShadowYposition = ((highYAxis - Dcandle->highPrice) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
      float ShadowHeight = ((Dcandle->highPrice - Dcandle->lowPrice) * CHART_PLOT_HEIGHT_PX) / diffYAxis;
      Serial.print("ShadowYposition: ");
      Serial.println(ShadowYposition);
      Serial.print("ShadowHeight: ");
      Serial.println(ShadowHeight);
      u8g2.drawVLine(((Schart->candleWidth * candleIndex) + (Schart->candleWidth / 2)), ShadowYposition + CHART_TEXT_HEIGHT_PX, ShadowHeight);
    }
  }
  u8g2.sendBuffer(); // transfer internal memory to the display
}

boolean Wifi_Connect_And_Get_Body_Response(const char *host, uint16_t port, const String RequestURL, String *ResponseBody)
{
  Serial.print("connecting to ");
  Serial.println(host);

  //Serial.printf("Using fingerprint '%s'\n", fingerprint);
  //client.setFingerprint(fingerprint);
  client.setInsecure();
  if (!client.connect(host, port))
  {
    Serial.println("connection failed");
    return false;
  }

  Serial.print("requesting URL: ");
  Serial.println(RequestURL);

  client.print(String("GET ") + RequestURL + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");

  // check HTTP response
  char httpStatus[32] = {0};
  client.readBytesUntil('\r', httpStatus, sizeof(httpStatus));
  if (strcmp(httpStatus, "HTTP/1.1 200 OK") != 0)
  {
    Serial.print("Unexpected response: ");
    Serial.println(httpStatus);
    delay(REQUEST_PERIOD_MS);
    return false;
  }

  // skip HTTP headers
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r")
    {
      break;
    }
  }

  // skip content length
  if (client.connected())
  {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  // get response
  *ResponseBody = "";
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    line.trim();
    if (line != "\r")
    {
      *ResponseBody += line;
    }
  }
  client.stop();
  return true;
}

float get_price()
{
  String response;
  float price;
  if (Wifi_Connect_And_Get_Body_Response(COINMARKETCAP_HOST, COINMARKETCAP_PORT, CoinMarketCap_RequestURL, &response))
  {
    if (CoinMarketCap_FindLastPrice_FromJson(&response, &price, 2))
    {
      return price;
    }
  }
  return 0;
}

boolean get_candle(Chart *staticChart, CandleStruct_t *candleStruct, float initPrice)
{
  if (initPrice > 0)
  {
    Candle tempCandle(initPrice);
    Serial.print("new candle created with open price: ");
    Serial.println(initPrice);
    while (tempCandle.getPriceIndex() < CANDLE_PRICE_COUNT)
    {
      delay(REQUEST_PERIOD_MS);
      if (tempCandle.addPrice(get_price()))
      {
        Serial.print("new price added to candle: ");
        Serial.println(tempCandle.closePrice);
        u8g2_show_chart(staticChart, &tempCandle);
      }
    }
    candleStruct->openPrice = tempCandle.openPrice;
    candleStruct->closePrice = tempCandle.closePrice;
    candleStruct->highPrice = tempCandle.highPrice;
    candleStruct->lowPrice = tempCandle.lowPrice;
    return true;
  }
  return false;
}

float Price = 0;

void setup()
{
  u8g2.begin();
  Serial.begin(115200);
  while (!SD.begin(SDchipSelect))
  {
    Serial.println("SD Card failed!");
    delay(500);
  }

  // print the type of card
  Serial.println();
  Serial.print("Card type:         ");
  switch (SD.type())
  {
  case 0:
    Serial.println("SD1");
    break;
  case 1:
    Serial.println("SD2");
    break;
  case 2:
    Serial.println("SDHC");
    break;
  default:
    Serial.println("Unknown");
  }

  Serial.print("Cluster size:          ");
  Serial.println(SD.clusterSize());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(SD.blocksPerCluster());
  Serial.print("Blocks size:  ");
  Serial.println(SD.blockSize());

  Serial.print("Total Blocks:      ");
  Serial.println(SD.totalBlocks());
  Serial.println();

  Serial.print("Total Cluster:      ");
  Serial.println(SD.totalClusters());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is:    FAT");
  Serial.println(SD.fatType(), DEC);

  volumesize = SD.totalClusters();
  volumesize *= SD.clusterSize();
  volumesize /= 1000;
  Serial.print("Volume size (Kb):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):  ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb):  ");
  Serial.println((float)volumesize / 1024.0);

  Serial.print("Card size:  ");
  Serial.println((float)SD.size() / 1000);

  FSInfo fs_info;
  SDFS.info(fs_info);

  Serial.print("Total bytes: ");
  Serial.println(fs_info.totalBytes);

  Serial.print("Used bytes: ");
  Serial.println(fs_info.usedBytes);

  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  while (Price == 0)
  {
    Price = get_price();
  }
}

void loop()
{
  Chart chart(&Price);
  while (chart.getCandleIndex() < CHART_SAVE_CANDLE_NUM)
  {
    CandleStruct_t CandleStruct;
    get_candle(&chart, &CandleStruct, Price);
    chart.addToChart(&CandleStruct);
    Price = chart.getCandleClosePrice(chart.getCandleIndex());
  }
  delay(REQUEST_PERIOD_MS);
}