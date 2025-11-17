# CoroutineDSL

Una biblioteca DSL (Domain Specific Language) para crear tareas no bloqueantes (corrutinas) en Arduino usando una sintaxis simple y elegante.

## 📋 Descripción

CoroutineDSL permite crear múltiples tareas que se ejecutan de forma concurrente sin bloquear el loop principal de Arduino. Utiliza un sistema de corrutinas basado en continuaciones que permite pausar y reanudar tareas en puntos específicos.

## ✨ Características

- ✅ Sintaxis DSL simple y legible
- ✅ Tareas no bloqueantes con `wait()`
- ✅ Soporte para tareas con auto-loop y tareas con retorno
- ✅ Control de estado (running, paused, stopped)
- ✅ Múltiples tareas concurrentes
- ✅ Sin dependencias externas (solo Arduino.h)

## 🚀 Instalación

1. Descarga o clona este repositorio
2. Copia la carpeta `CoRoutine` a tu directorio de bibliotecas de Arduino:
   - Windows: `Documents/Arduino/libraries/`
   - Mac: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
3. Reinicia el IDE de Arduino

## 📖 Uso Básico

### Tarea Simple (Auto-loop)

```cpp
#include <CoroutineDSL.h>

CoroutineTask blinkTask;

task(blink)
{
    digitalWrite(LED_BUILTIN, HIGH);
    wait(500);  // Espera 500ms sin bloquear
    
    digitalWrite(LED_BUILTIN, LOW);
    wait(500);
}
endtask

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    blinkTask.start();
}

void loop() {
    blink(blinkTask);  // Ejecuta la tarea
}
```

### Tarea con Retorno

```cpp
CoroutineTask sensorTask;

task_return(int, readSensor)
{
    wait(100);  // Simula tiempo de lectura
    int value = analogRead(A0);
    wait(50);
    endtask_return(value);  // Retorna y termina
}

void loop() {
    if (sensorTask.isRunning()) {
        int value = readSensor(sensorTask);
        Serial.println(value);
        sensorTask.start();  // Reinicia para leer de nuevo
    }
}
```

### Múltiples Tareas

```cpp
CoroutineTask task1, task2, task3;

task(task1) { /* ... */ } endtask
task(task2) { /* ... */ } endtask
task(task3) { /* ... */ } endtask

void loop() {
    task1(task1);
    task2(task2);
    task3(task3);
}
```

## 🎮 Control de Tareas

```cpp
task.start();    // Inicia la tarea
task.stop();     // Detiene la tarea
task.pause();    // Pausa la tarea
task.resume();   // Reanuda la tarea
task.reset();    // Reinicia el contador de programa
task.isRunning(); // Verifica si está ejecutándose
```

## 📁 Ejemplos

El proyecto incluye dos ejemplos:

1. **ExampleWithReturn**: Demuestra el uso de tareas con retorno de valores
2. **TwoTasks**: Muestra cómo ejecutar múltiples tareas concurrentes con control manual

## 🔧 API

### Macros DSL

- `task(name)`: Define una tarea void que se ejecuta en loop
- `task_return(type, name)`: Define una tarea que retorna un valor
- `wait(ms)`: Espera no bloqueante en milisegundos
- `endtask`: Cierra una tarea void (auto-loop)
- `endtask_return(value)`: Cierra una tarea con retorno

### Clase CoroutineTask

- `int pc`: Contador de programa (posición en la tarea)
- `TaskState state`: Estado actual de la tarea
- Métodos de control: `start()`, `stop()`, `pause()`, `resume()`, `reset()`, `isRunning()`

## 📝 Notas

- Las tareas se ejecutan de forma cooperativa (no preemptivas)
- Cada llamada a `wait()` crea un punto de pausa
- Las tareas void se reinician automáticamente al llegar a `endtask`
- Las tareas con retorno se detienen al llegar a `endtask_return()`

## 📄 Licencia

Este proyecto está disponible para uso libre.

## 👤 Autor

Alejandro Kruchowski + Cursor AI


