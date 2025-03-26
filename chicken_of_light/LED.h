#ifndef LED_H
#define LED_H

#include "program_parametrs.h"

class LED
{
public:
    inline LED(bool status = false,                     // начальное состояние, записывается в порт
               int pin = 14,                            // порт, в который будет происходить запись
               bool reverse_logic = false,              // начальное значение обратной логики
               unsigned long blink_time = BLINK_TIME);  // установка значение мигания по умолчанию
    inline operator bool() const;                       // перегрузка оператора bool - возвращает состояние
    inline bool read();                                 // считать состояние светодиода, возвращает значение переменной, не читает данные с порта
    inline void write(bool st_);                        // установить состояние светодиода, записывает значение в порт
    inline void setReverseLogic();                      // установить режим обратной логики, не вызывает перезаписи в порт
    inline void resetReverseLogic();                    // сбросить режим обратной логики, не вызывает перезаписи в порт
    inline void blink();                                // установить режим мигания
    inline void setBlinkTime(unsigned long blink_time); // установить время мигания
    inline ~LED() {}

private:
    int pin_;                  // пин, к которому подклчен светодиод
    unsigned long blink_time_; // время мигания светодиода, мс
    unsigned long last_time_;  // предыдущее время, мс
    bool status_;              // состояние светодиода - вкл./откл. - true - вкл, false - откл. (всегда)
    bool reverse_logic_;       // флаг использования обратной логики - влияет только на запись

protected:
    // геттеры и сеттеры для наследования
    inline bool getStatus() { return this->status_; }
    inline void setStatus(bool new_status_) { status_ = new_status_; }
    inline int getPin() { return this->pin_; }
    inline void setPin(int new_pin_) { pin_ = new_pin_; }
    inline bool getReverseLogic() { return this->reverse_logic_; }
    inline unsigned long getBlinkTime() { return this->blink_time_; }
    inline unsigned long getLastTime() { return this->last_time_; }
};

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
    // status_ = digitalRead(pin_);
    return status_;
}

void LED::write(bool st_)
{
    digitalWrite(pin_, st_ ? (reverse_logic_ ? LOW : HIGH)
                           : (reverse_logic_ ? HIGH : LOW));
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
    if (millis() - last_time_ > blink_time_)
    {
        write(!(read()));
        last_time_ = millis();
    }
}

void LED::setBlinkTime(unsigned long blink_time)
{
    blink_time_ = blink_time;
}
#endif
