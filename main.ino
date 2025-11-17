#include <array>

constexpr int letters = 27;
std::array<uint8_t, letters> pins {{
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
}};

std::array<int, 6> word_lengths {{5, 5, 5, 3, 5, 4}};

int mode = 0;

using mode_fn = void(*)();

int modePin = 27;
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void mode1() {
  constexpr std::array<int, letters> delays{{
    50, 50, 50, 50,    50, 50, 50, 50, 50,    50, 50, 50, 50, 50,   200, 0, 0,   200, 0, 0, 0, 0,   200, 0, 0, 0
  }};
  for(int i = 0; i < letters; i++) {
    digitalWrite(pins[i], LOW);
  }
  for(int i = 0; i < letters; i++) {
    digitalWrite(pins[i], HIGH);
    delay(delays[i]);
  }
}

void mode2() {
  constexpr std::array<int, letters> delays{{
    50, 50, 50, 50,    50, 50, 50, 50, 50,    50, 50, 50, 50, 50,   50, 50, 50,   50, 50, 50, 50, 50,   50, 50, 50, 50
  }};
  for(int i = 0; i < letters; i++) {
    digitalWrite(pins[i], LOW);
  }
  for(int i = 0; i < letters; i++) {
    digitalWrite(pins[i], HIGH);
    delay(delays[i]);
  }
}

std::array<mode_fn, 2> modes {{&mode1, &mode2}};

void setup() {
  pinMode(modePin, INPUT);
  for(auto pin : pins)
  {
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  // === Mode pin debouncing, copied from example ===
  // read the state of the switch into a local variable:
  int reading = digitalRead(modePin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        mode = (mode + 1) % modes.size();
      }
    }
  }

  // === run actual blinking behavior ===
  modes[mode]();

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
