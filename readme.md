# 0. О программе

  chicken_of_light - прошивка для платы aruino nano, которое реализует управление светильником.

# 1. Структура репозитория

    \chicken_of_light - каталог, содержащий в себе основной код прошивки;  
    \ - корневой каталог;  

# 2. Системные требования
   
	* ОС Windows 7 или выше;  
	* Arduino IDE 1.8.6 или выше;  

	Примечание: возможно использование вместо Arduino IDE AVR-компилятора языка C++ в сочетании с программой для прошивки контроллера.
	
# 3. Установка прошивки

   Для установки прошивки необходимо:  
    1) Установить Arduino IDE.  
    2) Открыть файл .\chicken_of_light\chicken_of_light.ino в среде Arduino IDE.  
    3) В разделе "Инструменты"->"Плата" выбрать поле "Arduino Nano".  
    4) В разделе "Инструменты"->"Порт" выбрать порт, через который будет производиться прошивка платы arduino nano.  
    5) В разделе "Скетч" выбрать пункт загрузка и дождаться окончания загрузки.  
   

# 4. Настройка программы

   Порты, к которым подключается оборудование указано в начале программы и могут быть заменены на другие.  
   Примечание: наименование портов начинается со слова "pin_".  
   Изначальный шаг увеличения значения яркости при работе устанавливается в функции setup (переменная step).
   Изначальное состояние алгоритма - неопределенное (undefined_state). Начальное состояние яркости(brightness) является нулевым.  
   
# 5. Инструкция по управлению

   Программа запускается автоматически и может функционировать в одном из 5 режимов:  
   - неопределенное состояние;  
   - режим рассвет;  
   - режим день;  
   - режим закат;  
   - режим ночь.  
   
## 5.1 Режим "Неопределенный"  

   Начальное состояние программы - неопределенное.
   Из неопределенного состояния она переходит в зависимости от состояния реле времени и освещенности в режим "Рассвет" или в режим "Закат".  
   
## 5.2 Режим "Рассвет"  

   В режиме "Рассвет" значение переменной brightness изменяется от 0 до 254 с шагом step. При достижении значения 254, программа переходит в режим "День".
   Если в режиме работы "Рассвет" была нажата кнопка "День", "Ночь", "Закат" или изменилось состояние реле времени, или реле освещенности, то происходит переход в соответствующий режим.  
   
  
## 5.3 Режим "День"  

   В режиме "День" значение переменной brightness соответсвует 254.
   Если в режиме работы "День" была нажата кнопка "День", "Рассвет", "Ночь", "Закат" или изменилось состояние реле времени, или реле освещенности, то происходит переход в соответствующий режим.  
   
## 5.4 Режим "Закат"  

   В режиме "Закат" значение переменной brightness изменяется от 254 до 0 с шагом step. При достижении значения 0, программа переходит в режим "Ночь".
   Если в режиме работы "Закат" была нажата кнопка "День","Рассвет", "Ночь", "Закат" или изменилось состояние реле времени, или реле освещенности, то происходит переход в соответствующий режим.  
   
## 5.3 Режим "Ночь"  

   В режиме "Ночь" значение переменной brightness соответсвует 0.
   Если в режиме работы "Ночь" была нажата кнопка "День", "Рассвет", "Ночь", "Закат" или изменилось состояние реле времени, или реле освещенности, то происходит переход в соответствующий режим.  
   