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
// IMPORTANTE: Para tareas con retorno usar value_task y wait_value
value_task(int, readSensor)
{
    int value;  // Declarar variables al principio
    
    wait_value(100);  // Usar wait_value() en value_task
    
    value = analogRead(A0);
    wait_value(50);
    
    task_return(value);  // Retorna el valor y termina
}
endtask_value  // Cierra la tarea con retorno

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

