#include "main.h"
#include "ARMS/config.h"
#include "Region-config.h"
#include "pros/adi.hpp"
#include <iostream>


//main robot code 

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */

//area for custom functions
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	int auto_v; 
	auto_v = auto_select.get_angle() / 62.5;
	auto_v = floor(auto_v);
	if (auto_v >= 5) {
		switch (arms::selector::auton)
		{
		case 3:
			T_wheel.set_value(true); //setting tracker wheel up
			nothing();
		case 0:
			T_wheel.set_value(false); //setting tracker wheel down
			skills();
			#define USING_TRACKER_WHEEL
		
		}
	}
	arms::init();
	pros::lcd::initialize();
	//arms::selector::init;
	pros::lcd::register_btn1_cb(on_center_button);
	hang.set_value(false);

	
}


/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	
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
	arms::init();
	pros::lcd::initialize();
	//arms::selector::init;
	//updates pot for a value
	
	int auto_v; 
	auto_v = auto_select.get_angle() / 62.5;
	auto_v = floor(auto_v);
	pros::lcd::print(2, "Pot auto:%d", auto_v);
	
	
	


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

//auto cata commands

void load() {
	int CataAngle_a = cata_track.get_angle();
	while (true) {	
    
    if ( 29700 > CataAngle_a)
		{
			Cata.brake();
			Cata.set_brake_mode(MOTOR_BRAKE_HOLD);
            break;
            
		}
		else {
			Cata.move(100);
		}
    }
}

void fire() {
	Cata.move_relative(1000,100);
}
void autonomous() {
	int auto_v; 
	auto_v = floor(auto_select.get_value() /750);
	pros::lcd::print(2, "Pot auto:%d", auto_v);
	pros::lcd::print(3, "Pot auto:%d", auto_select.get_value());
	arms::odom::reset({0,0},0);
	arms::odom::imu.reset();
	arms::selector::destroy();
	pros::lcd::print(1, "ARMS auto:%d", arms::selector::auton);
	
if (auto_v >= 5) {
	switch (arms::selector::auton)
	{
	case 0:
		 //setting tracker wheel up
		nothing();
	
	
	case 3:
		 //setting tracker wheel down
		skills();
	
	}
}
else {
	switch (auto_v)
	{
	case 0: // 4 on wheel
		 //setting tracker wheel up
		nothing();
		

	case 1: // 3 on wheel
		//setting tracker wheel up
		far();
		break;
	
	case 3: // 2 on wheel
		 //setting tracker wheel up
		Close();
		break;
		
	}
}
/*switch returns value of () and uses appropiate case, more efficent than if, elif because it only checks once */
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

// function for wing deploy
bool wing_state;
	void wing_toggle() {
		if (wing_state) {
			wings.set_value(false);
			pros::delay(1000);
			wing_state = false;
		}
		else {
			wings.set_value(true);
			pros::delay(1000);
			wing_state = true;
		}
	}



void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	arms::chassis::setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	int CataAngle;
	int select_value; 
	bool intake_movef;
	bool intake_moveb;
	bool cata_shoot;
	bool wing_t;
	bool wing_state;
	std::string select_type;
	CataAngle = cata_track.get_angle();
	bool safety = false;
	bool cata_down = false;
	
	cata_track.set_data_rate(5);
	cata_track.reset_position();
	
	int cata_Pull_Angle = 32291;
	int cata_Move_Angle = cata_Pull_Angle - 00050;
	while (true) {
		/* creating and setting variables*/
		int auto_v; 
		auto_v = floor(auto_select.get_value() /750);
		select_value = floor( drive_select.get_value() / 1400) ;


		/*screen printing dialouge*/
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		pros::lcd::set_text(0, "X: " + std::to_string(arms::odom::getPosition().x));
		pros::lcd::set_text(1, "Y: " + std::to_string(arms::odom::getPosition().y));
		pros::lcd::set_text(2, "H: " + std::to_string(arms::odom::getHeading()));
		pros::lcd::set_text(3, "Left: " + std::to_string(arms::odom::getLeftEncoder()));
		pros::lcd::set_text(4, "Right: " + std::to_string(arms::odom::getRightEncoder()));
		pros::lcd::set_text(5, "Middle: " + std::to_string(arms::odom::getMiddleEncoder()));
		pros::lcd::print(6, "Cata Angle:%d", CataAngle);
		pros::lcd::print(7, "drive-select:%d", select_value);
		master.print(0, 0, "safety: %d", auto_v);


		//odom and PID tuning dialouge
		
		if (master.get_digital_new_press(DIGITAL_X)) {
			Cata.move(100);
			pros::delay(30000);
			Cata.move(0);
		}
		

						 
		// controller profiles

		switch (select_value)
		{
		case 0:
			arms::chassis::tank(master.get_analog(ANALOG_LEFT_Y) * (double)100 / 127,
		                      (master.get_analog(ANALOG_LEFT_X) * master.get_analog(ANALOG_LEFT_X)) / 10);
			select_type = "parker";

			//button controls
			if (master.get_digital(DIGITAL_R2)){
				intake_movef = true;
			}
			else if (master.get_digital(DIGITAL_L2))
			{
				intake_moveb = true;
			}
			else {
				intake_movef = false;
				intake_moveb = false;
			}
			if (master.get_digital(DIGITAL_R1)) {
				cata_shoot = true;
			}
			else {
				cata_shoot = false;
			}
			if (master.get_digital(DIGITAL_L1)) {
				wing_t = true;
			}
			else {
				wing_t = false;
			}
		break;

		case 1:
			arms::chassis::arcade(master.get_analog(ANALOG_LEFT_Y) * (double)100 / 127,
								master.get_analog(ANALOG_RIGHT_X) * (double)100 /127);	

			select_type = "Alec";

			//button controls 
			if (master.get_digital(DIGITAL_R1)){
				intake_movef = true;
			}
			else if (master.get_digital(DIGITAL_L1))
			{
				intake_moveb = true;
			}
			else {
				intake_movef = false;
				intake_moveb = false;
			}
			if (master.get_digital(DIGITAL_R2)) {
				cata_shoot = true;
			}
			else {
				cata_shoot = false;
			}
			if (master.get_digital(DIGITAL_L2)) {
				wing_t = true;
			}
			else {
				wing_t = false;
			}
		break;

		case 2:
			arms::chassis::arcade(master.get_analog(ANALOG_LEFT_Y) * (double)100 / 127,
								master.get_analog(ANALOG_RIGHT_X) * (double)100 /127);
			select_type = "drew";

			//button controls
			if (master.get_digital(DIGITAL_R1)){
				intake_movef = true;
			}
			else if (master.get_digital(DIGITAL_R2))
			{
				intake_moveb = true;
			}
			else {
				intake_movef = false;
				intake_moveb = false;
			}
			if (master.get_digital(DIGITAL_L1)) {
				cata_shoot = true;
			}
			else {
				cata_shoot = false;
			}
			if (master.get_digital(DIGITAL_L2)) {
				wings.set_value (true);
				intake.move(-100);
			}
			else {
				wings.set_value (false);
			}
		break;
		
		}


		//intake controls
		if (intake_movef) {
			intake.move(100);
		}
		
		else if (intake_moveb){
			intake.move(-100);
		}
		
		else {
			intake.move(0);
		}
		
		//cata controls
		if ( 32291 > CataAngle && (cata_shoot== 1))
		{
			Cata.move_voltage(11000);
			intake.move_relative(1000,100);
			pros::delay(250);
			CataAngle = cata_track.get_angle();
		}
		
		 else if ( 32291 > CataAngle )
		{
			Cata.brake();
			Cata.set_brake_mode(MOTOR_BRAKE_HOLD);
			pros::delay(5);
			CataAngle = cata_track.get_angle();
			cata_down = true;
		}
		else if ( CataAngle > 32291 )
		{
			Cata.move_voltage(11000);
			pros::delay(5);
			CataAngle = cata_track.get_angle();
			cata_down = false;
		}

		//wings control
		if (wing_t) {
			wing_toggle();
		}
		
		//blocker controls 
		if (master.get_digital(DIGITAL_Y)) {
			blocka.set_value(true);
		}
		else if (master.get_digital(DIGITAL_B)) {
			blocka.set_value(false);
		}


		/* (master.get_digital(DIGITAL_DOWN)) {
			hang.set_value(false);
		}*/

	
		

		//safety trigger
		if (master.get_digital(DIGITAL_RIGHT)) {
			pros::delay(100);
			if (safety) {
			safety = false;
			}
			else {
			safety = true;
			}
		}

		//auto shoot 
		
		if (ball_sensor.get() <= 10 && safety && cata_down ) {
			pros::delay(100);
			Cata.move(100);
			pros::delay(100);
		}
		//catch if cata over rotates
		if (CataAngle < 2) {
			Cata.move(100);
		}
		if (master.get_digital(DIGITAL_A))
		{
			hang.set_value(true);
			
			
		}
		pros::delay(20);
	}
}
