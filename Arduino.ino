    //includes

#include<AccelStepper.h>
#include<Servo.h>
#include<math.h>

         //motor declarations

const int tiltdir = 8; // set tilt direction control to digital pin 8
const int tiltpulse = 9; // set tilt pulse control to digitial pin 9

const int pandir = 6; // set pan direction control to digital pin 6
const int panpulse = 7; // set pan pulse control to digital pin 7

const int joyH = 3;        //set joystick 1 axis 1 to analog pin 3
const int joyV = 4;        //set joystick 1 axis 2 to analog pin 4
const int joy2H = 1;       //set joystick 2 axis 1 to analog pin 1
const int joy2V = 2;       //set joystick 2 axis 2 to analog pin 2

AccelStepper pan = AccelStepper(1, panpulse, pandir); // initialize pan stepper on digital pins 8-11
AccelStepper tilt = AccelStepper(1, tiltpulse, tiltdir); // initialize pan stepper on digital pins 8-11

Servo LR;                  //initialize left/right servo
Servo UD;                  //initializer up/down servo

int X;
int Y;
float Xdist; long int Xdist2;
float Ydist; long int Ydist2;
float h; int dirX; int dirY;
int posh;                             //position variable 1 for joystick 1
int posv;                             //position variable 2 for joystick 1
int eyeh;                             //position variable 1 for joystick 2
int eyev;                             //position variable 2 for joystick 2
int ipan;                             //pan iteration to keep track of pan position
int itilt;                            //tilt iteration to keep track of tilt position
int eyepan;                           //pan iteration for eye servo
int eyetilt;                          //tilt iteration for eye servo
int panpos = 0;                      //initialize pan position to be 50 (out of 100)
int tiltpos = 0;                     //initialize tilt position to be 30 (out of 60)
int eyepos = 30;
float eyeposrad;
int eyetiltpos = 30;
float eyetiltposrad;
String coordinates;

         //variable declarations


void setup() {
       //set motor speeds

 pan.setMaxSpeed(1000); // Set the motor movement values
 pan.setAcceleration(1000);
 tilt.setMaxSpeed(1000); // Set the motor movement values
 tilt.setAcceleration(500);

  //attach servo pins

 LR.attach(12);             //attach left/right servo to pin 12
 UD.attach(13);             //attach up/down servo to pin 13

  //open serial port

Serial.begin(9600);           //start serial terminal

}

void loop() {

 ipan = 0;                     //reset neck movement indicators
 itilt = 0;

 if (pan.isRunning() != true && tilt.isRunning() != true) {

  posh = analogRead(joyH);          //read joystick position
  posv = analogRead(joyV);
  eyeh = analogRead(joy2H);        //read joystick position
  eyev = analogRead(joy2V);

Serial.print(posh);
Serial.print("  ");
Serial.print(posv);
Serial.print("     ");
Serial.print(eyeh);
Serial.print(" ");
Serial.print(eyev);

Serial.print(" ");
Serial.println(panpos);

 if ((400 > posh > 600) || (400 > posv > 600) || (600 < eyeh < 400) || (400 > eyev > 600)) {
 

  if (posh < 400 ){                //determine direction of joystick
    panpos = panpos - 3;                     //set pan direction based on joystick
   }
   
   else if (posh > 600 ){
     panpos = panpos + 3;
   }

  if (posv < 400 ){              //determine direction of joystick
    tiltpos = tiltpos - 3;                  //set tilt direction based on joystick
  }
  else if (posv > 600 ){
    tiltpos = tiltpos + 3;
  }  

  if (panpos < -60){               // account for lower neck pan position limit
     ipan = 0;
     panpos = -60;
   }
   else if (panpos > 60){        // account for upper neck pan position limit
     ipan = 0;
     panpos = 60;
   }  

   if (tiltpos < -30){             // account for lower neck tilt position limits
     itilt = 0;
     tiltpos = -30;
   }
   else if (tiltpos > 30){      // account for upper neck tilt position limits
     tiltpos = 30;
   }

    pan.moveTo(panpos);                          //set neck pan target position
    tilt.moveTo(tiltpos);                        //set neck tilt target position

   
  if (eyeh < 400 ){                //determine direction of joystick
    eyepos = eyepos - 3;
  }
   else if (eyeh > 600 ){
     eyepos = eyepos + 3;
   }    
   
  if (eyev < 400 ){                //determine direction of joystick
    eyetiltpos = eyetiltpos - 3;
  }
  else if (eyev > 600 ){
    eyetiltpos = eyetiltpos + 3;
  }
 
   if (eyepos > 60) {          //determine if at eye pan upper limit
     eyepos = 60;                   //account for eye pan upper limit
   }
   if (eyepos < 0) {
     eyepos = 0;                   //account for eye pan lower limit
   }
   if (eyetiltpos < 0) {
     eyetiltpos = 0;         //account for eye tilt lower limit
   }
   if (eyetiltpos > 60) {
     eyetiltpos = 60;         //account for eye tilt lower limit
   }


 }
 
  else  if (Serial.available()>0){

   while (Serial.available()){                                //clear serial buffer of data
    String coordinates = Serial.readStringUntil('\n');          //read serial port for position data
   // Serial.print("Data received: ");
   
                                   //calculate eye movement necessary

   String xval = getValue(coordinates, ':', 0);
   String yval = getValue(coordinates, ':', 1);
   
   
   X = xval.toInt();
   Y = yval.toInt();
   dirX = 1;
   dirY = 1;
   }

   Xdist = -1*(320 - X);
   if (Xdist < 0){
    Xdist = Xdist * -1;
    dirX = -1;
    }
   Ydist = -1*(240 - Y);
   if (Ydist < 0 ){
    Ydist = Ydist * -1;
    dirY = -1;
    }

   h = sqrt(sq(Xdist) +sq(Ydist));
   dirY = dirY * -1;

   eyeposrad = acos((Xdist/h));
   eyepan = dirX * eyeposrad * (180/PI);
   eyepos = eyepos + eyepan;
   eyetiltposrad = acos(Ydist/h);
   eyetilt = dirY * eyetiltposrad * (180/PI);
   eyetiltpos = eyetiltpos + eyetilt;

   //Serial.print("  X: ");
   //Serial.print(Xdist*dirX);
   //Serial.print("  Y: ");
   //Serial.print(Ydist*dirY);
   //Serial.print("  h: ");
   //Serial.print(h);
   //Serial.print(" acos(x/h): ");
   //Serial.print(eyepos);
   //Serial.println();
 }



 

 
 
                                   
 pan.run();                       //move neck pan motor
 tilt.run();                      //move neck tilt motor
 LR.write(eyepos);                //move eye pan servo
 UD.write(eyetiltpos);            //move eye tilt servo

 



  //sensor detects distance when motors not moving and image centered

}
//}
}

String getValue(String data, char separator, int index)       //
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
