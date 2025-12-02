#include <CoroutineDSL.h>

// Hardware Configuration
#define BTN0 0
#define BTN1 1
#define BTN2 2
#define BTN3 3

#define LED0 4
#define LED1 5
#define LED2 6
#define LED3 7

// Tareas
CoroutineTask led0Task;
CoroutineTask led1Task;
CoroutineTask led2Task;
CoroutineTask led3Task;
CoroutineTask button0Task;
CoroutineTask button1Task;
CoroutineTask button2Task;
CoroutineTask button3Task;

// Estado de los botones (para control)
bool btn0Pressed = false;
bool btn1Pressed = false;
bool btn2Pressed = false;
bool btn3Pressed = false;

// Modos para cada LED
enum LedMode { OFF, SLOW_BLINK, FAST_BLINK, ALWAYS_ON };
LedMode mode0 = SLOW_BLINK;
LedMode mode1 = FAST_BLINK;
LedMode mode2 = OFF;
LedMode mode3 = ALWAYS_ON;

// ----- LED 0: Parpadeo lento (1 segundo) -----
task(led0Controller)
{
    if (mode0 == OFF) {
        digitalWrite(LED0, LOW);
    } else if (mode0 == ALWAYS_ON) {
        digitalWrite(LED0, HIGH);
    } else if (mode0 == SLOW_BLINK) {
        digitalWrite(LED0, HIGH);
        wait(1000);
        digitalWrite(LED0, LOW);
        wait(1000);
    } else if (mode0 == FAST_BLINK) {
        digitalWrite(LED0, HIGH);
        wait(200);
        digitalWrite(LED0, LOW);
        wait(200);
    }
}
endtask

// ----- LED 1: Parpadeo rápido (200ms) -----
task(led1Controller)
{
    if (mode1 == OFF) {
        digitalWrite(LED1, LOW);
    } else if (mode1 == ALWAYS_ON) {
        digitalWrite(LED1, HIGH);
    } else if (mode1 == SLOW_BLINK) {
        digitalWrite(LED1, HIGH);
        wait(1000);
        digitalWrite(LED1, LOW);
        wait(1000);
    } else if (mode1 == FAST_BLINK) {
        digitalWrite(LED1, HIGH);
        wait(200);
        digitalWrite(LED1, LOW);
        wait(200);
    }
}
endtask

// ----- LED 2: Patrón "fade" con PWM -----
task(led2Controller)
{
    static int brightness = 0;  // Usar static para mantener el valor entre llamadas
    static int fadeDirection = 1;  // 1 = subiendo, -1 = bajando
    
    if (mode2 == OFF) {
        analogWrite(LED2, 0);
        brightness = 0;
        fadeDirection = 1;
    } else if (mode2 == ALWAYS_ON) {
        analogWrite(LED2, 255);
    } else if (mode2 == SLOW_BLINK) {
        // Fade suave paso a paso
        analogWrite(LED2, brightness);
        
        brightness += fadeDirection * 5;
        
        if (brightness >= 255) {
            brightness = 255;
            fadeDirection = -1;
        } else if (brightness <= 0) {
            brightness = 0;
            fadeDirection = 1;
        }
        
        wait(20);
    } else if (mode2 == FAST_BLINK) {
        // Fade rápido paso a paso
        analogWrite(LED2, brightness);
        
        brightness += fadeDirection * 15;
        
        if (brightness >= 255) {
            brightness = 255;
            fadeDirection = -1;
        } else if (brightness <= 0) {
            brightness = 0;
            fadeDirection = 1;
        }
        
        wait(5);
    }
}
endtask

// ----- LED 3: Controlado directamente por botón 3 (con debounce) -----
task(led3Controller)
{
    if (mode3 == OFF) {
        digitalWrite(LED3, LOW);
    } else if (mode3 == ALWAYS_ON) {
        digitalWrite(LED3, HIGH);
    } else if (mode3 == SLOW_BLINK) {
        digitalWrite(LED3, HIGH);
        wait(1000);
        digitalWrite(LED3, LOW);
        wait(1000);
    } else if (mode3 == FAST_BLINK) {
        digitalWrite(LED3, HIGH);
        wait(200);
        digitalWrite(LED3, LOW);
        wait(200);
    }
}
endtask

// ----- Monitor de Botón 0 (independiente) -----
task(button0Monitor)
{
    waitUntil(digitalRead(BTN0) == LOW);  // Esperar presión
    wait(50);  // Debounce
    if (digitalRead(BTN0) == LOW) {
        mode0 = (LedMode)((mode0 + 1) % 4);
        Serial.print("LED0 modo: ");
        Serial.println(mode0);
    }
    waitWhile(digitalRead(BTN0) == LOW);  // Esperar liberación
    wait(50);  // Debounce
}
endtask

// ----- Monitor de Botón 1 (independiente) -----
task(button1Monitor)
{
    waitUntil(digitalRead(BTN1) == LOW);
    wait(50);
    if (digitalRead(BTN1) == LOW) {
        mode1 = (LedMode)((mode1 + 1) % 4);
        Serial.print("LED1 modo: ");
        Serial.println(mode1);
    }
    waitWhile(digitalRead(BTN1) == LOW);
    wait(50);
}
endtask

// ----- Monitor de Botón 2 (independiente) -----
task(button2Monitor)
{
    waitUntil(digitalRead(BTN2) == LOW);
    wait(50);
    if (digitalRead(BTN2) == LOW) {
        mode2 = (LedMode)((mode2 + 1) % 4);
        Serial.print("LED2 modo: ");
        Serial.println(mode2);
    }
    waitWhile(digitalRead(BTN2) == LOW);
    wait(50);
}
endtask

// ----- Monitor de Botón 3 (independiente) -----
task(button3Monitor)
{
    waitUntil(digitalRead(BTN3) == LOW);
    wait(50);
    if (digitalRead(BTN3) == LOW) {
        mode3 = (LedMode)((mode3 + 1) % 4);
        Serial.print("LED3 modo: ");
        Serial.println(mode3);
    }
    waitWhile(digitalRead(BTN3) == LOW);
    wait(50);
}
endtask

void setup() {
    Serial.begin(9600);
    
    // Configurar pines (con pull-up externo)
    pinMode(BTN0, INPUT);
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
    pinMode(BTN3, INPUT);
    
    pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    
    // Iniciar tareas
    led0Task.start();
    led1Task.start();
    led2Task.start();
    led3Task.start();
    button0Task.start();
    button1Task.start();
    button2Task.start();
    button3Task.start();
    
    Serial.println("=== CoRoutine Hardware Test ===");
    Serial.println("Presiona BTN0-3 para cambiar modos de LED0-3");
    Serial.println("Modos: 0=OFF, 1=SLOW, 2=FAST, 3=ON");
}

void loop() {
    // Ejecutar todas las tareas (¡ninguna bloquea!)
    led0Controller(led0Task);
    led1Controller(led1Task);
    led2Controller(led2Task);
    led3Controller(led3Task);
    button0Monitor(button0Task);
    button1Monitor(button1Task);
    button2Monitor(button2Task);
    button3Monitor(button3Task);
}
