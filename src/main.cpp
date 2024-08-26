#include <Arduino.h>

// ����GPIO����
const int IR_PIN = 14; // ������������ӵ�GPIO����
volatile unsigned long pulseWidths[100]; // �洢���������
volatile int pulseIndex = 0;
volatile bool recording = false;
volatile unsigned long lastTime = 0;

void IRAM_ATTR handleInterrupt() {
  // GPIO�жϷ������̣���¼����
  if (recording && pulseIndex < 100) {
    unsigned long currentTime = micros();
    pulseWidths[pulseIndex++] = currentTime - lastTime; // ��������
    lastTime = currentTime; // �����ϴμ�¼��ʱ��
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);

  // ����GPIO�ж�
  attachInterrupt(digitalPinToInterrupt(IR_PIN), handleInterrupt, CHANGE);
}

void loop() {
  // ����Ƿ��а�����ʼ¼��
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'r') {
      // ��ʼ¼��
      Serial.println("Start recording...");
      pulseIndex = 0;
      recording = true;
      lastTime = micros(); // ��ʼ��ʱ���
      delay(1000); // ¼��1��
      recording = false;
      Serial.println("Recording stopped.");
      
      // ��ӡ¼�Ƶ���������
      for (int i = 0; i < pulseIndex; i++) {
        Serial.println(pulseWidths[i]);
      }
    }
  }
}
