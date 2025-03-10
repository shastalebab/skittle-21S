#include "main.h"
#include "pros/adi.hpp"
#include "api.h"


void setDriveLED() {
    leftDriveLED.set_all(0x127012);
    rightDriveLED.set_all(0x127012);
}
