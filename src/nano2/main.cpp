#include <Arduino.h> 
#include <SoftwareSerial.h>
 
// ピン定義 (A6, A7が使えないボードを想定し、デジタルピンへの変更を推奨) 
const int trigPins[] = {9, 10, 11, 12}; 
const int echoPins[] = {A4, A5, 8,7 }; // A6, A7はデジタル入力不可のため変更例 
SoftwareSerial mySerial(2, 3); // RX, TX
 
#define TEMP 21.0 
double sound_speed = 331.5 + 0.61 * TEMP; // m/sec 
 
// 距離を測定する共通関数 
long getDistance(int trigPin, int echoPin) { 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
 
  // タイムアウトを20ms(約3m分)に設定 
  long duration = pulseIn(echoPin, HIGH, 20000);  
   
  if (duration == 0) return 99; // 測定不能時は遠い値として返す 
 
  // 距離(cm)計算: (時間 / 2) * 音速(m/s) / 10000 
  int return_cm = (duration / 2.0) * sound_speed / 10000.0;
  if(return_cm > 99) return 99;
  return return_cm;
} 
 
void setup() { 
  Serial.begin(9600); 
  mySerial.begin(9600);
  for (int i = 0; i < 4; i++) { 
    pinMode(trigPins[i], OUTPUT); 
    pinMode(echoPins[i], INPUT); 
  } 
} 
 
void loop() { 
  long distances[4]; 
 
  for (int i = 0; i < 4; i++) { 
    distances[i] = getDistance(trigPins[i], echoPins[i]); 
    delay(20); // センサー間の干渉防止のための短い待ち時間 
  } 
 
  // 結果出力 (CSV形式) 
    Serial.print(distances[0]); Serial.print(","); 
    Serial.print(distances[1]); Serial.print(","); 
    Serial.print(distances[2]); Serial.print(","); 
    Serial.println(distances[3]); 

    mySerial.print(distances[0]); mySerial.print(","); 
    mySerial.print(distances[1]); mySerial.print(",");
    mySerial.print(distances[2]); mySerial.print(",");
    mySerial.println(distances[3]);

    delay(20); 
}