#include "tdg-ColorBar.h"

ColorBar::ColorBar()
{
  
}

ColorBar::ColorBar(int ledCount, int dataPin, uint16_t minDistance, uint16_t maxDistance)
{
  this->ledCount = ledCount;
  this->minDistance = minDistance;
  this->maxDistance = maxDistance;
  step = (maxDistance - minDistance) / (3 * ledCount);
  if(step <= 0)
  {
    step = 1;
  }
  distance = 0;
  leds = new CRGB[ledCount];
  FastLED.addLeds<NEOPIXEL, 3>(leds, ledCount);
}

void ColorBar::setDistance(uint16_t distance)
{
  this->distance = distance;
}

void ColorBar::display()
{
    uint16_t distanceSteps = distance / step;
    CRGB color;
    CRGB backgroundColor;
    if(distanceSteps < ledCount)
    {
      color = CRGB::Red;
      backgroundColor = CRGB::Yellow;
    }
    else if (distanceSteps < (2 * ledCount))
    {
      color = CRGB::Yellow;
      backgroundColor = CRGB::Green;
    }
    else
    {
      color = CRGB::Green;
      backgroundColor = CRGB::Green;
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
      leds[0] = CRGB::Green;
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

  
