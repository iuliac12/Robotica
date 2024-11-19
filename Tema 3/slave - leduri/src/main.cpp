#include <Arduino.h>
#include <SPI.h>

/* Pin buton analogic jucator 1 */
const int BUTTON_PIN_PLAYER1 = A0;

/* Pin buton analogic jucator 2 */
const int BUTTON_PIN_PLAYER2 = A1;

/* Pin pentru buzzer */
const int BUZZER_PIN = 9;

/* Pin pentru controlul intensitatii LED-urilor */
const int LEDS_INTENSITY_PIN = 3;

/* LED-uri RGB jucator 1 */
const int RGB_RED_PLAYER1 = 5;
const int RGB_GREEN_PLAYER1 = 4;
const int RGB_BLUE_PLAYER1 = 2;

/* LED-uri RGB jucator 2 */
const int RGB_RED_PLAYER2 = 8;
const int RGB_GREEN_PLAYER2 = 7;
const int RGB_BLUE_PLAYER2 = 6;

/* Toleranta pentru citirea butoanelor analogice */
const int TOLERANCE = 50;

/* Valorile pe care trebuie sa le aiba butoanele */
const int BUTTON_GREEN_PLAYER1 = 208;
const int BUTTON_BLUE_PLAYER1 = 405;
const int BUTTON_RED_PLAYER1 = 967;

const int BUTTON_GREEN_PLAYER2 = 214;
const int BUTTON_BLUE_PLAYER2 = 409;
const int BUTTON_RED_PLAYER2 = 976;

/* Timpuri de reactie pentru scoruri */
const unsigned long FAST_RESPONSE = 300;
const unsigned long MODERATE_RESPONSE = 600;
const unsigned long SLOW_RESPONSE = 1000;

/* Durata sunetului pentru raspuns corect */
const unsigned int CORRECT_RESPONSE_TONE_DURATION = 250;

/* Frecventa sunetului pentru raspuns corect */
const unsigned int CORRECT_RESPONSE_TONE_FREQUENCY = 1000;

/* Intensitatea LED-urilor */
const int LED_INTENSITY = 50;

/* Comanda primita prin rgb */
volatile char command = '\0';

/* Pentru a nu procesa aceeasi comanda de mai multe ori */
volatile bool newCommand = false;

/* Momentul in care a pornit timer-ul pentru apasarea butonului */
unsigned long pressStartTime = 0;

/* Ne spune daca sa asteptam raspunsul jucatorului */
bool waitingFlag = false;
bool player1Turn = true;

/* Verifica daca butonul specificat este apasat. */
bool isButtonPressed(int pin, int targetValue)
{
  int reading = analogRead(pin);
  return (reading >= targetValue - TOLERANCE && reading <= targetValue + TOLERANCE);
}

/* Reseteaza RGB-ul */
void RGBReset()
{
  digitalWrite(RGB_RED_PLAYER1, LOW);
  digitalWrite(RGB_GREEN_PLAYER1, LOW);
  digitalWrite(RGB_BLUE_PLAYER1, LOW);
  digitalWrite(RGB_RED_PLAYER2, LOW);
  digitalWrite(RGB_GREEN_PLAYER2, LOW);
  digitalWrite(RGB_BLUE_PLAYER2, LOW);
}

/* Activeaza un LED specific in functie de culoare si jucator. */
void LedActivate(char color)
{
  RGBReset();

  int ledPin;
  if (player1Turn)
  {
    ledPin = (color == 'r') ? RGB_RED_PLAYER1 : (color == 'g') ? RGB_GREEN_PLAYER1
                                                               : RGB_BLUE_PLAYER1;
  }
  else
  {
    ledPin = (color == 'r') ? RGB_RED_PLAYER2 : (color == 'g') ? RGB_GREEN_PLAYER2
                                                               : RGB_BLUE_PLAYER2;
  }
  digitalWrite(ledPin, HIGH);
}

/* Functia care asteapta apasarea butonului in functie de culoarea afisata pe rgb */
void startButton(char color)
{
  pressStartTime = millis();
  waitingFlag = true;
  LedActivate(color); // Activează LED-ul corespunzator
}

/* Verifica daca butonul apasat este corect*/
void ButtonCheck()
{
  /* Calculeaza cat timp i-a luat jucatorului sa apese un buton */
  unsigned long elapsedTime = millis() - pressStartTime;

  /* Selecteaza valorile pentru fiecare buton */
  int targetValueGreen = player1Turn ? BUTTON_GREEN_PLAYER1 : BUTTON_GREEN_PLAYER2;
  int targetValueBlue = player1Turn ? BUTTON_BLUE_PLAYER1 : BUTTON_BLUE_PLAYER2;
  int targetValueRed = player1Turn ? BUTTON_RED_PLAYER1 : BUTTON_RED_PLAYER2;

  /* Selecteaza butonul pentru jucatorul curent */
  int currentButtonPin = player1Turn ? BUTTON_PIN_PLAYER1 : BUTTON_PIN_PLAYER2;

  /* Ne indica daca a fost apasat butonul corect */
  bool correctPress = false;

  if (command == 'r' && isButtonPressed(currentButtonPin, targetValueRed))
  {
    correctPress = true;
  }
  else if (command == 'g' && isButtonPressed(currentButtonPin, targetValueGreen))
  {
    correctPress = true;
  }
  else if (command == 'b' && isButtonPressed(currentButtonPin, targetValueBlue))
  {
    correctPress = true;
  }

  if (correctPress)
  {
    waitingFlag = false;
    char score;

    if (elapsedTime <= FAST_RESPONSE)
    {
      score = 'b';
    }
    else if (elapsedTime <= MODERATE_RESPONSE)
    {
      score = 'u';
    }
    else if (elapsedTime <= SLOW_RESPONSE)
    {
      score = 'n';
    }
    else
    {
      score = 'x';
    }

    SPDR = score;

    if (score != 'x')
    {
      tone(BUZZER_PIN, CORRECT_RESPONSE_TONE_FREQUENCY, CORRECT_RESPONSE_TONE_DURATION);
    }

    RGBReset();
    player1Turn = !player1Turn;
  }
  else if (elapsedTime > SLOW_RESPONSE)
  {
    waitingFlag = false;
    SPDR = 'x';
    RGBReset();
    player1Turn = !player1Turn;
  }
}

void setup()
{
  Serial.begin(115200);
  SPCR |= bit(SPE);
  pinMode(MISO, OUTPUT);
  SPI.attachInterrupt();

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(LEDS_INTENSITY_PIN, OUTPUT);
  pinMode(RGB_RED_PLAYER1, OUTPUT);
  pinMode(RGB_GREEN_PLAYER1, OUTPUT);
  pinMode(RGB_BLUE_PLAYER1, OUTPUT);
  pinMode(RGB_RED_PLAYER2, OUTPUT);
  pinMode(RGB_GREEN_PLAYER2, OUTPUT);
  pinMode(RGB_BLUE_PLAYER2, OUTPUT);

  analogWrite(LEDS_INTENSITY_PIN, LED_INTENSITY);
  RGBReset();
}

/* Intrerupere SPI pentru a receptiona o comanda de la master. */
ISR(SPI_STC_vect)
{
  char receivedChar = SPDR;
  if (receivedChar != '#')
  {
    command = receivedChar;
    newCommand = true;
  }
}

/* Bucla principala care verifica raspunsurilor. */
void loop()
{
  analogWrite(LEDS_INTENSITY_PIN, LED_INTENSITY);

  if (newCommand)
  {
    newCommand = false;

    if (command == 'r' || command == 'g' || command == 'b')
    {
      startButton(command);
    }
    else
    {
      SPDR = '$';
    }
  }

  if (waitingFlag)
  {
    ButtonCheck();
  }
}
