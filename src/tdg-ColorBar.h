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
  uint16_t redZone;
  uint16_t yellowZone;
  uint16_t greenZone;
  uint16_t scaleSize;
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
    scaleSize = ledCount * 3;
    step = (maxDistance - minDistance) / scaleSize;
    Serial.print("Step size: ");
    Serial.println(step);
    redZone = ledCount;
    yellowZone = ledCount * 2;
    greenZone = scaleSize;
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
      uint16_t distanceSteps = 0;
      if(distance >= minDistance)
      {
        distanceSteps = (distance-minDistance) / step;
      }
      Serial.print("Distance steps calculated: ");
      Serial.println(distanceSteps);
      CRGB color;
      CRGB backgroundColor = CRGB::Black;
      if(distanceSteps <= redZone)
      {
        color = colorRed;
      }
      else if (distanceSteps <= yellowZone)
      {
        color = colorYellow;
        backgroundColor = colorRed;
        distanceSteps -= redZone;
      }
      else if (distanceSteps < greenZone)
      {
        color = colorGreen;
        backgroundColor = colorYellow;
        distanceSteps -= yellowZone;
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
      
      if(distanceSteps >= greenZone)
      {
        for(int i = 0; i < ledCount; i++)
        {
          leds[i] = colorGreen;
        }
      }
      else
      {
        for(int i = 0; i < distanceSteps; i++)
        {
          leds[i] = color;
        }
      }
      FastLED.show();
  }
};
