#include "WPILib.h"
#include "Input.h"
#include "enum.h"
#include <cmath>
#include <iostream>

/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 
class RobotDemo : public IterativeRobot
{
	Joystick left, right, control; // only joystick
	Input input;
	Jaguar leftMC, rightMC;
	RobotDrive myRobot; // robot drive system
	Victor intakeSpin, intakeUpDown1, intakeUpDown2, kicker;
	Timer timer, autoTimer;
	KickerPhase kickPhase;
	DriveMode driveMode;
	void ProcessInputs();
	void ApplyOutputs();
	void RobotThink();

public:
	RobotDemo():	// these must be initialized in the same order
		left(2),		// as they are declared above.
		right(1),
		control(3), //order important! change it in setup tab of DS
		leftMC(2),
		rightMC(3),
		myRobot(leftMC, rightMC),
		intakeSpin(4),
		intakeUpDown1(5),
		intakeUpDown2(6),
		kicker(7),
		timer(),
		autoTimer()
	{
		myRobot.SetExpiration(0.1);
		this->SetPeriod(0); 	//Set update period to sync with robot control packets (20ms nominal)
	}
	
/**
 * Robot-wide initialization code should go here.
 * 
 * Use this method for default Robot-wide initialization which will
 * be called when the robot is first powered on.  It will be called exactly 1 time.
 */
void RobotDemo::RobotInit() {
	input.leftX = 0;
	input.leftY = 0;
	input.rightX = 0;
	input.rightY = 0;
	input.controlY = 0;
	input.intake = 0;
	input.intakeSpin = 0;
	input.kick = false;
	kickPhase = OFF;
	driveMode = TENPERCENT;
	leftMC.EnableDeadbandElimination(true);
	rightMC.EnableDeadbandElimination(true);
}

/**
 * Initialization code for disabled mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters disabled mode. 
 */
void RobotDemo::PrintInput ()
{
	std::cout << "start of input dump:\n";
	std::cout << "input.leftX: " << input.leftX << std::endl;
	std::cout << "input.leftY: " << input.leftY << std::endl;
	std::cout << "input.rightX: " << input.rightX << std::endl;
	std::cout << "input.rightY: " << input.rightY << std::endl;
	std::cout << "input.controlY: " << input.controlY << std::endl;
	std::cout << "input.intake: " << input.intake << std::endl;
	std::cout << "input.intakeSpin: " << input.intakeSpin << std::endl;
	std::cout << "input.kick: " << input.kick << std::endl;
	std::cout << "\n\n\n\n";
}
void RobotDemo::DisabledInit() {
}

/**
 * Periodic code for disabled mode should go here.
 * 
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in disabled mode.
 */
void RobotDemo::DisabledPeriodic() {
}

/**
 * Initialization code for autonomous mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters autonomous mode.
 */
void RobotDemo::AutonomousInit() {
	autoTimer.Reset();
	autoTimer.Start();
	driveMode = NORMAL;
}

/**
 * Periodic code for autonomous mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in autonomous mode.
 */
void RobotDemo::AutonomousPeriodic(){
	//printf("%lf\n", autoTimer.Get());
	//input.kick = true;
	RobotThink();
	ApplyOutputs();
	if(autoTimer.Get() >= 7.0){
		//autoTimer.Stop();
		myRobot.TankDrive(0.0, 0.0);
		input.intakeSpin = -1;
	}
	/*if(autoTimer.Get() == 9.0){
		autoTimer.Stop();
		input.intakeSpin = 0;
	}*/
	else
		myRobot.TankDrive(0.5, 0.5);
}

/**
 * Initialization code for teleop mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters teleop mode.
 */
void RobotDemo::TeleopInit() {
	driveMode = TENPERCENT;
}

/**
 * Periodic code for teleop mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in teleop mode.
 */
void RobotDemo::TeleopPeriodic() {
	ProcessInputs();
	RobotThink();
	ApplyOutputs();
}

/**
 * Initialization code for test mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters test mode.
 */
void RobotDemo::TestInit() {
}

/**
 * Periodic code for test mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in test mode.
 */
void RobotDemo::TestPeriodic() {
}

};
void RobotDemo::ProcessInputs(){
	if(left.GetRawButton(4) || right.GetRawButton(4)){
		driveMode = NORMAL;
	}
	if(left.GetRawButton(5) || right.GetRawButton(5)){
		driveMode = TENPERCENT;
	}
	if(left.GetRawButton(3) || right.GetRawButton(3)){
		driveMode = DEADBAND;
	}
	if(driveMode == NORMAL || driveMode == TENPERCENT){
		input.leftY = left.GetY();
		input.rightY = right.GetY();
	}
	input.controlY = control.GetY();
	input.kick = right.GetTrigger();
	input.intake = 0;
	if(control.GetRawButton(7))
		input.intakeSpin = 1;
	else if(control.GetRawButton(5))
		input.intakeSpin = -1;
	else
		input.intakeSpin = 0;
	if(control.GetRawButton(8))
		input.intake = 1;
	else if(control.GetRawButton(6))
		input.intake = -1;
	else
		input.intake = 0;
}
void RobotDemo::RobotThink(){
	/*if(input.leftY > left.GetY()){
	    input.leftY -= 0.05;
	}
	else if(input.leftY < left.GetY()){
		input.leftY += 0.05;
	}
	if(input.rightY > right.GetY()){
		input.rightY -= 0.05;
	}
	else if(input.rightY < right.GetY()){
		input.rightY += 0.05;
	}*/
	//printf("%i\n", driveMode);
	switch(driveMode){
	case DEADBAND:
		SmartDashboard::PutString("Drive Mode", "Deadband");
		if(abs(left.GetY()) <= 0.05)
			input.leftY = 0;
		if(abs(right.GetY()) <= 0.05)
			input.rightY = 0;
		/*if(left.GetY() > -0.7 && left.GetY() < 0.7){
			input.leftY = -5.0/7.0 * left.GetY();
		}
		else{
			input.leftY = -5.0/3.0 * left.GetY();
		}
		if(right.GetY() > -0.7 && right.GetY() < 0.7){
			input.rightY = -5.0/7.0 * right.GetY();
		}
		else{
			input.rightY = -5.0/3.0 * right.GetY();
		}*/
		if((left.GetY() <= 0.45 && left.GetY() > 0.05) || (right.GetY() <= 0.45 && right.GetY() > 0.05))
			input.leftY = -((left.GetY() - 0.05) / 4.0);
		if(right.GetY() <= 0.45 && right.GetY() > 0.05)
			input.rightY = -((right.GetY() - 0.05) / 4.0);
		if((left.GetY() <= 0.7 && left.GetY() > 0.45) || (right.GetY() <= 0.7 && right.GetY() > 0.45))
			input.leftY = -(5.0/6.0 * left.GetY() - 165.0 / 600.0);
		if(right.GetY() <= 0.7 && right.GetY() > 0.45)
			input.rightY = -(5.0/6.0 * right.GetY() - 165.0 / 600.0);
		
		if((left.GetY() <= 1.0 && left.GetY() > 0.7) || (right.GetY() <= 1.0 && right.GetY() > 0.7))
			input.leftY = -(13.0/5.0 * left.GetY() - 1.6);
		if(right.GetY() <= 1.0 && right.GetY() > 0.7)
			input.rightY = -(13.0/5.0 * right.GetY() - 1.6);
		
		if((left.GetY() >= -0.45 && left.GetY() < -0.05) || (right.GetY() >= -0.45 && right.GetY() < -0.05))
			input.leftY = -((left.GetY() + 0.05) / 4.0);
		if(right.GetY() >= -0.45 && right.GetY() < -0.05)
			input.rightY = -((right.GetY() + 0.05) / 4.0);
		
		if((left.GetY() >= -0.7 && left.GetY() < -0.45) || (right.GetY() >= -0.7 && right.GetY() < -0.45))
			input.leftY = -(5.0/6.0 * left.GetY() + 165 / 600.0);
		if(right.GetY() >= -0.7 && right.GetY() < -0.45)
			input.rightY = -(5.0/6.0 * right.GetY() + 165 / 600.0);
		
		if((left.GetY() >= -1.0 && left.GetY() < -0.7) || (right.GetY() >= -1.0 && right.GetY() < -0.7))
			input.leftY = -(13.0/5.0 * left.GetY() + 1.6);
		if(right.GetY() >= -1.0 && right.GetY() < -0.7)
			input.rightY = -(13.0/5.0 * right.GetY() + 1.6);
		
		//printf("right y: %lf\n", right.GetY());
		//printf("output right: %lf\n", input.rightY);
		break;
	case TENPERCENT:
		SmartDashboard::PutString("Drive Mode", "Raw + deadband");
		/*input.leftY *= -1;
		input.rightY *= -1;
		input.leftY /= 4;
		input.rightY /= 4;*/
		if(abs(left.GetY()) <= 0.05)
			input.leftY = 0;
		if(abs(right.GetY()) <= 0.05)
			input.rightY = 0;
		input.leftY *= -1;
		input.rightY *= -1;
		break;
	case NORMAL:
		SmartDashboard::PutString("Drive Mode", "Raw");
		input.leftY *= -1;
		input.rightY *= -1;
		break;
	}
	input.controlY *= -1;
	if(input.kick && kickPhase == OFF){ //only start kicking if not kicking!
		kickPhase = KICK;
	}
	SmartDashboard::PutNumber("left motor power", input.leftY);
	SmartDashboard::PutNumber("right motor power", input.rightY);
}
void RobotDemo::ApplyOutputs(){
	intakeUpDown1.Set(input.intake);
	intakeUpDown2.Set(-input.intake);
	intakeSpin.Set(input.intakeSpin);
	myRobot.TankDrive(input.leftY, input.rightY);
	switch(kickPhase){
	case OFF:
		//printf("kicker OFF\n");
		timer.Reset();
		kicker.Set(0);
		break;
	case KICK:
		timer.Start();
		kicker.Set(0.1); // go forward for half a second
		//printf("KICKing\n");
		if(timer.Get() >= 0.02){
			//printf("Timer at 0.5, transitioning to RETURN\n");
			timer.Stop();
			timer.Reset();
			kickPhase = RETURN;
		}
		break;
	case RETURN:
		//timer.Reset(); // just to be safe
		timer.Start();
		kicker.Set(-0.1);
		//printf("RETURNing, timer at %lf\n", timer.Get());
		if(timer.Get() >= 0.02){
			//printf("transitioning to OFF\n");
			timer.Stop();
			//timer.Reset();
			kickPhase = OFF;
		}
	}
	//printf("%i\n", kickPhase);
	//printf("input.leftY: %lf\n", input.leftY);
	//printf("input.rightY: %lf\n", input.rightY);
}

START_ROBOT_CLASS(RobotDemo);

