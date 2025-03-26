#ifndef PROGRAM_PARAMETRS_H
#define PROGRAM_PARAMETRS_H

// Выходные сигналы
/// Пины кнопок
const int pin_panel_LED_day             = 2;
const int pin_panel_LED_dawn            = 3;
const int pin_panel_LED_sunset          = 4;
const int pin_panel_LED_night           = 5;
/// Пины светильника
const int pin_lighter                   = 6;
/// Пины лампочек на кнопках
const int pin_LED_day                   = 9;
const int pin_LED_dawn                  = 10;
const int pin_LED_sunset                = 11;
const int pin_LED_night                 = 12;
//Входные сигналы
///  Кнопки управления режимами
const int pin_button_day                = A1;
const int pin_button_dawn               = A2;
const int pin_button_sunset             = A3;
const int pin_button_night              = A4;
/// Реле времени
const int pin_rele_time                 = 7;
const int pin_rele_light                = 8;

/// Настройки программы
const unsigned int BLINK_TIME = 500;        // частота мигания светодиодов - 500 мс
const unsigned int ANTIDEBOUNCE_TIME = 200; // время антидребезга   - 200 мс
const float PWM_STEP   = 0.01f;             // шаг нарастания PWM
const float MAX_PWM    = 255.0f;            // максимальное значение яркости
const float MIN_PWM    = 0.0f;              // минимальное значеие яркости

#endif
