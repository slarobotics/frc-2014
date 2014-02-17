#include "WPILib.h"
#include "Input.h"
#include "enum.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 
class RobotDemo : public IterativeRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick left, right, control; // only joystick
	Input input;
	Victor intake1, intake2, intakeUpDown, kicker;
	Timer timer;
	KickerPhase kickPhase;
	void ProcessInputs();
	void ApplyOutputs();
	void RobotThink();

public:
	RobotDemo():
		myRobot(2, 3),	// these must be initialized in the same order
		left(1),		// as they are declared above.
		right(2),
		control(3), //order important! change it in setup tab of DS
		intake1(4),
		intake2(5),
		intakeUpDown(6),
		kicker(7),
		timer()
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
	kickPhase = OFF;
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
	timer.Reset();
	timer.Start();
}

/**
 * Periodic code for autonomous mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in autonomous mode.
 */
void RobotDemo::AutonomousPeriodic(){
	//printf("%lf\n", timer.Get());
	if(timer.Get() >= 3.0){
		timer.Stop();
		myRobot.TankDrive(0.0, 0.0);
	}
	else
		myRobot.TankDrive(1.0, 1.0);
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
	input.controlY = control.GetY();
	input.kick = right.GetTrigger() || left.GetTrigger(); //either trigger
	input.intake = 0;
	if(left.GetRawButton(2))
		input.intake = left.GetRawButton(2);
	if(right.GetRawButton(2))
		input.intake = !right.GetRawButton(2);
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
	//if(!10percentbutton)
	if(left.GetY() > -0.7 && left.GetY() < 0.7){
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
	}
	input.controlY *= -1;
	if(input.kick && kickPhase == OFF){ //only start kicking if not kicking!
		kickPhase = KICK;
	}
}
void RobotDemo::ApplyOutputs(){
	intakeUpDown.Set(input.controlY);
	intake1.Set(input.intake);
	intake2.Set(-input.intake);
	myRobot.TankDrive(input.leftY, input.rightY);
	switch(kickPhase){
	case OFF:
		//printf("kicker OFF\n");
		timer.Reset();
		kicker.Set(0);
		break;
	case KICK:
		timer.Start();
		kicker.Set(1); // go forward for half a second
		//printf("KICKing\n");
		if(timer.Get() >= 0.5){
			//printf("Timer at 0.5, transitioning to RETURN\n");
			timer.Stop();
			timer.Reset();
			kickPhase = RETURN;
		}
		break;
	case RETURN:
		//timer.Reset(); // just to be safe
		timer.Start();
		kicker.Set(-0.5);
		//printf("RETURNing, timer at %lf\n", timer.Get());
		if(timer.Get() >= 1.0){
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

