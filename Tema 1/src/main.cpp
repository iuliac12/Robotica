#include <Arduino.h>

// Pini pentru LED-uri de incarcare
const int ledLevel1 = 10;
const int ledLevel2 = 9;
const int ledLevel3 = 8;
const int ledLevel4 = 7;

// Pini pentru LED RGB 
const int ledRGB_R = 6;
const int ledRGB_G = 5;
const int ledRGB_B = 4;

// Pini pentru butoane
const int buttonStart = 3;
const int buttonStop = 2;

// Variabile de stare
bool chargingInProgress = false;
bool stationAvailable = true;
int currentChargeLevel = 0;

// Variabile pentru debounce la butonul de stop
unsigned long stopButtonPressTime = 0;
bool isStopButtonPressed = false;
const unsigned long debounceDelay = 1000; // o secunda pentru debounce

// Intervale de timp pentru schimbarea stadiilor de incarcare si clipirea LED-urilor
unsigned long lastChangeTime = 0;
unsigned long chargeLevelChangeInterval = 3000; 
unsigned long ledBlinkInterval = 500; 

// Timpi pentru clipirea independenta a fiecarui LED
unsigned long led1LastBlink = 0;
unsigned long led2LastBlink = 0;
unsigned long led3LastBlink = 0;
unsigned long led4LastBlink = 0;


/**
 * Porneste procesul de incarcare.
 * - Seteaza statia ca fiind ocupata si incepe incarcarea.
 * - Initializeaza nivelul de incarcare la 1.
 * - Trecerea LED-ului RGB la rosu pentru a indica ca statia este ocupata.
 */
void startCharging() {
    stationAvailable = false;
    chargingInProgress = true;
    currentChargeLevel = 1; // Incepe de la nivelul 1 de incarcare
    lastChangeTime = millis(); // Salveaza timpul de inceput al incarcarii

    // Statia este ocupata, RGB-ul se face rosu
    digitalWrite(ledRGB_G, LOW);
    digitalWrite(ledRGB_R, HIGH);
    digitalWrite(ledRGB_B, LOW);
}

/**
 * Reseteaza toate LED-urile de incarcare la starea initiala (oprit).
 * - Aceasta functie este apelata dupa finalizarea sau oprirea procesului de incarcare.
 */
void resetChargingLEDs() {
    digitalWrite(ledLevel1, LOW);
    digitalWrite(ledLevel2, LOW);
    digitalWrite(ledLevel3, LOW);
    digitalWrite(ledLevel4, LOW);
}


/**
 * Opreste procesul de incarcare fortat (prin apasarea butonului stop).
 * - Reseteaza nivelul de incarcare si toate LED-urile.
 * - Realizeaza o secventa de clipire a LED-urilor pentru a indica oprirea procesului.
 * - Trecerea LED-ului RGB la verde pentru a indica ca statia este disponibila.
 */
void stopCharging() {
    chargingInProgress = false;
    stationAvailable = true;
    currentChargeLevel = 0;
    resetChargingLEDs(); // Reseteaza toate LED-urile
    for (int i = 0; i < 3; i++) {
        // Clipire LED-uri pentru oprirea procesului
        digitalWrite(ledLevel1, HIGH);
        digitalWrite(ledLevel2, HIGH);
        digitalWrite(ledLevel3, HIGH);
        digitalWrite(ledLevel4, HIGH);
        delay(500);
        digitalWrite(ledLevel1, LOW);
        digitalWrite(ledLevel2, LOW);
        digitalWrite(ledLevel3, LOW);
        digitalWrite(ledLevel4, LOW);
        delay(500);
    }

    // Statia este disponibila din nou, RGB-ul se face verde
    digitalWrite(ledRGB_R, LOW);
    digitalWrite(ledRGB_G, HIGH);
    digitalWrite(ledRGB_B, LOW);
}


/**
 * Finalizeaza procesul normal de incarcare atunci cand nivelul maxim este atins.
 * - Reseteaza nivelul de incarcare si toate LED-urile.
 * - Realizeaza o secventa de clipire a LED-urilor pentru a indica finalizarea procesului.
 * - Trecerea LED-ului RGB la verde pentru a indica ca statia este disponibila.
 */
void completeCharging() {
    chargingInProgress = false;
    stationAvailable = true;
    currentChargeLevel = 0;
    resetChargingLEDs(); // Reseteaza toate LED-urile
    for (int i = 0; i < 4; i++) {
        // Clipire LED-uri pentru finalizarea procesului
        digitalWrite(ledLevel1, HIGH);
        digitalWrite(ledLevel2, HIGH);
        digitalWrite(ledLevel3, HIGH);
        digitalWrite(ledLevel4, HIGH);
        delay(500);
        digitalWrite(ledLevel1, LOW);
        digitalWrite(ledLevel2, LOW);
        digitalWrite(ledLevel3, LOW);
        digitalWrite(ledLevel4, LOW);
        delay(500);
    }

    // Statia este disponibila din nou, RGB-ul se face verde
    digitalWrite(ledRGB_R, LOW);
    digitalWrite(ledRGB_G, HIGH);
    digitalWrite(ledRGB_B, LOW);
}



/**
 * Actualizeaza LED-urile in functie de nivelul curent de incarcare.
 * - La fiecare nivel de incarcare, LED-urile clipesc intr-un mod specific.
 * - LED-urile inferioare raman aprinse odata ce nivelul respectiv este atins.
 */
void updateChargingLEDs();




// Initializarea pinilor
void setup() {
    pinMode(ledRGB_R, OUTPUT);
    pinMode(ledRGB_G, OUTPUT);
    pinMode(ledRGB_B, OUTPUT);
    pinMode(ledLevel1, OUTPUT);
    pinMode(ledLevel2, OUTPUT);
    pinMode(ledLevel3, OUTPUT);
    pinMode(ledLevel4, OUTPUT);
    pinMode(buttonStart, INPUT_PULLUP);
    pinMode(buttonStop, INPUT_PULLUP);

    // Starea initiala: RGB verde indica statie disponibila
    digitalWrite(ledRGB_G, HIGH);
    digitalWrite(ledRGB_R, LOW);
    digitalWrite(ledRGB_B, LOW);
}

void loop() {
    unsigned long currentTime = millis();

    // Verifica daca butonul de start este apasat si statia este disponibila
    if (digitalRead(buttonStart) == LOW && stationAvailable) {
        startCharging(); // Porneste procesul de incarcare
    }

    // Verifica daca butonul de stop este apasat
    if (digitalRead(buttonStop) == LOW) {
        if (!isStopButtonPressed) { // Butonul este apasat pentru prima data
            stopButtonPressTime = currentTime; // Salveaza momentul apasarii
            isStopButtonPressed = true; // Marcheaza ca butonul este apasat
        } else if (currentTime - stopButtonPressTime >= debounceDelay && chargingInProgress) {
            stopCharging(); // Opreste fortat procesul de incarcare
        }
    } else {
        isStopButtonPressed = false; // Reseteaza starea butonului dupa eliberare
    }

    // Daca incarcarea este in desfasurare, trece la urmatorul nivel de incarcare dupa intervalul definit
    if (chargingInProgress && currentTime - lastChangeTime > chargeLevelChangeInterval) {
        lastChangeTime = currentTime;
        currentChargeLevel++; // Creste nivelul curent de incarcare
        if (currentChargeLevel > 3) {
            completeCharging(); // Finalizeaza incarcarea daca nivelul maxim a fost atins
        }
    }

    // Actualizeaza LED-urile in functie de stadiul de incarcare
    if (chargingInProgress) {
        updateChargingLEDs();
    }
}




// Functia care actualizeaza LED-urile in functie de nivelul de incarcare
void updateChargingLEDs() {
    unsigned long currentTime = millis();

    if (currentChargeLevel >= 1 && currentTime - led1LastBlink >= ledBlinkInterval) {
        led1LastBlink = currentTime;
        digitalWrite(ledLevel1, !digitalRead(ledLevel1)); // Inversare stare LED
    }

    if (currentChargeLevel >= 2) {
        digitalWrite(ledLevel1, HIGH);  // LED-ul 1 ramane aprins
        if (currentTime - led2LastBlink >= ledBlinkInterval) {
            led2LastBlink = currentTime;
            digitalWrite(ledLevel2, !digitalRead(ledLevel2)); // Inversare stare LED
        }
    }

    if (currentChargeLevel >= 3) {
        digitalWrite(ledLevel1, HIGH); // LED-ul 1 ramane aprins
        digitalWrite(ledLevel2, HIGH); // LED-ul 2 ramane aprins
        if (currentTime - led3LastBlink >= ledBlinkInterval) {
            led3LastBlink = currentTime;
            digitalWrite(ledLevel3, !digitalRead(ledLevel3)); // Inversare stare LED
        }
    }

    if (currentChargeLevel >= 4) {
        digitalWrite(ledLevel1, HIGH); // LED-ul 1 ramane aprins
        digitalWrite(ledLevel2, HIGH); // LED-ul 2 ramane aprins
        digitalWrite(ledLevel3, HIGH); // LED-ul 3 ramane aprins
        if (currentTime - led4LastBlink >= ledBlinkInterval) {
            led4LastBlink = currentTime;
            digitalWrite(ledLevel4, !digitalRead(ledLevel4)); // Inversare stare LED
        }
    }
}
