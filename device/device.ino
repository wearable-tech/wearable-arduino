#include <math.h>

#define ANALOG_RED A0
#define ANALOG_I_RED A1
#define MAX 100

unsigned long red_values[100];
unsigned long i_red_values[100];

// Define various ADC prescaler
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);


void setup() {
    Serial.begin(9600);
    pinMode(ANALOG_RED, INPUT);
    pinMode(ANALOG_I_RED, INPUT);

    // set up the ADC
    ADCSRA &= ~PS_128;  // remove bits set by Arduino library

    ADCSRA |= PS_64;    // set our own prescaler to 64
    // ADCSRA |= PS_32;    // set our own prescaler to 32
    // ADCSRA |= PS_16;    // set our own prescaler to 16
}

void loop() {
//    int oximetry = calculate_oximetry();
//    int pulsation = calculate_pulsation();
//    Serial.println(oximetry * 1.3);
//    Serial.println(pulsation * 1.0);
    calculate_ratio();
    delay(5000);
}

int calculate_oximetry() {
    return random(0, 100);
}

int calculate_pulsation() {
    int sensorValue = analogRead(ANALOG_RED);
    return sensorValue;
}

void read_inputs() {
    for (int i = 0; i < MAX; i++) {
        red_values[i] = analogRead(ANALOG_RED);
        i_red_values[i] = analogRead(ANALOG_I_RED);
    }
}

double calculate_ratio() {
    double red_max = 0, red_min = 1023, i_red_max = 0, i_red_min = 1023;

    read_inputs();

    for (int i = 0; i < MAX; i++) {
        if (red_max < red_values[i]) {
            red_max = red_values[i];
        }
        if (red_min > red_values[i]) {
            red_min = red_values[i];
        }

        if (i_red_max < i_red_values[i]) {
            i_red_max = i_red_values[i];
        }
        if (i_red_min > i_red_values[i]) {
            i_red_min = i_red_values[i];
        }
    }

    for (int i = 0; i < MAX; i++) {
      Serial.print(red_values[i]);
      Serial.print(", ");
    }
    Serial.print("\n");
    Serial.print("red_max:");
    Serial.println(red_max);
    Serial.print("red_min: ");
    Serial.println(red_min);

    for (int i = 0; i < MAX; i++) {
      Serial.print(i_red_values[i]);
      Serial.print(", ");
    }
    Serial.print("\n");
    Serial.print("i_red_max:");
    Serial.println(i_red_max);
    Serial.print("i_red_min: ");
    Serial.println(i_red_min);

    double step = 5 / 1023.0;

    red_max = red_max * step;
    red_min = red_min * step;
    i_red_max = i_red_max * step;
    i_red_min = i_red_min * step;

    double ratio = log(red_min / red_max) / log(i_red_min / i_red_max);

    Serial.print("ratio: ");
    Serial.println(ratio);

    return ratio;
}
