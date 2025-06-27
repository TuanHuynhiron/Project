/*************************************************************

  You can use this sketch as a debug tool that prints all incoming values
  sent by a widget connected to a Virtual Pin 1 in the Blynk App.

  App dashboard setup:
    Slider widget (0...100) on V1
 *************************************************************/
#include <Arduino.h>
/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6nRqpd-ci"
#define BLYNK_TEMPLATE_NAME "pwm"
#define BLYNK_AUTH_TOKEN "_JniAUY6tAyeJ_jkMElHDrB1d1RBac4L"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "QH2008";
char pass[] = "12345678";

BlynkTimer timer;
const int ledPin = 16;  // chọn chân 16 xuất tín hiệu
uint16_t dutyCycle;
int v1;
double volt;
double vcc;
// cài đặt PWM
const int freq = 5000;  // tần số xung
const int ledChannel = 0; // kênh PWM
const int resolution = 8; // độ phân giải 8bit
// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("dutyCycle: ");
  Serial.println(pinValue);
  dutyCycle=(255*pinValue)/100;
  v1=pinValue;
}
BLYNK_WRITE(V2)
{
  // float pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  double d = param.asDouble();
  vcc=d;
}
BLYNK_WRITE(V3)
{
  // float pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  double d = param.asDouble();
  dutyCycle=((d/vcc)*255);

}
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  volt= ((float)(v1)*vcc)/100;
  Blynk.virtualWrite(V0, volt);
  
}
void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
  // setup kênh pwm
  ledcSetup(ledChannel, freq, resolution);
   
  // xuất pwm ra chân 16
  ledcAttachPin(ledPin, ledChannel);
  timer.setInterval(100L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  ledcWrite(ledChannel, dutyCycle);
  timer.run(); // Initiates BlynkTimer
}