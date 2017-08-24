/*
 * Lab 4 Part 1 - Elevator
 * Servo.h
 * Team members: Aditya Deshpande, Cheng Cheng
 *
*/

#ifndef SERVO_H

class Servo{
    private:
    double angle;
    PwmOut servo_pin;
    double period_ms;
    
    public:
    
    Servo(PwmOut pin, double per);
    void set_angle(double ang);
    double get_angle(void) const;
};
 
Servo::Servo(PwmOut pin, double per) : servo_pin(pin), period_ms(per){
    servo_pin.period_ms(period_ms);
    servo_pin.write(0);
}
 
void Servo::set_angle(double ang){
    angle = ang;
    double pulse_width = (1.5 * angle / 180) + 0.75;
    servo_pin.write(pulse_width / period_ms);
}     

double Servo::get_angle(void) const{
    return angle;
}              

#endif