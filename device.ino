setup() {
	Serial.begin(9600);
}

run() {
	Serial.print("Message to Android");
	delay(5000);
}