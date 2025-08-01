#include <LiquidCrystal.h>

// LCD pins configuration
const int rs = 8, en = 7, d4 = A0, d5 = A1, d6 = A2, d7 = A3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int contrast = 75;

// pins for shift register, buttons, and buzzer
int latchPin = 11;
int clockPin = 9;  
int dataPin = 12; 
int btnWhite = 2;
int btnYellow = 3;
int btnRed = 4;
int btnGreen = 5;
int btnBlue = 6;
int buzzer = 13;

byte leds = 0;

// arrays for generating the solution and storing guesses
int original[] = {0, 1, 2, 3, 4};
int auxiliar[5];  // copy of the original array used for shuffling
int solution[4];  // sequence to be guessed
int myGuess[4];   // current player guess

bool gameOver = false;
int count = 0;
int wrongGuesses = 0;
int maxLimit = 10; // after 10 wrong guesses, game over

// checks if myGuess is the correct solution
bool isSolution()
{
  for(int i = 0; i < 4; i++)
  {
    if(myGuess[i] != solution[i])
    {
      return false;
    }
  }
  return true;
}

// checks if a color has already been selected in the current guess
bool inMyGuess(int x)
{
  for(int i = 0; i < count; i++)
  {
    if(myGuess[i] == x)
    {
      return true;
    }
  }
  return false;
}

// generates a random sequence of 4 different colors
void generateSolution()
{
  for (int i = 0; i < 5; i++) 
  {
    auxiliar[i] = original[i];
  }

  // Fisher–Yates shuffle algorithm
  for (int i = 4; i > 0; i--) 
  {
    int j = random(i + 1);
    int aux = auxiliar[i];
    auxiliar[i] = auxiliar[j];
    auxiliar[j] = aux;
  }

  // take the first 4 elements as the solution to be guessed
  for (int i = 0; i < 4; i++) 
  {
    solution[i] = auxiliar[i];
  }
}

// turns off all LEDs
void resetLeds()
{
  for(int i = 0; i < 5; i++)
  {
    bitClear(leds, i);
  }
  updateShiftRegister();
}

// reads button presses and sets the corresponding LED
void readButton()
{
  if(digitalRead(btnWhite) == LOW && !inMyGuess(4))
  {
    while(digitalRead(btnWhite) == LOW);
    bitSet(leds, 4);
    myGuess[count] = 4;
    count++;
  }
  else
  {
    if(digitalRead(btnYellow) == LOW && !inMyGuess(3))
    {
      while(digitalRead(btnYellow) == LOW);
      bitSet(leds, 3);
      myGuess[count] = 3;
      count++;
    }
    else
    {
      if(digitalRead(btnRed) == LOW && !inMyGuess(2))
      {
        while(digitalRead(btnRed) == LOW);
        bitSet(leds, 2);
        myGuess[count] = 2;
        count++;
      }
      else
      {
        if(digitalRead(btnGreen) == LOW && !inMyGuess(1))
        {
          while(digitalRead(btnGreen) == LOW);
          bitSet(leds, 1);
          myGuess[count] = 1;
          count++;
        }
        else
        {
          if(digitalRead(btnBlue) == LOW && !inMyGuess(0))
          {
            while(digitalRead(btnBlue) == LOW);
            bitSet(leds, 0);
            myGuess[count] = 0;
            count++;
          }
        }
      }
    }
  }
  updateShiftRegister(); // update the active LEDs on the shift register
}

// sends the LED states to the shift register
void updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}

// displays on the LCD the number of colors that are in the correct position and the number of correct colors in wrong positions
void showHint()
{
  int right =0, wrong = 0;
  for(int i = 0; i < 4; i++)
  {
    if(solution[i] == myGuess[i])
    {
      right++;
    }
    else
    {
      for(int j = 0; j < 4; j++)
      {
        if(j != i && myGuess[i] == solution[j])
        {
          wrong++;
        }
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Correct: 0");
  lcd.setCursor(0, 1);
  lcd.print("Wrong: 0");
  lcd.setCursor(9, 0);
  lcd.print(right);
  lcd.setCursor(7, 1);
  lcd.print(wrong);
}

void gameLost()
{
  gameOver = true;

  tone(buzzer, 400, 250);
  delay(300);
  tone(buzzer, 300, 300);
  delay(300);
  tone(buzzer, 200, 400);

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Game Over");
  lcd.setCursor(0, 1);
  lcd.print("The solution was");

   // display the correct solution
  resetLeds();
  delay(4000);
  for(int i = 0; i < 4; i++)
  {
    bitSet(leds, solution[i]);
    updateShiftRegister();
    delay(2000);
  }
}

void gameWon()
{
  tone(buzzer, 800, 100);
  delay(150);
  tone(buzzer, 1000, 100);
  delay(150);
  tone(buzzer, 1200, 200);

  gameOver = true;

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("You won");
}

void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(btnWhite, INPUT_PULLUP);
  pinMode(btnYellow, INPUT_PULLUP);
  pinMode(btnRed, INPUT_PULLUP);
  pinMode(btnGreen, INPUT_PULLUP);
  pinMode(btnBlue, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  randomSeed(analogRead(A5)); 
  updateShiftRegister();
  generateSolution();

  analogWrite(10, contrast);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Correct: 0");
  lcd.setCursor(0, 1);
  lcd.print("Wrong: 0");
}

void loop() 
{
  while(!gameOver)
  {
     if(count < 4) // if less than 4 LEDs are chosen, read one more button
     {
       readButton();
     }
     else
     {
        count = 0;
        if(isSolution())
        {
          gameWon();
        }
        else
        {
          tone(buzzer, 500, 100);
          delay(150);
          tone(buzzer, 300, 100);

          showHint();
          wrongGuesses++;
          if(wrongGuesses == maxLimit)
          {
            gameLost();
          }
        }
        resetLeds();
     }
  }
}
