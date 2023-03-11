#include <Servo.h>

Servo baseArm;
Servo lowerArm;
Servo upperArm;
Servo clawArm;

// This is used for detecting if key is pressed. Send by computer via serial port.
String input;

int baseAngle = 90;
int lowerAngle = 140;
int upperAngle = 90;
int clawAngle = 90;

// Default turn boost speed overides turn speedn when shift is pressed in python script (sent "b" via serial to show it)
volatile int turnSpeed = 1;
const int turnBoostConstant = 2;
volatile bool runAutomation = false;
volatile int automationArray[80];
volatile int automationStep = 0;
volatile int nextAutomationSaveStep = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  baseArm.attach(3);
  lowerArm.attach(5);
  upperArm.attach(6);
  clawArm.attach(9);

  baseArm.write(baseAngle);
  lowerArm.write(lowerAngle);
  upperArm.write(upperAngle);
  clawArm.write(clawAngle);

  Serial.begin(9600);
  Serial.setTimeout(10);

  ResetAutomationMemory();
}

void loop() {
  // Wait until there is serial port connection
  while (!Serial.available()) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  digitalWrite(LED_BUILTIN, LOW);

  // Increase or decrease arm angles by checking input that is been sent by serial port
  SetAngleFromKeyboard();

  if (runAutomation == true) {
    StartAutomation();
  }

  // Write this angles to servo arms.
  SetAngleToServo();
}

void SetAngleFromKeyboard() {
  input = Serial.readStringUntil('\n');

  // Start automation
  if (input.indexOf("p") != -1) {
    StopAutomation();
  } else if (input.indexOf("o") != -1) {
    runAutomation = true;
  } else {
    runAutomation = false;
  }

  // Save automation postion
  if (input.indexOf("k") != -1) {
    SavePositionToMemory();
    runAutomation = false;
  }
  if (input.indexOf("l") != -1) {
    ResetAutomationMemory();
    runAutomation = false;
  }

  // Stop automation

  //set turn speed (boosted if shift is pressed)
  if (input.indexOf("b") != -1) {
    turnSpeed = turnBoostConstant;
  } else {
    turnSpeed = 1;
  }

  //check "base arm"
  if (input.indexOf("a") != -1) {
    baseAngle += turnSpeed;
    runAutomation = false;
  } else if (input.indexOf("d") != -1) {
    baseAngle -= turnSpeed;
    runAutomation = false;
  }

  //check "lower arm"
  if (input.indexOf("s") != -1) {
    lowerAngle += turnSpeed;
    runAutomation = false;
  } else if (input.indexOf("w") != -1) {
    lowerAngle -= turnSpeed;
    runAutomation = false;
  }

  //check "upper arm"
  if (input.indexOf("8") != -1) {
    upperAngle += turnSpeed;
    runAutomation = false;
  } else if (input.indexOf("2") != -1) {
    upperAngle -= turnSpeed;
    runAutomation = false;
  }

  //check "claw arm"
  if (input.indexOf("6") != -1) {
    clawAngle += turnSpeed;
    runAutomation = false;
  } else if (input.indexOf("4") != -1) {
    clawAngle -= turnSpeed;
    runAutomation = false;
  }


  // Serial.println( testAngle );
}

void SetAngleToServo() {
  // These make sure angle integer stays between 0 and 180
  if (baseAngle > 180) {
    baseAngle = 180;
  } else if (baseAngle < 0) {
    baseAngle = 0;
  }

  if (lowerAnglessw> 153) {
    lowerAngle = 153;
  } else if (lowerAngle < 0) {
    lowerAngle = 0;l
  }

  if (upperAngle > 180) {
    upperAngle = 180;
  } else if (upperAngle < 0) {
    upperAngle = 0;
  }

  if (clawAngle > 180) {
    clawAngle = 180;
  } else if (clawAngle < 0) {
    clawAngle = 0;
  }

  /*
  Serial.println(baseAngle);
  Serial.println(lowerAngle);
  Serial.println(upperAngle);
  */

  // Write angles to servos
  baseArm.write(baseAngle);
  lowerArm.write(lowerAngle);
  upperArm.write(upperAngle);
  clawArm.write(clawAngle);
}

void ResetAutomationMemory() {
  for (int i = 0; i <= 80; i++) {
    automationArray[i] = -1;
  }
  nextAutomationSaveStep = 0;
}

void SavePositionToMemory() {
  automationArray[nextAutomationSaveStep] = baseAngle;
  automationArray[nextAutomationSaveStep+1] = lowerAngle;
  automationArray[nextAutomationSaveStep+2] = upperAngle;
  automationArray[nextAutomationSaveStep+3] = clawAngle;
  nextAutomationSaveStep = nextAutomationSaveStep + 4;
}

void StartAutomation() {
  

  if (automationArray[0] == -1) {
    return;
  }

  int checkmark = 0;
  if (baseAngle > automationArray[automationStep]) {
    baseAngle--;
  } else if (baseAngle < automationArray[automationStep]) {
    baseAngle++;
  } else {
    checkmark++;
  }

  if (lowerAngle > automationArray[automationStep + 1]) {
    lowerAngle--;
  } else if (lowerAngle < automationArray[automationStep + 1]) {
    lowerAngle++;
  } else {
    checkmark++;
  }

  if (upperAngle > automationArray[automationStep + 2]) {
    upperAngle--;
  } else if (upperAngle < automationArray[automationStep + 2]) {
    upperAngle++;
  } else {
    checkmark++;
  }

  if (clawAngle > automationArray[automationStep + 3]) {
    clawAngle--;
  } else if (clawAngle < automationArray[automationStep + 3]) {
    clawAngle++;
  } else {
    checkmark++;
  }

  if (checkmark == 4) {
    delay(100);
    automationStep = automationStep + 4;
    if (automationArray[automationStep] == -1) {
      automationStep = 0;
    }
  }
}

void StopAutomation() {
  runAutomation = false;
  automationStep = 0;
}