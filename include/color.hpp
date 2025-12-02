#pragma once
#include "main.h"

// Expose color detection task and shared flags to other translation units
extern bool colorDetectionEnabled;
extern bool xPressedLast;
extern bool BallDetectedAuton;

// Task entry point
void colorDetectionTask(void* param);
