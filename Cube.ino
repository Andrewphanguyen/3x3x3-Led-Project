// =====================================================
// 3x3x3 LED CUBE
//
// PATTERN 0 = Moving LED
// PATTERN 1 = Moving Row
// PATTERN 2 = Moving Column
//
// BUTTON:
// A0 -> Button -> GND
//
// BEHAVIOR:
// Pattern finishes -> REPEAT SAME PATTERN
// Button pressed   -> RANDOM DIFFERENT PATTERN
//
// WIRING:
// NEGATIVE = HIGH = LED ON
// LAYER    = LOW  = GROUND / ON
// =====================================================


// =====================================================
// PIN SETUP
// =====================================================

// Negative pins

int neg1 = 5;
int neg2 = 6;
int neg3 = 7;
int neg4 = 8;
int neg5 = 9;
int neg6 = 10;
int neg7 = 11;
int neg8 = 12;
int neg9 = 13;


// Layer pins

int pos1 = 4;
int pos2 = 3;
int pos3 = 2;


// Button

int buttonPin = A0;


// =====================================================
// ARRAYS
// =====================================================

int negPins[9] = {
  neg1, neg2, neg3,
  neg4, neg5, neg6,
  neg7, neg8, neg9
};

int posPins[3] = {
  pos1, pos2, pos3
};


// =====================================================
// CURRENT PATTERN
// =====================================================

// -1 = RANDOM MODE

// If we are testing only one pattern, we assign it to that pattern
// Example:
//int currentPattern = 2;

int currentPattern = -1;



// =====================================================
// SETUP
// =====================================================

void setup() {
  
  
  Serial.begin(9600);

  // Set negative pins as OUTPUT

  for(int i = 0; i < 9; i++) {

    pinMode(negPins[i], OUTPUT);

  }


  // Set layer pins as OUTPUT

  for(int i = 0; i < 3; i++) {

    pinMode(posPins[i], OUTPUT);

  }


  // Button

  pinMode(buttonPin, INPUT_PULLUP);


  // Turn everything OFF

  turnEverythingOff();


  // Random seed for pattern randomness

  // A1 is left unused so it can read electrical noise

  randomSeed(analogRead(A1));

  if(!(currentPattern >= 0)) {
     chooseRandomPattern();
  }
  // Pick the first pattern
 

}


// =====================================================
// LOOP
// =====================================================

void loop() {

  // ---------------------------------
  // PATTERN 0
  // ---------------------------------

  if(currentPattern == 0) {

    movingLED();

  }


  // ---------------------------------
  // PATTERN 1
  // ---------------------------------

  else if(currentPattern == 1) {

    movingRow();

  }


  // ---------------------------------
  // PATTERN 2
  // ---------------------------------

  else if(currentPattern == 2) {

    movingColumn();

  }

   // ---------------------------------
  // Placeholder for adding new patterns
  // ---------------------------------

}


// =====================================================
// CHOOSE RANDOM PATTERN
// =====================================================

void chooseRandomPattern() {

  int newPattern;


  // Pick a new pattern

  do {

    newPattern = random(0, 3);

  } while(newPattern == currentPattern);


  currentPattern = newPattern;


  // Turn everything OFF before starting

  turnEverythingOff();


}


// =====================================================
// TURN EVERYTHING OFF
// =====================================================

void turnEverythingOff() {

  // Turn all layers OFF

  digitalWrite(pos1, HIGH);
  digitalWrite(pos2, HIGH);
  digitalWrite(pos3, HIGH);


  // Turn all negative pins OFF

  for(int i = 0; i < 9; i++) {

    digitalWrite(negPins[i], LOW);

  }

}


// =====================================================
// BUTTON CHECK
// =====================================================

bool buttonPressed() {

  // Button pressed

  if(digitalRead(buttonPin) == LOW) {

    // Small debounce delay

    delay(30);


    // Make sure button is still pressed

    if(digitalRead(buttonPin) == LOW) {

      // Pick a new random pattern

      chooseRandomPattern();


      // Wait until button is released

      while(digitalRead(buttonPin) == LOW) {

        delay(10);

      }


      return true;

    }

  }


  return false;

}

// =====================================================
// PRINT CUBE
// =====================================================
//
// This creates a graphical representation of the
// physical cube in the Serial Monitor.
//
// X = ON
// . = OFF
//
// layer = currently active layer
// led   = currently active LED
//
// =====================================================


void printCube(int cube[3][3][3]) {

  Serial.println();
  Serial.println("==============================");


  for(int layer = 0; layer < 3; layer++) {

    Serial.print("LAYER ");
    Serial.println(layer + 1);


    for(int row = 0; row < 3; row++) {

      for(int column = 0; column < 3; column++) {

        if(cube[layer][row][column] == 1) {

          Serial.print("X ");

        }

        else {

          Serial.print(". ");

        }

      }

      Serial.println();

    }

    Serial.println();

  }


  Serial.println("==============================");

}


// =====================================================
// PATTERN 0
// MOVING LED
// =====================================================
//
// One LED moves through the entire cube.
//
// Layer 1:
//
// 1 -> 2 -> 3
// 4 -> 5 -> 6
// 7 -> 8 -> 9
//
// Then Layer 2
//
// Then Layer 3
//
// =====================================================

void movingLED() {

  // Go through each layer

  for(int layer = 0; layer < 3; layer++) {


    // Go through each LED position

    for(int led = 0; led < 9; led++) {

      // Create empty cube

      int cube[3][3][3] = {0};


      // Convert LED number into row and column

      int row = led / 3;

      int column = led % 3;


      // Turn ON this LED in our graphical cube

      cube[layer][row][column] = 1;


      // Print the cube

      printCube(cube);


      // Keep this LED lit

      for(int i = 0; i < 100; i++) {


        // Check button

        if(buttonPressed()) {

          return;

        }


        // Turn everything OFF

        turnEverythingOff();


        // Select layer

        digitalWrite(posPins[layer], LOW);


        // Select LED

        digitalWrite(negPins[led], HIGH);


        // Multiplexing delay

        delay(7);

      }

    }

  }

}


// =====================================================
// PATTERN 1
// MOVING ROW
// =====================================================
//
// Row 1:
//
// ● ● ●
// . . .
// . . .
//
// Row 2:
//
// . . .
// ● ● ●
// . . .
//
// Row 3:
//
// . . .
// . . .
// ● ● ●
//
// The row is displayed across ALL 3 layers.
//
// =====================================================

void movingRow() {

  // There are 3 rows

  for(int row = 0; row < 3; row++) {

    // Create empty cube

    int cube[3][3][3] = {0};


    // Turn ON the row in all 3 layers

    for(int layer = 0; layer < 3; layer++) {

      for(int column = 0; column < 3; column++) {

        cube[layer][row][column] = 1;

      }

    }


    // Print cube

    printCube(cube);



    // Keep the row displayed

    for(int i = 0; i < 100; i++) {


      // Check button

      if(buttonPressed()) {

        return;

      }


      // ---------------------------------
      // LAYER 1
      // ---------------------------------

      turnEverythingOff();

      digitalWrite(pos1, LOW);

      turnRowOn(row);

      delay(7);


      // ---------------------------------
      // LAYER 2
      // ---------------------------------

      turnEverythingOff();

      digitalWrite(pos2, LOW);

      turnRowOn(row);

      delay(7);


      // ---------------------------------
      // LAYER 3
      // ---------------------------------

      turnEverythingOff();

      digitalWrite(pos3, LOW);

      turnRowOn(row);

      delay(7);

    }

  }

}


// =====================================================
// TURN ON A ROW HELPER FUNCTION
// =====================================================

void turnRowOn(int row) {

  // ---------------------------------
  // ROW 1
  // ---------------------------------

  if(row == 0) {

    digitalWrite(neg1, HIGH);
    digitalWrite(neg2, HIGH);
    digitalWrite(neg3, HIGH);

  }


  // ---------------------------------
  // ROW 2
  // ---------------------------------

  else if(row == 1) {

    digitalWrite(neg4, HIGH);
    digitalWrite(neg5, HIGH);
    digitalWrite(neg6, HIGH);

  }


  // ---------------------------------
  // ROW 3
  // ---------------------------------

  else if(row == 2) {

    digitalWrite(neg7, HIGH);
    digitalWrite(neg8, HIGH);
    digitalWrite(neg9, HIGH);

  }

}


// =====================================================
// PATTERN 2
// MOVING COLUMN
// =====================================================
//
// Column 1:
//
// ● . .
// ● . .
// ● . .
//
// Column 2:
//
// . ● .
// . ● .
// . ● .
//
// Column 3:
//
// . . ●
// . . ●
// . . ●
//
// The column moves from LEFT -> MIDDLE -> RIGHT.
//
// =====================================================

void movingColumn() {

  // There are 3 columns

  for(int column = 0; column < 3; column++) {

    // Create empty cube

    int cube[3][3][3] = {0};


    // Turn ON this column in all 3 layers

    for(int layer = 0; layer < 3; layer++) {

      for(int row = 0; row < 3; row++) {

        cube[layer][row][column] = 1;

      }

    }


    // Print cube

    printCube(cube);





    // Keep the column displayed

    for(int i = 0; i < 100; i++) {


      // Check button

      if(buttonPressed()) {

        return;

      }


      // ---------------------------------
      // LAYER 1
      // ---------------------------------

      turnEverythingOff();

      digitalWrite(pos1, LOW);

      turnColumnOn(column);

      delay(7);


      // ---------------------------------
      // LAYER 2
      // ---------------------------------

      turnEverythingOff();

      digitalWrite(pos2, LOW);

      turnColumnOn(column);

      delay(7);


      // ---------------------------------
      // LAYER 3
      // ---------------------------------

      turnEverythingOff();

      digitalWrite(pos3, LOW);

      turnColumnOn(column);

      delay(7);

    }

  }

}


// =====================================================
// TURN ON A COLUMN HELPER FUNCTION
// =====================================================

void turnColumnOn(int column) {

  // ---------------------------------
  // COLUMN 1
  // ---------------------------------

  if(column == 0) {

    digitalWrite(neg1, HIGH);
    digitalWrite(neg4, HIGH);
    digitalWrite(neg7, HIGH);

  }


  // ---------------------------------
  // COLUMN 2
  // ---------------------------------

  else if(column == 1) {

    digitalWrite(neg2, HIGH);
    digitalWrite(neg5, HIGH);
    digitalWrite(neg8, HIGH);

  }


  // ---------------------------------
  // COLUMN 3
  // ---------------------------------

  else if(column == 2) {

    digitalWrite(neg3, HIGH);
    digitalWrite(neg6, HIGH);
    digitalWrite(neg9, HIGH);

  }

}


