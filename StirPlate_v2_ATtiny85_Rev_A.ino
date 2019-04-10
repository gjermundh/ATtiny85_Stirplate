#include <TinyServo.h>

/* StirPlate_v2_tiny85_Rev_A
 *
 * This program is intended for an ATtiny85 hooked up to a LED, potentiometer and
 * an electronic speed controller (ESC) for controlling the speed of a 3 phase
 * brushless DC motor.
 *
 * Revision history:
 * A - First release
 */

/* Define pins used */
int PIN_POTENTIOMETER = A3;
int PIN_STOP_LED = 1;
int PIN_PWM_ESC = 0;

const byte SERVOS = 1;
const byte servoPin[SERVOS] = { PIN_PWM_ESC };

/* Define local variables */
boolean start_lock = true;
int val_pot = 0;
int val_esc = 0;
int val_esc_prev = 0;
int i = 0;

void setup()
{
  /* Initialize pins */
  pinMode(PIN_STOP_LED, OUTPUT);
  pinMode(PIN_POTENTIOMETER, INPUT);
  
  /* Initialize ESC */
  setupServos();
  moveServo(0,180);
  delay(1000);
  moveServo(0,0);

  for(i = 0; i < 25; i++)
  {
    digitalWrite(PIN_STOP_LED, HIGH);
    delay(100);
    digitalWrite(PIN_STOP_LED, LOW);
    delay(100);
  }
  
  /* Turn on stop LED */
  digitalWrite(PIN_STOP_LED, HIGH);
}

void loop()
{
  /* Read potentiometer */
  val_pot = analogRead(PIN_POTENTIOMETER);

  /* Check start lock (to prevent full speed starts) */
  if (val_pot < 10)
  {
    start_lock = false;
  }

  /* Write to servo as long as the start lock is disabled */
  if (!start_lock)
  { 
    val_esc = map(val_pot, 0, 1023, 0, 169);

    /* Clip upper to smooth out adjustments */
    if (val_esc - val_esc_prev > 2)
    {
      val_esc = val_esc_prev + 2;
    }

    /* Clip lower to smooth out adjustments */
    if (val_esc - val_esc_prev < -2)
    {
      val_esc = val_esc_prev - 2;
    }

    /* Write value to ESC */
    //esc.write(val_esc);
    moveServo(0, val_esc);
    
    /* Update stop LED */
    if (val_pot < 10)
    {
      digitalWrite(PIN_STOP_LED, HIGH);
    }
    else
    {
      digitalWrite(PIN_STOP_LED, LOW);
    }
    
    /* Update prev variable */
    val_esc_prev = val_esc;
  }

  /* Update servo and wait */
  delay(100);
}
