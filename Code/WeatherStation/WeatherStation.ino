/* Higrometric sensor */
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11
static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
static const unsigned long DELAY_MEASURE_MILLISECONDS = 5000ul;

/* LCD display: */
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

/* IR Receiver */
#include "IRremote.h"
static const int IR_REMOTE_RECEIVER_PIN = 6; // Signal Pin of IR receiver to Arduino Digital Pin 11
IRrecv irrecv(IR_REMOTE_RECEIVER_PIN);
decode_results results;

float temperature;
float humidity;
static const int PAGES_NUMBER = 5;
static int current_page = 0;
static bool lcd_on = false;


void displayLCD( String title, String value) {
  Serial.println("Diplay LCD");
  Serial.println("title: " + title);
  Serial.println("value: " + value);

  lcd.clear(); // this includes lcd.setCursor(0, 0);
  lcd.print(title);
  lcd.setCursor(0, 1);
  lcd.print(value);
}

void selectPage(int page) {
  Serial.println("Diplay page " + page);
  current_page = page % PAGES_NUMBER;

  switch (current_page) {
    case 0:
      displayLCD("Temperature:", String(String(temperature, 1) + "C"));
      break;
    case 1:
      displayLCD("Humidity:", String(String(humidity, 1) + "%"));
      break;
    default:
      diplayLCD("Page not Found", "'\(._.)/'");
  }
}

// takes action based on IR code received
void translateIR() {
  // describing Remote IR codes
  switch (results.value) {
    case 0xFFA25D:
      Serial.println("POWER");
      if (lcd_on) {
        lcd.display();
      } else {
        lcd.noDisplay();
      }
      lcd_on = !lcd_on;
      break;
    case 0xFFE21D: Serial.println("FUNC/STOP"); break;
    case 0xFF629D: Serial.println("VOL+"); break;
    case 0xFF22DD: Serial.println("FAST BACK");    break;
    case 0xFF02FD: Serial.println("PAUSE");    break;
    case 0xFFC23D: Serial.println("FAST FORWARD");   break;
    case 0xFFE01F: // DOWN
      Serial.println("DOWN");
      selectPage(current_page - 1);
      break;
    case 0xFFA857: Serial.println("VOL-");    break;
    case 0xFF906F: // UP
      Serial.println("UP");
      selectPage(current_page + 1);
      break;
    case 0xFF9867: Serial.println("EQ");    break;
    case 0xFFB04F: Serial.println("ST/REPT");    break;
    case 0xFF6897: // 0
      selectPage(0);
      break;
    case 0xFF30CF: // 1
      selectPage(1);
      break;
    case 0xFF18E7: // 2
      selectPage(2);
      break;
    case 0xFF7A85: // 3
      selectPage(3);
      break;
    case 0xFF10EF: // 4
      selectPage(4);
      break;
    case 0xFF38C7: // 5
      selectPage(5);
      break;
    case 0xFF5AA5: // 6
      selectPage(6);
      break;
    case 0xFF42BD: // 7
      selectPage(7);
      break;
    case 0xFF4AB5: // 8
      selectPage(8);
      break;
    case 0xFF52AD: // 9
      selectPage(9);
      break;
    case 0xFFFFFFFF: Serial.println(" REPEAT"); break;

    default:
      Serial.println(" other button   ");

  }// End Case

  delay(250); // Do not get immediate repeat
}

/*
   Poll for a measurement, keeping the state machine alive.  Returns
   true if a measurement is available.
*/
static bool measure_environment( float *temperature, float *humidity ) {
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if ( millis( ) - measurement_timestamp > DELAY_MEASURE_MILLISECONDS ) {
    if ( dht_sensor.measure( temperature, humidity ) == true ) {
      measurement_timestamp = millis( );
      return ( true );
    }
  }

  return ( false );
}

void setup( ) {
  //Initialize the serial port
  Serial.begin( 9600);
  Serial.println("IR Receiver Button Decode");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.

  irrecv.enableIRIn(); // Start the receiver
}

void loop( ) {
  // Measure temperature and humidity
  measure_environment( &temperature, &humidity );
  
  if (irrecv.decode(&results)) { // have we received an IR signal?
    translateIR();
    irrecv.resume(); // receive the next value
  }
}
