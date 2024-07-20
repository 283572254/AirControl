#include <Arduino.h>

// 定义GPIO引脚
const int IR_PIN = 14; // 红外接收器连接的GPIO引脚
volatile unsigned long pulseWidths[100]; // 存储脉宽的数组
volatile int pulseIndex = 0;
volatile bool recording = false;
volatile unsigned long lastTime = 0;

void IRAM_ATTR handleInterrupt() {
  // GPIO中断服务例程，记录脉宽
  if (recording && pulseIndex < 100) {
    unsigned long currentTime = micros();
    pulseWidths[pulseIndex++] = currentTime - lastTime; // 计算脉宽
    lastTime = currentTime; // 更新上次记录的时间
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);

  // 设置GPIO中断
  attachInterrupt(digitalPinToInterrupt(IR_PIN), handleInterrupt, CHANGE);
}

void loop() {
  // 检查是否有按键开始录制
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'r') {
      // 开始录制
      Serial.println("Start recording...");
      pulseIndex = 0;
      recording = true;
      lastTime = micros(); // 初始化时间戳
      delay(1000); // 录制1秒
      recording = false;
      Serial.println("Recording stopped.");
      
      // 打印录制的脉宽数据
      for (int i = 0; i < pulseIndex; i++) {
        Serial.println(pulseWidths[i]);
      }
    }
  }
}
