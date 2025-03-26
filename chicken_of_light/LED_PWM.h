#ifndef LED_PWM_H
#define LED_PWM_H

#include "LED.h"
#include "program_parametrs.h"

class LED_PWM : public LED
{
public:
    inline LED_PWM(bool status = false,
                   int npin = 6,
                   bool reverse_logic = false,
                   unsigned long blink_time = BLINK_TIME,
                   float step = PWM_STEP);
    inline bool read();          // считать состояние светодиода
    inline void write(bool st_); // установить состояние светодиода
    inline void write(float pwm_);
    inline void setStep(float step);
    inline float brightness();
    inline bool isMax(float max_ = MAX_PWM);
    inline bool isMin(float min_ = MIN_PWM);
    inline operator float() const;
    inline ~LED_PWM() {}

private:
    float brightness_;
    float step_;
};

LED_PWM::LED_PWM(bool status, int npin, bool reverse_logic, unsigned long blink_time, float step)
    : LED(status, npin, reverse_logic, blink_time)
{
    step_ = step;
    brightness_ = 0.0f;
    this->write(status);
}

bool LED_PWM::read()
{
    if (brightness_ >= MAX_PWM)
        setStatus(true);
    if (brightness_ <= MIN_PWM)
        setStatus(false);

    return getStatus();
}

void LED_PWM::write(bool st_)
{
    brightness_ = getReverseLogic() ? (st_ ? brightness_ -= step_ : brightness_ += step_)
                                    : (st_ ? brightness_ += step_ : brightness_ -= step_);
    constrain(brightness_, MIN_PWM, MAX_PWM);
    analogWrite(getPin(), brightness_);
}

void LED_PWM::write(float pwm_)
{
    brightness_ = pwm_;
    constrain(brightness_, MIN_PWM, MAX_PWM);
    analogWrite(getPin(), static_cast<int>(brightness_));
}

void LED_PWM::setStep(float step)
{
    step_ = step;
}

float LED_PWM::brightness()
{
    return brightness_;
}

bool LED_PWM::isMax(float max_)
{
    return brightness_ >= max_;
}

bool LED_PWM::isMin(float min_)
{
    return brightness_ <= min_;
}

LED_PWM::operator float() const
{
    return brightness_;
}
#endif
