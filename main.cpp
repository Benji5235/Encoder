#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRCS.h>

int main(void)
{
 RCS.InitializeTouchMenu("0800A9VHR"); // Run Menu to select Region (e.g., A, B, C, D)
 int lever = RCS.GetLever(); // Get a 0, 1, or 2 indicating which lever to pull
 LCD.WriteLine( RCS.Time() ); // Get the match time remaining
//  Sleep(2.0);
//  LCD.WriteLine(lever);
}