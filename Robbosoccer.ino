#include <WiFi.h>
#include <WiFiUdp.h>

// ========== WIFI SETTINGS ==========
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";

WiFiUDP udp;
const int UDP_PORT = 4210;

char packetBuf[255];
String cmd = "";
int Speed = 150;

// =====================================
// MOTOR PINS
// =====================================

// RIGHT MOTOR (Motor A)
int Rmotor_IN1 = 19;
int Rmotor_IN2 = 21;
int Rmotor_EN  = 18;

// LEFT MOTOR (Motor B)
int Lmotor_IN1 = 32;
int Lmotor_IN2 = 33;
int Lmotor_EN  = 25;

// PWM Channels
#define CH_Rmotor 0
#define CH_Lmotor 1


// =====================================
// SETUP
// =====================================
void setup() {
  Serial.begin(115200);

  // Required for ESP32 Arduino Core 3.x
  WiFi.useStaticBuffers(false);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start UDP listener BEFORE processing packets
  if (!udp.begin(UDP_PORT)) {
    Serial.println("Failed to start UDP!");
  }

  // Motor direction pins
  pinMode(Rmotor_IN1, OUTPUT);
  pinMode(Rmotor_IN2, OUTPUT);
  pinMode(Lmotor_IN1, OUTPUT);
  pinMode(Lmotor_IN2, OUTPUT);

  // PWM pins
  ledcAttach(Rmotor_EN, 5000, 8);
  ledcAttach(Lmotor_EN, 5000, 8);

  stopMotors();
}


// =====================================
// MOTOR CONTROL FUNCTIONS
// =====================================
void setMotor(int IN1, int IN2, int CH_EN, bool forward, int pwm) {
  digitalWrite(IN1, forward ? HIGH : LOW);
  digitalWrite(IN2, forward ? LOW  : HIGH);
  ledcWrite(CH_EN, pwm);
}

void stopMotor(int IN1, int IN2, int CH_EN) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  ledcWrite(CH_EN, 0);
}

void stopMotors() {
  stopMotor(Rmotor_IN1, Rmotor_IN2, CH_Rmotor);
  stopMotor(Lmotor_IN1, Lmotor_IN2, CH_Lmotor);
}


// =====================================
// MOVEMENTS
// =====================================
void forward() {
  setMotor(Rmotor_IN1, Rmotor_IN2, CH_Rmotor, true, Speed);
  setMotor(Lmotor_IN1, Lmotor_IN2, CH_Lmotor, true, Speed);
}

void backward() {
  setMotor(Rmotor_IN1, Rmotor_IN2, CH_Rmotor, false, Speed);
  setMotor(Lmotor_IN1, Lmotor_IN2, CH_Lmotor, false, Speed);
}

void turnRight() {
  setMotor(Rmotor_IN1, Rmotor_IN2, CH_Rmotor, true, Speed);   // Right forward
  setMotor(Lmotor_IN1, Lmotor_IN2, CH_Lmotor, false, Speed);  // Left backward
}

void turnLeft() {
  setMotor(Lmotor_IN1, Lmotor_IN2, CH_Lmotor, true, Speed);   // Left forward
  setMotor(Rmotor_IN1, Rmotor_IN2, CH_Rmotor, false, Speed);  // Right backward
}


// =====================================
// UDP CONTROL LOOP (Updated for 3.1)
// =====================================
void loop() {
  int size = udp.parsePacket();
  if (size > 0) {

    int len = udp.read(packetBuf, sizeof(packetBuf) - 1);
    if (len > 0) packetBuf[len] = '\0';

    cmd = String(packetBuf);
    cmd.trim();

    if (cmd == "F") forward();
    else if (cmd == "B") backward();
    else if (cmd == "L") turnLeft();
    else if (cmd == "R") turnRight();
    else if (cmd == "S") stopMotors();
  }
}