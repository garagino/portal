#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define pinSensor A0

// Endereço e configurações do LCD
#define endereco  0x27
#define colunas   16
#define linhas    2

// Definição dos pinos dos LEDs
#define vermelho 13
#define verde 12

int contador = 1;
bool estado = false;
bool estadoAnt = false;
bool timerAtivo = false;
unsigned long tempoInicial = 0;
unsigned long tempoDecorrido = 0;
unsigned long tempoDelay = 0;
bool delayAtivo = false;


LiquidCrystal_I2C lcd(endereco, colunas, linhas);

void setup() {
  pinMode(vermelho, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(pinSensor, INPUT);
  
  // Inicialização do LCD
  lcd.init(); // Inicia a comunicação com o display
  lcd.backlight(); // Liga a iluminação do display
  lcd.clear(); // Limpa o display

  // Mensagem inicial no LCD
  lcd.print("- Ola, Mundo! -"); 
  delay(1000); // Delay de 1s
  lcd.setCursor(0, 1); // Posiciona o cursor na primeira coluna da linha 2
  lcd.print("Fim do Setup ()");
  delay(1000); // Delay de 1s
  lcd.clear(); // Limpa o display

  // Inicializa o contador no LCD
  lcd.print("Volta ");
  lcd.print(contador);
  delay(1000);
}

void loop() {
  estado = !digitalRead(pinSensor);
  
  if (estado && !estadoAnt && !delayAtivo) {
    lcd.clear(); // Limpa o display
    lcd.print("Volta ");
    lcd.print(contador);
    
    if (!timerAtivo) {
      // Inicia o timer
      tempoInicial = millis();
      timerAtivo = true;
      delayAtivo = true;
      tempoDelay = millis(); // Inicia o delay
    } else {
      // Encerra o timer
      unsigned long tempoFinal = millis();
      tempoDecorrido = tempoFinal - tempoInicial;
      timerAtivo = false;
      contador++;
    }
  }
  
  if (timerAtivo) {
    // Atualiza o tempo decorrido continuamente
    unsigned long tempoAtual = millis();
    tempoDecorrido = tempoAtual - tempoInicial;
  }
  
  if (delayAtivo) {
    // Verifica se o delay de 1 segundo já passou
    if (millis() - tempoDelay >= 1000) {
      delayAtivo = false;
    }
  }
  
  // Exibe o tempo no formato mm:ss:ms
  unsigned long minutos = (tempoDecorrido / 60000) % 60;
  unsigned long segundos = (tempoDecorrido / 1000) % 60;
  unsigned long milissegundos = tempoDecorrido % 1000;
  
  char buffer[17]; // Buffer para a linha do LCD
  snprintf(buffer, sizeof(buffer), "%02lu:%02lu:%03lu", minutos, segundos, milissegundos);
  lcd.setCursor(0, 1); // Posiciona o cursor na primeira coluna da segunda linha
  lcd.print(buffer);
  
  if (estado) {
    digitalWrite(vermelho, LOW);  // Apaga o LED vermelho
    digitalWrite(verde, HIGH);    // Acende o LED verde
  } else {
    digitalWrite(vermelho, HIGH); // Acende o LED vermelho
    digitalWrite(verde, LOW);     // Apaga o LED verde
  }
  
  estadoAnt = estado;
}
