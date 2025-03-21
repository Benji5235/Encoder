/*Libraries*/
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHBattery.h>
#include <FEHServo.h>

/*Defines constants*/
#define pi 3.14159
#define one_inch 318/(pi*2.0*1.25)
#define one_degree 21.2/360.0
#define red_value 0.9
#define blue_value_low 1.1
#define blue_value_high 1.8
#define line 1.5
#define SERVO_MIN 500
#define SERVO_MAX 1250
#define actual_power 11.5/Battery.Voltage()

/*Declarations for all encoders, (servo) motors, CdS cells, and optosensors*/
DigitalEncoder right_encoder(FEHIO::P1_0);
DigitalEncoder left_encoder(FEHIO::P1_1);
DigitalEncoder backward_encoder(FEHIO::P1_2);
FEHMotor right_motor(FEHMotor::Motor3,9.0);
FEHMotor left_motor(FEHMotor::Motor2,9.0);
FEHMotor backward_motor(FEHMotor::Motor1,9.0);
AnalogInputPin CdS_cell(FEHIO::P2_0);
AnalogInputPin right_opto(FEHIO::P1_3);
AnalogInputPin left_opto(FEHIO::P1_4);
AnalogInputPin middle_opto(FEHIO::P1_5);
FEHServo bottom_servo(FEHServo::Servo7);
FEHServo top_servo(FEHServo::Servo1);

/*Drive functions (note that drive functions require inches > 1.2 inches)*/
//Drives the robot forward given a speed and how many inches it needs to travel
void move_forward(int left, int right, double inches)
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(actual_power*right);
    left_motor.SetPercent(actual_power*-left);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < (inches-1.2)*one_inch);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

//Drives the robot to the right given a speed and how many inches it needs to travel
void move_right(int percent, double inches)
{
    //Reset encoder counts
    backward_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    backward_motor.SetPercent((actual_power*percent*1.65));
    right_motor.SetPercent(actual_power*-percent);
    left_motor.SetPercent(actual_power*-percent);
    
    //While the average of the left and back encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts() + backward_encoder.Counts()) + left_encoder.Counts()/ 3. < (inches-1.2)*one_inch);

    //Turn off motors
    backward_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

//Drives the robot to the left given a speed and how many inches it needs to travel
void move_left(int percent, double inches)
{
    //Reset encoder counts
    backward_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    backward_motor.SetPercent((actual_power*-percent*1.65));
    right_motor.SetPercent(actual_power*percent);
    left_motor.SetPercent(actual_power*percent);
    
    //While the average of the left and back encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts() + backward_encoder.Counts()) + left_encoder.Counts()/ 3. < (inches-1.2)*one_inch);

    //Turn off motors
    backward_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

/*Turn functions*/
//Turns the robot left given a speed and degrees
void turn_left(int percent, int degrees)
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    backward_encoder.ResetCounts();
 
    //Set both motors to desired percent
    right_motor.SetPercent(actual_power*percent);
    left_motor.SetPercent(actual_power*percent);
    backward_motor.SetPercent(actual_power*percent);
 
    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts() + backward_encoder.Counts()) / 3. < one_degree*(degrees-2.0)*one_inch);
 
    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
    backward_motor.Stop();
}

//Turns the robot right given a speed and degrees
void turn_right(int percent, int degrees)
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    backward_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(actual_power*-percent);
    left_motor.SetPercent(actual_power*-percent);
    backward_motor.SetPercent(actual_power*-percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts() + backward_encoder.Counts()) / 3. < one_degree*(degrees-2.0)*one_inch);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
    backward_motor.Stop();
}

/*Line following functions (requires at least one optosensor on the line)*/
void line_follow(int percent, double inches)
{
    left_encoder.ResetCounts();
    right_encoder.ResetCounts();

    while ((left_encoder.Counts() + right_encoder.Counts())/2 < (inches-1.2)*one_inch)
    {
        right_motor.SetPercent(percent);
        left_motor.SetPercent(-percent);
        
        if (left_opto.Value() > line && middle_opto.Value() > line) //If needs slight left
        {
            backward_motor.SetPercent(10);
        }
        else if (right_opto.Value() > line && middle_opto.Value() > line)   //If needs slight right
        {
            backward_motor.SetPercent(-10);
        }
        else if (left_opto.Value() > line)  //If needs strong left
        {
            backward_motor.SetPercent(20);
        }
        else if (right_opto.Value() > line) //If needs strong right
        {
            backward_motor.SetPercent(-20);
        }
    }
    right_motor.Stop();
    left_motor.Stop();
    backward_motor.Stop();
}

/*Servo functions*/
void move_bottom_servo(float degree){
    bottom_servo.SetDegree(degree);
}
void move_top_servo(float degree){
    top_servo.SetDegree(degree);
}

/*Unique functions*/
//Returns 0 if blue, 1 if red, -1 if orange, changes background color accordingly
int detect_color(double voltage)
{
    int color;
    if (voltage<=red_value){
        color = 1;
        LCD.Clear();
        LCD.SetBackgroundColor(RED);
    }
    else if (voltage >= blue_value_low && voltage <= blue_value_high){
        color = 0;
        LCD.Clear();
        LCD.SetBackgroundColor(BLUE);
    }
    else
    {
        color = -1;
        LCD.Clear();
        LCD.SetBackgroundColor(ORANGE);
    }
    return color;
}

void move_right_window(int percent, double inches)
{
    //Reset encoder counts
    backward_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    backward_motor.SetPercent((actual_power*percent*0.6));
    right_motor.SetPercent(actual_power*-2.0*percent);
    left_motor.SetPercent(actual_power*-2.0*percent);
    
    //While the average of the left and back encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts() + backward_encoder.Counts()) + left_encoder.Counts()/ 3. < (inches-1.2)*one_inch);

    //Turn off motors
    backward_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

void move_left_window(int percent, double inches)
{
    //Reset encoder counts
    backward_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    backward_motor.SetPercent((actual_power*-percent*0.6));
    right_motor.SetPercent(actual_power*2.0*percent);
    left_motor.SetPercent(actual_power*2.0*percent);
    
    //While the average of the left and back encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts() + backward_encoder.Counts()) + left_encoder.Counts()/ 3. < (inches-1.2)*one_inch);

    //Turn off motors
    backward_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

int main(void)
{
    /*0. Initialize variables*/
    float x, y; 
    // //Touch screen (twice?) to calibrate MIN: 546, MAX: 1431
    // bottom_servo.TouchCalibrate();
    // bottom_servo.SetMin(SERVO_MIN);
    // bottom_servo.SetMin(SERVO_MAX);
    // top_servo.TouchCalibrate();
    // top_servo.SetMin(SERVO_MIN);
    // top_servo.SetMin(SERVO_MAX);

    // move_bottom_servo(90);
    // Sleep(2.0);
    // move_bottom_servo(180);
    // Sleep(2.0);

    /*1. Touch to start robot*/
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Testing:");
    LCD.WriteLine("Touch the screen");
    LCD.WriteLine("Battery life:");
    LCD.WriteLine(Battery.Voltage());
    while(!LCD.Touch(&x,&y));
    while(LCD.Touch(&x,&y));   
    while(CdS_cell.Value()>red_value); 

    /*2. Go back and push the bottom*/
    move_right(30, 3.0);
    Sleep(2.0);

    /*3. Go up the ramp*/
    move_forward(30, 31, 3.0);
    move_right(30, 2.0);
    move_forward(40, 42, 32.0);
    turn_left(30, 90);
    move_forward(-30, -30, 5);
    Sleep(2.0);

    /*4. Slide the robot to the right to open window*/
    move_forward(30, 30, 13.0);
    move_right(15, 6.0);
    turn_left(30, 90);
    move_forward(30,30,8.0);
    Sleep(2.0);
    move_right_window(25, 12.0);
    move_forward(30, 30, 5.0);
    Sleep(2.0);

    /*5. Get robot to move on opposite side of window*/
    move_forward(-30, -30, 5.0);
    move_right(15, 4.0);
    move_forward(30, 30, 4.25);
    Sleep(2.0);

    /*6. Move window back to close it*/
    move_left_window(30, 12.0);
    move_forward(30, 30, 5.0);

    return 0;
}