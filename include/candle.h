#ifndef __CANDLE_H
#define __CANDLE_H
//-------------------------------------------------------------------
#include <Arduino.h>
//-------------------------------------------------------------------
class Candle
{
private:
    uint32_t priceIndex;

public:
    float highPrice;
    float lowPrice;
    float openPrice;
    float closePrice;
    boolean addPrice(float price);
    uint32_t getPriceIndex();
    Candle(float CandleOpenPrice);
};
//-------------------------------------------------------------------
#endif