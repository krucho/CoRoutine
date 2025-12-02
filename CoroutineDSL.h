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

// ========== DSL - Task Definitions ==========

// Define una tarea void (auto-loop)
#define task(name) \
    void name(CoroutineTask &__task) { \
        if (!__task.isRunning()) return; \
        switch(__task.pc) { case 0:

// Define una tarea con retorno (se ejecuta una vez)
#define value_task(type, name) \
    type name(CoroutineTask &__task) { \
        static type __result = type{}; \
        if (!__task.isRunning()) return __result; \
        if (__task.pc == 0) __result = type{}; \
        switch(__task.pc) { case 0:

// ========== DSL - Control Flow (for void tasks) ==========

// Delay no bloqueante (para task)
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

// ========== DSL - Control Flow (for value tasks) ==========

// Delay no bloqueante (para value_task)
#define wait_value(ms) \
    do { \
        __task.pc = __LINE__; case __LINE__: \
        if (__task.waitStart == 0) __task.waitStart = millis(); \
        if (millis() - __task.waitStart < (unsigned long)(ms)) return __result; \
        __task.waitStart = 0; \
    } while (0)

// Espera HASTA QUE (para value_task)
#define waitUntil_value(condition) \
    do { \
        __task.pc = __LINE__; case __LINE__: \
        if (!(condition)) return __result; \
    } while (0)

// Espera MIENTRAS (para value_task)
#define waitWhile_value(condition) \
    do { \
        __task.pc = __LINE__; case __LINE__: \
        if (condition) return __result; \
    } while (0)

// Retorna un valor y detiene la tarea
#define task_return(value) \
        __result = (value); __task.pc = 0; __task.state = TASK_STOPPED;

// Cierra la tarea con retorno (solo la llave externa + return dummy para compilador)
#define endtask_value \
        } __task.pc = 0; __task.state = TASK_STOPPED; return __result; \
    }

#endif
