/* 
   EN: Library for reading data from Capacitive Soil Moisture Sensor v1.2 or similar
   RU: Библиотека для считывания данных с Capacitive Soil Moisture Sensor v1.2 или аналогичных
   --------------------------
   (с) 2021-2002 Разживин Александр | Razzhivin Alexander
   kotyara12@yandex.ru | https://kotyara12.ru | tg: @kotyara1971
*/

#ifndef __RE_MOISTURE_H__
#define __RE_MOISTURE_H__

#include <stdint.h>
#include <driver/gpio.h>
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include "reParams.h"
#include "reSensor.h"
#include "reADC.h"
#include "project_config.h"

#ifdef __cplusplus
extern "C" {
#endif

class rMoistureItem: public rMapItem {
  public:
    rMoistureItem(rSensor *sensor, const char* itemName, 
      const adc1_channel_t channel, const adc_atten_t atten, const bool cal_enabled, const double coefficient,
      const type_bounds_t in_bounds, const value_t in_min, const value_t in_max,
      const sensor_filter_t filterMode, const uint16_t filterSize,
      const char* formatNumeric, const char* formatString 
      #if CONFIG_SENSOR_TIMESTAMP_ENABLE
      , const char* formatTimestamp
      #endif // CONFIG_SENSOR_TIMESTAMP_ENABLE
      #if CONFIG_SENSOR_TIMESTRING_ENABLE  
      , const char* formatTimestampValue, const char* formatStringTimeValue
      #endif // CONFIG_SENSOR_TIMESTRING_ENABLE
      );
    bool initItem() override;
    value_t convertValue(const value_t rawValue) override;

    // Correction of the value depending on the temperature
    void setTempCorrection(double coefficent, value_t base_point);
    void setTemperature(value_t temperature);
  protected:
    void registerItemParameters(paramsGroup_t * group) override;
    sensor_status_t getRawValue(value_t * rawValue) override;
  private:
    // ADC data 
    adc1_channel_t _channel;
    adc_atten_t _atten;
    bool _cal_enable = false;
    esp_adc_cal_characteristics_t _chars;
    // Temperature correction
    double  _temp_coef = 0.0;
    paramsEntryHandle_t _prm_temp_coef;
    value_t _temp_base = 0.0;
    paramsEntryHandle_t _prm_temp_base;
    value_t _temp_curr = 0.0;
};

#ifdef __cplusplus
}
#endif

#endif // __RE_MOISTURE_H__