#include <CoroutineDSL.h>

CoroutineTask blinkTask;
CoroutineTask sensorTask;

// ----- Tarea void (auto-loop) -----
task(blink)
{
    digitalWrite(LED_BUILTIN, HIGH);
    wait(500);

    digitalWrite(LED_BUILTIN, LOW);
    wait(500);
}
endtask

// ----- Tarea con retorno (se ejecuta una vez) -----
task_return(int, readSensor)
{
    wait(100);  // Simula tiempo de lectura del sensor
    
    int value = analogRead(A0);
    wait(50);   // Procesamiento adicional
    
    endtask_return(value);  // Retorna el valor y termina
}

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    
    blinkTask.start();
    sensorTask.start();
}

void loop() {
    blink(blinkTask);
    
    // La tarea con retorno se ejecuta una vez y retorna un valor
    if (sensorTask.isRunning()) {
        int sensorValue = readSensor(sensorTask);
        Serial.print("Sensor value: ");
        Serial.println(sensorValue);
        
        // Reiniciar para leer de nuevo
        sensorTask.start();
    }
}

