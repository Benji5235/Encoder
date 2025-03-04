#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRCS.h>

#define pi 3.14159

FEHMotor left_motor(FEHMotor::Motor2, 9.0);
FEHMotor right_motor(FEHMotor::Motor1, 9.0);
DigitalInputPin distanceSensor(FEHIO::P3_0);
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P1_0);


void move_forward(int percent, int counts){

}

int main(void) 
{ 
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //One inch (theoretically)
    int one_inch = (318/(2*pi*1.25));
    while ((right_encoder.Counts() + left_encoder.Counts())/2<one_inch*24){
        left_motor.SetPercent(30);
        right_motor.SetPercent(30);
    }

    //Stops the motor for 3 seconds
    left_motor.Stop();
    right_motor.Stop();
    Sleep(3.0);

    //Move back until 5cm away
    while (distanceSensor.Value()!=1){
        left_motor.SetPercent(-15);
        right_motor.SetPercent(-15);
    }

    right_motor.Stop();
    left_motor.Stop();
} 