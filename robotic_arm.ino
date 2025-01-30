#include <Servo.h>

Servo servo1, servo2, servo3, servo4, servo5, servo6;

int buttonPin = 7; // Button connected to pin 7
int greenLED = 12; // Green LED for "Ready"
int yellowLED = 13; // Yellow LED for "In Motion"
int redLED = 8;     // Red LED for "Error"

void setup() {
  // Attach servos to their pins
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);
  servo5.attach(10);
  servo6.attach(11);

  // Initialize Serial communication
  Serial.begin(9600);
  Serial.println("Robotic Arm Ready! Send commands like servo1:90 or grip:45");

  // Set up button and LEDs
pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor
pinMode(greenLED, OUTPUT);
pinMode(yellowLED, OUTPUT);
pinMode(redLED, OUTPUT);


  // Indicate system is ready
  digitalWrite(greenLED, HIGH);
}

void loop() {
  // Check for button press
  if (digitalRead(buttonPin) == LOW) { // Button pressed, LOW because it's connected to GND
    digitalWrite(greenLED, LOW); // Turn off green LED (Ready)
    digitalWrite(yellowLED, HIGH); // Indicate "In Motion"
    performAction(); // Execute predefined action
    digitalWrite(yellowLED, LOW); // Turn off yellow LED
    digitalWrite(greenLED, HIGH); // Back to "Ready"
    delay(1000); // Debounce delay to prevent multiple readings from one press
  }

  // Check for Serial input
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Read input until newline
    handleCommand(input); // Process the command
  }
}

void handleCommand(String command) {
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, HIGH); // Indicate "In Motion"

  if (command.startsWith("grip:")) {
    int gripAngle = command.substring(5).toInt(); // Extract gripper angle
    servo6.write(gripAngle); // Control the gripper
    Serial.println("Gripper adjusted to angle: " + String(gripAngle));
  } else if (command.startsWith("servo")) {
    int servoNum = command.substring(5, 6).toInt(); // Extract servo number
    int angle = command.substring(7).toInt();       // Extract angle
    moveServo(servoNum, angle);
  } else {
    digitalWrite(redLED, HIGH); // Indicate "Error"
    Serial.println("Invalid Command!");
    delay(1000); // Hold the error LED for 1 second
    digitalWrite(redLED, LOW);
  }

  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, HIGH); // Back to "Ready"
}

void moveServo(int servoNum, int angle) {
  switch (servoNum) {
    case 1: servo1.write(angle); break;
    case 2: servo2.write(angle); break;
    case 3: servo3.write(angle); break;
    case 4: servo4.write(angle); break;
    case 5: servo5.write(angle); break;
    case 6: servo6.write(angle); break;
    default: Serial.println("Invalid Servo Number!");
  }
  Serial.println("Servo" + String(servoNum) + " moved to angle: " + String(angle));
}

void performAction() {
  // Example pick-and-place sequence
  servo1.write(90); // Rotate base
  servo2.write(45); // Lower shoulder
  servo3.write(90); // Bend elbow
  servo6.write(90); // Open gripper
  delay(1000);

  servo2.write(0); // Lift shoulder
  delay(1000);

  servo1.write(0); // Reset base position
  servo6.write(0); // Close gripper
}


