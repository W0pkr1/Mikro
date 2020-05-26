#include<Keypad.h> // Keypad Libary
#include<LiquidCrystal.h> // LCD libary
#include<Servo.h>// Servo libary

LiquidCrystal lcd(A0,A1,A2,A3,A4,A5); // LCD bemenetek

char keys[4][4]={ // Keypad elrendezése 4x4-s mátrixban
 {'1','2','3','A'},
 {'4','5','6', 'B'},
 {'7','8','9', 'C'},
 {'*','0','#', 'D'}};
 
byte rowPin[4]={13,12,11,10}; // Sor bemenetek
byte colPin[4]={9,8,7,6}; // Oszlop bemenetek

Servo servo_Motor; 
String key[4];  //4 elemű string létrehozása
char password[4];  // password tömb 
int initial = 0; // password megadásához változó
int position = 0; // változó a pozició meghatározására
int wrong = 0; // változó a rossz beütésekre
int total = 0; // változó a totális beütésekre
 
Keypad keypad=Keypad(makeKeymap(keys),rowPin,colPin,4,4); // a keypad megadása


void setLocked(int locked)// Függvény a servo motor zárására 
  {
    if (locked)
      {
          servo_Motor.attach(4);
          servo_Motor.write(10);
          delay(1000);
      }
    else
      {
          servo_Motor.attach(4);
          servo_Motor.write(90);
          delay(1000);
      }
  }



void setup()
{
 
lcd.begin(16,1); // Az LCD pixelek megadása 
lcd.setCursor(0,0);
lcd.print("Security Lock"); // Security Lock kiírása
delay(2000);  // 2 ms-es várás
lcd.clear();  // clearelés

}

void loop()
{
  char pressed=keypad.getKey();// Input bekérése a Keymapből


  if(initial<4){  //elsőnek bekérjük a jelszót amit a password tömb fog tárolni
    lcd.setCursor(0,0);
    lcd.print("Type password");
    if(pressed)  //ha gombnyomás történik
      {
        password[position]=pressed;  //a passwordben tárolódik az adott lenyomott gomb és a position ugrik egyet.
        position++;
        initial++;
        lcd.setCursor(position,1);
        lcd.print('*');  //csillaggal kiíratjuk a beütött jegyet
      }
    
  }
  
  if(initial == 4){  // amint bevittük a passwordot a rendszer bezár
    setLocked(true);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter Password:");
    position=0;
  }
 
  
 if(pressed && initial==4) // ha valamilyen gombot lenyomunk mikor már bevittük a jelszót akkor elkezdődik a móka
 {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Password:"); // és kicsillagozzuk a helyét 
  lcd.setCursor(position,1);
  lcd.print('*');
  delay(500);
  
     if (pressed =="#"){ // ha lenyomjuk a hastaget akkor írja ki a kulcsot amit eddig beütöttünk
      lcd.clear();
        
        for(int i =0;i<=position;i++){
          lcd.setCursor(i,1);
          lcd.print(key[i]);  // key követi, hogy eddig mit nyomtunk le tehát ez kiírja az eddigi beütött számainkat
        }
        
        delay(3000);  //3 milisec után eltünteti azokat a képernyőről
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Enter Password:");
        
        for(int k=0;k<=position;k++){  //és újra kiírja a csillagokat 
        lcd.setCursor(k,1);
        lcd.print('*');
        }
    }
    else if(pressed == password[position])  //ha helyesen vittünk be egy számot
      {
          key[position]=pressed;   //azt tároljuk a key-ben hogy tudjuk ábrázolni ha szükséges 
          position++;   // és a pozicióhoz hozzá adunk egyet 
      }
 
    else if (pressed != password[position] )  //Ha rossz számot ütünk be 
      {
          key[position]=pressed;   // azt tároljuk a key-ben szintén
          wrong++;                 // megnöveljük a wrongot 
           
          position ++;             // és növelünk a pozición
      }

    if(position == 4)              // ha már bevittünk 4 számot akkor  akövetkezőket vizsgáljuk
      {
          if( wrong >0)            // ha van 1 beütött rossz karakterünk
          
            {
                total++;           // megnöveljük a rossz próbálkozások számát
                wrong = 0;         //nullázuk a rossz számokat jelölő változót
                position = 0;      // nullázuk a poziciót
                lcd.clear();
                lcd.print("WRONG"); //kiírjuk, hogy rossz a  jelszó
                lcd.setCursor(5,1);
                lcd.print("PASSWORD");
                delay(1000);
                setLocked(true);   //zárva marad a lock
            }

          else if(position == 4 && wrong == 0)  //ha minden karakterünk helyes
            {
              
                position = 0;
                wrong = 0;
                lcd.clear();
                lcd.print("PASSWORD");
                lcd.setCursor(6,1);
                lcd.print("ACCEPTED");
                delay(2000);
                lcd.clear();
                lcd.print("Door Open");
                delay(2000);
                setLocked(false);  //kinyitjuk a zárat 3 percre
                
                for(int j=0;j<180;j++){ //ameddig a 3 perc tart ha lenyomjuk a csillagot 
                  delay(1000);
                  if(pressed=="*"){ // új kódot adhatunk meg
                    initial=0;
                    break;
                  }
                  
                  if(j== 180){   // más esetben visszazár
                    setLocked(true);
                  }
                }
                
            }

          if(total ==3) // ha elszúrtuk 3x akkor a rendszer kizár minket 5 percre.
            {
                position=0;
                total=0;
                lcd.print("LOCKED OUT");
                delay(300000);
            }

        }

   }

   
}


