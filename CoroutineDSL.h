#ifndef COROUTINE_DSL_H
#define COROUTINE_DSL_H

#include <Arduino.h>

enum TaskState {
    TASK_STOPPED,
    TASK_RUNNING,
    TASK_PAUSED
};

class CoroutineTask {
public:
    int pc = 0;              
    TaskState state = TASK_STOPPED;
    unsigned long waitStart = 0; // Timestamp for non-blocking delays

    void start()  { pc = 0; state = TASK_RUNNING; waitStart = 0; }
    void stop()   { state = TASK_STOPPED; }
    void pause()  { if (state == TASK_RUNNING) state = TASK_PAUSED; }
    void resume() { if (state == TASK_PAUSED) state = TASK_RUNNING; }
    void reset()  { pc = 0; waitStart = 0; }

    bool isRunning() { return state == TASK_RUNNING; }
};

// ========== DSL ==========

// Define una tarea void (auto-loop)
#define task(name) \
    void name(CoroutineTask &__task) { \
        if (!__task.isRunning()) return; \
        switch(__task.pc) { case 0:

// Define una tarea con retorno (se ejecuta una vez)
#define task_return(type, name) \
    type name(CoroutineTask &__task) { \
        static type __result = type{}; \
        if (!__task.isRunning()) return __result; \
        if (__task.pc == 0) __result = type{}; \
        switch(__task.pc) { case 0:

// Delay no bloqueante (Improved: uses object state instead of static)
#define wait(ms) \
    do { \
        __task.pc = __LINE__; case __LINE__: \
        if (__task.waitStart == 0) __task.waitStart = millis(); \
        if (millis() - __task.waitStart < (unsigned long)(ms)) return; \
        __task.waitStart = 0; \
    } while (0)

// Espera HASTA QUE la condición sea verdadera (Yield si es falsa)
#define waitUntil(condition) \
    do { \
        __task.pc = __LINE__; case __LINE__: \
        if (!(condition)) return; \
    } while (0)

// Espera MIENTRAS la condición sea verdadera (Yield si es verdadera)
#define waitWhile(condition) \
    do { \
        __task.pc = __LINE__; case __LINE__: \
        if (condition) return; \
    } while (0)

// Cierra la tarea void y vuelve a empezar (auto-loop)
#define endtask \
        } __task.pc = 0; __task.waitStart = 0; return; \
    }

// Cierra la tarea con retorno (se ejecuta una vez)
#define endtask_return(value) \
        } __result = (value); __task.pc = 0; __task.state = TASK_STOPPED; return __result; \
    }

#endif
