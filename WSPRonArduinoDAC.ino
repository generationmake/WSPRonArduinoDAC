/* This sketch will generate WSPR signals on the DAC of a SAMD Arduino */

/** sine table for 8 values:
 *
 *  000 = +0.000 =  512
 *  045 = +0.707 =  873
 *  090 = +1.000 = 1023
 *  135 = +0.707 =  873
 *  180 = +0.000 =  512
 *  225 = -0.707 =  151
 *  270 = -1.000 =    1
 *  315 = -0.707 =  151
 */
//const int sin_table[]={512,873,1023,873,512,151,1,151};
//const int sin_table[]={512,582,612,582,512,442,412,442};
const int sin_table[]={512,519,522,519,512,505,502,505};

/** sine table for 16 values:
 *
 *  000.0 = +0.000 =  512
 *  022.5 = +0.383 =
 *  045.0 = +0.707 =  873
 *  067.5 = +0.924 =
 *  090.0 = +1.000 = 1023
 *  112.5 = +0.924 =
 *  135.0 = +0.707 =  873
 *  157.5 = +0.383 =
 *  180.0 = +0.000 =  512
 *  202.5 = -0.383 =
 *  225.0 = -0.707 =  151
 *  247.5 = -0.924 =
 *  270.0 = -1.000 =    1
 *  292.5 = -0.924 =
 *  315.0 = -0.707 =  151
 *  337.5 = -0.383 =
 */
const int sin_table16[]={512,516,519,521,522,521,519,516,512,508,505,503,502,503,505,508};

#include <SAMDTimerInterrupt.h>
#include <JTEncode.h>

#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#define WSPR_TONE_SPACING       146          // ~1.46 Hz
#define WSPR_DELAY              683          // Delay value for WSPR
#define BASE_FREQ               1500.0   // NF base frequency
#define PTT_PIN                 2        // PTT pin

// Class instantiation
SAMDTimer ITimer0(TIMER_TC3);
JTEncode jtencode;

// Global variables
char call[] = "N0CALL";
char loc[] = "AA00";
uint8_t dbm = 27;
uint8_t tx_buffer[255];
uint8_t symbol_count;
uint16_t tone_delay, tone_spacing;

// Loop through the string, transmitting one character at a time.
void encode()
{
  uint8_t i;

  // Reset the tone to the base frequency and turn on the output
  digitalWrite(LED_BUILTIN, HIGH);
  // set PTT for transmission
  digitalWrite(PTT_PIN, HIGH);

  // Now transmit the channel symbols
  for(i = 0; i < symbol_count; i++)
  {
    ITimer0.attachInterrupt(16.0*(BASE_FREQ+(float)(tx_buffer[i] * tone_spacing)/100.0), TimerHandler0);
    delay(tone_delay);
  }

  // Turn off the output
  ITimer0.detachInterrupt();
  digitalWrite(LED_BUILTIN, LOW);
  // set PTT for transmission
  digitalWrite(PTT_PIN, LOW);
}

void set_tx_buffer()
{
  // Clear out the transmit buffer
  memset(tx_buffer, 0, 255);

  jtencode.wspr_encode(call, loc, dbm, tx_buffer);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, OUTPUT);
  analogWrite(A0,0);

  // Use the Arduino's on-board LED as a keying indicator.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // set PTT pin
  pinMode(PTT_PIN, OUTPUT);
  digitalWrite(PTT_PIN, LOW);

  // Set the proper frequency, tone spacing, symbol count, and
  // tone delay depending on mode
  symbol_count = WSPR_SYMBOL_COUNT; // From the library defines
  tone_spacing = WSPR_TONE_SPACING;
  tone_delay = WSPR_DELAY;

  // Encode the message in the transmit buffer
  // This is RAM intensive and should be done separately from other subroutines
  set_tx_buffer();
}

void loop() {
  int seconds=millis()/1000;  // get seconds from start
  if((seconds%120)==0) encode();     // start with WSPR transmission every 2 minutes
}

void TimerHandler0()
{
  static int counter=0;
  analogWrite(A0,sin_table16[counter]);
  counter++;
  counter &=0x000f;
}
