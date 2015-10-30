const int analogInPin = A0;

// Define various ADC prescaler
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);


void setup() {
    Serial.begin(9600);
    pinMode(A0, INPUT);

    // set up the ADC
    ADCSRA &= ~PS_128;  // remove bits set by Arduino library

    ADCSRA |= PS_64;    // set our own prescaler to 64
    // ADCSRA |= PS_32;    // set our own prescaler to 32
    // ADCSRA |= PS_16;    // set our own prescaler to 16
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
