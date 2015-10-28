void setup() {
	Serial.begin(9600);
}

void loop() {
        Serial.print(6);
	Serial.print("123.45");
	Serial.print("678");
	delay(5000);
}

int calculate_oximetry() {
	return random(0, 100);
}

int calculate_pulsation() {
	return random(0, 100);
}
