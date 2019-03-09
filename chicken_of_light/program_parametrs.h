#ifndef PROGRAM_PARAMETRS_H
#define PROGRAM_PARAMETRS_H

/// Настройки программы
extern const unsigned int BLINK_TIME = 500;        // частота мигания светодиодов - 500 мс
extern const unsigned int ANTIDEBOUNCE_TIME = 200; // время антидребезга   - 200 мс
extern const double PWM_STEP   = 1.0;              // шаг нарастания PWM
extern const double MAX_PWM    = 255.0;            // максимальное значение яркости
extern const double MIN_PWM    = 0.0;              // минимальное значеие яркости

#endif
