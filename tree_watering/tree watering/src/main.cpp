#include <Arduino.h>

#define float_switch1 2
#define float_switch2 3

#define pump_output 8
#define pump_on_led 13

#define float_switch_supply 4
#define supply_led 11

#define on_led 12

#define check_timeout 1000

bool pump_on;

long last_poll_time;

void setup()
{
  pinMode(float_switch1, INPUT_PULLUP);
  pinMode(float_switch2, INPUT_PULLUP);
  pinMode(float_switch_supply, INPUT_PULLUP);

  pinMode(pump_output, OUTPUT);
  pinMode(pump_on_led, OUTPUT);
  digitalWrite(pump_on_led, LOW);

  pinMode(supply_led, OUTPUT);
  digitalWrite(supply_led, LOW);

  pinMode(on_led, OUTPUT);
  digitalWrite(on_led, HIGH);

  digitalWrite(pump_output, HIGH);
  pump_on = false;

  last_poll_time = millis();

  Serial.begin(9600);
}

bool time_to_poll()
{
  return abs(millis() - last_poll_time) > check_timeout;
}

bool water_low()
{
  return digitalRead(float_switch1) == LOW && digitalRead(float_switch2) == LOW;
}

bool has_supply_water()
{
  return digitalRead(float_switch_supply) == HIGH;
}

void turn_pump_on()
{
  Serial.println("turning pump ON...");
  digitalWrite(pump_output, LOW);
  digitalWrite(pump_on_led, HIGH);
  pump_on = true;
}

void turn_pump_off()
{
  Serial.println("turning pump OFF...");
  digitalWrite(pump_output, HIGH);
  digitalWrite(pump_on_led, LOW);
  pump_on = false;
}

void loop()
{
  if (time_to_poll() || pump_on)
  {
    Serial.println("checking status...");
    last_poll_time = millis();
    if (has_supply_water())
    {
      digitalWrite(supply_led, LOW);
      if (water_low())
      {
        Serial.println("water is low");
        if (!pump_on)
        {
          turn_pump_on();
        }
      }
      else
      {
        Serial.println("water is not low");
        if (pump_on)
        {
          turn_pump_off();
        }
      }
    }
    else
    {
      Serial.println("water supply is empty");
      digitalWrite(supply_led, HIGH);
      if (pump_on) {
        turn_pump_off();
      }
    }
  }
}