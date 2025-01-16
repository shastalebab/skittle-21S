#include "main.h"

// big money $_$
/////
// For installation, upgrading, documentations, and tutorials, check out our
// website! https://ez-robotics.github.io/EZ-Template/
/////

// Chassis constructor
ez::Drive chassis(
	// These are your drive motors, the first motor is used for sensing!
	{-20, -19, -18},  // Left Chassis Ports (negative port will reverse it!)
	{11, 12, 13},	  // Right Chassis Ports (negative port will reverse it!)

	21,	   // IMU Port
	3.25,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
	450);  // Wheel RPM = cartridge * (motor gear / wheel gear)

// Are you using tracking wheels?  Comment out which ones you're using here!
//  `2.75` is the wheel diameter
//  `4.0` is the distance from the center of the wheel to the center of the robot
// ez::tracking_wheel right_tracker({-'A', -'B'}, 2.75, 4.0);  // ADI Encoders
ez::tracking_wheel vertical_tracker(1, 2.75, 0.7);	// Rotation sensors
ez::tracking_wheel horiz_tracker(2, 2.75, -0.5);
// list of motors to get temperature
pros::Motor intake1(4);
pros::Motor intake2(6);
pros::Motor driveleft1(-13);
pros::Motor driveleft2(-14);
pros::Motor driveleft3(-15);
pros::Motor driveright1(17);
pros::Motor driveright2(18);
pros::Motor driveright3(20);

vector<jas::motors::motordata> motorbar{{intake1, "intake 1"}, {driveleft1, "drive l1"},  {driveleft2, "drive l2"},	 {driveleft3, "drive l3"},
										{intake2, "intake 2"}, {driveright1, "drive r1"}, {driveright2, "drive r2"}, {driveright3, "drive r3"}};
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

	// Are you using tracking wheels?  Comment out which ones you're using here!
	chassis.odom_tracker_right_set(&vertical_tracker);
	// chassis.odom_tracker_left_set(&left_tracker);
	chassis.odom_tracker_back_set(&horiz_tracker);	// Replace `back` to `front` if your tracker is in the front!

	// Configure your chassis controls
	chassis.opcontrol_curve_buttons_toggle(false);	// Enables modifying the controller curve with buttons on the
													// joysticks
	chassis.opcontrol_drive_activebrake_set(4);		// Sets the active brake kP. We recommend ~2.  0 will disable.
	chassis.opcontrol_curve_default_set(5, 0);		// Defaults for curve. If using tank, only the first parameter is
													// used. (Comment this line out if you have an SD card!)

	// Set the drive to your own constants from autons.cpp!
	default_constants();

	// These are already defaulted to these buttons, but you can change the
	// left/right curve buttons here!
	// chassis.opcontrol_curve_buttons_left_set(pros::E_CONTROLLER_DIGITAL_LEFT,
	// pros::E_CONTROLLER_DIGITAL_RIGHT);  // If using tank, only the left side is
	// used.
	// chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y,
	// pros::E_CONTROLLER_DIGITAL_A);

	// Autonomous Selector using lvgl
	j_auton_selector.jautonpopulate(
		{jas::jasauton(blue_50WP, 1, 1, "Blue 50% WP", "Blue 2 ring in positive corner", 2, 0, false),
		 jas::jasauton(testautonBlue, 1, 2, "Blue test auton", "Testing for blue autons", 0, 0, false),
		 jas::jasauton(testcolorsortRed, 1, 2, "Blue color sort test", "Testing for blue color sorting functions", 1, 5, true),

		 jas::jasauton(red_50WP, 0, 1, "Red 50% WP", "Red 2 ring in positive corner", 2, 0, false),
		 jas::jasauton(testautonRed, 0, 2, "Red test auton", "Testing for red autons", 0, 0, false),
		 jas::jasauton(testcolorsortRed, 0, 2, "Red color sort test", "Testing for red color sorting functions", 5, 1, true),

		 jas::jasauton(blue_7ring, 1, 0, "Blue 7 ring WP", "Blue 6 ring in negative corner + 1 ring on alliance", 6, 0, true),
		 jas::jasauton(blue_4greed, 1, 0, "Blue 4 ring no WP", "Blue 4 ring in negative corner", 4, 0, false),
		 jas::jasauton(blue_4ring, 1, 0, "Blue 4 ring WP", "Blue 3 ring in negative corner + 1 ring on alliance", 3, 0, true),
		 jas::jasauton(blue_6ring, 1, 0, "Blue 6 ring no WP", "Blue 6 ring in negative corner", 6, 0, false),

		 jas::jasauton(red_7ring, 0, 0, "Red 7 ring WP", "Red 6 ring in negative corner + 1 ring on alliance", 6, 0, true),
		 jas::jasauton(red_4greed, 0, 0, "Red 4 ring no WP", "Red 4 ring in negative corner", 4, 0, false),
		 jas::jasauton(red_4ring, 0, 0, "Red 4 ring WP", "Red 3 ring in negative corner + 1 ring on alliance", 3, 0, true),
		 jas::jasauton(red_6ring, 0, 0, "Red 6 ring no WP", "Red 6 ring in negative corner", 6, 0, false),
		 jas::jasauton(skills, 0, 2, "Old skills auton", "Unfinished, inconsistent skills auton", 6, 6, true),
		 //jas::jasauton(skills50, 0, 2, "New skills auton", "Potential 50 pt skills auton- does not work yet, takes too long", 5, 6, true),

		 jas::jasauton(move_forward, 2, 2, "Move forward", "Drive straight forward", 0, 0, false),
		 });

	// auton builder modules, for testing
	/*j_auton_selector.jautonpopulate(
		{jas::jasauton(testautonbuilder, 2, 2, "Test auton modules", "Test auton builder modules manually", 4, 0, true),
		 jas::jasauton(goal_rush, 2, 2, "Goal rush module", "Positive corner POI, goal rush mogo", 0, 0, false),
		 jas::jasauton(alliancestake, 2, 2, "Alliance stake module", "Mutual POI, alliance stake score", 0, 0, true),

		 jas::jasauton(left_mogo, 2, 2, "Left mogo module", "Left side POI, closest mogo", 0, 0, false),
		 jas::jasauton(gr_mogo, 2, 2, "Left mogo module", "Left side POI, closest mogo", 0, 0, false),
		 jas::jasauton(right_mogo, 2, 2, "Right mogo module", "Right side POI, closest mogo", 0, 0, false),

		 jas::jasauton(left_corner, 2, 2, "Left corner module", "Left side POI, 2 ring corner rush", 2, 0, false),
		 jas::jasauton(right_corner, 2, 2, "Right corner module", "Right side POI, 2 ring corner rush", 2, 0, false),

		 jas::jasauton(red_ring_rush, 0, 2, "Red ring rush module", "Red alliance POI, cluster of 2 rings near mid", 2, 0, false),
		 jas::jasauton(blue_ring_rush, 1, 2, "Blue ring rush module", "Blue alliance POI, cluster of 2 rings near mid", 2, 0, false),

		 jas::jasauton(left_btm_ring, 2, 2, "Left bottom ring module", "Left side POI, bottom ring of ring stack", 1, 0, false),
		 jas::jasauton(mid_top_ring, 2, 2, "Mid top ring module", "Mutual POI, top ring of ring stack", 1, 0, false),
		 jas::jasauton(right_btm_ring, 2, 2, "Right bottom ring module", "Right side POI, bottom ring of ring stack", 1, 0, false),

		 jas::jasauton(ladder, 2, 2, "Ladder module", "POI for the central ladder, always goes to mid", 6, 6, true)});
*/
	// Initialize chassis and auton selector
	chassis.initialize();
	_init_fs();
	screeninit();
	master.rumble(".");
	pros::Task tempcheckcontroller(tempcheckctrl);
	pros::Task colordetection(colorDetect);
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
	// . . .
	lv_obj_set_tile(mainscreen, autobuilder, LV_ANIM_ON);
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
	chassis.odom_xyt_set(0_in, 0_in, 0_deg);	// Set the current position, you can start at a specific position with this
	chassis.drive_brake_set(MOTOR_BRAKE_HOLD);	// Set motors to hold.  This helps autonomous consistency

	/*
	Odometry and Pure Pursuit are not magic

	It is possible to get perfectly consistent results without tracking wheels,
	but it is also possible to have extremely inconsistent results without tracking wheels.
	When you don't use tracking wheels, you need to:
	 - avoid wheel slip
	 - avoid wheelies
	 - avoid throwing momentum around (super harsh turns, like in the example below)
	You can do cool curved motions, but you have to give your robot the best chance
	to be consistent
	*/

	// autonomous consistency
	if(lv_tileview_get_tile_act(mainscreen) == autobuilder)
		autocallback();
	else if(lv_tileview_get_tile_act(mainscreen) == manbuilder)
		mancallback();
	else {
		if(noselection == false) {
			printf("Running auton");
			jautonrun();
		}
	}
	// ez::as::auton_selector.selected_auton_call();  // Calls selected auton from
	// autonomous selector
}

/**
 * Gives you some extras to run in your opcontrol:
 * - run your autonomous routine in opcontrol by pressing DOWN and B
 *   - to prevent this from accidentally happening at a competition, this
 *     is only enabled when you're not connected to competition control.
 * - gives you a GUI to change your PID values live by pressing X
 */
void ez_template_etxras() {
	if(!pros::competition::is_connected()) {
		// PID Tuner
		// - after you find values that you're happy with, you'll have to set them in auton.cpp

		// Enable / Disable PID Tuner
		//  When enabled:
		//  * use A and Y to increment / decrement the constants
		//  * use the arrow keys to navigate the constants
		if(master.get_digital_new_press(DIGITAL_X)) chassis.pid_tuner_toggle();

		// Trigger the selected autonomous routine
		if(master.get_digital(DIGITAL_B) && master.get_digital(DIGITAL_DOWN)) {
			pros::motor_brake_mode_e_t preference = chassis.drive_brake_get();
			autonomous();
			chassis.drive_brake_set(preference);
		}

		// Blank pages for odom debugging
		if(chassis.odom_enabled() && !chassis.pid_tuner_enabled()) {
			// This is Blank Page 1, it will display X, Y, and Angle
			if(ez::as::page_blank_is_on(0)) {
				screen_print("x: " + std::to_string(chassis.odom_x_get()) + "\ny: " + std::to_string(chassis.odom_y_get()) +
								 "\nangle: " + std::to_string(chassis.odom_theta_get()),
							 1);  // Don't override the top Page line
			}
			// This is Blank Page 2, it will display every tracking wheel.
			// Make sure the tracking wheels read POSITIVE going forwards or right.
			else if(ez::as::page_blank_is_on(1)) {
				if(chassis.odom_tracker_left != nullptr)
					screen_print("left tracker: " + std::to_string(chassis.odom_tracker_left->get()), 1);
				else
					screen_print("no left tracker", 1);

				if(chassis.odom_tracker_right != nullptr)
					screen_print("right tracker: " + std::to_string(chassis.odom_tracker_right->get()), 2);
				else
					screen_print("no right tracker", 2);

				if(chassis.odom_tracker_back != nullptr)
					screen_print("back tracker: " + std::to_string(chassis.odom_tracker_back->get()), 3);
				else
					screen_print("no back tracker", 3);

				if(chassis.odom_tracker_front != nullptr)
					screen_print("front tracker: " + std::to_string(chassis.odom_tracker_front->get()), 4);
				else
					screen_print("no front tracker", 4);
			}
		}

		chassis.pid_tuner_iterate();  // Allow PID Tuner to iterate
	} else {
		// Remove all blank pages when connected to a comp switch
		if(ez::as::page_blank_amount() > 0) ez::as::page_blank_remove_all();

		// Disable PID tuner
		if(chassis.pid_tuner_enabled()) chassis.pid_tuner_disable();
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
	// This is preference to what you like to drive on
	chassis.drive_brake_set(MOTOR_BRAKE_BRAKE);
	scrpage = 2;
	lv_event_send(pageswitch, LV_EVENT_CLICKED, NULL);

	while(true) {
		// Gives you some extras to make EZ-Template easier
		ez_template_etxras();

		chassis.opcontrol_tank();  // Tank control
								   // chassis.opcontrol_arcade_standard(ez::SPLIT);   // Standard split arcade
								   // chassis.opcontrol_arcade_standard(ez::SINGLE);  // Standard single arcade
								   // chassis.opcontrol_arcade_flipped(ez::SPLIT);    // Flipped split arcade
								   // chassis.opcontrol_arcade_flipped(ez::SINGLE);   // Flipped single arcade

		// . . .
		// Put more user control code here!
		// . . .
		setIntake();
		setMogo();
		setLadyBrown();
		setDoinker();

		if(lv_tileview_get_tile_act(mainscreen) == motortemps) {
			for(int m = 0; m < motorbar.size(); m++) {
				lv_event_send(motorboxes[m], LV_EVENT_REFRESH, NULL);
			}
		}

		pros::delay(ez::util::DELAY_TIME);	// This is used for timer calculations! Keep this ez::util::DELAY_TIME
	}
}