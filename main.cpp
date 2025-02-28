#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

//Declarations for encoders & motors
//This is a change required for reasons also Evan lol
DigitalEncoder right_encoder(FEHIO::P1_0);
DigitalEncoder left_encoder(FEHIO::P2_0);
DigitalEncoder forward_encoder(FEHIO::P0_1);
FEHMotor right_motor(FEHMotor::Motor0,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);
FEHMotor forward_motor(FEHMotor::Motor2,9.0);


void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-(percent));

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_left()
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    int counts = 255;

    //Set both motors to desired percent
    left_motor.Stop();
    right_motor.SetPercent(30);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_right()
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    int counts = 255;

    //Set both motors to desired percent
    left_motor.SetPercent(30);
    right_motor.Stop();

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

int main(void)
{
    int motor_percent = 30; //Input power level here
    int expected_counts = 567; //Input theoretical counts here

    float x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Shaft Encoder Exploration Test");
    LCD.WriteLine("Touch the screen");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    move_forward(motor_percent, 1200); //see function
    Sleep(10.0);
    move_forward(motor_percent, 1200);
    Sleep(1.0);
    move_forward(-motor_percent, 1200);

    Sleep(2.0); //Wait for counts to stabilize
    //Print out data
    LCD.Write("Theoretical Counts: ");
    LCD.WriteLine(expected_counts);
    LCD.Write("Motor Percent: ");
    LCD.WriteLine(motor_percent);
    LCD.Write("Actual LE Counts: ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.Write("Actual RE Counts: ");
    LCD.WriteLine(right_encoder.Counts());

    return 0;
}