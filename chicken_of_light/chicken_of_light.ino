#include "LED.h"
#include "ZEPPELIN.h"
#include "LED_PWM.h"
#include "program_parametrs.h"
/**
* Аппаратнозависимые переменные
*/
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
/**
* Программные переменные
*/
/// Тип - состояние конечного автомата
enum AUTOMAT_STATE{
    undefined_state = 0,
    day_state,
    dawn_state,
    sunset_state,
    night_state
};

/// Переменная состояни конечного автомата
 AUTOMAT_STATE light_algorithm_state{undefined_state};

 LED panel_LED_day(false,pin_panel_LED_day);
 LED panel_LED_dawn(false,pin_panel_LED_dawn);
 LED panel_LED_sunset(false,pin_panel_LED_sunset);
 LED panel_LED_night(false,pin_panel_LED_night);

 LED_PWM lighter(false,pin_lighter);

 LED LED_day(false,pin_LED_day);
 LED LED_dawn(false,pin_LED_dawn);
 LED LED_sunset(false,pin_LED_sunset);
 LED LED_night(false,pin_LED_night);

 ZEPPELIN button_day(false,pin_button_day);
 ZEPPELIN button_dawn(false,pin_button_dawn);
 ZEPPELIN button_sunset(false,pin_button_sunset);
 ZEPPELIN button_night(false,pin_button_night);

 ZEPPELIN rele_time(false,pin_rele_time, true);
 ZEPPELIN rele_light(false,pin_rele_light, true);
/// Инициализация 
void setup()  { 
    // уже проведена в конструкторах
} 
/// Основной рабочий цикл 
void loop()  { 
	rele_light.read();
	rele_time.read();
	button_day.read();
	button_dawn.read();
	button_night.read();
	button_sunset.read();
    switch(light_algorithm_state){                              // Проверяем состояние конечного автомата
                case undefined_state:                               // если это неопределенное состояние
        {
                        lighter.write(false);

                        panel_LED_day.write(false);
                        panel_LED_dawn.write(false);
                        panel_LED_sunset.write(false);
                        panel_LED_night.write(false);

                        LED_day.write(false);
                        LED_dawn.write(false);
                        LED_sunset.write(false);
                        LED_night.write(false);

                        if (!(  rele_time == false &&                            // если включено реле времени
                                rele_light== true )) {                           // и свет отключен
                                light_algorithm_state = sunset_state;           // переходим в закат
            } else {
                                light_algorithm_state = dawn_state;             // иначе - переходим в рассвет
            }
        } break;
                case dawn_state:                            // если мы в режиме рассвета
        {
                        lighter.write(true);

                        panel_LED_day.write(false);
                        panel_LED_dawn.blink();//panel_LED_dawn.write(true);
                        panel_LED_sunset.write(false);
                        panel_LED_night.write(false);

                        LED_day.write(false);
                        panel_LED_dawn.blink();//LED_dawn.write(true);
                        LED_sunset.write(false);
                        LED_night.write(false);
                        if( button_night == false
                            && button_night.isChange() ) {                                      // если была нажата кнопка ночь
                                light_algorithm_state = sunset_state;                           // переводим состояние алгоритма в режим закат
                        } else if (button_sunset == false
                                   && button_sunset.isChange()) {                               // если же была нажата кнопка закат
                                light_algorithm_state = sunset_state;                           // переводим состояние алгоритма в режим закат
                        } else if (button_day == false
                                   && button_day.isChange()) {                                  // если же была нажата кнопка день
                light_algorithm_state = day_state;              // переводим в состояние день
                        } else if (lighter.isMax()) {                       // если мы зажгли нашу лампу на полную
                light_algorithm_state = day_state;              // переводим алгоритм в состояние день
                        }  else if (!(  rele_time == false &&                            // если включено реле времени
                                        rele_light== true )&&
                                       (rele_time.isChange() || rele_light.isChange())) {       // и реле освещенности выключено
                                light_algorithm_state = sunset_state;                           // то переходим в состояние закат
                        } else if(    rele_time == false &&                              // если включено реле времени
                                      rele_light== true  &&
                                     (rele_time.isChange() || rele_light.isChange())) {         // и реле освещенности выключено
                light_algorithm_state = dawn_state;             // остаёмся в режиме рассвет
            }
        } break;
        case day_state:
        {
                        lighter.write(MAX_PWM);

                        panel_LED_day.write(true);
                        panel_LED_dawn.write(false);
                        panel_LED_sunset.write(false);
                        panel_LED_night.write(false);

                        LED_day.write(true);
                        LED_dawn.write(false);
                        LED_sunset.write(false);
                        LED_night.write(false);
                        if( button_night == false
                            && button_night.isChange() ) {
                light_algorithm_state = sunset_state;   
                        } else if (button_sunset == false
                                   && button_sunset.isChange()) {
                light_algorithm_state = sunset_state;   
                        } else if (button_dawn == false
                                   && button_dawn.isChange()) {
                light_algorithm_state = day_state;
                        } else if (!(  rele_time == false &&                            // если включено реле времени
                                       rele_light== true )&&
                                      (rele_time.isChange() || rele_light.isChange())) {
                light_algorithm_state = sunset_state;
                        } else if(    rele_time == false &&                            // если включено реле времени
                                      rele_light== true  &&
                                     (rele_time.isChange() || rele_light.isChange())) {
                light_algorithm_state = day_state;
            }
        } break;
        case sunset_state:
        {
                        lighter.write(false);

                        panel_LED_day.write(false);
                        panel_LED_dawn.write(false);
                        panel_LED_sunset.blink();//panel_LED_sunset.write(true);
                        panel_LED_night.write(false);

                        LED_day.write(false);
                        LED_dawn.write(false);
                        LED_sunset.blink();//LED_sunset.write(true);
                        LED_night.write(false);

                        if( button_night == false
                            && button_night.isChange() ) {
                light_algorithm_state = night_state;    
                        } else if (button_sunset == false
                                   && button_sunset.isChange()) {
                light_algorithm_state = sunset_state;   
                        } else if (button_day == false
                                   && button_day.isChange()) {
                light_algorithm_state = dawn_state; 
                        } else if (button_dawn == false
                                   && button_dawn.isChange()) {
                light_algorithm_state = dawn_state; 
                        } else if (lighter.isMin()) {
                light_algorithm_state = night_state;    
                        } else if (!(  rele_time == false &&                            // если включено реле времени
                                       rele_light== true )&&
                                      (rele_time.isChange() || rele_light.isChange())) {
                light_algorithm_state = night_state;
                        } else if(rele_time == false &&                            // если включено реле времени
                                  rele_light== true  &&
                                 (rele_time.isChange() || rele_light.isChange())) {
                light_algorithm_state = dawn_state;
            }
        } break;
        case night_state:
        {
                        lighter.write(MIN_PWM);

                        panel_LED_day.write(false);
                        panel_LED_dawn.write(false);
                        panel_LED_sunset.write(false);
                        panel_LED_night.write(true);

                        LED_day.write(false);
                        LED_dawn.write(false);
                        LED_sunset.write(false);
                        LED_night.write(true);

                        if(button_night == false
                           && button_night.isChange()) {
                light_algorithm_state = night_state;    
                        } else if (button_sunset == false
                                   && button_sunset.isChange()) {
                light_algorithm_state = sunset_state;   
                        } else if (button_dawn == false
                                   && button_dawn.isChange()) {
                light_algorithm_state = dawn_state;
                        } else if (button_day == false
                                   && button_day.isChange()) {
                light_algorithm_state = dawn_state;
                        } else if (!(  rele_time == false &&                            // если включено реле времени
                                       rele_light== true )&&
                                      (rele_time.isChange() || rele_light.isChange())) {
                light_algorithm_state = night_state;
                        } else if(rele_time == false &&                            // если включено реле времени
                                  rele_light== true  &&
                                 (rele_time.isChange() || rele_light.isChange())) {			
                light_algorithm_state = dawn_state;
            }
        } break;
        default:
            light_algorithm_state = undefined_state; 
    }
    delay(50);                                      // тактируем вызов алгоритма по времени
}
