#include <math.h>

#define ANALOG_RED A0
#define ANALOG_I_RED A1
#define MAX 200

unsigned long red_values[100];
unsigned long i_red_values[100];

void setup() {
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    delay(2000);
}

void loop() {
    /* int oximetry = calculate_oximetry();
    int pulsation = calculate_pulsation();
    Serial.println(oximetry * 1.0);
    Serial.println(pulsation * 1.0);
    // calculate_ratio();
    delay(5000);
    */
    int buffer_red[MAX];
    int buffer_i_red[MAX];
    
    // ligar vermelhor
    digitalWrite(13, HIGH);
    for(int i = 0; i < MAX; i++)
    {
       buffer_red[i] = analogRead(ANALOG_RED);
    }
    digitalWrite(13, LOW);
    
    delay(2000);
    digitalWrite(13, HIGH);
    
    for(int i = 0; i < 200; i++)
    {
       buffer_i_red[i] = analogRead(ANALOG_I_RED);
    }

    digitalWrite(13, LOW);
    
    for(int i = 0; i < 200; i++)
    {
      Serial.println(buffer_red[i]);
      Serial.print("\n");
    }
    for(int i = 0; i < 200; i++)
    {
      Serial.println(buffer_i_red[i]);
      Serial.print("\n");
    }
    delay(2000);
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
