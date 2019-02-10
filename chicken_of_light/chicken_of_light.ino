/**
* Аппаратнозависимые переменные
*/
// Выходные сигналы
/// Пины кнопок
const int pin_panel_LED_day  	  = 2;
const int pin_panel_LED_dawn 	  = 3;
const int pin_panel_LED_sunset 	= 4;
const int pin_panel_LED_night 	= 5;
/// Пины светильника
const int pin_lighter	 		      = 6;
/// Пины лампочек на кнопках
const int pin_LED_day  			= 9;
const int pin_LED_dawn 			= 10;
const int pin_LED_sunset 		= 11;
const int pin_LED_night 		= 12;
//Входные сигналы
///  Кнопки управления режимами
const int pin_button_day  		= A1;
const int pin_button_dawn 		= A2;
const int pin_button_sunset 	= A3;
const int pin_button_night 		= A4;
// Реле времени
const int pin_rele_time 		= 7;
const int pin_rele_light 		= 8;
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
/*class LED{
public:
	LED(bool status=false,int pin_=13);
private:
	bool status_;
	int pin_;	
}*/
/// Переменная состояни конечного автомата
AUTOMAT_STATE light_algorithm_state;
int brightness;
int step;
/// Инициализация 
void setup()  { 
	// выходные пины
	pinMode(pin_panel_LED_day, OUTPUT);
	pinMode(pin_panel_LED_dawn, OUTPUT);
	pinMode(pin_panel_LED_sunset, OUTPUT);
	pinMode(pin_panel_LED_night, OUTPUT);

	pinMode(pin_lighter, OUTPUT);
	
	pinMode(pin_LED_day, OUTPUT);
	pinMode(pin_LED_dawn, OUTPUT);
	pinMode(pin_LED_sunset, OUTPUT);
	pinMode(pin_LED_night, OUTPUT);
	
	// начальное значение выходов
	digitalWrite(pin_panel_LED_day, HIGH ); 
	digitalWrite(pin_panel_LED_dawn, HIGH ); 
	digitalWrite(pin_panel_LED_sunset, HIGH ); 
	digitalWrite(pin_panel_LED_night, HIGH ); 

	analogWrite(pin_lighter, 0);

	digitalWrite(pin_LED_day, HIGH );
	digitalWrite(pin_LED_dawn, HIGH );
	digitalWrite(pin_LED_sunset, HIGH );
	digitalWrite(pin_LED_night, HIGH );

	// входные пины
	pinMode(pin_button_day, INPUT);
	pinMode(pin_button_dawn, INPUT);
	pinMode(pin_button_sunset, INPUT);
	pinMode(pin_button_night, INPUT);

	pinMode(pin_rele_time, INPUT);

	pinMode(pin_rele_light, INPUT);
	
	// подтяжка
	digitalWrite(pin_button_day, HIGH );
	digitalWrite(pin_button_dawn, HIGH );
	digitalWrite(pin_button_sunset, HIGH );
	digitalWrite(pin_button_night, HIGH );

	digitalWrite(pin_rele_time, HIGH );

	digitalWrite(pin_rele_light, HIGH );

	// Начальное состояние конечного автомата
	light_algorithm_state = undefined_state;
	brightness = 0;
	step = 1;
} 
/// Основной рабочий цикл 
void loop()  { 


	switch(light_algorithm_state){								// Проверяем состояние конечного автомата
		case undefined_state:									// если это неопределенное состояние
		{
			brightness = 0;										// яркость устанавливаем в 0
			constrain(brightness,0,254);						// проверяем её на выход за границы - необязательно
			analogWrite(pin_lighter,brightness);				// записываем в порт

			digitalWrite(pin_panel_LED_day, HIGH ); 			// устанавливаем положение лампочек
			digitalWrite(pin_panel_LED_dawn, HIGH ); 
			digitalWrite(pin_panel_LED_sunset, HIGH ); 
			digitalWrite(pin_panel_LED_night, HIGH ); 

			digitalWrite(pin_LED_day, HIGH );
			digitalWrite(pin_LED_dawn, HIGH );
			digitalWrite(pin_LED_sunset, HIGH );
			digitalWrite(pin_LED_night, HIGH );

			if (!(digitalRead(pin_rele_time) == LOW &&			// если включено реле времени
						digitalRead(pin_rele_light) == HIGH)) { // и свет отключен
				light_algorithm_state = sunset_state;			// переходим в рассвет
			} else {
				light_algorithm_state = dawn_state; 			// иначе - переходим в закат
			}
		} break;
		case dawn_state:										// если мы в режиме рассвета
		{
			brightness += step;									// увеличиваем на величину step значение яркости
			constrain(brightness,0,254);						// проверяем на выход за границы
			analogWrite(pin_lighter,brightness);				// записываем значение яркости в порт

			digitalWrite(pin_panel_LED_day, HIGH ); 			// устанавливаем состояние лампочек
			digitalWrite(pin_panel_LED_dawn, LOW ); 
			digitalWrite(pin_panel_LED_sunset, HIGH ); 
			digitalWrite(pin_panel_LED_night, HIGH ); 

			digitalWrite(pin_LED_day, HIGH );
			digitalWrite(pin_LED_dawn, LOW );
			digitalWrite(pin_LED_sunset, HIGH );
			digitalWrite(pin_LED_night, HIGH );
			if(digitalRead(pin_button_night) == LOW) {			// если была нажата кнопка ночь
				light_algorithm_state = sunset_state;			// переводим состояние алгоритма в режим закат
			} else if (digitalRead(pin_button_sunset) == LOW) {	// если же была нажата кнопка закат
				light_algorithm_state = sunset_state;			// переводим состояние алгоритма в режим закат
			} else if (digitalRead(pin_button_day) == LOW) {	// если же была нажата кнопка день
				light_algorithm_state = day_state;				// переводим в состояние день
			} else if (brightness == 254) {						// если мы зажгли нашу лампу на полную
				light_algorithm_state = day_state;				// переводим алгоритм в состояние день
			}  else if (!(digitalRead(pin_rele_time) == LOW &&	// если же не правда, что реле времени включено 
						digitalRead(pin_rele_light) == HIGH)) {	// и реле освещенности выключено
				light_algorithm_state = sunset_state;			// то переходим в состояние закат
			} else if((digitalRead(pin_rele_time) == LOW &&		// если же реле времени включено
						digitalRead(pin_rele_light) == HIGH)) { // и реле освещенности выключено
				light_algorithm_state = dawn_state;				// остаёмся в режиме рассвет
			}
		} break;
		case day_state:
		{
			brightness = 254;
			constrain(brightness,0,254);
			analogWrite(pin_lighter,brightness);

			digitalWrite(pin_panel_LED_day, LOW ); 
			digitalWrite(pin_panel_LED_dawn, HIGH ); 
			digitalWrite(pin_panel_LED_sunset, HIGH ); 
			digitalWrite(pin_panel_LED_night, HIGH ); 

			digitalWrite(pin_LED_day, LOW );
			digitalWrite(pin_LED_dawn, HIGH );
			digitalWrite(pin_LED_sunset, HIGH );
			digitalWrite(pin_LED_night, HIGH );
			if(digitalRead(pin_button_night) == LOW) {
				light_algorithm_state = sunset_state;	
			} else if (digitalRead(pin_button_sunset) == LOW) {
				light_algorithm_state = sunset_state;	
			} else if (digitalRead(pin_button_dawn) == LOW) {
				light_algorithm_state = day_state;
			}  else if (!(digitalRead(pin_rele_time) == LOW &&
						digitalRead(pin_rele_light) == HIGH)) {
				light_algorithm_state = sunset_state;
			} else if((digitalRead(pin_rele_time) == LOW &&
						digitalRead(pin_rele_light) == HIGH)) {
				light_algorithm_state = day_state;
			}
		} break;
		case sunset_state:
		{
			brightness -= step;
			constrain(brightness,0,254);
			analogWrite(pin_lighter,brightness);

			digitalWrite(pin_panel_LED_day, HIGH ); 
			digitalWrite(pin_panel_LED_dawn, HIGH ); 
			digitalWrite(pin_panel_LED_sunset, LOW ); 
			digitalWrite(pin_panel_LED_night, HIGH ); 

			digitalWrite(pin_LED_day, HIGH );
			digitalWrite(pin_LED_dawn, HIGH );
			digitalWrite(pin_LED_sunset, LOW );
			digitalWrite(pin_LED_night, HIGH );
			if(digitalRead(pin_button_night) == LOW) {
				light_algorithm_state = night_state;	
			} else if (digitalRead(pin_button_sunset) == LOW) {
				light_algorithm_state = sunset_state;	
			} else if (digitalRead(pin_button_day) == LOW) {
				light_algorithm_state = dawn_state;	
			} else if (digitalRead(pin_button_dawn) == LOW) {
				light_algorithm_state = dawn_state;	
			} else if (brightness == 0) {
				light_algorithm_state = night_state;	
			} else if (!(digitalRead(pin_rele_time) == LOW &&
						digitalRead(pin_rele_light) == HIGH)) {
				light_algorithm_state = night_state;
			} else if((digitalRead(pin_rele_time) == LOW &&
						digitalRead(pin_rele_light) == HIGH)) {
				light_algorithm_state = dawn_state;
			}
		} break;
		case night_state:
		{
			brightness = 0;
			constrain(brightness,0,254);
			analogWrite(pin_lighter,brightness);

			digitalWrite(pin_panel_LED_day, LOW ); 
			digitalWrite(pin_panel_LED_dawn, HIGH ); 
			digitalWrite(pin_panel_LED_sunset, HIGH ); 
			digitalWrite(pin_panel_LED_night, HIGH ); 

			digitalWrite(pin_LED_day, LOW );
			digitalWrite(pin_LED_dawn, HIGH );
			digitalWrite(pin_LED_sunset, HIGH );
			digitalWrite(pin_LED_night, HIGH );
			if(digitalRead(pin_button_night) == LOW) {
				light_algorithm_state = night_state;	
			} else if (digitalRead(pin_button_sunset) == LOW) {
				light_algorithm_state = sunset_state;	
			} else if (digitalRead(pin_button_dawn) == LOW) {
				light_algorithm_state = dawn_state;
			} else if (digitalRead(pin_button_day) == LOW) {
				light_algorithm_state = dawn_state;
			} else if (!(digitalRead(pin_rele_time) == LOW &&
						digitalRead(pin_rele_light) == HIGH)) {
				light_algorithm_state = night_state;
			} else if((digitalRead(pin_rele_time) == LOW &&
						digitalRead(pin_rele_light) == HIGH)) {
				light_algorithm_state = dawn_state;
			}
		} break;
		default:
			light_algorithm_state = undefined_state; 
	}
  	delay(50);  									// тактируем вызов алгоритма по времени
}
  
