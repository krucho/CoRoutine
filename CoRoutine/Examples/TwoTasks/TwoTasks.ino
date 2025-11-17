#include <CoroutineDSL.h>

CoroutineTask blinkTask;
CoroutineTask printerTask;

// ----- Tarea 1: Blink -----
task(blink)
{
    digitalWrite(LED_BUILTIN, HIGH);
    wait(500);

    digitalWrite(LED_BUILTIN, LOW);
    wait(500);
}
endtask

// ----- Tarea 2: Mensaje por serial -----
task(printer)
{
    Serial.println("Hola desde printer()");
    wait(1000);
}
endtask

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);

    blinkTask.start();
    printerTask.start();
}

void loop() {
    blink(blinkTask);
    printer(printerTask);

    // Control manual por serial
    if (Serial.available()) {
        char c = Serial.read();

        if (c=='b') blinkTask.pause();
        if (c=='B') blinkTask.resume();

        if (c=='p') printerTask.pause();
        if (c=='P') printerTask.resume();

        if (c=='r') { blinkTask.reset(); printerTask.reset(); }
        if (c=='x') { blinkTask.stop();  printerTask.stop(); }
    }
}
