#include "WPILib.h"
#include "Input.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 
class RobotDemo : public IterativeRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick left, right; // only joystick
	Input input;
	Victor intake1, intake2;
	void ProcessInputs();
	void ApplyOutputs();
	void RobotThink();

public:
	RobotDemo():
		myRobot(2, 3),	// these must be initialized in the same order
		left(1),		// as they are declared above.
		right(2),
		intake1(4),
		intake2(5)
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
}

/**
 * Initialization code for disabled mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters disabled mode. 
 */
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
}

/**
 * Periodic code for autonomous mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in autonomous mode.
 */
void RobotDemo::AutonomousPeriodic() {
}

/**
 * Initialization code for teleop mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters teleop mode.
 */
void RobotDemo::TeleopInit() {
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
	input.intake = left.GetRawButton(2);
}
void RobotDemo::RobotThink(){
	if(input.leftY > left.GetY()){
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
	}
}
void RobotDemo::ApplyOutputs(){
	if(input.intake){
		intake1.Set(1);
		intake2.Set(2);
	}
	else{
		intake1.Set(0);
		intake2.Set(0);
	}
	myRobot.TankDrive(input.leftY, input.rightY);
	//printf("%lf\n", input.leftY);
}

START_ROBOT_CLASS(RobotDemo);

