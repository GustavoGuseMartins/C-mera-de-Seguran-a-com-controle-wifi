/*
ESP32-CAM Remote Control 
*/

const char* ssid = "***";
const char* password = "**********";

#include "esp_wifi.h"
#include "esp_camera.h"
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

//define qual microcontrolador é o usado
#define CAMERA_MODEL_AI_THINKER

#if defined(CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#else
#error "Camera model not selected"
#endif

void startCameraServer();//definindo funçao pra iniciar o serv

const int ServoPin = 2;//servo da base definido no pino 2
const int ServoPin1 = 14;//servo da cam definido no pino 14
void initServo() 
{
  ledcAttach(2, 50, 16); // 50 hz PWM, 16-bit resolution, range from 3250 to 6500.
  ledcAttach(14, 50, 16); // 50 hz PWM, 16-bit resolution, range from 3250 to 6500.
  ledcWrite(2,3250);
  ledcWrite(14,3250);
}

void setup() 
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //como eu to com problema de fonte ruim desativei esse negocio pra ele nao desligar quando tiver instabilidade na tensao

  Serial.begin(115200);//baud rate

  //configurações da camera
  Serial.setDebugOutput(true); //envia descriçao masi complet dos erros 
  Serial.println();
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //se o esp tem pseudoram ele usa img com mais qualidade, nao muda mt pq o ESPCAM sempre vai ter psram (evita mais erro se falta energia)
  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  //diminui qualidade pra aumentar frame rate ao inicializar
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);

  // Iniciar os servos 
  initServo();
  // Iniciar o FLASH
  ledcAttach(4, 5000, 8);
  
  //LOGAR NA INTERNET
  Serial.println("ssid: " + (String)ssid);
  Serial.println("password: " + (String)password);
  
  WiFi.begin(ssid, password);
  delay(500);//Tentar novamente caso n dê

  long int StartTime=millis();
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(500);
      if ((StartTime+10000) < millis()) break;
  } 
  
  //inicia serv
  startCameraServer();

  //display das infos do server
  if (WiFi.status() == WL_CONNECTED) 
  {
    Serial.println("");
    Serial.println("WiFi connected");    
    Serial.print("Camera conectada! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' para conectar.");
  } else {
    Serial.println("");
    Serial.println("WiFi disconnected");      
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.softAPIP());
    Serial.println("' to connect");
    char* apssid = "ESP32-CAM";
    char* appassword = "12345678";///era pra criar um wifi proprio do esp mas nao ta dando certo
    WiFi.softAP((WiFi.softAPIP().toString()+"_"+(String)apssid).c_str(), appassword);    
  }

  for (int i=0;i<5;i++) 
  {
    ledcWrite(4,10);  // liga e desliga o led 5x ao iniciar
    delay(200);
    ledcWrite(4,0);
    delay(200);    
  }       
}

void loop() {
  delay(1000);
}
