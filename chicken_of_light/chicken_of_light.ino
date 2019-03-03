/**
* Аппаратнозависимые переменные
*/
// Выходные сигналы
/// Настройки программы
const unsigned int BLINK_TIME = 500;        // частота мигания светодиодов - 500 мс
const unsigned int ANTIDEBOUNCE_TIME = 200; // время антидребезга   - 200 мс
const double PWM_STEP   = 1.0;              // шаг нарастания PWM
const double MAX_PWM    = 255.0;            // максимальное значение яркости
const double MIN_PWM    = 0.0;              // минимальное значеие яркости
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
class LED {
public:
        inline LED(bool status = false,
            int pin = 14,
            bool reverse_logic = false,
            unsigned long blink_time = BLINK_TIME);
        inline operator bool() const;                                  // перегрузка оператора bool
        inline virtual bool read();                                    // считать состояние светодиода
        inline virtual void write(bool st_);                           // установить состояние светодиода
        inline void setReverseLogic();                                 // установить режим обратной логики
        inline void resetReverseLogic();                               // сбросить режим обратной логики
        inline virtual void blink();                                           // установить режим мигания
        inline virtual void setBlinkTime(unsigned long blink_time);            // установить время мигания
        inline virtual ~LED() {}
private:
        int     pin_;                       // пин, к которому подклчен светодиод
        unsigned long blink_time_;          // время мигания светодиода
        unsigned long last_time_;           // предыдущее время
        bool    status_;                    // состояние светодиода - вкл./откл.
        bool    reverse_logic_;             // обратная логика

protected:
        inline bool    getStatus() { return this->status_; }
        inline void    setStatus(bool new_status_) { status_ = new_status_; }
        inline int     getPin() { return this->pin_; }
        inline void    setPin(int new_pin_) { pin_ = new_pin_; }   
        inline bool    getReverseLogic() { return this->reverse_logic_; }
        inline unsigned long getBlinkTime() { return this->blink_time_; }
        inline unsigned long getLastTime()  { return this->last_time_; }

};
class LED_PWM: public LED {
public:
    inline LED_PWM(bool status = false,
            int npin = 6,
            bool reverse_logic = false,
            unsigned long blink_time = BLINK_TIME,
            double step = PWM_STEP);
    inline virtual bool    read();                                    // считать состояние светодиода
    inline virtual void    write(bool st_);                           // установить состояние светодиода
    inline void            write(double pwm_);
    inline void            setStep(double step);
    inline double          brightness();
    inline bool            isMax(double max_ = MAX_PWM);
    inline bool            isMin(double min_ = MIN_PWM);
    inline operator double() const;
    inline virtual ~LED_PWM() {}
private:
    double brightness_;
    double step_;
};

class ZEPPELIN {
public:
        ZEPPELIN(bool status = false,
            int pin = 2,
            bool reverse_logic = false,
            unsigned long anti_debounce_time = ANTIDEBOUNCE_TIME);
        operator bool() const;                          // перегрузка оператора bool
        bool read();                                    // считать состояние цепелина

        void setReverseLogic();                         // установить режим обратной логики
        void resetReverseLogic();                       // сбросить режим обратной логики
        void setAntiDebounce(unsigned long anti_debounce_time = ANTIDEBOUNCE_TIME);
        void resetAntiDebounce();

        bool isChange();                                // состояние входа изменилось
        bool isClicked();                               // кнопка без фиксации состояния - была нажата
        bool isUp();                                    // состояние входа было изменено с нулевого уровня на первый
        bool isDown();                                  // состояние входа было изменено с первого на нулевой уровень
private:
        bool    out_status_;                            // выход функции
        bool    status_;                                // состояние светодиода - вкл./откл.
        int     pin_;                                   // пин, к которому подклчен светодиод
        int     click_count_;                           // количество кликов
        bool    anti_debounce_state_;                   // режим анти-дребезга
        bool    reverse_logic_;                         // обратная логика
        bool    prev_status_;                           // предыдущее значение статуса
        unsigned long anti_debounce_time_;              // время антидребезга
        unsigned long last_time_;                       // переменная времени

        bool    change_;
        bool    click_;
        bool    up_;
        bool    down_;

        void classify();                                // распознавание состояний
};
/// Переменная состояни конечного автомата
static AUTOMAT_STATE light_algorithm_state{undefined_state};

static LED panel_LED_day(false,pin_panel_LED_day);
static LED panel_LED_dawn(false,pin_panel_LED_dawn);
static LED panel_LED_sunset(false,pin_panel_LED_sunset);
static LED panel_LED_night(false,pin_panel_LED_night);

static LED_PWM lighter(false,pin_lighter);

static LED LED_day(false,pin_LED_day);
static LED LED_dawn(false,pin_LED_dawn);
static LED LED_sunset(false,pin_LED_sunset);
static LED LED_night(false,pin_LED_night);

static ZEPPELIN button_day(false,pin_button_day);
static ZEPPELIN button_dawn(false,pin_button_dawn);
static ZEPPELIN button_sunset(false,pin_button_sunset);
static ZEPPELIN button_night(false,pin_button_night);

static ZEPPELIN rele_time(false,pin_rele_time);
static ZEPPELIN rele_light(false,pin_rele_light);
/// Инициализация 
void setup()  { 
    // уже проведена в конструкторах
} 
/// Основной рабочий цикл 
void loop()  { 

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

                        if (!(  rele_time.read() == false &&                            // если включено реле времени
                                rele_light.read()== true )) {                           // и свет отключен
                                light_algorithm_state = sunset_state;           // переходим в рассвет
            } else {
                                light_algorithm_state = dawn_state;             // иначе - переходим в закат
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
                        if( button_night.read() == false
                            && button_night.isChange() ) {                                      // если была нажата кнопка ночь
                                light_algorithm_state = sunset_state;                           // переводим состояние алгоритма в режим закат
                        } else if (button_sunset.read() == false
                                   && button_sunset.isChange()) {                               // если же была нажата кнопка закат
                                light_algorithm_state = sunset_state;                           // переводим состояние алгоритма в режим закат
                        } else if (button_day.read() == false
                                   && button_day.isChange()) {                                  // если же была нажата кнопка день
                light_algorithm_state = day_state;              // переводим в состояние день
                        } else if (lighter.isMax()) {                       // если мы зажгли нашу лампу на полную
                light_algorithm_state = day_state;              // переводим алгоритм в состояние день
                        }  else if (!(  rele_time.read() == false &&                            // если включено реле времени
                                        rele_light.read()== true )&&
                                       (rele_time.isChange() || rele_light.isChange())) {       // и реле освещенности выключено
                                light_algorithm_state = sunset_state;                           // то переходим в состояние закат
                        } else if(    rele_time.read() == false &&                              // если включено реле времени
                                      rele_light.read()== true  &&
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
                        if( button_night.read() == false
                            && button_night.isChange() ) {
                light_algorithm_state = sunset_state;   
                        } else if (button_sunset.read() == false
                                   && button_sunset.isChange()) {
                light_algorithm_state = sunset_state;   
                        } else if (button_dawn.read() == false
                                   && button_dawn.isChange()) {
                light_algorithm_state = day_state;
                        } else if (!(  rele_time.read() == false &&                            // если включено реле времени
                                       rele_light.read()== true )&&
                                      (rele_time.isChange() || rele_light.isChange())) {
                light_algorithm_state = sunset_state;
                        } else if(    rele_time.read() == false &&                            // если включено реле времени
                                      rele_light.read()== true  &&
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

                        if( button_night.read() == false
                            && button_night.isChange() ) {
                light_algorithm_state = night_state;    
                        } else if (button_sunset.read() == false
                                   && button_sunset.isChange()) {
                light_algorithm_state = sunset_state;   
                        } else if (button_day.read() == false
                                   && button_day.isChange()) {
                light_algorithm_state = dawn_state; 
                        } else if (button_dawn.read() == false
                                   && button_dawn.isChange()) {
                light_algorithm_state = dawn_state; 
                        } else if (lighter.isMin()) {
                light_algorithm_state = night_state;    
                        } else if (!(  rele_time.read() == false &&                            // если включено реле времени
                                       rele_light.read()== true )&&
                                      (rele_time.isChange() || rele_light.isChange())) {
                light_algorithm_state = night_state;
                        } else if(rele_time.read() == false &&                            // если включено реле времени
                                  rele_light.read()== true  &&
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

                        if(button_night.read() == false
                           && button_night.isChange()) {
                light_algorithm_state = night_state;    
                        } else if (button_sunset.read() == false
                                   && button_sunset.isChange()) {
                light_algorithm_state = sunset_state;   
                        } else if (button_dawn.read() == false
                                   && button_dawn.isChange()) {
                light_algorithm_state = dawn_state;
                        } else if (button_day.read() == false
                                   && button_day.isChange()) {
                light_algorithm_state = dawn_state;
                        } else if (!(  rele_time.read() == false &&                            // если включено реле времени
                                       rele_light.read()== true )&&
                                      (rele_time.isChange() || rele_light.isChange())) {
                light_algorithm_state = night_state;
                        } else if(rele_time.read() == false &&                            // если включено реле времени
                                  rele_light.read()== true  &&
                                 (rele_time.isChange() || rele_light.isChange())) {
                light_algorithm_state = dawn_state;
            }
        } break;
        default:
            light_algorithm_state = undefined_state; 
    }
    delay(50);                                      // тактируем вызов алгоритма по времени
}


LED::LED(bool status, int pin, bool reverse_logic, unsigned long blink_time)
{
    status_ = status;
    pin_ = pin;
    reverse_logic_ = reverse_logic;
    blink_time_ = blink_time;
    last_time_ = millis();

    pinMode(pin_, OUTPUT);
    this->write(status);
}


LED::operator bool() const
{
    return status_;
}

bool LED::read()
{
    //status_ = digitalRead(pin_);
    return status_;
}

void LED::write(bool st_)
{
    digitalWrite(pin_, st_ ? (reverse_logic_ ? LOW : HIGH )
                           : (reverse_logic_ ? HIGH : LOW ));
    status_ = st_;
}

void LED::setReverseLogic()
{
    reverse_logic_ = true;
}

void LED::resetReverseLogic()
{
    reverse_logic_ = false;
}

void LED::blink()
{
    if(millis()-last_time_ > blink_time_) {
        write(!(read()));
        last_time_ = millis();
    }
}

void LED::setBlinkTime(unsigned long blink_time)
{
    blink_time_ = blink_time;
}

ZEPPELIN::ZEPPELIN(bool status, int pin, bool reverse_logic, unsigned long anti_debounce_time)
{
    status_ = status;
    pin_ = pin;
    reverse_logic_ = reverse_logic;
    anti_debounce_time_ = anti_debounce_time;

    last_time_ = millis();
    pinMode(pin_, INPUT);
    digitalWrite(pin_, HIGH );
    status_ = this->read();
    prev_status_ = status_;
    click_count_ = 0;
}


bool ZEPPELIN::read()
{
     status_ = reverse_logic_ ? digitalRead(pin_) == HIGH : digitalRead(pin_) == LOW;
     classify();
     prev_status_ = status_;
     return out_status_;
}

void ZEPPELIN::setReverseLogic()
{
    reverse_logic_ = true;
}

void ZEPPELIN::resetReverseLogic()
{
    reverse_logic_ = false;
}

void ZEPPELIN::setAntiDebounce(unsigned long anti_debounce_time)
{
    anti_debounce_time_ = anti_debounce_time;
}

void ZEPPELIN::resetAntiDebounce()
{
    anti_debounce_time_ = 0;
}

bool ZEPPELIN::isChange()
{
    return change_;
}

bool ZEPPELIN::isClicked()
{
    return click_;
}

bool ZEPPELIN::isUp()
{
    return up_;
}

bool ZEPPELIN::isDown()
{
    return down_;
}

void ZEPPELIN::classify()
{
    change_ = status_ ^ prev_status_;            // change

    if(change_){                                 // up and down
        up_     = (!prev_status_)&&(status_);
        down_   = (prev_status_)&&(!status_);
    }

    if(up_){
        click_ = true;
        click_count_ ++;
    }


    if(change_){
       last_time_ = millis();
       anti_debounce_state_ = true;
    }

    if(millis() - last_time_ > anti_debounce_time_ && anti_debounce_time_){
       out_status_ = status_;
       change_ = true;              // костыльно, но всё же...
       anti_debounce_time_ = false;
    }

    if(!anti_debounce_state_){
        out_status_ = status_;
    }

}

ZEPPELIN::operator bool() const
{
    return out_status_;
}

LED_PWM::LED_PWM(bool status, int npin, bool reverse_logic, unsigned long blink_time, double step)
: LED (status,npin,reverse_logic,blink_time)
{
    step_ = step ;                    
    brightness_ = 0.0 ;
    this->write(status);
}

bool LED_PWM::read()
{
    if(brightness_ >= MAX_PWM) setStatus(true);
    if(brightness_ <= MIN_PWM) setStatus(false);

    return getStatus();
}

void LED_PWM::write(bool st_)
{
    brightness_ = getReverseLogic() ? (st_ ? brightness_ -= step_ : brightness_ += step_)
                                    : (st_ ? brightness_ += step_ : brightness_ -= step_);
    constrain(brightness_,MIN_PWM,MAX_PWM);
    analogWrite(getPin(),brightness_);
}

void LED_PWM::write(double pwm_)
{
    brightness_ = pwm_;
    constrain(brightness_,MIN_PWM,MAX_PWM);
    analogWrite(getPin(),static_cast<int>(brightness_));
}

void LED_PWM::setStep(double step)
{
    step_ = step;
}

double LED_PWM::brightness()
{
    return  brightness_;
}

bool LED_PWM::isMax(double max_)
{
    return brightness_ >= max_;
}

bool LED_PWM::isMin(double min_)
{
    return brightness_ <= min_;
}

LED_PWM::operator double() const
{
    return  brightness_;
}
