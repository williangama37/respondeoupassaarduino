#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>

// Function prototype
void proximoJogador();

#define PIN_LED 3
#define NUM_LEDS 40
#define BRILHO 50
#define BUZZER 7

#define BOTAO_SORTEAR 8
#define BOTAO_PASSAR 9
#define BOTAO_FALSO 10
#define BOTAO_VERDADEIRO 11

#define TEMPO_RESPOSTA 20000 // Aumentado para 20 segundos
#define TOTAL_PERGUNTAS 40
#define MAX_PARTIDAS 10
#define PONTOS_ACERTO 10
#define PONTOS_ERRO -1
#define SCROLL_DELAY 200 // Reduzido para rolagem mais rápida

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 20, 4);

int jogador1 = 0;
int jogador2 = 0;
int jogadorAtual = 1;
int perguntasRespondidas = 0;
int perguntaAtual = 0;
bool emPergunta = false;
unsigned long tempoInicio;
unsigned long ultimoScroll;
int scrollOffset = 0;
char perguntaBuffer[80]; // Aumentado para suportar perguntas longas

// Array de respostas em PROGMEM
const bool respostas[TOTAL_PERGUNTAS] PROGMEM = {
  false, true, true, false, false, false, true, true, true, true,
  true, false, true, false, true, true, false, true, true, false,
  false, false, true, false, true, true, true, false, true, false,
  false, false, true, false, false, true, false, false, true, true
};

// Array de perguntas em PROGMEM
const char pergunta_0[] PROGMEM = "O Sol nasce no oeste e se poe no leste.";
const char pergunta_1[] PROGMEM = "A agua ferve a 100 graus Celsius ao nivel do mar.";
const char pergunta_2[] PROGMEM = "A Australia eh o menor continente do mundo.";
const char pergunta_3[] PROGMEM = "O Monte Everest eh a montanha mais alta do sistema solar.";
const char pergunta_4[] PROGMEM = "A Mona Lisa foi pintada por Michelangelo.";
const char pergunta_5[] PROGMEM = "O Brasil faz fronteira com todos os paises da America do Sul.";
const char pergunta_6[] PROGMEM = "A Peninsula Iberica eh formada por Portugal e Espanha.";
const char pergunta_7[] PROGMEM = "O DNA eh a molecula responsavel pela hereditariedade.";
const char pergunta_8[] PROGMEM = "A Revolucao Francesa ocorreu no seculo XVIII.";
const char pergunta_9[] PROGMEM = "O Oceano Pacifico eh o maior oceano do mundo.";
const char pergunta_10[] PROGMEM = "A capital da Argentina eh Buenos Aires.";
const char pergunta_11[] PROGMEM = "As baleias sao peixes.";
const char pergunta_12[] PROGMEM = "Albert Einstein formulou a Teoria da Relatividade.";
const char pergunta_13[] PROGMEM = "A Guerra Fria foi um conflito armado entre EUA e URSS.";
const char pergunta_14[] PROGMEM = "O carbono eh um elemento essencial para a vida.";
const char pergunta_15[] PROGMEM = "A Arabia Saudita eh o maior pais do Oriente Medio em area.";
const char pergunta_16[] PROGMEM = "A bussola aponta sempre para o norte geografico.";
const char pergunta_17[] PROGMEM = "A lactose eh um tipo de acucar encontrado no leite.";
const char pergunta_18[] PROGMEM = "A Idade Media precedeu o Renascimento.";
const char pergunta_19[] PROGMEM = "O Rio Amazonas eh o rio mais longo do mundo.";
const char pergunta_20[] PROGMEM = "A capital do Canada eh Toronto.";
const char pergunta_21[] PROGMEM = "Os anfibios vivem exclusivamente em ambientes aquaticos.";
const char pergunta_22[] PROGMEM = "Isaac Newton descobriu a lei da gravidade ao ver uma maca cair.";
const char pergunta_23[] PROGMEM = "A Primeira Guerra Mundial terminou em 1917.";
const char pergunta_24[] PROGMEM = "A fotossintese eh o processo que as plantas usam para produzir alimento.";
const char pergunta_25[] PROGMEM = "O Japao eh um arquipelago.";
const char pergunta_26[] PROGMEM = "A velocidade da luz eh mais rapida que a do som.";
const char pergunta_27[] PROGMEM = "A glicose eh um tipo de proteina.";
const char pergunta_28[] PROGMEM = "O Imperio Romano caiu no seculo V d.C.";
const char pergunta_29[] PROGMEM = "A capital da Australia eh Sydney.";
const char pergunta_30[] PROGMEM = "Os repteis sao animais de sangue quente.";
const char pergunta_31[] PROGMEM = "Galileu Galilei inventou o telescopio.";
const char pergunta_32[] PROGMEM = "A Guerra Civil Americana ocorreu no seculo XIX.";
const char pergunta_33[] PROGMEM = "A atmosfera da Terra eh composta principalmente por oxigenio.";
const char pergunta_34[] PROGMEM = "O Polo Norte eh um continente coberto de gelo.";
const char pergunta_35[] PROGMEM = "A insulina eh um hormonio produzido pelo pancreas.";
const char pergunta_36[] PROGMEM = "O Iluminismo foi um movimento intelectual do seculo XVII.";
const char pergunta_37[] PROGMEM = "O Mar Mediterraneo esta entre a Europa e a Asia.";
const char pergunta_38[] PROGMEM = "A hemoglobina eh responsavel pelo transporte de oxigenio no sangue.";
const char pergunta_39[] PROGMEM = "A Revolucao Industrial comecou na Inglaterra.";

const char* const perguntas[] PROGMEM = {
  pergunta_0, pergunta_1, pergunta_2, pergunta_3, pergunta_4, pergunta_5, pergunta_6, pergunta_7, pergunta_8, pergunta_9,
  pergunta_10, pergunta_11, pergunta_12, pergunta_13, pergunta_14, pergunta_15, pergunta_16, pergunta_17, pergunta_18, pergunta_19,
  pergunta_20, pergunta_21, pergunta_22, pergunta_23, pergunta_24, pergunta_25, pergunta_26, pergunta_27, pergunta_28, pergunta_29,
  pergunta_30, pergunta_31, pergunta_32, pergunta_33, pergunta_34, pergunta_35, pergunta_36, pergunta_37, pergunta_38, pergunta_39
};

// Função para gerar cores aleatórias
uint32_t getRandomColor() {
  int r = random(0, 256); // Vermelho aleatório
  int g = random(0, 256); // Verde aleatório
  int b = random(0, 256); // Azul aleatório
  return leds.Color(r, g, b);
}

void setup() {
  lcd.init();
  lcd.backlight();
  leds.begin();
  leds.setBrightness(BRILHO);
  pinMode(BUZZER, OUTPUT);
  pinMode(BOTAO_SORTEAR, INPUT_PULLUP);
  pinMode(BOTAO_PASSAR, INPUT_PULLUP);
  pinMode(BOTAO_FALSO, INPUT_PULLUP);
  pinMode(BOTAO_VERDADEIRO, INPUT_PULLUP);
  randomSeed(analogRead(0));
  lcd.setCursor(0, 0);
  lcd.print(F("Roleta da Duvida!"));
  lcd.setCursor(0, 1);
  lcd.print(F("Aperte p sortear"));
}

void loop() {
  if (!emPergunta) {
    animacaoEspera(); // Chama a animação de festa
    if (digitalRead(BOTAO_SORTEAR) == LOW) {
      sortearPergunta();
    }
  } else {
    // Atualiza o scroll do texto da pergunta
    if (millis() - ultimoScroll >= SCROLL_DELAY) {
      atualizarPergunta();
      ultimoScroll = millis();
    }
    // Verifica timeout ou ações do jogador
    if (millis() - tempoInicio > TEMPO_RESPOSTA) {
      lcd.clear();
      lcd.print(F("Tempo esgotado!"));
      delay(2000);
      proximoJogador();
    }
    if (digitalRead(BOTAO_PASSAR) == LOW) {
      lcd.clear();
      lcd.print(F("Passou a vez."));
      delay(1000);
      proximoJogador();
    }
    if (digitalRead(BOTAO_VERDADEIRO) == LOW) responderPergunta(true);
    if (digitalRead(BOTAO_FALSO) == LOW) responderPergunta(false);
  }
}

void animacaoEspera() {
  static unsigned long lastUpdate = 0;
  static int effectStep = 0;
  static int effectType = 0;

  if (millis() - lastUpdate < 50) return; // Controla a velocidade da animação
  lastUpdate = millis();

  // Alterna entre diferentes efeitos de festa
  if (effectStep == 0) {
    effectType = random(3); // Escolhe um dos 3 efeitos aleatoriamente
  }

  if (effectType == 0) { // Efeito 1: Ondas de cores
    leds.setPixelColor(effectStep % NUM_LEDS, getRandomColor());
    leds.show();
  } else if (effectType == 1) { // Efeito 2: Piscar
    uint32_t color = getRandomColor();
    for (int i = 0; i < NUM_LEDS; i++) {
      leds.setPixelColor(i, (effectStep % 2 == 0) ? color : leds.Color(0, 0, 0));
    }
    leds.show();
  } else { // Efeito 3: Corrida de LED
    leds.clear();
    leds.setPixelColor(effectStep % NUM_LEDS, getRandomColor());
    leds.show();
  }

  effectStep++;
  if (effectStep >= NUM_LEDS * 2) effectStep = 0; // Reinicia após completar ciclos
}

void sortearPergunta() {
  perguntaAtual = random(TOTAL_PERGUNTAS);
  int posicaoLed = perguntaAtual % NUM_LEDS;
  for (int i = 0; i < NUM_LEDS * 2; i++) {
    // Preenche o fundo com LEDs azuis e vermelhos alternados
    for (int j = 0; j < NUM_LEDS; j++) {
      if (j == i % NUM_LEDS) {
        leds.setPixelColor(j, leds.Color(255, 255, 255)); // LED branco em movimento
      } else {
        leds.setPixelColor(j, (j % 2 == 0) ? leds.Color(0, 0, 255) : leds.Color(255, 0, 0)); // Azul ou vermelho
      }
    }
    leds.show();
    tone(BUZZER, 1000, 50);
    delay(50);
  }

  //leds - Após o giro, mantém apenas o LED branco na posição final
  leds.clear();
  leds.setPixelColor(posicaoLed, leds.Color(255, 255, 255));
  leds.show();
  mostrarPergunta();
}

void mostrarPergunta() {
  // Carrega a pergunta em um buffer global
  strcpy_P(perguntaBuffer, (char*)pgm_read_word(&(perguntas[perguntaAtual])));
  scrollOffset = 0;
  ultimoScroll = millis();
  atualizarPergunta();
  tempoInicio = millis();
  emPergunta = true;
}

void atualizarPergunta() {
  lcd.clear();
  int len = strlen(perguntaBuffer);
  // Exibe até 4 linhas, cada uma com 20 caracteres, começando do offset
  for (int line = 0; line < 4; line++) {
    int startIndex = scrollOffset + (line * 20);
    if (startIndex < len) {
      char lineBuffer[21];
      int charsToCopy = min(20, len - startIndex);
      strncpy(lineBuffer, perguntaBuffer + startIndex, charsToCopy);
      lineBuffer[charsToCopy] = '\0'; // Adiciona terminador nulo
      lcd.setCursor(0, line);
      lcd.print(lineBuffer);
    }
  }
  // Incrementa o offset para o próximo scroll
  scrollOffset++;
  if (scrollOffset > len) scrollOffset = 0; // Reinicia o scroll
}

void responderPergunta(bool respostaJogador) {
  bool respostaCorreta = pgm_read_byte(&(respostas[perguntaAtual]));
  lcd.clear();
  if (respostaJogador == respostaCorreta) {
    lcd.print(F("Correto! +10 pts"));
    if (jogadorAtual == 1) jogador1 += PONTOS_ACERTO;
    else jogador2 += PONTOS_ACERTO;
    efeitoLED(0, 255, 0);
  } else {
    lcd.print(F("Errado! -1 pt"));
    if (jogadorAtual == 1) jogador1 += PONTOS_ERRO;
    else jogador2 += PONTOS_ERRO;
    efeitoLED(255, 0, 0);
  }
  lcd.setCursor(0, 1);
  lcd.print(F("J1: "));
  lcd.print(jogador1);
  lcd.print(F(" J2: "));
  lcd.print(jogador2);
  delay(3000);
  proximoJogador();
}

void efeitoLED(int r, int g, int b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, leds.Color(r, g, b));
  }
  leds.show();
}

void proximoJogador() {
  perguntasRespondidas++;
  if (perguntasRespondidas >= MAX_PARTIDAS) {
    lcd.clear();
    lcd.print(F("Fim do jogo!"));
    lcd.setCursor(0, 1);
    lcd.print(F("J1:"));
    lcd.print(jogador1);
    lcd.print(F(" J2:"));
    lcd.print(jogador2);
    while (1);
  }
  jogadorAtual = (jogadorAtual == 1) ? 2 : 1;
  lcd.clear();
  lcd.print(F("Jogador "));
  lcd.print(jogadorAtual);
  lcd.setCursor(0, 1);
  lcd.print(F("Aperte p sortear"));
  emPergunta = false;
}