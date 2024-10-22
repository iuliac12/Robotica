# Tema 1: EV Charger
În cadrul acestui proiect, vom simula funcționarea unei stații de încărcare pentru un vehicul electric, utilizând mai multe LED-uri și butoane pentru a reflecta diferitele etape ale procesului de încărcare. Sistemul va gestiona interacțiunile cu utilizatorul prin intermediul butoanelor, implementând mecanisme de debouncing pentru a asigura fiabilitatea în detectarea stărilor acestora.

<details>
  <summary><b>Descriere</b></summary>

  ## 1. Detalii tehnice:
  Led-ul RGB reprezintă disponibilitatea stației. Dacă stația este liberă led-ul va fi verde, iar dacă stația este ocupată se va face roșu.

  Led-urile simple reprezintă gradul de încărcare al bateriei, pe care îl vom simula printr-un loader progresiv (L1 = 25%, L2 = 50%, L3 = 75%, L4 = 100%). Loader-ul se încarcă prin aprinderea succesivă a led-urilor, la un interval fix de 3s. LED-ul care semnifică procentul curent de încărcare va avea starea de clipire, LED-urile din urma lui fiind aprinse continuu, iar celelalte stinse.

  Apăsarea scurtă a butonului de start va porni încărcarea. Apăsarea acestui buton în timpul încărcării nu va face nimic.

  Apăsarea lungă a butonului de stop va opri încărcarea forțat și va reseta stația la starea liberă. Apăsarea acestui buton cât timp stația este liberă nu va face nimic.

  ## 2. Flow:
  Starea stației este ‘liberă’. Loader-ul este stins, iar led-ul pentru disponibilitate este verde.

  Se apasă butonul pentru start.

  Led-ul pentru disponibilitate se face roșu, iar încărcarea începe prin aprinderea primului LED L1.

  Led-ul 1 clipește timp de 3s, celelalte fiind stinse.

  După încărcarea primului procent de 25% led-ul rămâne aprins și se trece la următorul led, care va începe să clipească.

  La finalizarea încărcării toate led-urile vor clipi simultan de 3 ori, iar apoi se vor stinge, pentru a semnaliza finalizarea procesului.

  Led-ul pentru disponibilitate se face verde.

  Dacă oricând de la pornirea încărcării până la finalizarea acesteia este apăsat lung (min 1s) butonul de stop, încărcarea se întrerupe prin animația de final (toate led-urile clipesc de 3 ori), iar led-ul pentru disponibilitate devine verde.

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

  ## Schema electrica a circuitului
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
  
</details>
