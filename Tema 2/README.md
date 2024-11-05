# Tema 2: TypeRacer Game
Proiectul constă în dezvoltarea unui joc de tastare rapidă, similar cu TypeRacer, unde utilizatorii concurează pentru a tasta corect și rapid textele afișate. Sistemul urmărește viteza și acuratețea tastării, oferind feedback și rezultate pentru performanța jucătorilor.

<details>
  <summary><b>Descriere</b></summary>

  ## 1. Detalii tehnice:

  - LED RGB: În starea de repaus, LED-ul este alb. La începutul rundei, clipește 3 secunde pentru numărătoare inversă, apoi devine verde când utilizatorul tastează corect și roșu la greșeli.
  - Buton Start/Stop: În modul repaus, inițiază o nouă rundă după o numărătoare inversă de 3 secunde. În timpul rundei, poate opri imediat jocul.
  - Buton Dificultate: Controlează viteza de apariție a cuvintelor. În starea de repaus, alternează între dificultățile Easy, Medium și Hard, afișând mesajul corespunzător în terminal.

  ## 2. Flow:
 
 Jocul pornește în starea de repaus, cu LED-ul RGB aprins în culoarea albă. În această stare, utilizatorul poate selecta dificultatea jocului apăsând butonul de dificultate. La fiecare apăsare, dificultatea trece la următorul nivel – Easy, Medium sau Hard – și un mesaj corespunzător (ex.: “Easy mode on!”) apare în terminal.

 Pentru a începe runda, utilizatorul apasă butonul de Start/Stop. LED-ul RGB va clipi timp de 3 secunde (numărătoarea inversă), iar în terminal se va afișa secvențial: "3", "2", "1". Cuvintele încep să apară în terminal, unul câte unul, și utilizatorul trebuie să le introducă rapid și corect.

 - Tastare corectă: LED-ul RGB rămâne verde și un nou cuvânt este afișat imediat.
 - Tastare incorectă: LED-ul RGB devine roșu, indicând eroarea. Utilizatorul poate folosi tasta BackSpace pentru a corecta cuvântul.

 Schimbarea cuvintelor: Apariția cuvintelor este controlată de dificultate. La dificultate mai mare, timpul disponibil pentru a introduce fiecare cuvânt corect este mai scurt. Dacă timpul expira și utilizatorul nu introduce corect cuvântul, un nou cuvânt va apărea automat.

 !!! Runda durează 30 de secunde. La sfârșitul acestui timp:

 - Jocul se oprește automat, LED-ul RGB revine la culoarea albă.
 - În terminal este afișat scorul rundei, adică numărul total de cuvinte introduse corect în timpul jocului.

</details>


<details>
  <summary> <b> Componente </b> </summary>

 ## Componente:
  - Arduino UNO (ATmega328P microcontroller)
  - 1x LED RGB (pentru a semnaliza dacă cuvântul corect e scris greșit sau nu)
  - 2x Butoane (pentru start/stop rundă și pentru selectarea dificultății)
  - 5x Rezistoare (3x 220/330 ohm, 2x 1000 ohm)
  - Breadbord
  - Fire de legătură
  
</details>


<details>
  <summary> <b> Schema electrica </b> </summary>

  ## Schema electrica a circuitului in TinkerCAD 
  ![setup2](https://github.com/user-attachments/assets/c19ca2a5-95ee-439c-a3b2-e99b1dc102a4)

  
</details>


<details>
  <summary> <b> Setup </b> </summary>
  ## Poze cu montajul:
  <img src="https://github.com/user-attachments/assets/8b9ab654-dda4-43c6-96a1-b0399b3dba56" alt="setup1" width="300">
  <img src="https://github.com/user-attachments/assets/8c151673-9c18-4660-a71f-4092dd0ba063" alt="setup2" width="300">
  <img src="https://github.com/user-attachments/assets/cc3c5603-1f7b-432f-b05a-37eb7ea7e9e5" alt="setup3" width="300">


</details>


<details>
  <summary> <b> Video </b> </summary>

  ## Link:
  https://drive.google.com/file/d/1S-G5NAloQxpgbFxmg5efKcf98vzXb3nL/view?usp=drivesdk 
</details>
