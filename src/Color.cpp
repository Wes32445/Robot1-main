#include "main.h" 
#include "subsystems.hpp"
#include "pros/optical.h"
#include "color.hpp"


// Manual toggle control 
bool colorDetectionEnabled = true; 
bool xPressedLast = false; 
bool BallDetectedAuton = false; 

// motor 19 move control 
// motor 5 is color sensor 

// Color detection ranges
bool isRed(int hue ) {
    return (hue >= 0 && hue <= 10) || (hue >= 350 && hue <= 360);
}
bool isBlue(int hue) {
    return (hue >= 200 && hue <= 250);
}

// Detect either red or blue in auton 
bool detectBallColorAuton(pros::Optical& colorSensor) {
    int hue = colorSensor.get_hue();
    if (isRed(hue) || isBlue(hue)) {
        return true; 
    }
    pros::delay(10);
    return false; 
}

// continous object detection task: Test 1 
void colorDetectionTask(void* param) {
    pros::Controller controller(pros::E_CONTROLLER_MASTER);
    pros::Optical colorSensor(5); // Assuming port 5 for color sensor
    colorSensor.set_led_pwm(50);
    while (true) {
        // Toggle detection with X button 
        bool xPressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_X);
        if (xPressed && !xPressedLast) {
            colorDetectionEnabled = !colorDetectionEnabled; 
            controller.rumble(colorDetectionEnabled ? "." : "..");
        }
        xPressedLast = xPressed;

        int hue = colorSensor.get_hue();
        if (isRed(hue) || isBlue(hue)) {
            BallDetectedAuton = true;
        } else {
            BallDetectedAuton = false;
        }

        //run motor logic based on color detection
        if (colorDetectionEnabled) {
            if (BallDetectedAuton) {
                pros::Motor motor19(19);
                motor19.move_velocity(50); // Move motor
            } else {
                pros::Motor motor19(19);
                motor19.move_velocity(0); // Stop motor
            }
        } else {
            pros::Motor motor19(19);
            motor19.move_velocity(0); // Stop motor if detection is disabled
        }
        pros::delay(20); // Adjust delay as needed
    }
}