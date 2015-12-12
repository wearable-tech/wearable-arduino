#include <math.h>

#define MAX 250
#define PEAK 400
#define INFRARED 7
#define RED 8

int i = 0;
int time_readed = 0;
int maximums[] = {0, 0, 0, 0};
int minimums[] = {0, 0, 0, 0};
int max1_time = 0, max2_time = 0, max3_time = 0, max4_time = 0;
bool going_up = false;

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(INFRARED, OUTPUT);
  pinMode(RED, OUTPUT);
//  Serial.println("CLEARDATA");
//  Serial.println("LABEL,Time,valor_vermelho,tempo_vermelho,valor_infra, tempo_infra");
}

int define_maximums(int previous, int value_readed) {
  if (value_readed > PEAK) {
    if ((previous > value_readed) && going_up) {
      if (maximums[0] == 0) {
        max1_time = time_readed;
        maximums[0] = previous;
      }
      else if (maximums[1] == 0) {
        maximums[1] = previous;
        max2_time = time_readed;
      }
      else if (maximums[2] == 0) {
        maximums[2] = previous;
        max3_time = time_readed;
      }
      else if (maximums[3] == 0) {
        maximums[3] = previous;
        max4_time = time_readed;
      }
      
      return 1;
    }
  }
  
  return 0;
}

int define_minimums(int previous, int value_readed, int last_point) {
  if ((previous < value_readed) && !going_up) {
    for (int i = 0; i < 4; i++) {
      if (last_point == -1 && minimums[i] == 0) {
        if (previous < minimums[i-1])
          minimums[i-1] = previous;
          break;
      }
      else if (minimums[i] == 0) {
        minimums[i] = previous;
        break;
      }
    }
    
    return 1;
  }
  
  return 0;
}

double calculate_frequency() {
//  //Serial.print("DATA,TIME,"); Serial.print("max1"); Serial.print(","); Serial.print(max1_time); Serial.print("\n");
//  //Serial.print("DATA,TIME,"); Serial.print("max2"); Serial.print(","); Serial.print(max2_time); Serial.print("\n");
//  //Serial.print("DATA,TIME,"); Serial.print("max3"); Serial.print(","); Serial.print(max3_time); Serial.print("\n");
//  //Serial.print("DATA,TIME,"); Serial.print("max4"); Serial.print(","); Serial.print(max4_time); Serial.print("\n");
  double frequency1 = 1 / ((max2_time - max1_time) / 1000.0);
  double frequency2 = 1 / ((max3_time - max2_time) / 1000.0);
  double frequency3 = 1 / ((max4_time - max3_time) / 1000.0);
  
  int count = 0;
  double sum = 0;
  
  if(frequency1 < 3.3333 && frequency1 > 0.5) {
    count++;
    sum += frequency1;
  }
  if(frequency2 < 3.3333 && frequency2 > 0.5) {
    count++;
    sum += frequency2;
  }
  if(frequency3 < 3.3333 && frequency3 > 0.5) {
    count++;
    sum += frequency3;
  }

  double frequency = 0;
  if (count)
    frequency = sum / count;
  
//  //Serial.print("DATA,TIME,"); Serial.print("frequencia1"); Serial.print(","); Serial.print(frequency1); Serial.print("\n");
//  //Serial.print("DATA,TIME,"); Serial.print("frequencia2"); Serial.print(","); Serial.print(frequency2); Serial.print("\n");
//  //Serial.print("DATA,TIME,"); Serial.print("frequencia3"); Serial.print(","); Serial.print(frequency3); Serial.print("\n");
//  //Serial.print("DATA,TIME,"); Serial.print("frequencyuencia"); Serial.print(","); Serial.print(frequency); Serial.print("\n");
//  //Serial.print("DATA,TIME,"); Serial.print("BPM"); Serial.print(","); Serial.print(frequency*60); Serial.print("\n");

  return frequency;
}

void define_points(int led) {
  for (int i = 0; i < 4; i++) {
    maximums[i] = 0;
    minimums[i] = 0;
  }
  
  int previous = 0;
  int last_point = 0; //if last_point = 1, last_point is maximum, elseif last_point = -1, last_point = minimum
  digitalWrite(led, HIGH);
  delay(2500);
  for(int i = 0; i < MAX; i++) {
    time_readed = millis();
  
    int value_readed = analogRead(A0);
    
    if (i >= 50) {
      if (define_maximums(previous, value_readed)) last_point = 1;
      else if (define_minimums(previous, value_readed, last_point)) last_point = -1;
      
      if (previous < value_readed) {
        going_up = true;
      }
      else if (previous > value_readed) {
        going_up = false;
      }
  
      previous = value_readed;
      
      if(led == RED) {
//        //Serial.print("DATA,TIME,"); Serial.print(value_readed); Serial.print(","); Serial.print(time_readed); //Serial.print(",,"); Serial.print("\n"); 
      }
      else {
//        //Serial.print("DATA,TIME,"); Serial.print(",,"); Serial.print(value_readed); Serial.print(","); Serial.//print(time_readed); Serial.print("\n"); 
      }

      delay(22);
    }
  }
  digitalWrite(led, LOW);
}

double calculate_oximetry(double average_red_valleys, double average_red_peaks, double average_infrared_valleys, double average_infrared_peaks) {
  double R = log(average_red_valleys / average_red_peaks) / log(average_infrared_valleys / average_infrared_peaks);
  double oximetry = 98.304 - 1.53445 * R;

//  //Serial.print("DATA,TIME,"); Serial.print(",,"); Serial.print("R"); Serial.print(","); Serial.print(R); Serial.print("\n");
//  //Serial.print("DATA,TIME,"); Serial.print(",,"); Serial.print("Oxigenacao"); Serial.print(","); Serial.print(//oximetry); Serial.print("\n");

  return oximetry;
}

void loop() {
   define_points(RED);
   
   double frequency = calculate_frequency();
   
   double average_red_peaks = (maximums[0] + maximums[1] + maximums[2] + maximums[3]) / 4.0;
   double average_red_valleys = (minimums[0] + minimums[1] + minimums[2] + minimums[3]) / 4.0;
//   //Serial.print("DATA,TIME,"); Serial.print("media_average_red_peaks"); Serial.print(","); Serial.print(//average_red_peaks); Serial.print("\n");
//   //Serial.print("DATA,TIME,"); Serial.print("media_average_red_valleys"); Serial.print(","); Serial.print(//average_red_valleys); Serial.print("\n");
   
   define_points(INFRARED);
   double average_infrared_peaks = (maximums[0] + maximums[1] + maximums[2] + maximums[3]) / 4.0;
   double average_infrared_valleys = (minimums[0] + minimums[1] + minimums[2] + minimums[3]) / 4.0;
//   //Serial.print("DATA,TIME,"); Serial.print(",,"); Serial.print("media_average_infrared_peaks"); Serial.//print(","); Serial.print(average_infrared_peaks); Serial.print("\n");
//   //Serial.print("DATA,TIME,"); Serial.print(",,"); Serial.print("media_average_infrared_valleys"); Serial.//print(","); Serial.print(average_infrared_valleys); Serial.print("\n");

   double oximetry = calculate_oximetry(average_red_valleys, average_red_peaks, average_infrared_valleys, average_infrared_peaks);
   
   Serial.println(oximetry);
   Serial.println(frequency);

   delay(300000);
}
