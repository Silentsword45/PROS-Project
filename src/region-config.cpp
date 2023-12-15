#include "Region-config.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pid.hpp"



pros::Motor lF(-10, pros::v5::MotorGears::blue); // port 1, blue gearbox, not reversed
pros::Motor lM(-9, pros::v5::MotorGears::blue); // port 1, blue gearbox, not reversed
pros::Motor lB(8, pros::v5::MotorGears::blue); // port 1, blue gearbox,  reversed
pros::Motor rF(1, pros::v5::MotorGears::blue); // port 1, blue gearbox, not reversed
pros::Motor rM(2, pros::v5::MotorGears::blue); // port 1, blue gearbox, not reversed
pros::Motor rB(-3, pros::v5::MotorGears::blue); // port 1, blue gearbox,  reversed


pros::Motor intake(5);
pros::Motor Cata(-4);
pros::adi::DigitalOut wings(1);
pros::Rotation cata_track (7);
pros::adi::Potentiometer drive_select(3);
pros::adi::Potentiometer auto_select (4);
pros::Rotation T_wheel(6);
pros::adi::DigitalOut blocka(5);
pros::adi::DigitalOut hang(6);
pros::Distance ball_sensor(19);
pros::Imu IMU(20);

pros::MotorGroup left_motors({-10,-9,8}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);
pros::MotorGroup right_motors({1,2,-3}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);


//drivetrain setup
lemlib::Drivetrain drivetrain(&left_motors, // left motor group
                              &right_motors, // right motor group
                              13, // 13 inch track width 
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              360, // drivetrain rpm is 360
                              2 // chase power is 2. If we had traction wheels, it would have been 8
);
lemlib::TrackingWheel horizontal(&T_wheel, 2.75, -3.75); 


//odom wheel setup
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to nullptr as we don't have one
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have one
                            &horizontal, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &IMU // inertial sensor
);
 
// forward/backward PID
lemlib::ControllerSettings lateralController (
    15, // kP
    50, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    5 // slew rate: Smaller number = more slew - slew = slow increase in speed
);
 
// turning PID
lemlib::ControllerSettings angularController (
    3, // kP
    19.25, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    2 // slew rate
);

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors); 