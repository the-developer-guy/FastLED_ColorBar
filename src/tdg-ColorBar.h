#include <Arduino.h>
#include <FastLED.h>

template <int pin = 3>
class ColorBar
{
  private:
  const uint8_t defaultBrightness = 12;
  CRGB *leds;
  uint16_t distance;
  uint16_t step;
  uint16_t ledCount;
  uint16_t minDistance;
  uint16_t maxDistance;
  uint8_t brightness;

  CRGB colorRed;
  CRGB colorYellow;
  CRGB colorGreen;
  
  public:
  ColorBar(){};
  ColorBar(int ledCount, uint16_t minDistance, uint16_t maxDistance)
  {
    this->ledCount = ledCount;
    this->minDistance = minDistance;
    this->maxDistance = maxDistance;
    brightness = defaultBrightness;
    step = (maxDistance - minDistance) / (3 * ledCount);
    setBrightness(defaultBrightness);

    if(step <= 0)
    {
      step = 1;
    }
    distance = 0;
    leds = new CRGB[ledCount];
    FastLED.addLeds<NEOPIXEL, pin>(leds, ledCount);
  }
  
  void setBrightness(uint8_t brightness)
  {
    this->brightness = brightness;
    colorRed.setRGB(brightness, 0, 0);
    colorYellow.setRGB(brightness/2, brightness/4, 0);
    colorGreen.setRGB(0, brightness, 0);
  }

  void setDistance(uint16_t distance)
  {
    this->distance = distance;
  }

  void display()
  {
      uint16_t distanceSteps = distance / step;
      CRGB color;
      CRGB backgroundColor;
      if(distanceSteps < ledCount)
      {
        color = colorRed;
        backgroundColor = colorYellow;
      }
      else if (distanceSteps < (2 * ledCount))
      {
        color = colorYellow;
        backgroundColor = colorGreen;
      }
      else
      {
        color = colorGreen;
        backgroundColor = colorGreen;
      }
      
      for(int i = 0; i < ledCount; i++)
      {
        leds[i] = backgroundColor;
      }
      
      if(distanceSteps >= (3 * ledCount - 1))
      {
        for(int i = 0; i < ledCount; i++)
        {
          leds[i] = CRGB::Black;
        }
        leds[0] = colorGreen;
      }
      else
      {
        uint8_t mod = distanceSteps % ledCount;
        
        for(int i = ledCount - mod; i >= 0; i--)
        {
          leds[i] = color;
        }
      }
      FastLED.show();
  }
};
