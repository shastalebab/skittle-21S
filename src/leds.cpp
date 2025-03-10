#include "main.h"
#include "pros/adi.hpp"
#include "api.h"


void setDriveLED() {
    x = 0x127012;
    leftDriveLED.set_all(x);
    rightDriveLED.set_all(x);
}
