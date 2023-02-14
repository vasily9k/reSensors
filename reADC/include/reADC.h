/* 
   EN: Voltage measurement at ADC inputs and conversion to real values
   RU: Измерение напряжения на ADC входах и пересчет в реальные значения
   --------------------------
   (с) 2022-2023 Разживин Александр | Razzhivin Alexander
   kotyara12@yandex.ru | https://kotyara12.ru | tg: @kotyara1971
*/

#ifndef __RE_ADC_H__
#define __RE_ADC_H__

#include <stdint.h>
#include <driver/gpio.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include <reParams.h>
#include <reSensor.h>

#ifdef __cplusplus
extern "C" {
#endif

class reADCItem: public rSensorItem {
  public:
    reADCItem(rSensor *sensor, const char* itemName, 
      const adc_oneshot_unit_handle_t adc_unit_handle, const adc_cali_handle_t adc_cali_handle,
      const adc_channel_t channel, const adc_atten_t atten, const adc_bitwidth_t bitwidth, 
      const double coefficient,
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
  protected:
    adc_channel_t getChannel();
    adc_atten_t getAtten();
    adc_bitwidth_t getBitwidth();
    void setChannel(adc_oneshot_unit_handle_t unit_handle, adc_channel_t channel);

    void registerItemParameters(paramsGroup_t * group) override;
    sensor_status_t getRawValue(value_t * rawValue) override;
  private:
    adc_oneshot_unit_handle_t _unit_handle = nullptr;
    adc_cali_handle_t         _cali_handle = nullptr;
    adc_channel_t             _channel;
    adc_atten_t               _atten;
    adc_bitwidth_t            _bitwidth;
    double                    _coefficient = 1.0;
};

class reADCGpio: public reADCItem {
  public: 
    reADCGpio(rSensor *sensor, const char* itemName, 
      const int adc_gpio, const bool use_calibration,
      const adc_atten_t atten, const adc_bitwidth_t bitwidth, 
      const double coefficient,
      const sensor_filter_t filterMode, const uint16_t filterSize,
      const char* formatNumeric, const char* formatString 
      #if CONFIG_SENSOR_TIMESTAMP_ENABLE
      , const char* formatTimestamp
      #endif // CONFIG_SENSOR_TIMESTAMP_ENABLE
      #if CONFIG_SENSOR_TIMESTRING_ENABLE  
      , const char* formatTimestampValue, const char* formatStringTimeValue
      #endif // CONFIG_SENSOR_TIMESTRING_ENABLE
      );
    ~reADCGpio();
    bool initItem() override;
  private:
    int _adc_gpio = -1;
    bool _calibration = false;
    adc_cali_handle_t _cali_handle = nullptr;
};

#ifdef __cplusplus
}
#endif

#endif // __RE_ADC_H__