/*
 * Lab 4 Part 1
 * DCMotor.h
 * Team members: Aditya Deshpande, Cheng Cheng
 *
*/ 
#ifndef DCMOTOR_H
#define DCMOTOR_H

enum direction {CLOCK_WISE, COUNTER_CLOCK_WISE};
        
class DCMotor{
    
    private:
    DigitalOut enable;
    DigitalOut in1;
    DigitalOut in2;
    double speed;
    direction rotation_dir;
    
    public:
    
    DCMotor(DigitalOut m_pin, DigitalOut i1, DigitalOut i2);
    void set_speed(double v);
    double get_speed() const;
    
    void speed_up();
    void speed_down();
    
    void turn_off();
    
    void rotate_clockwise();
    void rotate_anticlockwise();
    int direction() const;
};

DCMotor::DCMotor(DigitalOut m_pin, DigitalOut i1, DigitalOut i2) : enable(m_pin), speed(0), rotation_dir(CLOCK_WISE), in1(i1),in2(i2){
    in1 = 0;
    in2 = 0;
    //enable.period_ms(20);
}

void DCMotor::set_speed(double v){
    if (v > 20) v = 20;
    if (v < 0) v = 0;
    speed = v;
    double duty_cycle = (1.0/20)*speed;
    //enable.write(duty_cycle);
    enable = 1;
}

double DCMotor::get_speed() const{
    return speed;
}

void DCMotor::speed_up(){
    speed++;
    set_speed(speed);
}

void DCMotor::speed_down(){
    speed--;
    set_speed(speed);
}


void DCMotor::turn_off(){
    set_speed(0);
    in1 = 0;
    in2 = 0;
}    
                    
void DCMotor::rotate_clockwise(){
    rotation_dir = CLOCK_WISE;
    in1 = 1;
    in2 = 0;
}

void DCMotor::rotate_anticlockwise(){
    rotation_dir = COUNTER_CLOCK_WISE;
    in1 = 0;
    in2 = 1;
}

int DCMotor::direction() const{
    return rotation_dir;
}    

    
#endif    