#include "Arduino.h"
// downlaod and install the AltSoftSerial library  http://www.pjrc.com/teensy/td_libs_AltSoftSerial.html"
#include "AltSoftSerial.h"

// downlaod and install OBD9141 library  https://github.com/iwanders/OBD9141
#include "OBD9141.h"

#define RX_PIN 8   // connect to transceiver Rx
#define TX_PIN 9  // connect to transceiver Tx

AltSoftSerial altSerial;


/*
  This example shows how to use the library to read the diagnostic trouble codes
  from the ECU and print these in a human readable format to the serial port.
  Huge thanks goes out to to https://github.com/produmann for helping with
  development of this feature and the extensive testing on a real car.
*/

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

    // Trouble code consists of a letter and then four digits, we write this
    // human readable ascii string into the dtc_buf which we then write to the
    // serial port.
    uint8_t dtc_buf[5];

    if (init_success){
        uint8_t res;
        while(1){
            // res will hold the number of trouble codes that were received.
            // If no diagnostic trouble codes were retrieved it will be zero.
            // The ECU may return trouble codes which decode to P0000, this is
            // not a real trouble code but instead used to indicate the end of
            // the trouble code list.
            res = obd.readTroubleCodes();
            if (res){
                Serial.print("Read ");
                Serial.print(res);
                Serial.println(" codes:");
                for (uint8_t index = 0; index < res; index++)
                {
                  // retrieve the trouble code in its raw two byte value.
                  uint16_t trouble_code = obd.getTroubleCode(index);

                  // If it is equal to zero, it is not a real trouble code
                  // but the ECU returned it, print an explanation.
                  if (trouble_code == 0)
                  {
                    Serial.println("P0000 (reached end of trouble codes)");
                  }
                  else
                  {
                    // convert the DTC bytes from the buffer into readable string
                    OBD9141::decodeDTC(trouble_code, dtc_buf);
                    // Print the 5 readable ascii strings to the serial port.
                    Serial.write(dtc_buf, 5);
                    Serial.println();
                  }
                }
            }
            else
            {
              Serial.println("No trouble codes retrieved.");
            }
            Serial.println();

            delay(200);
        }
    }
    delay(3000);
}
