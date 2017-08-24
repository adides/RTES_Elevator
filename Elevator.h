/*
 * Lab 4: Elevator
 * Elevator.h
 * Team members: Aditya Deshpande, Cheng Cheng
 *
*/
#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "SelectionPanel.h"
#include "Servo.h"
#include "DCMotor.h"

Serial pcx(USBTX, USBRX);

enum elevator_direction { UP, DOWN};

class Elevator{
    
    private:
    int current_floor;
    bool doors_open;
    bool in_motion;
    bool processing_floor;
    int last_floor;
        
    elevator_direction direction;    
    const int MAX_FLOOR;
    
    SelectionPanel<> selection_panel;
    Servo door1;
    Servo door2;
    DCMotor dm1;
    
    void process_floor();
    
    public:
    
    Elevator(PwmOut s1, PwmOut s2, double per, DigitalOut m_pin, DigitalOut i1, DigitalOut i2, int cur_floor, int num_floors);
    
    void open_doors();
    void close_doors();
    
    void up();
    void down();
    void stop();
    
    void request(int el);
    void update_current_floor(int el);
    bool is_requested(int n);
    void process_next_request();
};



Elevator::Elevator(PwmOut s1, PwmOut s2, double per, DigitalOut m_pin, DigitalOut i1, DigitalOut i2, int cur_floor, int num_floors) : door1(s1,per), door2(s2,per), dm1(m_pin,i1,i2),current_floor(cur_floor), MAX_FLOOR(num_floors){
    doors_open = false;
    in_motion = false;
    processing_floor = false;
    direction = UP;
    last_floor = 0;
}
    
void Elevator::open_doors(){
    if(!in_motion){
        doors_open = true;
        door1.set_angle(0);
        door2.set_angle(0);
    }
}

void Elevator::close_doors(){
    door1.set_angle(90);
    door2.set_angle(90);

    doors_open = false;
}
    
void Elevator::up(){
    if (!doors_open){
        in_motion = true;
        dm1.set_speed(18);
        dm1.rotate_clockwise();
        direction = UP;
    }    
}

void Elevator::down(){
    if (!doors_open){
        in_motion = true;
        dm1.set_speed(18);
        dm1.rotate_anticlockwise();
        direction = DOWN;
    }                        
}

void Elevator::stop(){
    dm1.turn_off();
    in_motion = false;            
}

void Elevator::request(int el){
    selection_panel.select(el);   
}

void Elevator::update_current_floor(int el){

        current_floor = el;
 
}

void Elevator::process_floor(){
    //pcx.printf("CURRENT FLOOR: %d\n\r", current_floor);
    if (selection_panel.is_selected(current_floor)){
        stop();
        open_doors();
        wait(2);
        close_doors();
        selection_panel.unselect(current_floor);    
    }
    last_floor = current_floor;    
}

bool Elevator::is_requested(int n){
    return selection_panel.is_selected(n);
}
    
void Elevator::process_next_request(){

    
    if (selection_panel.none_selected()) return;
    
    //pcx.printf("Current Floor: %d ",current_floor);
    if (current_floor > 0 && current_floor <= MAX_FLOOR){
       //pcx.printf("process\n\r");
       process_floor();
    }   
    
    if (!in_motion){     
    
    
    if (direction == DOWN){
        pcx.printf("DOWN1\n\r");
        for (int i = last_floor-1; i >= 1; i--){
            if (selection_panel.is_selected(i)){
                 down();
                 return;
            }     
        }
    }
    
    if (direction == UP){
        pcx.printf("UP1\n\r");
        for (int i = last_floor+1; i <= MAX_FLOOR; i++){
            if (selection_panel.is_selected(i)){
                 up();
                 return;
            }     
        }
    }
    if (direction == UP){
        pcx.printf("UP2\n\r");
        for (int i = last_floor-1; i >= 1; i--){
            if (selection_panel.is_selected(i)){
                 down();
                 return;
            }     
        }
    }
    
    if (direction == DOWN){
        pcx.printf("DOWN2\n\r");
        for (int i = last_floor+1; i <= MAX_FLOOR; i++){
            if (selection_panel.is_selected(i)){
                 up();
                 return;
            }     
        }
    }  
    }else{
        if (direction == UP && current_floor == MAX_FLOOR){
            stop();
        }
        else if (direction == DOWN && current_floor == 1){
            stop();
        }        
     }   
        
                  
}                         
#endif
 