# Tema 3: Quick Time
Proiectul constă în crearea unui joc de reflexe destinat a doi jucători, în care fiecare jucător trebuie să apese rapid butonul asociat culorii afișate pe LED-ul RGB al echipei sale. Jocul include mai multe runde, iar punctajele sunt afișate pe un ecran LCD. La sfârșitul jocului, este declarat câștigător jucătorul cu cel mai mare punctaj.

<details>
  <summary><b>Descriere</b></summary>

  ## 1. Hardware:

Două plăci Arduino Uno: Una este configurată ca master și cealaltă ca slave, comunicând prin protocol SPI.

LED-uri și Butoane:
  - Fiecare jucător dispune de 3 LED-uri colorate (roșu, verde, albastru) și 3 butoane asociate
  - Un LED RGB indică culoarea activă pentru runda curentă.
    
Ecran LCD:
  - Afișează punctajele jucătorilor în timp real.
  - Mesaje personalizate, precum starea jocului și rezultatele finale.
    
Servomotor:
  - Indică progresul jocului, rotindu-se pentru a semnala sfârșitul timpului alocat.
    
Buzzer (optional):
  - Sunete pentru răspunsuri corecte/greșite, începutul și finalul jocului.

  ## 2. Flow:
 
 Jocul începe cu afișarea unui mesaj de bun venit pe ecranul LCD, iar jucătorii pot iniția partida apăsând un buton. La fiecare rundă, LED-ul RGB al jucătorului activ indică o culoare aleatorie, iar acesta trebuie să apese rapid butonul asociat culorii respective pentru a acumula puncte, afișate în timp real pe LCD. 
 
 Un răspuns corect crește scorul în funcție de viteza reacției, iar unul greșit nu modifică punctajul. Rândurile jucătorilor alternează până la finalizarea jocului, marcată de o rotație completă a servomotorului. 
 
 Jocul se încheie prin afișarea scorurilor finale și a câștigătorului pe LCD, după care revine la starea inițială.

  ## 3. Detalii tehnice:

Arduino Master
 - Controlează LCD-ul, servomotorul și logica jocului.
 - Menține punctajul și decide LED-ul RGB care trebuie aprins.

Arduino Slave
 - Controlează butoanele și LED-urile.
 - Comunică prin SPI cu master-ul pentru a primi culoarea activă și a raporta apăsările butoanelor.

 Elemente Opționale
 - Personalizare: introducerea numelui jucătorilor prin USART sau joystick-uri.
 - Animații și Sunete: animații pe LCD sau LED-uri pentru începutul jocului / buzzer pentru feedback auditiv.
 - Dificultate: posibilitatea de a ajusta durata rundelor sau viteza de apariție a culorilor.   

</details>


<details>
  <summary> <b> Componente </b> </summary>

 ## Componente:
  - 6x LED-uri (2 grupuri de câte 3 leduri, în cadrul unui grup trebuie să avem culori diferite)
  - 2x LED RGB (1 pentru fiecare jucător)
  - 7x butoane (3 pentru fiecare jucător, 1 de start)
  - 1x LCD
  - 1x servomotor
  - 2x Breadboard
  - Fire de legatura
  - 2x Arduino Uno
  - Buzzer
  
</details>


<details>
  <summary> <b> Schema electrica </b> </summary>

  ## Schema electrica a circuitului in TinkerCAD 
  ![setup3](https://github.com/user-attachments/assets/cb5a29b7-58c8-46e0-9ccb-ad71499b481b)

</details>


<details>
  <summary> <b> Setup </b> </summary>
  <img src="https://github.com/user-attachments/assets/76587e1a-dc3d-444b-8b4b-6c72f263698b" alt="setup1" width="300">
  <img src="https://github.com/user-attachments/assets/28b3cf5e-dd8a-4c1e-9ae1-aacf19f24d9f" alt="setup2" width="300">
  <img src="https://github.com/user-attachments/assets/6865ca37-e844-417c-9f58-0112646ac3be" alt="setup3" width="500">


</details>


<details>
  <summary> <b> Video </b> </summary>
  
  ## Link:
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/fg7UXB8JGXM/0.jpg)](https://www.youtube.com/watch?v=fg7UXB8JGXM)

</details>
