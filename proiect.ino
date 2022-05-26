// notele muzicale folosite calculate pe baza formulei din bibliografie
#define NOTE_A4  440
#define NOTE_DS5  622
#define NOTE_E5  659
#define NOTE_AS5  932
#define NOTE_E6  1318
#define NOTE_AS6  1865

// valoarea de referinta
#define TRESHOLD 500

// timpul max de inactivitate dupa care se opreste pianul 20s
#define MAX_TIME 20000
 
// pinii analogi pentru fiecare fotorezistenta
int photoresistor_1 = A0;
int photoresistor_2 = A1;
int photoresistor_3 = A2;
int photoresistor_4 = A3;
int photoresistor_5 = A4;
int photoresistor_6 = A5;

// pini digitali
int laser_1 = 4;
int laser_2 = 5;
int laser_3 = 6;
int laser_4 = 7;
int laser_5 = 8;
int laser_6 = 9;
int green_led = 10;
int red_led = 11;
int button = 12;
int speaker = 13;

// var globala care descrie starea pianului
boolean isOn = false;

// ton-urile redate de speaker
int tones[6] = {NOTE_A4, NOTE_DS5, NOTE_E5, NOTE_AS5, NOTE_E6, NOTE_AS6};  

// var globale folosite la determinarea schimbarii starii pianului
int lastState = LOW;
int currentState = LOW;

// var globale folosite la masurarea timpului de la ultimul sunet scos de pian
unsigned long currentTime = 0;
unsigned long lastTime = 0;
                   
void setup () {
  Serial.begin(9600);

  pinMode(laser_1, OUTPUT);
  pinMode(laser_2, OUTPUT);
  pinMode(laser_3, OUTPUT);
  pinMode(laser_4, OUTPUT);
  pinMode(laser_5, OUTPUT);
  pinMode(laser_6, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(speaker, OUTPUT);
}

// ii spune led-ului ce culoare sa aiba
void writeLed(int red, int green) {
  digitalWrite(red_led, red);
  digitalWrite(green_led, green);
}

// activeaza laserele pianului
void activateLasers() {
  digitalWrite(laser_1, HIGH);
  digitalWrite(laser_2, HIGH);
  digitalWrite(laser_3, HIGH);
  digitalWrite(laser_4, HIGH);
  digitalWrite(laser_5, HIGH);
  digitalWrite(laser_6, HIGH);
}

// opreste laserele pianului
void stopLasers() {
  digitalWrite(laser_1, LOW);
  digitalWrite(laser_2, LOW);
  digitalWrite(laser_3, LOW);
  digitalWrite(laser_4, LOW);
  digitalWrite(laser_5, LOW);
  digitalWrite(laser_6, LOW);
}

// anunta utilizatorul doar despre schimbarile de stare ale pianului
void printPianoState() {
  if (currentState == lastState)
    return;
    
  if (isOn == true)
    Serial.println("The Piano is UP");
  else
    Serial.println("The Piano is DOWN");
}

void loop () {
  // ton-ul care se va canta
  int tonToSing = tones[0];
  // ne spune daca se apasa pe "clape"
  boolean canSing = false; 

  lastState = currentState;
  // se citeste noua stare
  currentState = digitalRead(button);

  // daca se apasa butonul se aprinde sau se stinge pianul
  if (currentState == HIGH) {
    if (isOn == false)
      isOn = true;
    else
      isOn = false;
    delay(1000);
  }

  // anuntare utilizator
  printPianoState();

  // timp-ul curent
  currentTime = millis();

  // daca a trecut suficient timp opreste pianul
  if (isOn == true && currentTime - lastTime > MAX_TIME) {
    isOn = false;
  }

  if (isOn == true) {
    // led-ul devine verde
    writeLed(0, 255);
    // se activeaza laserele
    activateLasers();
    
    // valorile citite de fotorezistente
    int value_1 = analogRead(photoresistor_1); 
    int value_2 = analogRead(photoresistor_2);
    int value_3 = analogRead(photoresistor_3);
    int value_4 = analogRead(photoresistor_4);
    int value_5 = analogRead(photoresistor_5);
    int value_6 = analogRead(photoresistor_6);
    
//    Serial.println("Photoresistor 1:");
//    Serial.println(value_1);
//    Serial.println("Photoresistor 2:");
//    Serial.println(value_2);
//    Serial.println("Photoresistor 3:");
//    Serial.println(value_3);
//    Serial.println("Photoresistor 4:");
//    Serial.println(value_4);
//    Serial.println("Photoresistor 5:");
//    Serial.println(value_5);
//    Serial.println("Photoresistor 6:");
//    Serial.println(value_6);
    
    if(value_1 <= TRESHOLD){
      canSing = true;
      tonToSing = tones[0];
     }
     
     if(value_2 <= TRESHOLD){
      canSing = true;
      tonToSing = tones[1];
    }
  
    if(value_3 <= TRESHOLD){
        canSing = true;
        tonToSing = tones[2];
     } 
  
    if(value_4 <= TRESHOLD){
        canSing = true;
        tonToSing = tones[3];
     }
  
    if(value_5 <= TRESHOLD){
        canSing = true;
        tonToSing = tones[4];
      }
  
    if(value_6 <= TRESHOLD){
        canSing = true;
        tonToSing = tones[5];
     }

    // canta pianul daca e cazul nota respectiva
     if (canSing) {
      lastTime = millis();
      tone(speaker, tonToSing);
      } else {
          noTone (speaker);
        }
        
  } else {
    // led-ul devine rosu
    writeLed(255, 0);
    // se inchid laserele
    stopLasers();
    // inchide speaker
    noTone(speaker);
    // timpul de referinta e prezentul
    lastTime = millis();
  }
}
