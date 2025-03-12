#include "main.h"
#include "stormlib/api.hpp"
#include "subsystems_auton.hpp"

// big money $_$
/////
// For installation, upgrading, documentations, and tutorials, check out our
// website! https://ez-robotics.github.io/EZ-Template/
/////


// Chassis constructor
ez::Drive chassis(
	// These are your drive motors, the first motor is used for sensing!
	{19, 20, -18},	 // Left Chassis Ports (negative port will reverse it!)
	{-12, 11, -13},	 // Right Chassis Ports (negative port will reverse it!)

	3,			   // IMU Port
	3.7454365097,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
	450);		   // Wheel RPM = cartridge * (motor gear / wheel gear)

ez::tracking_wheel horiz_tracker(14, 2.0279680025, -0.91);	// Create tracking wheel

// List of motors to get temperature
pros::Motor intake1(16);
pros::Motor intake2(17);
pros::Motor driveleft1(18);
pros::Motor driveleft2(19);
pros::Motor driveleft3(20);
pros::Motor driveright1(11);
pros::Motor driveright2(12);
pros::Motor driveright3(13);

vector<jas::motors::motordata> motorbar{{intake1, "intake 1"},	   {driveleft1, "drive l1"},  {driveleft2, "drive l2"},
										{driveleft3, "drive l3"},  {intake2, "intake 2"},	  {driveright1, "drive r1"},
										{driveright2, "drive r2"}, {driveright3, "drive r3"}, {ladybrown, "ladybrown"}};
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// Print our branding over your terminal :D
	ez::ez_template_print();

	pros::delay(500);  // Stop the user from doing anything while legacy ports configure

	chassis.odom_tracker_back_set(&horiz_tracker);	// Initlaize tracking wheel

	// Configure your chassis controls
	chassis.opcontrol_curve_buttons_toggle(false);	// Enables modifying the controller curve with buttons on the
													// joysticks
	chassis.opcontrol_drive_activebrake_set(4);		// Sets the active brake kP. We recommend ~2.  0 will disable.
	chassis.opcontrol_curve_default_set(5, 0);		// Defaults for curve. If using tank, only the first parameter is
													// used. (Comment this line out if you have an SD card!)

	// Set drive constants
	default_constants();

	// Auton selector
	j_auton_selector.jautonpopulate({
		jas::jasauton(red_4pos, 0, 1, "Red 4 ring pos", "Red 4 ring positive side.", 5, 0, false),
		jas::jasauton(red_6ring, 0, 0, "Red 6 ring", "Red 5 + 1 negitive side.", 6, 0, false),
		jas::jasauton(red_negsolowp, 0, 0, "Red negative solo WP", "Red 3 + 2 + 1 negitive side.", 3, 2, true),
		
		jas::jasauton(blue_4pos, 1, 1, "Blue 4 ring pos", "Red 4 ring positive side.", 5, 0, false),
		jas::jasauton(blue_6ring, 1, 0, "Blue 6 ring", "Blue 5 + 1 negitive side.", 6, 0, true),
		jas::jasauton(blue_negsolowp, 1, 0, "Blue negative solo WP", "Blue 3 + 2 + 1 negitive side.", 3, 2, true),
		
		jas::jasauton(move_forward, 2, 2, "Move forward", "Drive straight forward.", 0, 0, false),
		jas::jasauton(ram, 2, 2, "ram", "Ram corner", 2, 0, false),
		jas::jasauton(testautonRed, 0, 2, "red test", "intake + clamp test.", 0, 0, false),
		jas::jasauton(testautonBlue, 1, 2, "blue test", "intake + clamp test.", 0, 0, false),
	});

	// Initialize chassis and auton selector
	chassis.initialize();
	_init_fs();
	screeninit();
	master.rumble(".");
	pros::Task tempchecktask(tempcheckTask);
	pros::Task colortask(colorTask);
	pros::Task unjamtask(unjamTask);
	pros::Task ladybrowntask(ladybrownTask);
	pros::Task distancetask(distanceTask);
	pros::Task ledAlliance(ledAllianceTask);
	ladybrown.set_brake_mode(MOTOR_BRAKE_HOLD);
	LEDmanager.initialize(20);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	// . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	// Start screen on auton selector
	lv_obj_set_tile(mainscreen, autoselector, LV_ANIM_ON);
	//comp = true;
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	chassis.pid_targets_reset();				// Resets PID targets to 0
	chassis.drive_imu_reset();					// Reset gyro position to 0
	chassis.drive_sensor_reset();				// Reset drive sensors to 0
	chassis.odom_xyt_set(0_in, 0_in, 0_deg);	// Reset robot position
	chassis.drive_brake_set(MOTOR_BRAKE_HOLD);	// Set motors to hold

	// Auton callback handling
	intakeLevel.set(true);
	if(lv_tileview_get_tile_act(mainscreen) == autobuilder)
		autocallback();
	else if(lv_tileview_get_tile_act(mainscreen) == manbuilder)
		mancallback();
	else {
		if(noselection == false) jautonrun();
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol() {
	//OPstarted = true;
	// Update the state of the robot to be optimal for driver control
	chassis.drive_brake_set(MOTOR_BRAKE_BRAKE);
	lv_obj_set_tile(mainscreen, motortemps, LV_ANIM_ON);
	intakeLevel.set(true);
	
	driverClock.start();
	pros::Task ledTimer(ledTimeTask);

	while(true) {

		// Tank control
		chassis.opcontrol_tank();

		// Subsystems
		opcontrolIntake();
		opcontrolMogo();
		opcontrolLadyBrown();
		opcontrolDoinkerR();
		opcontrolDoinkerL();

		/*
		if (driverClock.timeLeft() > 40 * 1000) {
			LEDmanager.setColor(SPURFLY);
		} else if (driverClock.timeLeft() <= 40 * 1000 && driverClock.timeLeft() > 30 * 1000) {
			LEDmanager.pulse(0xb00c8f, 4, 500); 
		} else {
			LEDmanager.rainbow();
		}
		*/
		pros::delay(ez::util::DELAY_TIME);
	}
}