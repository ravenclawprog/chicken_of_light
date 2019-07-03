#include "LED.h"
#include "ZEPPELIN.h"
#include "LED_PWM.h"
#include "program_parametrs.h"
#include <map>
#include <string>

/**
* Аппаратнозависимые переменные
*/
// Выходные сигналы
/*const int pin_LED[] = {2,3,4,5,9,10,11,12};       // ToDo: заменить на pair?
const int pin_rele[] = {7,8};
const int pin_button[] = {A1,A2,A3,A4};
const int pin_lighter = 6;*/
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

list<pair<string, AUTOMAT_STATE> > s_state = {{"undefined",undefined_state},
                                                {"dawn",dawn_state},
                                                {"day",day_state},
                                                {"sunset",sunset_state},
                                                {"night",night_state}};
list<pair<string, int> > s_LED = { {"panel LED day",pin_panel_LED_day} ,
                   {"panel LED dawn", pin_panel_LED_dawn},
                   {"panel LED sunset", pin_panel_LED_sunset},
                   {"panel LED night",pin_panel_LED_night},
                   {"LED day" ,pin_LED_day},
                   {"LED dawn" ,pin_LED_dawn},
                   {"LED sunset" ,pin_LED_sunset},
                   {"LED night", pin_LED_night} };
list<pair<string, int> > s_button = { {"button day", pin_button_day},
                      {"button dawn", pin_button_dawn},
                      {"button sunset", pin_button_sunset},
                      {"button night",pin_button_night} };
list<pair<string, int> > s_rele = { {"rele time", pin_rele_time},
                    {"rele light",pin_rele_light};
// Ассоциативный массив LED-лампочек
map<string, LED> m_LED;
// Ассоциативный массив кнопок
map<string, ZEPPELIN> m_button;
// Ассоциативный массив реле
map<string, ZEPPELIN> m_rele;
// Переменная класс светильника
LED_PWM lighter(false,pin_lighter);

/// Переменная состояния алгоритма
 AUTOMAT_STATE light_algorithm_state{undefined_state};

// Функция чтения состояний - входных сигналов
// Функция использует глобальные переменные m_button и m_rele для вызова данных для считывания
void readButtonAndRele();
// Функция записи состояния LED-лампочек и светильника - выходных сигналов
// Функция использует глобальные переменные m_LED, lighter, light_algorithm_state
void writeLEDAndLEDPWM();
// Функция осуществляет переход из одного состояния алгоритма в другой при помощи сигналов от светильника, кнопок, реле и предыдущего состояния
// Функция использует глобальные переменные lighter, light_algorithm_state,m_button и m_rele
void algorithmStateJump();

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
    for(list<pair<string, int>>::iterator i_list = s_LED.begin();
       i_list != s_LED.end(); ++i_list){
      m_LED.insert( pair<string,LED> ( *i_list.first(), LED( false,*i_list.second() ) ) );
    }
    for(list<pair<string, int>>::iterator i_list = s_button.begin();
       i_list != s_LED.end(); ++i_list) {
      m_button.insert( pair<string,LED> ( *i_list.first(), ZEPPELIN( false,*i_list.second() ) ) );
    }
    for(list<pair<string, int>>::iterator i_list = s_rele.begin();
       i_list != s_LED.end(); ++i_list) {
      m_button.insert( pair<string,LED> ( *i_list.first(), ZEPPELIN( false,*i_list.second() ) ) );
    }
} 
/// Основной рабочий цикл 
void loop()  { 

    delay(50);                                      // тактируем вызов алгоритма по времени
}

void readButtonAndRele() {
  // чтение состояний кнопок
  for(map<string,ZEPPELIN>::iterator i_map = m_button.begin();
    i_map == m_button.end(); ++i_map){
      i_map->second.read();
  }
  // чтение состояний реле
  for(map<string,ZEPPELIN>::iterator i_map = m_button.begin();
    i_map == m_button.end(); ++i_map){
      i_map->second.read();
  }
}

void writeLEDAndLEDPWM() {
  switch(light_algorithm_state) {                                 // Проверяем состояние конечного автомата
        case undefined_state:                                     // если это неопределенное состояние
        {
              lighter.write(false);
              for(map<string,LED>::iterator i_map = m_LED.begin();
              i_map == m_LED.end(); ++i_map) {
                  i_map->second.write(false);
              }
        } break;
        case dawn_state:                                          // режим рассвет
        {
              lighter.write(true);
              for(map<string,LED>::iterator i_map = m_LED.begin();
              i_map == m_LED.end(); ++i_map) {
                if(i_map->first.find("dawn")==!=std::string::npos)
                  i_map->second.write(false);
                else
                  i_map->second.blink();
              }
        } break;
        case day_state:                                           // режим день
        {
              lighter.write(true);
              for(map<string,LED>::iterator i_map = m_LED.begin();
              i_map == m_LED.end(); ++i_map) {
                if(i_map->first.find("day")==!=std::string::npos)
                  i_map->second.write(false);
                else
                  i_map->second.write(true);
              }
        } break;
        case sunset_state:                                        // режим закат
        {
              lighter.write(false);
              for(map<string,LED>::iterator i_map = m_LED.begin();
              i_map == m_LED.end(); ++i_map) {
                if(i_map->first.find("sunset")==!=std::string::npos)
                  i_map->second.write(false);
                else
                  i_map->second.blink();
              }
        } break;
        case night_state:                                         // режим ночь
        {
              lighter.write(false);
              for(map<string,LED>::iterator i_map = m_LED.begin();
              i_map == m_LED.end(); ++i_map) {
                if(i_map->first.find("night")==!=std::string::npos)
                  i_map->second.write(false);
                else
                  i_map->second.write(true);
              }
        } break;
        case default:                                             // не может быть, но всё же
        {
              lighter.write(false);
              for(map<string,LED>::iterator i_map = m_LED.begin();
              i_map == m_LED.end(); ++i_map) {
                  i_map->second.write(false);
              }
        } break;
   }
}

void algorithmStateJump()
{
  switch(light_algorithm_state){                              // Проверяем состояние конечного автомата
        case undefined_state:                                 // если это неопределенное состояние
        {
          if (!(  m_rele["rele time"] == false &&             // если включено реле времени
                  m_rele["rele light"]== true ))              // и свет отключен
              light_algorithm_state = night_state;            // переходим в ночь
          else 
              light_algorithm_state = dawn_state;             // иначе - переходим в рассвет
        } break;
        case dawn_state,day_state,sunset_state,night_state:
        {
          if( m_button["button night"] == false
              && m_button["button night"].isChange() ) {                                      // если была нажата кнопка ночь
              light_algorithm_state = sunset_state;                           // переводим состояние алгоритма в режим закат
          } else if (m_button["button sunset"] == false
                     && m_button["button sunset"].isChange()) {                               // если же была нажата кнопка закат
              light_algorithm_state = sunset_state;                           // переводим состояние алгоритма в режим закат
          } else if (m_button["button day"] == false
                     && m_button["button day"].isChange()) {                                  // если же была нажата кнопка день
              light_algorithm_state = dawn_state;              // переводим в состояние день
          } else if (m_button["button dawn"] == false
                     && m_button["button dawn"].isChange()) {                                  // если же была нажата кнопка день
              light_algorithm_state = dawn_state;              // переводим в состояние день
          } else if (lighter.isMax() 
                    && light_algorithm_state == dawn_state) {                       // если мы зажгли нашу лампу на полную
              light_algorithm_state = day_state;              // переводим алгоритм в состояние день
          } else if (lighter.isMin() 
                    && light_algorithm_state == sunset_state) {
              light_algorithm_state = night_state;
          } else if (!(m_rele["rele time"] == false &&                            // если включено реле времени
                       m_rele["rele light"]== true )&&
                      (m_rele["rele time"].isChange() || 
                       m_rele["rele light"].isChange())) {       // и реле освещенности выключено
              light_algorithm_state = sunset_state;                           // то переходим в состояние закат
          } else if (  m_rele["rele time"] == false &&                              // если включено реле времени
                       m_rele["rele light"]== true  &&
                    (  m_rele["rele time"].isChange() ||
                       m_rele["rele light"].isChange())) {         // и реле освещенности выключено
              light_algorithm_state = dawn_state;             // остаёмся в режиме рассвет
          }
        } break;
  }      
}