#include <CoroutineDSL.h>

CoroutineTask serialTask;
CoroutineTask buttonTask;

// Simulación de un botón (cambia cada 3 segundos)
bool virtualButton = false;

task(serialReader)
{
    char c;  // Declarar al principio
    
    Serial.println("Esperando datos por Serial...");
    
    // Espera HASTA QUE haya datos disponibles (no bloqueante)
    waitUntil(Serial.available() > 0);
    
    c = Serial.read();
    Serial.print("Recibido: ");
    Serial.println(c);
    
    // Pequeña pausa para no saturar
    wait(100);
}
endtask

task(buttonWatcher)
{
    Serial.println("Esperando que el botón virtual sea TRUE...");
    
    // Espera MIENTRAS el botón sea false (equivalente a waitUntil(virtualButton == true))
    waitWhile(virtualButton == false);
    
    Serial.println("¡Botón presionado! (Virtualmente)");
    
    // Espera mientras siga presionado (para no repetir)
    waitWhile(virtualButton == true);
    
    Serial.println("Botón liberado.");
}
endtask

void setup() {
    Serial.begin(9600);
    serialTask.start();
    buttonTask.start();
}

void loop() {
    // Ejecutar tareas
    serialReader(serialTask);
    buttonWatcher(buttonTask);
    
    // Simular cambio de estado del botón cada 3 segundos
    static unsigned long lastChange = 0;
    if (millis() - lastChange > 3000) {
        virtualButton = !virtualButton;
        lastChange = millis();
        Serial.print("Simulación: Botón ahora es ");
        Serial.println(virtualButton ? "TRUE" : "FALSE");
    }
}
