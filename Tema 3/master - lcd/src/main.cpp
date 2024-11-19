#include <LiquidCrystal.h>
#include <Wire.h>
#include <Servo.h>
#include <SPI.h>

void sendColor();
char sendCommand(char command);
void updateScore(char rating);
void displayWinner();
void resetGame();
String getPlayerName();

/* Numarul de coloane al LCD-ului */
const int LCD_COLUMNS = 16;

/* Numarul de randuri al LCD-ului */
const int LCD_ROWS = 2;

/* Unghi maxim servo */
const int SERVO_MAX_ANGLE = 180;

/* Durata jocului in milisecunde */
const unsigned long GAME_DURATION_MS = 30000;

/* Intervalul dintre runde */
const unsigned long ROUND_INTERVAL_MS = 1000;

/* Intarzierea pentru debounce */
const unsigned long DEBOUNCE_DELAY_MS = 250;

/* Numarul total de runde */
const int TOTAL_ROUNDS = 30;

/* Pin pentru buton */
const int BUTTON_PIN = 7;

/* Pin pentru SPI */
const int SERVO_PIN = 9;

/* Pin RS pentru LCD */
const int LCD_RS_PIN = 8;

/* Pin EN pentru LED */
const int LCD_EN_PIN = 6;

/* Pin D4 pentru LED */
const int LCD_D4_PIN = 5;

/* Pin D5 pentru LED */
const int LCD_D5_PIN = 4;

/* Pin D6 pentru LED */
const int LCD_D6_PIN = 3;

/* Pin D7 pentru LED */
const int LCD_D7_PIN = 2;

/* Configurare LCD */
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

/* Configurare servo */
Servo ServoTimer;

/* Culori pentru rgb */
const char COLORS[] = {'r', 'g', 'b'};

/* Numarul de culori disponibile */
const int COLORS_COUNT = sizeof(COLORS) / sizeof(COLORS[0]);

/* Variabila unde se retine timpul de start al jocului */
unsigned long gameStartTime = 0;

/* Variabila cat a durat runda */
unsigned long lastRoundTime = 0;

/* Variabila care stocheaza daca a inceput jocul */
bool gameStarted = false;

/* Variabila folosita pentru a arata ca asteapta raspuns de la slave */
bool waitingTime = false;

/* Stocheaza raspunsul primit de la SPI */
char response;

/*
Stocheaza informatiile despre player
*/
String player1, player2;
int player1Score = 0;
int player2Score = 0;
int counterRound = 0;
bool isPlayer1Turn = true;

void setup()
{
  Serial.begin(115200);
  SPI.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  ServoTimer.attach(SERVO_PIN);

  lcd.print("Bun venit!");
  lcd.setCursor(0, 1);
  lcd.print("Grije la fire");
}

void loop()
{
  if (!gameStarted && digitalRead(BUTTON_PIN) == LOW)
  {
    /* Debounce pentru buton */
    delay(DEBOUNCE_DELAY_MS);
    gameStarted = true;
    lcd.clear();
    lcd.print("Introdu Player 1:");
    player1 = getPlayerName();

    lcd.clear();
    lcd.print("Introdu Player 2:");
    player2 = getPlayerName();

    gameStartTime = millis();
  }

  if (gameStarted)
  {
    unsigned long currentMillis = millis();

    /* Verificam daca timpul jocului s-a scurs */
    if (currentMillis - gameStartTime >= GAME_DURATION_MS)
    {
      gameStarted = false;
      displayWinner();
      resetGame();
      return;
    }

    /* Actualizam pozitia servo-ului in functie de timpul scurs */
    int angle = (currentMillis - gameStartTime) * SERVO_MAX_ANGLE / GAME_DURATION_MS;
    ServoTimer.write(angle);

    /* Rulam rundele la fiecare interval prestabilit */
    if (currentMillis - lastRoundTime >= ROUND_INTERVAL_MS)
    {
      if (counterRound < TOTAL_ROUNDS)
      {
        sendColor();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(player1 + ": " + String(player1Score));
        lcd.setCursor(0, 1);
        lcd.print(player2 + ": " + String(player2Score));
        delay(ROUND_INTERVAL_MS);
        waitingTime = true;
        lastRoundTime = currentMillis;
        isPlayer1Turn = !isPlayer1Turn;
        counterRound++;
      }
    }

    /* Asteptam sa primim raspuns */
    if (waitingTime)
    {
      /* Trimite o comanda pentru a primii raspuns */
      response = sendCommand('#');
      updateScore(response);
      waitingTime = false;
    }
  }
}

/* Trimite o culoare aleatorie catre slave prin SPI. */
void sendColor()
{
  char colorCommand = COLORS[random(0, COLORS_COUNT)];
  sendCommand(colorCommand);
}

/* Actualizam scorul jucatorului in functie de raspunsul primit */
void updateScore(char rating)
{
  int points = 0;
  switch (rating)
  {
  case 'b':
    points = 3;
    break;
  case 'u':
    points = 2;
    break;
  case 'n':
    points = 1;
    break;
  case 'x':
    points = 0;
    break;
  }

  /* Actualieaza scorul jucatorilor */
  if (!isPlayer1Turn)
  {
    player1Score += points;
  }
  else
  {
    player2Score += points;
  }
}

/* Trimite o comanda catre slave si returneaza raspunsul. */
char sendCommand(char command)
{
  digitalWrite(SS, LOW);
  char response = SPI.transfer(command);
  digitalWrite(SS, HIGH);
  return response;
}

/* Afiseaza scorurile jucatorilor */
void displayWinner()
{
  lcd.clear();
  lcd.print(player1 + " score:");
  lcd.setCursor(0, 1);
  lcd.print(player1Score);
  delay(3000);

  lcd.clear();
  lcd.print(player2 + " score:");
  lcd.setCursor(0, 1);
  lcd.print(player2Score);
  delay(3000);

  lcd.clear();
  if (player1Score > player2Score)
  {
    lcd.print("Winner: " + player1);
    lcd.setCursor(0, 1);
    lcd.print("Loser: " + player2);
  }
  else if (player2Score > player1Score)
  {
    lcd.print("Winner: " + player2);
    lcd.setCursor(0, 1);
    lcd.print("Loser: " + player1);
  }
  else
  {
    lcd.print("It's a Draw!");
  }
  delay(2000);
}

/* Reseteaza jocul si revine la starea initiala. */
void resetGame()
{
  player1Score = 0;
  player2Score = 0;
  counterRound = 0;
  isPlayer1Turn = true;
  lcd.clear();
  lcd.print("Mai incerca!");
}

/* Functie care preia numele jucatorului din terminal si il afiseaza pe lcd. */
String getPlayerName()
{
  String playerName = "";
  lcd.setCursor(0, 1);

  while (true)
  {
    if (Serial.available() > 0)
    {
      char receivedChar = Serial.read();
      if (receivedChar == '\n')
      {
        break;
      }
      else if (receivedChar == '\b' && playerName.length() > 0)
      {
        playerName.remove(playerName.length() - 1);
        lcd.setCursor(playerName.length(), 1);
        lcd.print(' ');
        lcd.setCursor(playerName.length(), 1);
      }
      else if (isPrintable(receivedChar) && playerName.length() < LCD_COLUMNS)
      {
        playerName += receivedChar;
        lcd.print(receivedChar);
      }
    }
  }

  return playerName;
}
