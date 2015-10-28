void setup() {
	Serial.begin(9600);
}

void loop() {
	Serial.print("Message to Android");
	delay(5000);
}

int calculate_oximetry() {
	return random(0, 100);
}

int calculate_pulsation() {
	return random(0, 100);
}