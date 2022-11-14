#include <Keypad.h>
#include <ctype.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char numberKeys[ROWS][COLS] = {
    { '1','4','7','E' },
    { 'A','B','C','D' },
    { '2','5','8','0' },
    { '3','6','9','F' }
    
};
boolean alpha = false;   // Start with the numeric keypad.
byte rowPins[ROWS] = {6,7,8,9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2,3,4,5}; //connect to the column pinouts of the keypad
// Create two new keypads, one is a number pad and the other is a letter pad.
Keypad numpad( makeKeymap(numberKeys), rowPins, colPins, sizeof(rowPins), sizeof(colPins) );
unsigned long startTime;
const byte ledPin = 13;                            // Use the LED on pin 13.
void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);                 // Turns the LED on.
    numpad.begin( makeKeymap(numberKeys) );
    numpad.addEventListener(keypadEvent_num);  // Add an event listener.
    numpad.setHoldTime(500);                   // Default is 1000mS
}

char key;
void loop() {
        key = numpad.getKey( );
    if (alpha && millis()-startTime>100) {           // Flash the LED if we are using the letter keymap.
        digitalWrite(ledPin,!digitalRead(ledPin));
        startTime = millis();
    }
}

static char virtKey = NO_KEY;      // Stores the last virtual key press. (Alpha keys only)
static char physKey = NO_KEY;      // Stores the last physical key press. (Alpha keys only)
static char buildStr[12];
static byte buildCount;
static byte pressCount;

static byte kpadState;

// Take care of some special events.

void keypadEvent_ltr(KeypadEvent key) {
    // in here when in alpha mode.
    swOnState( key );
} // end ltrs keypad events

void keypadEvent_num( KeypadEvent key ) {
    // in here when using number keypad
    kpadState = numpad.getState( );
    swOnState( key );
} // end numbers keypad events

void swOnState( char key ) {
    switch( kpadState ) {
        case PRESSED:
            if (isalpha(key)) {              // This is a letter key so we're using the letter keymap.
                if (physKey != key) {        // New key so start with the first of 3 characters.
                    pressCount = 0;
                    virtKey = key;
                    physKey = key;
                }
                else {                       // Pressed the same key again...
                    virtKey++;                   // so select the next character on that key.
                    pressCount++;                // Tracks how many times we press the same key.
                }
                    if (pressCount > 2) {    // Last character reached so cycle back to start.
                        pressCount = 0;
                        virtKey = key;
                    }
                    Serial.print(virtKey);   // Used for testing.
                }
                if (isdigit(key) || key == ' ' || key == '.')
                    Serial.print(key);
                if (key == '#')
                    Serial.println();
                break;

        case HOLD:
            if (key == '#')  {               // Toggle between keymaps.
                if (alpha == true)  {        // We are currently using a keymap with letters
                    alpha = false;           // Now we want a keymap with numbers.
                    digitalWrite(ledPin, LOW);
                }
                else  {                      // We are currently using a keymap with numbers
                    alpha = true;            // Now we want a keymap with letters.
                }
            }
            else  {                          // Some key other than '#' was pressed.
                buildStr[buildCount++] = (isalpha(key)) ? virtKey : key;
                buildStr[buildCount] = '\0';
                Serial.println();
                Serial.println(buildStr);
            }
            break;

        case RELEASED:
            if (buildCount >= sizeof(buildStr))  buildCount = 0;  // Our string is full. Start fresh.
            break;
    }  // end switch-case
}// end switch on state function
