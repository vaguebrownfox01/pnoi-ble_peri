#pragma once

#ifndef Pnoiext_h
#define Pnoiext_h

#include <Wire.h>

#define LED_PIN_RED 12
#define LED_PIN_GREEN 27
#define LED_PIN_BLUE 33

void init_pnoi_ext();
void glow_white();
void glow_red();
void glow_green();
void glow_blue();
void glow_pink();

#endif