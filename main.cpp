/*Libraries*/
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHBattery.h>
#include <FEHServo.h>
#include <FEHRCS.h>

/*Defines constants*/
#define pi 3.14159
#define one_inch 318/(pi*2.0*1.25)
#define one_degree 21.2/360.0
#define red_value 0.9
#define blue_value_low 1.1
#define blue_value_high 1.8
#define line 1.5 //1.5 is a placeholder value, will be measured eventually
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
AnalogInputPin middle_opto(FEHIO::P1_4);
AnalogInputPin left_opto(FEHIO::P1_5);

/*Drive functions (*Note that drive functions require inches > 1.2 inches)*/
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

    //Set motors to desired percent
    backward_motor.SetPercent((actual_power*percent*1.65));
    right_motor.SetPercent(actual_power*-percent);
    left_motor.SetPercent(actual_power*-percent);
    
    //While the average of the left, right, and back encoder is less than counts,
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

    //Set motors to desired percent
    backward_motor.SetPercent((actual_power*-percent*1.65));
    right_motor.SetPercent(actual_power*percent);
    left_motor.SetPercent(actual_power*percent);
    
    //While the average of the left, right, and back encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts() + backward_encoder.Counts()) + left_encoder.Counts()/ 3. < (inches-1.2)*one_inch);

    //Turn off motors
    backward_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

//Drives the robot forward given a speed and how many inches it needs to travel
void move_forward_lever(int left, int right, double inches)
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    backward_encoder.ResetCounts();

    //Set both motors to desired percent
    backward_motor.SetPercent(actual_power*right);
    right_motor.SetPercent(actual_power*-left);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((backward_encoder.Counts() + right_encoder.Counts()) / 2. < (inches-1.2)*one_inch);

    //Turn off motors
    right_motor.Stop();
    backward_motor.Stop();
}

//Drives the robot to the right given a speed and how many inches it needs to travel
void move_right_lever(int percent, double inches)
{
    //Reset encoder counts
    backward_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set motors to desired percent
    left_motor.SetPercent((actual_power*percent*1.65));
    right_motor.SetPercent(actual_power*-percent);
    backward_motor.SetPercent(actual_power*-percent);
    
    //While the average of the left, right, and back encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts() + backward_encoder.Counts()) + left_encoder.Counts()/ 3. < (inches-1.2)*one_inch);

    //Turn off motors
    backward_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

//Drives the robot to the left given a speed and how many inches it needs to travel
void move_left_lever(int percent, double inches)
{
    //Reset encoder counts
    backward_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set motors to desired percent
    backward_motor.SetPercent((actual_power*-percent*1.65));
    right_motor.SetPercent(actual_power*percent);
    left_motor.SetPercent(actual_power*percent);
    
    //While the average of the left, right, and back encoder is less than counts,
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
 
    //Set motors to desired percent
    right_motor.SetPercent(actual_power*percent);
    left_motor.SetPercent(actual_power*percent);
    backward_motor.SetPercent(actual_power*percent);
 
    //While the average of the left, right, and back encoder is less than counts,
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

    //Set motors to desired percent
    right_motor.SetPercent(actual_power*-percent);
    left_motor.SetPercent(actual_power*-percent);
    backward_motor.SetPercent(actual_power*-percent);

    //While the average of the left, right, and back encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts() + backward_encoder.Counts()) / 3. < one_degree*(degrees-2.0)*one_inch);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
    backward_motor.Stop();
}

/*Line following functions*/
//YET TO BE TESTED
//Follows a line until it reaches the line's end, requires at least one opto sensor to be on the line
void line_follow(int percent, double inches)
{
    //Reset encoder counts
    left_encoder.ResetCounts();
    right_encoder.ResetCounts();
    backward_encoder.ResetCounts();

    //Runs while at least one opto sensor detects the line
    //Idea is to keep running until the robot reaches the end of the line
    while (left_opto.Value() > line || middle_opto.Value() > line || right_opto.Value() > line)
    {
        //Sets right and left motor to desired percentages
        right_motor.SetPercent(percent);
        left_motor.SetPercent(-percent);
        
        if (left_opto.Value() > line && middle_opto.Value() > line) //If needs slight left
        {
            right_motor.SetPercent(10 + percent);
        }
        else if (right_opto.Value() > line && middle_opto.Value() > line)   //If needs slight right
        {
            left_motor.SetPercent(-10 - percent);
        }
        else if (left_opto.Value() > line)  //If needs strong left
        {
            right_motor.SetPercent(20 + percent);
        }
        else if (right_opto.Value() > line) //If needs strong right
        {
            left_motor.SetPercent(-20 - percent);
        }
    }

    //Stops the motors
    right_motor.Stop();
    left_motor.Stop();
    backward_motor.Stop();
}

//YET TO BE TESTED
//Drives forward until one of the opto sensors detect a line
void detect_line(int percent){
    //Resets encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    backward_encoder.ResetCounts();

    //Drives forward until at least one opto sensor detects the line
    while (left_opto.Value() < line && middle_opto.Value() < line && right_opto.Value() < line){
        right_motor.SetPercent(percent);
        left_motor.SetPercent(-percent);
    }

    //Stops the motors
    right_motor.Stop();
    left_motor.Stop();
}

/*Unique functions*/
//Returns 0 if blue, 1 if red, -1 if orange, displays message for each color
int detect_color(double voltage)
{
    //Initializes variable
    int color;
    if (voltage<=red_value){    //Sets color to 1, writes "Red" in red
        color = 1;
        LCD.Clear();
        LCD.SetFontColor(RED);
        LCD.WriteLine("Red");
    }
    else if (voltage >= blue_value_low && voltage <= blue_value_high){  //Sets color to 0, writes "Blue" in blue
        color = 0;
        LCD.Clear();
        LCD.SetFontColor(BLUE);
        LCD.WriteLine("Blue");
    }
    else    //Sets color to -1, displays an orange failure message
    {
        color = -1;
        LCD.Clear();
        LCD.SetFontColor(ORANGE);
        LCD.WriteLine("Color not detected...");
    }

    //Returns 0, 1, or -1 depending on the CdS value
    return color;
}

//Moves the robot RIGHT SPECIFICALLY FOR THE WINDOW, assumes robot is in place
//Takes in a movement speed and distance traveled
void move_right_window(int percent, double inches)
{
    //Reset encoder counts
    backward_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set motors to desired power
    backward_motor.SetPercent((actual_power*percent*0.6));
    right_motor.SetPercent(actual_power*-2.0*percent);
    left_motor.SetPercent(actual_power*-2.0*percent);
    
    //While the average of the left, right, and back encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts() + backward_encoder.Counts()) + left_encoder.Counts()/ 3. < (inches-1.2)*one_inch);

    //Turn off motors
    backward_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

//Moves the robot LEFT SPECIFICALLY FOR THE WINDOW, assumes robot is in place
//Takes in a movement speed and distance traveled
void move_left_window(int percent, double inches)
{
    //Reset encoder counts
    backward_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set motors to desired power
    backward_motor.SetPercent((actual_power*-percent*0.6));
    right_motor.SetPercent(actual_power*2.0*percent);
    left_motor.SetPercent(actual_power*2.0*percent);
    
    //While the average of the left, right, and back encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts() + backward_encoder.Counts()) + left_encoder.Counts()/ 3. < (inches-1.2)*one_inch);

    //Turn off motors
    backward_motor.Stop();
    right_motor.Stop();
    left_motor.Stop();
}

//Where the main code is written
//Feel free to add other functions, such as movements specifically for the wedge of the robot
int main(void)
{
    /*0. Initialize variables*/
    float x, y; 
    //RCS.InitializeTouchMenu("0800A9VHR"); // Code communicating with the RCS
    //int lever = RCS.GetLever(); // Get a 0, 1, or 2 indicating which lever to pull

    /*Testing*/
    /*0. Touch to start robot/Starting information*/
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Touch the screen");
    // if(lever == 0){  //Displays the lever information, taken out for sake of being time consumming, required for actual run
    //     LCD.WriteLine("Lever: A");
    // }
    // else if(lever == 1){
    //     LCD.WriteLine("Lever: B");
    // }
    // else if(lever == 2){
    //     LCD.WriteLine("Lever: C");
    // }
    // else{
    //     LCD.WriteLine("Lever not detected");
    // }
    LCD.WriteLine("Battery life:");
    LCD.WriteLine(Battery.Voltage());
    while(!LCD.Touch(&x,&y));
    while(LCD.Touch(&x,&y));   
    //while(CdS_cell.Value()>red_value); //Moves only when light is detected

    /*1. Position to first task*/
    // move_forward(30, 30, 6);
    // move_right(30, 3);
    // move_forward(40, 40, 36);
    // turn_left(30, 90);
    // move_forward(-30, -30, 3);
    // move_right(30, 4);
    move_right_lever(30, 10);

    /*2. LEVERS*/

    /*3. BUTTONS*/

    /*4. WINDOW*/

    /*5. APPLE BUCKET*/

    /*6. COMPOST*/

    return 0;
}