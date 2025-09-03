void setup() {
  //Serial line initialization:
  Serial.begin(9600);
  
}

void loop() {

  while (true) { //Main while loop:
    Serial.write("Seeeduino hallo!\n");
    delay(1000);
  }

}
