#include <chart.h>

Chart::Chart(float *firstPrice)
{
    if (firstPrice > 0)
    {
        candleIndex = 0;
        candle[0].openPrice = *firstPrice;
        candle[0].closePrice = *firstPrice;
        candle[0].highPrice = *firstPrice;
        candle[0].lowPrice = *firstPrice;

        maxPrice = *firstPrice;
        minPrice = *firstPrice;
    }
}

int Chart::addToChart(CandleStruct_t *CurrentCandle)
{
    if (candleIndex >= CHART_SAVE_CANDLE_NUM)
    {
        return -1;
    }
    candleIndex++;
    candle[candleIndex - 1].openPrice = CurrentCandle->openPrice;
    candle[candleIndex - 1].closePrice = CurrentCandle->closePrice;
    candle[candleIndex - 1].highPrice = CurrentCandle->highPrice;
    candle[candleIndex - 1].lowPrice = CurrentCandle->lowPrice;

    maxPrice = max(maxPrice, CurrentCandle->highPrice);
    minPrice = min(minPrice, CurrentCandle->lowPrice);

    return candleIndex;
}

float Chart::getMaxPrice()
{
    return maxPrice;
}

float Chart::getMinPrice()
{
    return minPrice;
}

uint32_t Chart::getCandleIndex()
{
    return candleIndex;
}

float Chart::getCandleOpenPrice(uint32_t index)
{
    if (index > CHART_SAVE_CANDLE_NUM)
        return 0;
    else
        return candle[index - 1].openPrice;
}

float Chart::getCandleClosePrice(uint32_t index)
{
    if (index > CHART_SAVE_CANDLE_NUM)
        return 0;
    else
        return candle[index - 1].closePrice;
}

float Chart::getCandleHighPrice(uint32_t index)
{
    if (index > CHART_SAVE_CANDLE_NUM)
        return 0;
    else
        return candle[index - 1].highPrice;
}

float Chart::getCandleLowPrice(uint32_t index)
{
    if (index > CHART_SAVE_CANDLE_NUM)
        return 0;
    else
        return candle[index - 1].lowPrice;
}

void Chart::show()
{
}