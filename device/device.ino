const int analogInPin = A0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    int oximetry = calculate_oximetry();
    int pulsation = calculate_pulsation();
    Serial.println(oximetry * 1.3);
    Serial.println(pulsation * 1.0);
    delay(5000);
}

int calculate_oximetry() {
    return random(0, 100);
}

int calculate_pulsation() {
    int sensorValue = analogRead(analogInPin);
    return sensorValue;
}
