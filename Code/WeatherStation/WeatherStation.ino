//www.elegoo.com
//2018.10.25


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
static const short screens = 2;

void setup( ) {
  //Initialize the serial port
  Serial.begin( 9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("Hello, World!");
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

static void display( String title, String value) {
  lcd.clear();
  lcd.print(title);
  lcd.setCursor(0, 1);
  lcd.print(value);
}

void loop( ) {
  float temperature;
  float humidity;

  static int index = 0;

  /* Measure temperature and humidity.  If the functions returns
    true, then a measurement is available. */
  if ( measure_environment( &temperature, &humidity ) == true ) {
    if (index%2==0) {
      display("Temperature:", String(String(temperature,1) + " C"));
      index = 1;
    }else{
      display("Humidity:", String(String(humidity,1) + "%"));
      index = 0;
    }
    Serial.print("T = ");
    Serial.print(temperature,1);
    Serial.print("°C, H = ");
    Serial.print(humidity,1);
    Serial.println("%\n--------------------");
  }
}
