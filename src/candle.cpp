#include <Candle.h>
//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
Candle::Candle(float CandleOpenPrice)
{
    if (CandleOpenPrice > 0)
    {
        openPrice = CandleOpenPrice;
        closePrice = CandleOpenPrice;
        highPrice = CandleOpenPrice;
        lowPrice = CandleOpenPrice;
        priceIndex = 1;
    }
}
//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
boolean Candle::addPrice(float price)
{
    if (price > 0 && priceIndex > 0)
    {
        highPrice = max(highPrice, price);
        lowPrice = min(lowPrice, price);
        closePrice = price;
        priceIndex++;
        return true;
    }
    return false;
}
//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
uint32_t Candle::getPriceIndex()
{
    return priceIndex;
}