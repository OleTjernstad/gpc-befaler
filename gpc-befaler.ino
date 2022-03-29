
const int MAX_LEVEL = 15;
int sequence[MAX_LEVEL];
int sound[MAX_LEVEL];
int gamer_sequence[MAX_LEVEL];
int level = 1;
int note = 0;
int velocity = 1000;
int running = 0;

const int LED1 = 13;
const int LED2 = 12;
const int LED3 = 11;
const int LED4 = 10;

const int LED_RUNNING = 9;

const int S1 = A0;
const int S2 = A1;
const int S3 = A2;
const int S4 = A3;

const int S_START = A4;

const int SPEAKER = 3;

#define NOTE_C4 262
#define NOTE_G3 196
#define NOTE_A3 220
#define NOTE_B3 247
#define NOTE_C4 262

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 7, 5, 4, 2, A5);

// notes in the melody:
int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4};

void setup()
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("Trykk start");

    Serial.begin(9600);

    pinMode(LED1, OUTPUT); // green led
    pinMode(LED2, OUTPUT); // red led
    pinMode(LED3, OUTPUT); // yellow led
    pinMode(LED4, OUTPUT); // blue led

    pinMode(LED_RUNNING, OUTPUT); // white on led

    pinMode(S1, INPUT_PULLUP); // button 1 green
    pinMode(S2, INPUT_PULLUP); // button 2 red
    pinMode(S3, INPUT_PULLUP); // button 3 yellow
    pinMode(S4, INPUT_PULLUP); // button 4 blue

    pinMode(S_START, INPUT_PULLUP); // button start

    pinMode(SPEAKER, OUTPUT); // buzzer

    // iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 8; thisNote++)
    {

        // to calculate the note duration, take one second
        // divided by the note type.
        // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(SPEAKER, melody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(SPEAKER);
    }
}

void loop()
{
    if (level == 1)
    {
        generate_sequence();

        for (int i = 13; i >= 10; i--)
        { // flashing leds sequence
            digitalWrite(i, HIGH);
            delay(60);
            digitalWrite(i, LOW);
        }
    }
    if (digitalRead(S_START) == LOW)
    {
        digitalWrite(LED_RUNNING, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("Runde:         ");

        delay(200);
        running = 1; // Start the game
    }

    if (running == 1 || level != 1)
    {
        if (level < MAX_LEVEL)
        {
            // Print level number to screen
            lcd.setCursor(7, 0);
            lcd.print(level);
        }

        show_sequence();
        get_sequence();
    }
}

void generate_sequence()
{
    randomSeed(millis()); // true random

    for (int i = 0; i < MAX_LEVEL; i++)
    {
        sequence[i] = random(10, 14);

        switch (sequence[i])
        { // convert color to sound
        case 10:
            note = 349; // Fa
            break;
        case 11:
            note = 329; // Mi
            break;
        case 12:
            note = 293; // Re
            break;
        case 13:
            note = 261; // Do
            break;
        }
        sound[i] = note;
    }
}

void show_sequence()
{
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);

    for (int i = 0; i < level; i++)
    {
        digitalWrite(sequence[i], HIGH);
        tone(SPEAKER, sound[i]);
        delay(velocity);
        digitalWrite(sequence[i], LOW);
        noTone(SPEAKER);
        delay(200);
    }
}

void get_sequence()
{
    int flag = 0; // flag correct sequence

    for (int i = 0; i < level; i++)
    {
        flag = 0;

        while (flag == 0)
        {

            if (digitalRead(S1) == LOW)
            {
                digitalWrite(LED1, HIGH);
                tone(SPEAKER, 261); // Do
                delay(velocity);
                noTone(SPEAKER);
                gamer_sequence[i] = 13;
                flag = 1;
                delay(200);

                if (gamer_sequence[i] != sequence[i])
                {
                    wrong_sequence();
                    return;
                }
                digitalWrite(LED1, LOW);
            }

            if (digitalRead(S2) == LOW)
            {
                digitalWrite(LED2, HIGH);
                tone(SPEAKER, 293); // Re
                delay(velocity);
                noTone(SPEAKER);
                gamer_sequence[i] = 12;
                flag = 1;
                delay(200);

                if (gamer_sequence[i] != sequence[i])
                {
                    wrong_sequence();
                    return;
                }
                digitalWrite(LED2, LOW);
            }

            if (digitalRead(S3) == LOW)
            {
                digitalWrite(LED3, HIGH);
                tone(SPEAKER, 329); // Mi
                delay(velocity);
                noTone(SPEAKER);
                gamer_sequence[i] = 11;
                flag = 1;
                delay(200);

                if (gamer_sequence[i] != sequence[i])
                {
                    wrong_sequence();
                    return;
                }
                digitalWrite(LED3, LOW);
            }

            if (digitalRead(S4) == LOW)
            {
                digitalWrite(LED4, HIGH);
                tone(SPEAKER, 349); // Fa
                delay(velocity);
                noTone(SPEAKER);
                gamer_sequence[i] = 10;
                flag = 1;
                delay(200);

                if (gamer_sequence[i] != sequence[i])
                {
                    wrong_sequence();
                    return;
                }
                digitalWrite(LED4, LOW);
            }
        }
    }
    right_sequence();
}

void right_sequence()
{
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(250);

    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(500);

    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(500);

    if (level == MAX_LEVEL)
    {
        lcd.setCursor(0, 0);
        lcd.print("Gratulerer");
        lcd.setCursor(0, 1);
        lcd.print("Koden: 1548");
        running = 0;
        digitalWrite(LED_RUNNING, LOW);
        level = 1;
        velocity = 500;
    }
    if (level < MAX_LEVEL)
    {
        level++;
    }
    velocity -= 50; // increases difficulty
}

void wrong_sequence()
{

    lcd.setCursor(0, 0);
    lcd.print("Feil !!!  "); // Notify user that the answer was wrong

    digitalWrite(LED_RUNNING, LOW);
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
        tone(SPEAKER, 233);
        delay(250);

        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        noTone(SPEAKER);
        delay(250);
    }
    running = 0;
    level = 1;
    velocity = 500;
}