#include "main.h"
#include "okapi/api/chassis/model/xDriveModel.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/util/mathUtil.hpp"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include <memory>

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
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
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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

	Motor turnermotor(5);

	std::shared_ptr<ChassisController> drive = ChassisControllerBuilder()
			.withMotors(
					-12,
					11,
					14,
					-13
				   )
			.withDimensions(AbstractMotor::gearset::green, {{4_in, 14.5_in}, imev5GreenTPR})
			.build();
	auto xModel = std::dynamic_pointer_cast<XDriveModel>(drive->getModel());

	drive->moveDistance(24_in);
	pros::delay(700);
	drive->turnAngle(90_deg);

	/*xModel->strafe(200);
	pros::delay(500);
	xModel->stop();
	pros::delay(700);
	turnermotor.moveVelocity(200);*/

	pros::delay(500);
	turnermotor.moveVelocity(0);

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
	//pros::Controller master(pros::E_CONTROLLER_MASTER);

	//pros::Motor spindlething(10);

	Controller controller;
	ControllerButton spindleupbutton(ControllerDigital::R1);
	ControllerButton spindledownbutton(ControllerDigital::R2);

	ControllerButton turnerfwdbutton(ControllerDigital::L2);
	ControllerButton turnerbwbutton(ControllerDigital::L1);

	Motor spindlemotor(10);

	Motor turnermotor(5);

	std::shared_ptr<ChassisController> drive = ChassisControllerBuilder()
			.withMotors(
					-12,
					11,
					14,
					-13
				   )
			.withDimensions(AbstractMotor::gearset::green, {{4_in, 14.5_in}, imev5GreenTPR})
			.build();
	auto xModel = std::dynamic_pointer_cast<XDriveModel>(drive->getModel());

	while (true) {
		/*pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);*/
		

		

		//drive->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY),
		//		controller.getAnalog(ControllerAnalog::leftX));

		xModel->fieldOrientedXArcade(controller.getAnalog(ControllerAnalog::leftX), controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::rightX), okapi::QAngle(0, 0, 0, 0));

		//xModel->strafe(100);

		/*if (!master.get_digital(DIGITAL_R2))
			spindlething.move_velocity(0);
		if (master.get_digital(DIGITAL_R2))
			spindlething.move_velocity(120);

		if (!master.get_digital(DIGITAL_R1))
			spindlething.move_velocity(0);
		if (master.get_digital(DIGITAL_R1))
			spindlething.move_velocity(-120);*/

		if (spindleupbutton.changedToPressed()) {
			spindlemotor.moveVelocity(200);
		} else if (spindleupbutton.changedToReleased()) {
			spindlemotor.moveVelocity(0);
		}

		if (spindledownbutton.changedToPressed()) {
			spindlemotor.moveVelocity(-200);
		} else if (spindledownbutton.changedToReleased()) {
			spindlemotor.moveVelocity(0);
		}


		if (turnerfwdbutton.changedToPressed()) {
			turnermotor.moveVelocity(100);
		} else if (turnerfwdbutton.changedToReleased()) {
			turnermotor.moveVelocity(0);
		}

		if (turnerbwbutton.changedToPressed()) {
			turnermotor.moveVelocity(-100);
		} else if (turnerbwbutton.changedToReleased()) {
			turnermotor.moveVelocity(0);
		}

		pros::delay(10);
	}

}
