#ifndef __CHART_H
#define __CHART_H
//-------------------------------------------------------------------
#include <candle.h>
//-------------------------------------------------------------------
#define REQUEST_PERIOD_MS 30000
#define CANDLE_PERIOD_MS 180000
#define CANDLE_PRICE_COUNT (CANDLE_PERIOD_MS / REQUEST_PERIOD_MS)
//-------------------------------------------------------------------
#define CHART_SAVE_CANDLE_NUM 10
#define CHART_SHOW_CANDLE_NUM 10
#define CHART_TEXT_HEIGHT_PX 14
#define CHART_PLOT_HEIGHT_PX 50
#define CHART_PLOT_LENGTH_PX 128
//-------------------------------------------------------------------
struct CandleStruct_t
{
    float highPrice;
    float lowPrice;
    float openPrice;
    float closePrice;
};
//-------------------------------------------------------------------
class Chart
{
private:
    uint32_t candleIndex;
    CandleStruct_t candle[CHART_SAVE_CANDLE_NUM];
    float maxPrice;
    float minPrice;

public:
    //const uint32_t candleHeight = (CHART_PLOT_HEIGHT_PX / 2);
    const uint32_t candleWidth = (CHART_PLOT_LENGTH_PX / CHART_SHOW_CANDLE_NUM);
    //const uint32_t candleYposition = (CHART_PLOT_HEIGHT_PX / 2) + CHART_TEXT_HEIGHT_PX;
    //const uint32_t candleXposition = ((CHART_PLOT_LENGTH_PX / 2) - (candleWidth / 2));
    uint32_t getCandleIndex();
    float getMaxPrice();
    float getMinPrice();
    float getCandleOpenPrice(uint32_t index);
    float getCandleClosePrice(uint32_t index);
    float getCandleHighPrice(uint32_t index);
    float getCandleLowPrice(uint32_t index);
    int addToChart(CandleStruct_t *CurrentCandle);
    void show();
    Chart(float *firstPrice);
};
//-------------------------------------------------------------------
#endif