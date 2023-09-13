#include <Wire.h>

int airquality= 0;
int doduc=0;
int relay2 = 8;
int relay3 = 6;

char c,b;
void setup() {
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */
  Serial.begin(9600); 
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, LOW);

  pinMode(relay3, OUTPUT);
  digitalWrite(relay3, LOW);
}

void loop() {
  doduc = analogRead(A0);
  Serial.println(doduc);
  airquality = analogRead(A1);
  Serial.println(airquality);
  delay(1000);
  Serial.print("c= ");
  Serial.print(c);
  Serial.print("  b= ");
  Serial.println(b);
  if (b == '1') {
      digitalWrite(relay2, HIGH);
    }
  else if (b == '0') {
      digitalWrite(relay2, LOW);
    } 
  if (c == '1') {
      digitalWrite(relay3, HIGH);
    }
  else if (c == '0') {
      digitalWrite(relay3, LOW);
    } 
}
// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  while (0 < Wire.available()) {
    c = Wire.read();      /* receive byte as a character */
    b=Wire.read();
//    Serial.print(c);           /* print the character */
  }
  Serial.println();             /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent() {
  String result = "";
  result = result + String(doduc) + "a" + String(airquality);
  const char* request = result.c_str();
  Wire.write(request);  /*send string on request */
}
