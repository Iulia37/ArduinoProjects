# 🎮 Arduino Mastermind Game

This project is a simplified version of the classic **Mastermind** logic game implemented using an Elegoo Uno R3.

## 🧠 Game Objective

Guess the correct combination of 4 unique colors (out of 5 possible) in the correct order. You have up to **10 attempts** to solve it!

---

## 🔧 Hardware Components

- Elegoo Uno R3
- 74HC595 Shift Register
- 5 Push Buttons (each one coresponding to a color: White, Yellow, Red, Green, Blue)
- 5 LEDs (one for each color)
- 1 Buzzer
- 16x2 LCD Display
- Resistors (220Ω for LEDs, for buttons I used INPUT_PULLUP)
- Breadboard & jumper wires

---

## 🎮 How It Works

- At startup, a random 4-color combination is generated using a shuffled array.
- The player selects colors by pressing buttons (each press lights up an LED).
- Once 4 colors are selected:
  - The guess is evaluated
  - The LCD shows:
    - How many colors are **correct and in the right position**.
    - How many colors are **correct but in the wrong position**.
- The buzzer gives audio feedback:
  - ✅ High-pitched tones for a win
  - ❌ Descending tones for game over
  - ⚠️ Mid-pitched tones for incorrect guess
- The game ends when:
  - The player guesses the correct combination
  - OR reaches the 10-attempt limit

