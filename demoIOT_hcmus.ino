// include thư viện
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>

//Define wifi cho module và authentication cho firebase
#define WIFI_SSID "duytien"
#define WIFI_PASSWORD "18050101"
#define FIREBASE_HOST "tien-eb218-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "LHwryoGULjAgOn90q8C6HcBIM0TgrsOShMdpnSHY"

//define biến global
FirebaseData firebase_data;
const int khoan_cach_duoi = D3; // for Arduino microcontroller
int khoan_cach_tren = D5;
int chuyen_dong = D7;
bool check_chuyen_dong = false;
bool check_chuyen_dong_old = false;
bool check_cua = false;
bool check_cua_old = false;
String path = "/";
FirebaseJson firebase_json;

void setup(){
  Serial.begin(9600);
  pinMode(khoan_cach_duoi, INPUT);
  pinMode(khoan_cach_tren, INPUT);
  pinMode(chuyen_dong, INPUT);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD); // kết nối với wifi vói giá trị đã được define
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH); // kết nối với firebase
  Firebase.reconnectWiFi(true);
  if(!Firebase.beginStream(firebase_data,path))
  {
    Serial.println("REASON: " + firebase_data.errorReason()); // nếu kết nối có lỗi log ra lỗi
    Serial.println();
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void loop()
{
  //đọc giá trị
  int  val_khoan_cach_duoi = digitalRead(khoan_cach_duoi);
  int  val_chuyen_dong = digitalRead(chuyen_dong);
  int val_khoan_cach_tren = digitalRead(khoan_cach_tren);
  Serial.println(val_khoan_cach_duoi);
  // kiểm tra giá trị đọc được với các điều kiện
  if(val_khoan_cach_duoi == LOW && val_khoan_cach_tren == LOW){
     Serial.println("co nguoi vao");
     check_cua = true;
  }
  else{
    check_cua = false;
  }
  if(val_chuyen_dong == HIGH){
    Serial.println("chuyen dong");
    check_chuyen_dong = true;
  }
  else{
    check_chuyen_dong = false;
  }

  // gửi giá trị lên firebase
  if(check_cua != check_cua_old){
    Firebase.setBool(firebase_data,path+"/data/1", check_cua); 
  }
  check_cua_old = check_cua;
  if(check_chuyen_dong != check_chuyen_dong_old){
    Firebase.setBool(firebase_data,path+"/data/2", check_chuyen_dong);
  }
  check_chuyen_dong_old = check_chuyen_dong;
    
}
