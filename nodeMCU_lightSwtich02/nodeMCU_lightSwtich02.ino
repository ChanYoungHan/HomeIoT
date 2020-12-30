#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#define BLYNK_PRINT Serial

char auth[] = "GnCBNY4yVH2j95PCaNmN1QniKvU1QgfL";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "HanNet";
char pass[] = "chan0812";

Servo switchServo;
int oper_angle = 22; // 집마다 설정 다름
int lightServoSwtich = 0; //중립 0, On : 1, Off : 2;
int lightServoFlag = 1; // 지속적 신호에 대한 Flag
int middleAngle = 90 + 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  switchServo.attach(4); //D2
  switchServo.write(middleAngle); //Prevent reset move 180 degree
  Serial.println("Start!! nodeMCU_lightSwitch");
}

BLYNK_WRITE(V0){
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  if(pinValue == 1){
    Serial.println("Swtich On");
    lightServoSwtich = 1;    
  } if(pinValue == 0) {       
    Serial.println("Swtich Off");
    lightServoSwtich = 2;
  }
}

BLYNK_WRITE(V1){
  int pinValue = param.asInt();
  oper_angle = pinValue;
  Serial.print("Servo Max Angle is Changed : ");
  Serial.println(oper_angle);
}

BLYNK_WRITE(V2){
  int pinValue = param.asInt();
//  Serial.print("Received Signal from Google to turn on : ");
//  Serial.println(pinValue);
  if(pinValue == 1){
    Serial.println("Swtich On");
    lightServoSwtich = 1;    
  } if(pinValue == 0) {       
    Serial.println("Swtich Off");
    lightServoSwtich = 2;
  }
  //Serial.println("Servo Activated!!!!!");
}


void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  
  if (lightServoSwtich == 1 && lightServoFlag == 1){ // light On
    Serial.println("ServoActivate : light On");
    switchServo.write(middleAngle - oper_angle);
    delay(2000);
    lightServoFlag = 2;
    lightServoSwtich = 0;
    switchServo.write(middleAngle);
  } if(lightServoSwtich == 2 && lightServoFlag == 2){  // light Off
    Serial.println("ServoActivate : light Off");
    switchServo.write(middleAngle + oper_angle);
    delay(2000);
    lightServoFlag = 1;
    lightServoSwtich = 0;
    switchServo.write(middleAngle);
  }
}
