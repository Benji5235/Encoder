/*Libraries*/
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHBattery.h>

/*Defines constants*/
#define pi 3.14159
#define one_inch 318/(pi*2.0*1.25)
#define one_degree 21.2/360.0
#define red_value 0.9
#define blue_value_low 1.1
#define blue_value_high 1.8
#define line 1.5

/*Declarations for all encoders, (servo) motors, CdS cells, and optosensors*/
DigitalEncoder right_encoder(FEHIO::P1_0);
DigitalEncoder left_encoder(FEHIO::P1_1);
DigitalEncoder backward_encoder(FEHIO::P1_2);
FEHMotor right_motor(FEHMotor::Motor3,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);
FEHMotor backward_motor(FEHMotor::Motor2,9.0);
AnalogInputPin CdS_cell(FEHIO::P2_0);
AnalogInputPin right_opto(FEHIO::P1_3);
AnalogInputPin left_opto(FEHIO::P1_4);
AnalogInputPin middle_opto(FEHIO::P1_5);


/*Drive functions (note that drive functions require inches > 1.2 inches)*/
//Drives the robot forward given a speed and how many inches it needs to travel
void move_forward(int left, int right, double inches)
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(right);
    left_motor.SetPercent(-left);

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
    left_encoder.ResetCounts();

    backward_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);
    
    //While the average of the left and back encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + backward_encoder.Counts()) / 2. < (inches-1.2)*one_inch);

    //Turn off motors
    backward_motor.Stop();
    left_motor.Stop();
}

//Drives the robot to the left given a speed and how many inches it needs to travel
void move_left(int percent, double inches)
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    backward_encoder.ResetCounts();

    //Set both motors to desired percent
    backward_motor.SetPercent(-percent);
    right_motor.SetPercent(percent);

    //While the average of the left and back encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts() + backward_encoder.Counts()) / 2. < (inches-1.2)*one_inch);

    //Turn off motors
    right_motor.Stop();
    backward_motor.Stop();
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
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);
    backward_motor.SetPercent(percent);
 
    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts() + backward_encoder.Counts()) / 3. < one_degree*degrees*one_inch);
 
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
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(-percent);
    backward_motor.SetPercent(-percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts() + backward_encoder.Counts()) / 3. < one_degree*degrees*one_inch);

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


int main(void)
{
    float x, y; //for touch screen

    /*1. Touch to start robot*/
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Milestone 2");
    LCD.WriteLine("Touch the screen");
    LCD.WriteLine("Battery life:");
    LCD.WriteLine(Battery.Voltage());
    while(!LCD.Touch(&x,&y));
    while(LCD.Touch(&x,&y));    
    while(CdS_cell.Value()>red_value);  
    
    /*2. Drives up ramp to table*/
    move_forward(35, 35, 3);
    //Sleep(2.0); //Pauses to move right
    //move_right(30, 1.5);  //TEST
    backward_motor.SetPercent(35);   //BACKUP
    left_motor.SetPercent(-35);
    Sleep(0.50);
    backward_motor.Stop();
    left_motor.Stop();
    move_forward(50, 49, 25);
    move_forward(35, 35, 15.0);

    /*3. Goes up to light to read CdS cell value IF ROBOT VEERS TOO MUCH WHILE TRAVELING RAMP CAUSES PROBLEM, ALSO DISTANCE TO LIGHT IS VARIABLE*/
    move_forward(-35, -35, 3.5);   //Next three lines use table, go backwards, turn, flush with wall, go
    turn_left(30, 88);  
    move_forward(-35, -35, 4.0);
    move_forward(36, 35, 16.25);
    Sleep(2.0); //Pauses before reading value
    int value = detect_color(CdS_cell.Value());
    LCD.WriteLine(CdS_cell.Value());
    Sleep(2.0); //Pauses before moving depending on value of CdS_cell

    /*4. Move depending on CdS value, includes failure condition*/
    if (CdS_cell.Value()<red_value){
        //move_right(30, 2.0);  //TEST
        backward_motor.SetPercent(30);   //BACKUP
        right_motor.SetPercent(-30);
        Sleep(0.25);
        backward_motor.Stop();
        right_motor.Stop();
        move_forward(30, 30, 10);
    }
    else if (CdS_cell.Value()>blue_value_low && CdS_cell.Value()<blue_value_high)
    {
        //move_left(30, 2.0);   //TEST
        backward_motor.SetPercent(-30);  //BACKUP
        left_motor.SetPercent(30);
        Sleep(0.25);
        backward_motor.Stop();
        left_motor.Stop();
        move_forward(30, 30, 10);
    }
    else
    {
        LCD.WriteLine("Blue");
        //move_left(30, 2.0);   //TEST
        backward_motor.SetPercent(-30);  //BACKUP
        left_motor.SetPercent(30);
        Sleep(0.25);
        backward_motor.Stop();
        left_motor.Stop();
        move_forward(30, 30, 10);
        
    }
    LCD.WriteLine(value);   //Displays the result of the detect_color method

    move_forward(-35, -35, 24);
    move_forward(35, 35, 5);
    turn_left(30, 90);
    move_forward(31, 30, 32);
    backward_motor.SetPercent(30);   //BACKUP
    right_motor.SetPercent(-30);
    Sleep(0.125);
    backward_motor.Stop();
    right_motor.Stop();
    move_forward(30, 40, 15);

    return 0;
}