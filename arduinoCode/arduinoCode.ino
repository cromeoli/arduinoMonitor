#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

String receivedData = "";
int temperature = 0;
int ramUsed = 0;

// Create custom characters
byte line_1[8] = {
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte line_2[8] = {
  0b11111,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};

byte line_3[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};

byte line_4[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};

byte line_5[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
};

byte line_6[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000,
  0b00000,
};

byte line_7[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000,
};

void breathe() {
  // Patrones predefinidos para cada fase
  String patrones[] = {
    "       --       ",  // Iteración 0: centro
    "      ----      ",  // Iteración 1: expandir
    "     ------     ",  // Iteración 2: expandir más
    "    --------    ",  // Iteración 3: máximo
    "   --      --   ",  // Iteración 4: contraer centro
    "  --        --  ",  // Iteración 5: contraer más
    " -            - ",  // Iteración 6: solo extremos
    "                "   // Iteración 7: vacío
  };
  
  // Mostrar cada patrón
  for(int i = 0; i < 8; i++) {
    lcd.setCursor(0, 1);
    lcd.print(patrones[i]);
    delay(100);
  }
}

char zima[] = "Z I M A";

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, line_1);
  lcd.createChar(1, line_2);
  lcd.createChar(2, line_3);
  lcd.createChar(3, line_4);
  lcd.createChar(4, line_5);
  lcd.createChar(5, line_6);
  lcd.createChar(6, line_7);
  
  // Animación
  for(int i = 0; i < 16; i++) {
    if(i < 7) {
      lcd.setCursor(i, 1);
      lcd.write(i);
    }

    else if(i < 16) {
      lcd.setCursor(i, 1);
      lcd.print(char(255));
      
      if(i >= 9) {
        int zimaIndex = i - 9;
        if(zimaIndex >= 0 && zimaIndex < 7) { 
          lcd.setCursor(i, 0);
          lcd.print(zima[zimaIndex]);
        }
      }
    }
    
    delay(500);
  }
  delay(2500);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Esperando conex.");
  
  lcd.setCursor(0, 1);
}

void loop() {
  // Leer datos del puerto serie
  if (Serial.available() > 0) {
    receivedData = Serial.readString();
    receivedData.trim();
    
    // Parsear datos: "TEMP:45,RAM:60"
    if (receivedData.startsWith("TEMP:")) {
      int tempIndex = receivedData.indexOf("TEMP:") + 5;
      int commaIndex = receivedData.indexOf(",");
      temperature = receivedData.substring(tempIndex, commaIndex).toInt();
      
      int ramIndex = receivedData.indexOf("RAM:") + 4;
      ramUsed = receivedData.substring(ramIndex).toInt();
      
      // Mostrar en LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CPU: ");
      lcd.print(temperature);
      lcd.print((char) 223);

      lcd.print("GPU: ");

      
      lcd.setCursor(0, 1);
      lcd.print("RAM: ");
      lcd.print(ramUsed);
      lcd.print("%");

      lcd.print("Z I M A");

    }
  } else {
    breathe();
  }
  
  delay(1000);
}
