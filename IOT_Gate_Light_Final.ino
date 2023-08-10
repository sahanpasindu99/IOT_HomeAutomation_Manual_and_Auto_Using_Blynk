#define BLYNK_TEMPLATE_ID "TMPL6i7Fuhryp"
#define BLYNK_TEMPLATE_NAME "HomeLighting"
#define BLYNK_AUTH_TOKEN "Ow93Ujbzv63KLES_cGhkBGuhf0pJUITh"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Roman";//Enter your WIFI name
char pass[] = "sahan1234";//Enter your WIFI password

//LED System

const int ledPin = D4;
const int ledPin2 = D3;
const int ledPin3 = D2;

const int ldrPin = D8;

int ldrValue;
int ledvalue;
bool isManualMode = false;
bool isLedOn = false;
bool isLed2On = false;
bool isLed3On = false;
int carheight;

//GATE System

bool isGateOpen=false;
bool isGateManualMode=false;

#define TRIGGER_PIN D1
#define ECHO_PIN D0

#define IN1 D5
#define IN2 D6
#define ENA D7

#define MOTOR_ROTATION_TIME 1000 // 5 seconds

WidgetLCD lcdlight(V14);
WidgetLCD lcd(V7);
BlynkTimer timer;

void setup() {
  Serial.begin(9600);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

    pinMode(ledPin, OUTPUT);
      pinMode(ledPin2, OUTPUT);
        pinMode(ledPin3, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
    Serial.println("Connected to WiFi");
    Blynk.begin(auth, ssid, pass);

    lcd.clear(); //Use it to clear the LCD Widget
    lcd.print(4, 0, "Connected to WIFI");
    delay(1000);

  lcdlight.clear(); //Use it to clear the LCD Widget
  lcdlight.print(4, 0, "Welcome"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  lcdlight.print(2, 1, "to the future");

  lcd.clear(); //Use it to clear the LCD Widget
  lcd.print(4, 0, "Gate"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  lcd.print(4, 1, "Closed");

   timer.setInterval(100L, ldrCheck);
   timer.setInterval(100L, gateCheck);
}

void loop() {
  
  Blynk.run();
  timer.run();

  
}

void rotateMotorForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 150); // Full speed rotation
}

void rotateMotorBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 110); // Full speed rotation
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}
void ldrCheck()
{
  // Read LDR value
  ldrValue = digitalRead(ldrPin);

  // If in automated mode, turn on LED when it's dark
  if (!isManualMode && ldrValue < 1)
  {
    digitalWrite(ledPin,  LOW);
    digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
  }
  else if (!isManualMode && ldrValue >0)
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);
  }

  // If in manual mode, control the LED based on isLedOn variable
  if (isManualMode)
  {
    if (isLedOn)
    {
      digitalWrite(ledPin, HIGH);
      
    }
    else
    {
      digitalWrite(ledPin, LOW);
    }
    if (isLed2On)
    {
      digitalWrite(ledPin2, HIGH);
      
    }
    else
    {
      digitalWrite(ledPin2, LOW);
    }
    if (isLed3On)
    {
      digitalWrite(ledPin3, HIGH);
      
    }
    else
    {
      digitalWrite(ledPin3, LOW);
    }

  }
}
BLYNK_WRITE(V0)
{
  int ledState = param.asInt();
   if (isManualMode)
  {
    if (ledState == 1)
    {
      isLedOn = true;
      digitalWrite(ledPin, HIGH);
    }
    else
    {
      isLedOn = false;
      digitalWrite(ledPin, LOW);
    }
  }
  else
  {
    Blynk.virtualWrite(V0, isLedOn ? 1 : 0); // Update the LED state button in the Blynk app
  }
  
}

BLYNK_WRITE(V3)
{
  int led2State = param.asInt();
   if (isManualMode)
  {
    if (led2State == 1)
    {
      isLed2On = true;
      digitalWrite(ledPin2, HIGH);
    }
    else
    {
      isLed2On = false;
      digitalWrite(ledPin2, LOW);
    }
  }
  else
  {
    Blynk.virtualWrite(V3, isLed2On ? 1 : 0); // Update the LED state button in the Blynk app
  }
  
}

BLYNK_WRITE(V6)
{
  int led3State = param.asInt();
   if (isManualMode)
  {
    if (led3State == 1)
    {
      isLed3On = true;
      digitalWrite(ledPin3, HIGH);
    }
    else
    {
      isLed3On = false;
      digitalWrite(ledPin3, LOW);
    }
  }
  else
  {
    Blynk.virtualWrite(V6, isLed3On ? 1 : 0); // Update the LED state button in the Blynk app
  }
  
}
// Function to handle manual mode switch
BLYNK_WRITE(V4)
{
  int mode = param.asInt();
  if (mode == 1)
  {
    isManualMode = true;
    lcdlight.clear(); //Use it to clear the LCD Widget
    lcdlight.print(3, 0, "Light Mode"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcdlight.print(5, 1, "Manual");

  }
  else
  {
    isManualMode = false;
    lcdlight.clear(); //Use it to clear the LCD Widget
    lcdlight.print(3, 0, "Light Mode"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcdlight.print(5, 1, "Auto");
  }
}
//Gate  Manual Mode

void gateCheck()
{
  stopMotor();
  if (!isGateManualMode)
  {
    long duration, distance;

  // Trigger the ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure the pulse duration
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance
  distance = duration * 0.034/2;
  carheight = 15 - distance;
  Blynk.virtualWrite(V8,carheight);
  Serial.print("Distance in centimeters : ");
  Serial.println(distance);
  delay(100);

  if (distance < 14) {
    rotateMotorForward(); // Rotate forward for 5 seconds
    // open.on();
    // close.off();
    lcd.clear(); //Use it to clear the LCD Widget
    lcd.print(4, 0, "Gate"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcd.print(4, 1, "Opening");
    delay(1500);
    stopMotor(); // Stop for 5 seconds
    lcd.clear(); //Use it to clear the LCD Widget
    lcd.print(4, 0, "Gate"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcd.print(4, 1, "Opened");
    delay(5000); 
    rotateMotorBackward(); 
    lcd.clear(); //Use it to clear the LCD Widget
    lcd.print(4, 0, "Gate"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcd.print(4, 1, "Closing");
    delay(1000);
    stopMotor();
    // open.off();
    // close.on();
    lcd.clear(); //Use it to clear the LCD Widget
    lcd.print(4, 0, "Gate"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcd.print(4, 1, "Closed");
  }
  }
    stopMotor();
  // If in manual mode, control the LED based on isLedOn variable
  if (isGateManualMode)
  {
    stopMotor();
    if (isGateOpen)
    {
      rotateMotorForward();
      lcd.clear(); //Use it to clear the LCD Widget
      lcd.print(4, 0, "Gate"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
      lcd.print(4, 1, "Opened");
    }
    else
    {
      rotateMotorBackward();
      lcd.clear(); //Use it to clear the LCD Widget
      lcd.print(4, 0, "Gate"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
      lcd.print(4, 1, "Closed");
    }
    
  }
}

BLYNK_WRITE(V13)
{
  int openstate = param.asInt();
   if (isGateManualMode)
  {
    if (openstate == 1)
    {
      isGateOpen = true;
     // digitalWrite(ledPin3, HIGH);
    }
    else
    {
      isGateOpen = false;
      //digitalWrite(ledPin3, LOW);
    }
  }
  else
  {
    Blynk.virtualWrite(V13, isGateOpen ? 1 : 0); // Update the LED state button in the Blynk app
  }
  
}

BLYNK_WRITE(V12)
{
  int gatemode = param.asInt();
  if (gatemode == 1)
  {
    isGateManualMode = true;
    
  }
  else
  {
    isGateManualMode = false;
  }
}