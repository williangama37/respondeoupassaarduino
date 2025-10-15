# 🕹️ Jogo de Perguntas e Respostas com Arduino  

## 🎯 Descrição do Projeto  
Este projeto é um jogo interativo para jogar em dupla, o **Responde ou Passa** foi desenvolvido com **Arduino Nano**, LEDs **NeoPixel**, **display LCD 20x4 I2C**, **buzzer** e **botões físicos**.  
O objetivo é testar conhecimentos gerais de forma divertida, com **efeitos visuais e sonoros** que indicam acertos e erros.

---

## ⚙️ Componentes Utilizados

| Componente | Quantidade | Descrição |
|-------------|-------------|-----------|
| Arduino Nano | 1 | Microcontrolador principal |
| LEDs NeoPixel (WS2812) | 40 | Representam as perguntas e efeitos visuais |
| Display LCD 20x4 I2C | 1 | Exibe perguntas e pontuação |
| Botão "Verdadeiro" | 1 | Responde Verdadeiro |
| Botão "Falso" | 1 | Responde Falso |
| Botão "Sortear" | 1 | Sorteia nova pergunta |
| Botão "Passar" | 1 | Pula a pergunta sem pontuar |
| Buzzer | 1 | Emite som de acerto/erro e cronômetro |
| Fonte 5V | 1 | Alimentação dos LEDs e Arduino |

---

## 🧩 Regras do Jogo

- O jogo contém **40 perguntas de Verdadeiro ou Falso**.  
- O jogador pressiona o botão **Sortear** para escolher uma pergunta aleatória.  
- O texto da pergunta aparece no **display LCD**.  
- O jogador tem **15 segundos** para responder.  
- LEDs e buzzer indicam o resultado:  
  - 🟢 **Verde** → Resposta correta (+10 pontos)  
  - 🔴 **Vermelho** → Resposta incorreta (–1 ponto)  
- O botão **Passar** permite pular uma pergunta (sem pontuar).  

---

## 🧠 Pontuação

| Ação | Pontos |
|------|---------|
| Resposta correta | +10 |
| Resposta incorreta | -1 |
| Passar | 0 |

---

## 💡 Lista de Perguntas

O jogo possui **40 perguntas de conhecimentos gerais**, com respostas de **Verdadeiro/Falso**.  
Você pode visualizar todas no arquivo [`perguntas.txt`](perguntas.txt).

Exemplo:  
> 1️⃣ A água ferve a 100°C ao nível do mar → ✅ Verdadeiro  
> 2️⃣ O Sol gira ao redor da Terra → ❌ Falso  

---

## 🖥️ Bibliotecas Necessárias

Certifique-se de instalar as seguintes bibliotecas na IDE Arduino:

- **Adafruit_NeoPixel**  
- **LiquidCrystal_I2C**  
- **Wire.h**

---

## 🧰 Estrutura do Repositório

| Arquivo | Descrição |
|----------|------------|
| `jogo_perguntas.ino` | Código principal do jogo |
| `perguntas.txt` | Lista de perguntas e gabarito |
| `esquema_circuito.png` | Esquema de ligação dos componentes |
| `README.md` | Documento de descrição do projeto |

---
