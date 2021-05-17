#include "Arduino.h"
// downlaod and install the AltSoftSerial library  http://www.pjrc.com/teensy/td_libs_AltSoftSerial.html"
#include "AltSoftSerial.h"

// downlaod and install OBD9141 library  https://github.com/iwanders/OBD9141
#include "OBD9141.h"

#define RX_PIN 8   // connect to transceiver Rx
#define TX_PIN 9  // connect to transceiver Tx

AltSoftSerial altSerial;

OBD9141 obd;


void setup(){
    Serial.begin(115200);
    delay(2000);
    while (!Serial) { ; }
    
    obd.begin(altSerial, RX_PIN, TX_PIN);

}
    
void loop(){
    Serial.println("Looping");

    bool init_success =  obd.init();
    Serial.print("init_success:");
    Serial.println(init_success);

    //init_success = true;
    // Uncomment this line if you use the simulator to force the init to be
    // interpreted as successful. With an actual ECU; be sure that the init is 
    // succesful before trying to request PID's.

    if (init_success){
        bool res;
        while(1){
            res = obd.getCurrentPID(0x11, 1);
            if (res){
                Serial.print("Result 0x11 (throttle): ");
                Serial.println(obd.readUint8());
            }
            
            res = obd.getCurrentPID(0x0C, 2);
            if (res){
                Serial.print("Result 0x0C (RPM): ");
                Serial.println(obd.readUint16()/4);
            }


            res = obd.getCurrentPID(0x0D, 1);
            if (res){
                Serial.print("Result 0x0D (speed): ");
                Serial.println(obd.readUint8());
            }
            Serial.println();

            delay(100);
        }
    }
}
