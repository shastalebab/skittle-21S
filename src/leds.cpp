#include "main.h"
#include "pros/adi.hpp"
#include "api.h"


void setDriveLED(int x) {
    leftDriveLED.set_all(x);
    rightDriveLED.set_all(x);
}
