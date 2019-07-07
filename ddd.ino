// On ESP8266:
// Runs at both 80MHz and 160MHz CPU frequency at 56000bps with only intermittent errors.
// Connect pin 12 to 14.

#include <SoftwareSerial.h>

#ifdef ESP32
#define BAUD_RATE 96000


#else
#define BAUD_RATE 9600
#endif

// Reminder: the buffer size optimizations here, in particular the isrBufSize that only accommodates
// a single 8N1 word, are on the basis that any char written to the loopback SoftwareSerial adapter gets read
// before another write is performed. Block writes with a size greater than 1 would usually fail. 

SoftwareSerial swMB1(14, 12, false, 256);//To main board
SoftwareSerial swBA1(4, 5, false, 256);//from Bill Acceptor 1

SoftwareSerial swMB2(13, 15, false, 256);//To main board
SoftwareSerial swBA2(2, 0, false, 256);//from Bill Acceptor 2
void setup() {
	Serial.begin(BAUD_RATE);
	swMB1.begin(BAUD_RATE);
  swMB2.begin(BAUD_RATE);
  swBA2.begin(BAUD_RATE);
  swBA1.begin(BAUD_RATE);

	// ESP8266 internal cache RAM needs warm up - allow write and ISR to load
	swMB1.write(static_cast<uint8_t>(0));
  swMB2.write(static_cast<uint8_t>(0));
  swBA2.write(static_cast<uint8_t>(0));
  swBA1.write(static_cast<uint8_t>(0));
 
	Serial.println("\nSoftware serial test started");

//	for (char ch = ' '; ch <= 'z'; ch++) {
//		swMB1.write(ch);
//	}
//	swMB1.println("");
}

/*Received from Bill acceptor*/
#define PowerON1 0X80
#define PowerON2 0X8F
//#define MYR1 0X45
#define MYR10 0X43
//#define MYR10 0X41
//#define MYR10 0X43
#define MYR1 0X40
#define MYR5 0X42
#define MYR20 0X46
#define MotorFailure 0x20
#define ChecksumError 0x21
#define BillJam 0x22
#define BillRemove 0x23
#define StackerOpen 0x24
#define SensorProblem 0x25
#define BillFish 0x27
#define StackerProblem 0x28
#define BillReject 0x29
#define Reserved 0x2E
#define ResponseWhenErrorExclusion 0x2F
#define BillEnable 0x3E
#define BillInhibit 0x5E
/*Transmit to Bill acceptor*/
#define OnHold 0X18
#define ACCEPT 0X02
#define REJECT 0X0F
#define Reset 0x30
#define RequestStatus 0x0C

uint8_t data,data2,data3,data4;
void loop() {
	if (swMB1.available() > 0) {
//	  Serial.write(swMB1.read());
    data = swMB1.read();
    Serial.write(data);
	}
 
  if (swMB2.available() > 0) {
    data2 = swMB2.read();
    Serial.write(data2);
  }
  
  if (swBA2.available() > 0) {
    data3 = swBA2.read();
    Serial.write(data3);
    if(data3 == MYR1){
      swBA2.write(ACCEPT);
    }else if (data3 == MYR5){
      swBA2.write(ACCEPT);
    }else if (data3 == MYR20|| data3 == MYR10){
      swBA2.write(ACCEPT);
    }else if (data3 == PowerON1 || data3 == PowerON2){
      swBA2.write(ACCEPT);
    }
    swMB2.write(data3);
  }
  
  if (swBA1.available() > 0) {
    data4 = swBA1.read();
    Serial.write(data4);
    if(data4 == MYR1){
      swBA1.write(ACCEPT);
    }else if (data4 == MYR5){
      swBA1.write(ACCEPT);
    }else if (data4 == MYR20 || data4 == MYR10){
      swBA1.write(ACCEPT);
    }else if (data4 == PowerON1 || data4 == PowerON2){
      swBA1.write(ACCEPT);
    }
    swMB1.write(data4);
  }
	if (Serial.available() > 0) {
//		swMB1.write(Serial.read());
//    swMB2.write(Serial.read());
//    swBA2.write(Serial.read());
    swBA1.write(Serial.read());
	}

}
