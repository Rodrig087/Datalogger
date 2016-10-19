#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <DS3231.h>
#include "DHT.h"
#include "MQ135.h"
#include <EEPROM.h>

#define DHTPIN 2 
#define DHTTYPE DHT22
#define RZERO 1459

int hora;
int minuto;
int segundo;
int dia;
int mes;
int anio;
int temp;
int humd;
int nh3;
int ban;
int opcion = 1;
boolean ban1 = 0;
boolean bans = 0;

byte temp_max;
byte temp_min;
byte humd_max;
byte humd_min;
byte nh3_max;
byte nh3_min;
byte Direccion;

String s_hora;
String s_minuto;
String s_temp;
String s_humd;
String s_nh3;
String nh;
String Datos;

DS3231 clock;
RTCDateTime dt;
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
DHT dht(DHTPIN, DHTTYPE);
MQ135 gasSensor = MQ135(A0);

void setup() {
  Serial.begin(9600);
  display.begin();
  dht.begin();

  clock.begin();
  //clock.setDateTime(__DATE__, __TIME__);
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT);

  temp_max = dht.readTemperature();
  temp_min = dht.readTemperature();
  
  humd_max = dht.readHumidity();
  humd_min = dht.readHumidity();
  
  nh3_max = gasSensor.getPPM();
  nh3_min = gasSensor.getPPM();

  EEPROM.write(Direccion,temp_max);
  EEPROM.write(Direccion+1,temp_min);
  EEPROM.write(Direccion+2,humd_max);
  EEPROM.write(Direccion+3,humd_min);
  EEPROM.write(Direccion+4,nh3_max);
  EEPROM.write(Direccion+5,nh3_min);
  
  
  display.setContrast(50);
   
}

void Portada(int h, int m, int t, int w, int n){

  if (hora <= 9){
    s_hora = "0"+String(h);  
  }else{
    s_hora = String(h);
  }
  if (minuto <= 9){
    s_minuto = "0"+String(m);  
  }else{
    s_minuto = String(m);
  }

  if (n <= 9){
    nh = "0"+String(n);
  }else{
    nh = String(n);
  }
    
  String hora_act = " "+s_hora+":"+s_minuto;
  String temp_act = " "+String(t)+ " C";
  String humd_act = " "+String(w)+ " %";
  String amon_act = " "+nh+" ppm";
 
  display.setTextSize(1);
  display.setCursor(0,5);
  display.setTextColor(WHITE, BLACK);
  display.print(" Hora:");
  display.setTextColor(BLACK);
  display.println(hora_act);
  display.setCursor(0,15);
  display.setTextColor(WHITE, BLACK);
  display.print(" Temp:");
  display.setTextColor(BLACK);
  display.println(temp_act);
  display.setCursor(0,25);
  display.setTextColor(WHITE, BLACK);
  display.print(" Humd:");
  display.setTextColor(BLACK);
  display.println(humd_act);
  display.setCursor(0,35);
  display.setTextColor(WHITE, BLACK);
  display.print(" NH3: ");
  display.setTextColor(BLACK);
  display.println(amon_act);
  
  display.display();
  display.clearDisplay();   // clears the screen and buffer
  
}

void M_reloj(){

  dt = clock.getDateTime();
  int h_1 = dt.hour;
  int m_1 = dt.minute;
  int s_1 = dt.second;
  int ms_1 = dt.month;
  int d_1 = dt.day;
  int a_1 = dt.year;

  String s_h1;
  String s_m1;
  String s_s1;
  String s_d1;
  String s_ms1;
  
  if (h_1 <= 9){
    s_h1 = "0"+String(h_1);  
  }else{
    s_h1 = String(h_1);
  }
  if (m_1 <= 9){
    s_m1 = "0"+String(m_1);  
  }else{
    s_m1 = String(m_1);
  }
  if (s_1 <= 9){
    s_s1 = "0"+String(s_1);  
  }else{
    s_s1 = String(s_1);
  }
  if (d_1 <= 9){
    s_d1 = "0"+String(d_1);  
  }else{
    s_d1 = String(d_1);
  }
  if (ms_1 <= 9){
    s_ms1 = "0"+String(ms_1);  
  }else{
    s_ms1 = String(ms_1);
  }
  
  String hora_1 = " "+s_h1+":"+s_m1+":"+s_s1;
  String fecha_1 = " "+s_d1+"/"+s_ms1+"/"+String(a_1);;
  
  display.setTextSize(1);
  display.setCursor(5,5);
  display.setTextColor(WHITE, BLACK);
  display.print(" Hora-Fecha ");
  display.setCursor(10,20);
  display.setTextColor(BLACK);
  display.println(hora_1); 
  display.setCursor(5,35);
  display.setTextColor(BLACK);
  display.println(fecha_1); 

  display.display();
  display.clearDisplay();   // clears the screen and buffer
  
}

void M_temperatura(int t, int t_max, int t_min){

  String temp_act = " "+String(t)+ " C";
  String temp_max = " "+String(t_max)+ " C";
  String temp_min = " "+String(t_min)+ " C";
  
  display.setTextSize(1);
  display.setCursor(4,5);
  display.setTextColor(WHITE, BLACK);
  display.print(" Temperatura ");
  display.setCursor(0,15);
  display.setTextColor(WHITE, BLACK);
  display.print(" act: ");
  display.setTextColor(BLACK);
  display.println(temp_act);
  display.setCursor(0,25);
  display.setTextColor(WHITE, BLACK);
  display.print(" max: ");
  display.setTextColor(BLACK);
  display.println(temp_max); 
  display.setCursor(0,35);
  display.setTextColor(WHITE, BLACK);
  display.print(" min: ");
  display.setTextColor(BLACK);
  display.println(temp_min); 

  display.display();
  display.clearDisplay();   // clears the screen and buffer
  
}

void M_humedad(int h, int h_max, int h_min){

  String hmd_act = " "+String(h)+ " %";
  String hmd_max = " "+String(h_max)+ " %";
  String hmd_min = " "+String(h_min)+ " %";
  
  display.setTextSize(1);
  display.setCursor(14,5);
  display.setTextColor(WHITE, BLACK);
  display.print(" Humedad ");
  display.setCursor(0,15);
  display.setTextColor(WHITE, BLACK);
  display.print(" act: ");
  display.setTextColor(BLACK);
  display.println(hmd_act);
  display.setCursor(0,25);
  display.setTextColor(WHITE, BLACK);
  display.print(" max: ");
  display.setTextColor(BLACK);
  display.println(hmd_max); 
  display.setCursor(0,35);
  display.setTextColor(WHITE, BLACK);
  display.print(" min: ");
  display.setTextColor(BLACK);
  display.println(hmd_min); 

  display.display();
  display.clearDisplay();   // clears the screen and buffer
  
}

void M_amoniaco(int a, int a_max, int a_min){
  
  String amn_act = " "+String(a)+ " ppm";
  String amn_max = " "+String(a_max)+ " ppm";
  String amn_min = " "+String(a_min)+ " ppm";
  
  display.setTextSize(1);
  display.setCursor(11,5);
  display.setTextColor(WHITE, BLACK);
  display.print(" Amoniaco ");
  display.setCursor(0,15);
  display.setTextColor(WHITE, BLACK);
  display.print(" act:");
  display.setTextColor(BLACK);
  display.println(amn_act);
  display.setCursor(0,25);
  display.setTextColor(WHITE, BLACK);
  display.print(" max:");
  display.setTextColor(BLACK);
  display.println(amn_max); 
  display.setCursor(0,35);
  display.setTextColor(WHITE, BLACK);
  display.print(" min:");
  display.setTextColor(BLACK);
  display.println(amn_min); 

  display.display();
  display.clearDisplay();   // clears the screen and buffer
  
}

void Comunicacion(int h, int m, int d, int ms, int an, int t, int hm, int a ){

  String C_hora;
  String C_minuto;
  String C_dia;
  String C_mes;
  String C_anio;
  String C_temp;
  String C_humd;
  String C_nh3;

  if (h <= 9){
    C_hora = "0"+String(h);  
  }else{
    C_hora = String(h);
  }
  if (m <= 9){
    C_minuto = "0"+String(m);  
  }else{
    C_minuto = String(m);
  }
  if (d <= 9){
    C_dia = "0"+String(d);  
  }else{
    C_dia = String(d);
  }
  if (ms <= 9){
    C_mes = "0"+String(ms);  
  }else{
    C_mes = String(ms);
  }
  if (an <= 9){
    C_anio = "0"+String(an);  
  }else{
    C_anio = String(an);
  }
  if (t <= 9){
    C_temp = "0"+String(t);  
  }else{
    C_temp = String(t);
  }
  if (hm <= 9){
    C_humd = "0"+String(hm);  
  }else{
    C_humd = String(hm);
  }
  if (a <= 9){
    C_nh3 = "00"+String(a);  
  }else if (a>9 and a<=99){
    C_nh3 = "0"+String(a);
  }else if (a>99){
    C_nh3 = "0"+C_nh3;
  }
  
  
  Datos=String(C_hora)+String(C_minuto)+String(C_mes)+String(C_dia)+String(C_anio)+String(C_temp)+String(C_humd)+String(C_nh3);
  Serial.println(Datos);
  delay(200);
  
}

void loop() {
//////////////////////////////////////////////////////////////////////

//Se desplaza a traves del menu de 5 elementos
  boolean menu = digitalRead(10);
  delay(50);
  
  if (menu==0 and ban1==0){
    opcion++;
    ban1 = 1;
    if (opcion>5){
      opcion=1;  
    }
  } else if (menu==1){
      ban1=0;  
  }

//////////////////////////////////////////////////////////////////////  

//Obtiene los datos de cada uno de los sensores
  dt = clock.getDateTime();
  hora = dt.hour;
  minuto = dt.minute;
  segundo = dt.second;
  mes = dt.month;
  dia = dt.day;
  anio = dt.year;
  temp = dht.readTemperature();
  humd = dht.readHumidity();
  nh3 = gasSensor.getPPM();
  
//////////////////////////////////////////////////////////////////////

//Envia trama de datos por Uart cada 20 minutos
  if ((minuto%20==0)&&(segundo==0)&&bans==0){
    Comunicacion(hora, minuto, dia, mes, anio, temp, humd, nh3 );   
    bans = 1;   
  }
  if(segundo==5){
    bans = 0;  
  }
  
//////////////////////////////////////////////////////////////////////  

//Actualiza los valores maximos y minimos para temperatura, humedad y amoniaco   
  if (temp>temp_max){
    temp_max = temp;
  }
  if (temp<temp_min){
    temp_min = temp;
  }
  if (humd>humd_max){
    humd_max = humd;
  }
  if (humd<humd_min){
    humd_min = humd;
  }
  if (nh3>nh3_max){
    nh3_max = nh3;
  }
  if (humd<humd_min){
    nh3_min = nh3;
  } 
  
//////////////////////////////////////////////////////////////////////

//Revisa el estado del menu para visualizar la portada o cada uno de los datos  
  if (opcion == 1){
    Portada(hora,minuto,temp,humd,nh3);
  }
  else if (opcion == 2){
    M_reloj();
  }
  else if (opcion == 3){
    M_temperatura(temp,temp_max, temp_min);
  }
  else if (opcion == 4){
    M_humedad(humd, humd_max, humd_min);
  }
  else if (opcion == 5){
    M_amoniaco(nh3, nh3_max, nh3_min);
  }
  
//////////////////////////////////////////////////////////////////////
}
