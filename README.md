# Tema 1: EV Charger
În cadrul acestui proiect, vom simula funcționarea unei stații de încărcare pentru un vehicul electric, utilizând mai multe LED-uri și butoane pentru a reflecta diferitele etape ale procesului de încărcare. Sistemul va gestiona interacțiunile cu utilizatorul prin intermediul butoanelor, implementând mecanisme de debouncing pentru a asigura fiabilitatea în detectarea stărilor acestora.

<details>
  <summary><b>Descriere</b></summary>

  ## 1. Detalii tehnice:
  LED RGB: Indică starea stației.
  - Verde: Stația este liberă.
  - Roșu: Stația este ocupată.
  - 
LED-uri simple (L1, L2, L3, L4): Semnalizează nivelul de încărcare al bateriei.
  - L1: 25%
  - L2: 50%
  - L3: 75%
  - L4: 100%
  - 
Funcționare LED-uri:
  - LED-urile se activează la un interval de 3 secunde.
  - LED-ul corespunzător nivelului curent de încărcare clipește, în timp ce cele anterioare rămân aprinse.
  - 
Buton de control:
  - Start: Pornește încărcarea. Dacă este apăsat în timpul încărcării, nu are efect.
  - Stop: Oprește încărcarea forțat și resetează stația. Nu are efect dacă stația este liberă.

  ## 2. Flow:
  La început, stația se află în starea „liberă”, cu LED-ul verde activat și loader-ul stins. Când utilizatorul apasă butonul de start, LED-ul de disponibilitate devine roșu, semnalizând că stația este ocupată, iar încărcarea începe prin aprinderea LED-ului L1, care va clipește timp de 3 secunde.

După această perioadă, L1 rămâne aprins, iar L2 începe să clipească. Procesul de activare continuă până când toate LED-urile indică o încărcare de 100%. Odată ce încărcarea este completă, toate LED-urile clipesc simultan de 3 ori pentru a semnala terminarea procesului, după care se sting și LED-ul de disponibilitate revine la verde.

Dacă butonul de stop este apăsat lung în timpul încărcării, aceasta se va opri imediat, activând animația de final (toate LED-urile clipesc de 3 ori) și resetând stația la starea „liberă”, cu LED-ul verde. Apăsarea butonului de stop când stația este liberă nu va produce efecte.


</details>


<details>
  <summary> <b> Componente </b> </summary>

 ## Componente:
  - 4x LED-uri (pentru a simula procentul de încărcare)
  - 1x LED RGB (pentru starea de liber sau ocupat)
  - 2x Butoane (pentru start încărcare și stop încărcare)
  - 9x Rezistoare (7x 220ohm, 2x 1K)
  - Breadboard
  - Linii de legătură
  
</details>


<details>
  <summary> <b> Schema electrica </b> </summary>

  ## Schema electrica a circuitului in TinkerCAD 
  ![setup](https://github.com/user-attachments/assets/9e0892b0-428a-4b1a-aafd-666db2830bd4)

  
</details>


<details>
  <summary> <b> Setup </b> </summary>
  
  ## Poze cu montajul:
<img src="https://github.com/user-attachments/assets/b3647c80-f51f-4903-9506-825c08fea1bb" alt="setup1" width="300">
<img src="https://github.com/user-attachments/assets/ac2cfd8e-2514-4446-813e-46794dd36e47" alt="setup2" width="300">
<img src="https://github.com/user-attachments/assets/5ae396f1-88af-4437-8e3e-5b6afec00699" alt="setup3" width="300">


</details>


<details>
  <summary> <b> Video </b> </summary>

  ## Link:
  https://drive.google.com/file/d/1oj4YF9c2sbRw6qnyjYA-aW5heLhdKSXW/view?usp=drivesdk
</details>
