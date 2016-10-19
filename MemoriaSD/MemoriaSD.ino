#include <SPI.h>
#include <SD.h>

File dataFile;

const int chipSelect = 10;

// Buffer to store incoming commands from serial port
byte banf=0;
byte banfin=0;
int i=0;
String inData;
String tiempo;
String hora;
String minuto;
String fecha;
String temp;
String humd;
String amon;
String Filename;

void setup() {
    Serial.begin(9600);
    pinMode(10, OUTPUT);
    if (!SD.begin(10)) {
        Serial.println("Fallo inicializacion MicroSD");
        return;
    }
    Serial.println("Iniciando...");
}

void loop() {

    String dataString = "";
    while (Serial.available() > 0)
    {
        char recieved = Serial.read();
        
        inData += recieved;
        

        // Process message when new line character is recieved
        if (recieved == '\n') //'\n'
        {
            for (i=0;i<19;i++){
              if ((i>=0)&&(i<=3)){
                  tiempo += inData[i];  
              }
              if ((i>=0)&&(i<=1)){
                  hora += inData[i];  
              }
              if ((i>=2)&&(i<=3)){
                  minuto += inData[i];  
              }
              if ((i>=4)&&(i<=11)){
                  fecha += inData[i];  
              } 
              if ((i>=12)&&(i<=13)){
                  temp += inData[i];  
              }
              if ((i>=14)&&(i<=15)){
                  humd += inData[i];  
              }
              if ((i>=16)&&(i<=18)){
                  amon += inData[i];  
              }
            }

            if (banfin==0){
                Filename = fecha+".txt";
                banfin = 1;  
            }
            if ((hora=="00")&&(minuto=="00")&&(banf==0)){
                Filename = fecha+".txt";
                banf = 1;  
            }
            if ((hora=="00")&&(minuto=="10")){
                banf = 0;  
            }

            dataString = tiempo + "  " + temp + "  " + humd + "  " + amon;
            dataFile = SD.open(Filename, FILE_WRITE);

            // if the file is available, write to it:
            if (dataFile) {
                Serial.println("Escribiendo en la MicroSD");
                dataFile.println(dataString);
                dataFile.close();
                // print to the serial port too:
                Serial.println(dataString);
            }
            // if the file isn't open, pop up an error:
            else {
                Serial.println("error abriendo la MicroSD");
            }

            // You can put some if and else here to process the message juste like that:

            inData = ""; // Clear recieved buffer
            tiempo = "";
            hora = "";
            minuto = "";
            fecha = "";
            temp = "";
            humd = "";
            amon = "";
        }
    }
}
