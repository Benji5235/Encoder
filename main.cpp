#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRCS.h>

FEHMotor left_motor(FEHMotor::Motor2, 9.0);
FEHMotor right_motor(FEHMotor::Motor1, 9.0);
DigitalInputPin distanceSensor(FEHIO::P3_0);
int turn_dir = 0;
bool var = true;
int main(void) 
{ 
    int objectInRange = distanceSensor.Value();
    LCD.WriteLine(objectInRange);

    //Go straight
    while (objectInRange == 1){
        objectInRange = distanceSensor.Value();
        LCD.WriteLine(objectInRange);
        right_motor.SetPercent(-25);
        left_motor.SetPercent(-25);
    }

    //Back up a little bit
    left_motor.SetPercent(20);
    right_motor.SetPercent(20);
    Sleep(1.0);

    //Turn left
    objectInRange = distanceSensor.Value();
    LCD.WriteLine(objectInRange);
    right_motor.SetPercent(-20);
    left_motor.SetPercent(20);
    Sleep(1.65);

    //Go straight
    while (objectInRange == 1){
        objectInRange = distanceSensor.Value();
        LCD.WriteLine(objectInRange);
        right_motor.SetPercent(-25);
        left_motor.SetPercent(-25);
    }

    //Back up a little bit
    left_motor.SetPercent(20);
    right_motor.SetPercent(20);
    Sleep(1.0);

    //Turn right
    objectInRange = distanceSensor.Value();
    LCD.WriteLine(objectInRange);
    right_motor.SetPercent(20);
    left_motor.SetPercent(-20);
    Sleep(1.75);
    

    //Go straight
    while (objectInRange == 1){
        objectInRange = distanceSensor.Value();
        LCD.WriteLine(objectInRange);
        right_motor.SetPercent(-25);
        left_motor.SetPercent(-25);
    }

    //End
    LCD.WriteLine(objectInRange);
    right_motor.Stop();
    left_motor.Stop();

    // while(var)
    // {
    //     //Go straight
    //     while (objectInRange > 0.5){
    //         right_motor.SetPercent(30);
    //         left_motor.SetPercent(30);
    //     }

    //     //Turn right
    //     if (objectInRange < 0.5){
    //         if (turn_dir == 0)
    //         {
    //             right_motor.SetPercent(-20);
    //             left_motor.SetPercent(20);
    //             Sleep(1.0);
    //             left_motor.Stop();
    //             right_motor.Stop();
    //             turn_dir = 1;
    //         }
    //         if (turn_dir == 1)
    //         {
    //             right_motor.SetPercent(20);
    //             left_motor.SetPercent(-20);
    //             Sleep(1.0);
    //             left_motor.Stop();
    //             right_motor.Stop();
    //             turn_dir = 0;
    //         }
    //     }
            
    // }
} 