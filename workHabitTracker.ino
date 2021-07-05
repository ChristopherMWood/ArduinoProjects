#include <LiquidCrystal.h>
#include <Stepper.h>

//LCD Setup
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//Ultrasonic Sensor Setup
const int trigPin = 11;
const int echoPin = 12;

long duration;
int distance;
int distanceCm, distanceInch;

//Arnie Work States
enum States {
  WELCOME,
  WATCHING,
  LOOKING,
  EMOTION
};

int cyclesWorking = 0;
int cyclesNotWorking = 0;
int totalCycles = 0;
int currentStateCycles = 0;
const int CycleDelay = 180;
int currentTimeBetweenStateCheck = 0;
const int SecondsBetweenStateCheck = 4;
States CurrentArnieState = WELCOME;

void calculateInchesAway() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  distanceCm = duration*0.034/2;
  distanceInch = duration*0.0133/2;
}

void updateState() {
    States previousState = CurrentArnieState;
  
    if (totalCycles > 25) {
      int emotionCalc = (totalCycles / 5) % 60;
      
      if (emotionCalc <= 5) {
        CurrentArnieState = EMOTION;
      }
      else if (distanceInch > 10 && distanceInch < 45) {
      }
        CurrentArnieState = WATCHING;
      } else if (distanceInch >= 45) {
        CurrentArnieState = LOOKING;
      }
    }

    if (previousState != CurrentArnieState) {
      currentStateCycles = 0;
    }
}

void println(int text) {
    lcd.println(text);
}

void print(int text) {
    lcd.print(text);
}

void println(String text) {
    lcd.println(text);
}

void print(String text) {
    lcd.print(text);
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.begin(16,2);
  Serial.begin(9600);
}

void loop() {
  if (currentTimeBetweenStateCheck > (SecondsBetweenStateCheck * 1000)) {
    updateState();
    currentTimeBetweenStateCheck = 0;
  }

  delay(CycleDelay);
  currentTimeBetweenStateCheck += CycleDelay;
  calculateInchesAway();
  totalCycles++;
  currentStateCycles++;

  //UPDATE LOOP VARIABLES
  if (CurrentArnieState == WATCHING) {
    cyclesWorking++;
  } else if (CurrentArnieState == LOOKING) {
    cyclesNotWorking++;
  }

  if (currentStateCycles < 15) {
    //PRINT CHANGE OF STATE MESSAGE
    lcd.setCursor(0,0);
    if (CurrentArnieState == WELCOME) {
      println("Welcome to Work    ");
      lcd.setCursor(0,1);
      println("employee: 2886     ");
    } else if (CurrentArnieState == WATCHING) {
      println("You are now       ");
      lcd.setCursor(0,1);
      println("being observed...  ");
      cyclesWorking++;
    } else if (CurrentArnieState == LOOKING) {
      println("Back to work       ");
      lcd.setCursor(0,1);
      println("employee: 2886    ");
      cyclesNotWorking++;
    } else if (CurrentArnieState == EMOTION) {
      int workToBreakRatio = cyclesWorking / cyclesNotWorking;
      
      if (workToBreakRatio == 0) {
        println("Arnie Angry    ");
        lcd.setCursor(0,1);
        println("This is theft...    ");
      } else if (workToBreakRatio == 1) {
        println("Arnie concerned    ");
        lcd.setCursor(0,1);
        println("I expect more    ");
      } else if (workToBreakRatio == 8) {
        println("Arnie unimpressed    ");
        lcd.setCursor(0,1);
        println("Work harder...    ");
      } else if (workToBreakRatio > 12) {
        println("Arnie happy    ");
        lcd.setCursor(0,1);
        println("Keep it up    ");
      }
      
      println("Back to work       ");
      lcd.setCursor(0,1);
      println("employee: 2886    ");
      cyclesNotWorking++;
    }
    delay(10);
  } else {
    //PRINT CURRENT WORK STATS
    lcd.setCursor(0,0);
    int secondsWorking = cyclesWorking / 5;
    int minutesWorking = secondsWorking / 60;
    print("Working: ");
    print(minutesWorking);
    print("m ");
    print(secondsWorking - (minutesWorking * 60));
    println("s        ");
    
    lcd.setCursor(0,1);
    int secondsNotWorking = cyclesNotWorking / 5;
    int minutesNotWorking = secondsNotWorking / 60;
    print("Breaks: ");
    print(minutesNotWorking);
    print("m ");
    print(secondsNotWorking - (minutesNotWorking * 60));
    println("s        ");
    delay(10);
  }
}
