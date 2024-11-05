#include <Arduino.h>

// Pini pentru LED RGB
const int redPin = 6;
const int greenPin = 5;
const int bluePin = 4;

// Pini pentru butoane
const int buttonStartPin = 3;
const int buttonDifficultyPin = 2;

// Variabile pentru debouncing la butoane
volatile unsigned long lastDifficultyPress = 0;
volatile unsigned long lastStartPress = 0;
const unsigned long debounceDelay = 50;

// Constante
const int roundTime = 30000; //  30 s timpul rundei
const int startAnimation = 3; //  3 secunde numaratoare inversa

// Array de cuvinte pentru joc
const int wordsCount = 30;
const String words[wordsCount] = {
 "cheerful", "fluffy", "gorgeous", "justice", "melody", "feather", "crystal", "honesty", "invention", "journey", "eclipse", "nature", "planet",
  "fantasy", "motion", "puzzle", "library","sky", "cat", "fog", "tree", "lake", "fish", "bird", "ship", "rock", "fire", "wave", "sun", "moon" 
};


int difficulty = -1; // -1: Unknown, 0: Easy, 1: Medium, 2: Hard

// Variabile pentru starea jocului
volatile bool difficultyButtonPressed = false;
volatile bool StartGame = false;
volatile bool StopGame = false;
volatile bool isGameStarted = false;
volatile bool ChangeWord = false;

int timePerWord = 0;

long roundTimeStart = 0; 
int Score = 0;
String currentWord = ""; 
String inputText = ""; 


/*
* Functie pentru a seta culoarea LED-ului RGB prin setarea valorilor pinilor
*/

void setColor(int redValue, int greenValue, int blueValue) {
  digitalWrite(redPin, redValue);
  digitalWrite(greenPin, greenValue);
  digitalWrite(bluePin, blueValue);
}


/*
* ISR pentru butonul de dificultate
* Modifica dificultatea daca jocul nu este pornit
* Foloseste o metoda de debouncing pentru a preveni apasarea repetata
* a butonului in intervale scurte de timp.
*/

void triggerCycleDifficulty() {

    // Verifica daca timpul scurs de la ultima apasare este mai mare decat intarzierea de debouncing
  if (millis() - lastDifficultyPress > debounceDelay) {
    lastDifficultyPress = millis();

    if (isGameStarted) {
      return;
    }

    difficultyButtonPressed = true;
  }
}

/*
* ISR pentru butonul Start/Stop
* Se activeaza atunci cand butonul de Start/Stop este apasat.
* Foloseste o metoda de debouncing pentru a preveni apasarea repetata
* a butonului in intervale scurte de timp.
*/

void triggerStartStopGame() {

    // Verifica daca timpul scurs de la ultima apasare este mai mare decat intarzierea de debouncing
  if (millis() - lastStartPress > debounceDelay) {
    lastStartPress = millis();

    if (isGameStarted) {
      StopGame = true;
    }
    else {
      StartGame = true;
    }
  }
}

/*
* Functie pentru a gestiona ciclarea dificultatii jocului
* Aceasta functie este apelata atunci cand butonul de dificultate este apasat.
* Seteaza timpul alocat pentru fiecare cuvant in functie de dificultate.
*/

void cycleDifficulty() {
  difficultyButtonPressed = false;
  difficulty = (difficulty + 1) % 3; // Easy / Medium / Hard

  switch (difficulty) {
    case 0:
      Serial.println("Easy mode on!");
      timePerWord = 4;
      break;
    case 1:
      Serial.println("Medium mode on!");
      timePerWord = 3;
      break;
    case 2:
      Serial.println("Hard mode on!");
      timePerWord = 2;
      break;
    default:
      Serial.println("Invalid difficulty. Please try again!");
      difficulty = -1;
      timePerWord = 0;
      break;
  }
}

/*
*  Functie pentru a porni jocul, cu o numaratoare inversa si configurarea temporizatorului
*/

void startGame() {

  StartGame = false;
  if (difficulty == -1) {
    Serial.println("Please choose difficulty.");
    return;
  }

  Score = 0;
  inputText = "";

  // Numaratoarea inversa inainte de a incepe jocul
  Serial.println("Game starting in...");
  for (int i = startAnimation; i > 0; i--) {
    Serial.println(i);
    setColor(HIGH, HIGH, HIGH);
    delay(500);
    setColor(LOW, LOW, LOW);
    delay(500);
  }
  Serial.println("GO!");

  setColor(LOW, HIGH, LOW); // green LED
  noInterrupts();

  // Configurare temporizator 1 pentru schimbarea cuvintelor la intervale, in functie de dificultate
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  const unsigned long clock = 16000000; // 16 MHz 
  const unsigned long prescaler = 1024;
  const unsigned long timeCompareTarget = (clock / prescaler) * timePerWord - 1;

  OCR1A = timeCompareTarget;

  TCCR1B |= (1 << WGM12); // Mod CTC
  TCCR1B |= (1 << CS12) | (1 << CS10); // Seteaza prescaler-ul la 1024
  TIMSK1 |= (1 << OCIE1A); // Activeaza intreruperea pentru comparare

  interrupts();

  roundTimeStart = millis();
  isGameStarted = true;
  ChangeWord = true;
}

/*
* Intrerupere pentru temporizator pentru a schimba cuvantul cand expira timpul per cuvant
*/

ISR(TIMER1_COMPA_vect) {
  ChangeWord = true;
}


/*
* Functie pentru a schimba cuvantul cu unul nou, ales aleator
*/

void changeWord() {
  ChangeWord = false;
  TCNT1 = 0; // Reseteaza timer

 // Alege un cuvant aleator
  int index = random(0, wordsCount);
  currentWord = words[index];

  inputText = ""; // Reseteaza inputul utilizatorului
  Serial.print("\n\n");
  Serial.println(currentWord);
}

/*
*  Functie pentru a reseta jocul la starea inactiva
*/
void resetState() {

  // led inactiv
  setColor(HIGH, HIGH, HIGH);
  difficulty = -1;
  timePerWord = 0;
  difficultyButtonPressed = false;
  StartGame = false;
  StopGame = false;
  isGameStarted = false;
  Score = 0;
  ChangeWord = false;
  currentWord = "";
  inputText = "";

  /// dezactivam temporizatorul
  noInterrupts();

  // Resetare Timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 0;

  TCCR1B &= ~(1 << WGM12);   ///dezactiveaza modul CTC
  TCCR1B &= ~(1 << CS12) & ~(1 << CS10);  /// reseteaza prescalerul la 0
  TIMSK1 &= ~(1 << OCIE1A);  ///dezactiveaza intreruperea 

  interrupts(); /// reactiveaza intreruperile dupa toate modificarile

  Serial.println("You must choose the difficulty before you can start the game.");
}

// Stops the game
void stopGame() {
  Serial.println("\n\nTime's up!");
  Serial.print("\nScore: ");
  Serial.println(Score);
  Serial.println("Game stopped!");

  resetState();
}

// Initial load
void setup() {
  Serial.begin(115200);

  // Sursa pentru generare numere aleatoare
  randomSeed(analogRead(0)); 

  // Setare pini led
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Setare pini butoane
  pinMode(buttonStartPin, INPUT_PULLUP);
  pinMode(buttonDifficultyPin, INPUT_PULLUP);

  // configureaza intreruperi pentru cele 2 butoane
  attachInterrupt(digitalPinToInterrupt(buttonDifficultyPin), triggerCycleDifficulty, LOW);
  attachInterrupt(digitalPinToInterrupt(buttonStartPin), triggerStartStopGame, LOW);

  resetState();
}

void loop() {

  // Verifica daca butonul de dificultate a fost apasat
  if (difficultyButtonPressed) {
    cycleDifficulty(); // Schimba dificultatea jocului
  }

  // Verifica daca butonul de start a fost apasat pentru a porni jocul
  if (StartGame) {
    startGame(); // Porneste jocul
  }

  // Verifica daca jocul trebuie oprit
  if (StopGame) {
    stopGame(); // Opreste jocul si afiseaza scorul final
  }

  // Executa codul principal al jocului daca acesta este pornit
  if (isGameStarted) {
    
    // Verifica daca trebuie schimbat cuvantul
    if (ChangeWord) {
      changeWord(); // Alege un nou cuvant pentru joc
    }

    // Verifica daca timpul rundei a expirat si daca jocul nu a fost oprit deja
    if ((millis() - roundTimeStart >= roundTime) && !StopGame) {
      StopGame = true; // Opreste jocul cand timpul rundei a expirat
    }


    /// setari pentru input
    ///
    ///

    // Verifica daca utilizatorul a introdus ceva in consola seriala
    if (Serial.available()) 
    {
      char c = Serial.read(); // Citeste caracterul introdus

      // Daca utilizatorul apasa backspace
      if (c == '\b') { 
        if (inputText.length() > 0) {
          inputText.remove(inputText.length() - 1); // Sterge ultimul caracter din inputText (bufferul de text al utilizatorului)

          String clearText = "";
          clearText += "\r"; // Muta cursorul la inceputul liniei
          for (unsigned int i = 0; i <= inputText.length(); i++) {
            clearText += " "; // Adaugam spatii in functie de lungimea actuala a textului de intrare

          }
          clearText += "\r"; // Muta cursorul inapoi la inceputul liniei

          Serial.print(clearText); // Afiseaza linia curatata in terminal
          Serial.print(inputText); // Afiseaza inputul actualizat dupa stergere
        }
      }

      // Daca caracterul citit este un nou rand
      else if (c == '\n') { 
        inputText = ""; // inputul utilizatorului devine vid
      }

      // Daca este un caracter normal, il adaugam la input
      else { 
        inputText += c; 
      }

      // Verifica daca inputul utilizatorului corespunde inceputului cuvantului actual
      if (currentWord.startsWith(inputText)) { 
        setColor(LOW, HIGH, LOW); // Aprinde LED-ul verde pentru a arata ca inputul este corect
      }
      else { 
        setColor(HIGH, LOW, LOW); // Aprinde LED-ul rosu pentru a arata ca utilizatorul trebuie sa corecteze inputul
      }

      // Verifica daca utilizatorul a introdus cuvantul complet si corect
      if (currentWord.equals(inputText)) { 
        Score++; // Incrementeaza scorul
        ChangeWord = true; // Solicita un nou cuvant
      }
    }
  }
}
