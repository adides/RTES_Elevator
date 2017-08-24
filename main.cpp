/*
 * Lab 4: Elevator
 * main.cpp
 * Team members: Aditya Deshpande, Cheng Cheng
 *
*/
#include "mbed.h"
#include "SelectionPanel.h"
#include "Elevator.h"
#include "rtos.h"

/* Interrupt inputs to read the keypad*/
InterruptIn key3(p8);
InterruptIn key5(p7);
InterruptIn key6(p6);

/* IR interrupt input */
InterruptIn ir_in(p18);

/* Digital output pins to drive the keypad inputs */
DigitalOut key1(p10);
DigitalOut key8(p9);

Ticker switch_row_ticker;       // Ticker to switch across the keypad rows
Timer release_timer;            // To check if the key has actually been released
Timer ir_t;

int floor_number;
volatile bool keyPressed;       // Is the key being pressed ?
volatile int tmp;

double frequency = 0;

enum floor {MIDWAY, FIRST, SECOND, THIRD, FOURTH, FIFTH};

Serial pc(USBTX, USBRX);

SelectionPanel<> requests;

Elevator otis(p21,p22,20,p23,p24,p25,1,5);

void ir_rising_edge(){
    double interval = ir_t.read();
    ir_t.stop();
    ir_t.reset();
    if (interval > 0){
        frequency = 1.0 / interval;
    
    }
    ir_t.start();        
}

int get_floor(){
    if (frequency > 80 && frequency < 120) return FIRST;
    else if (frequency > 180 && frequency < 220) return SECOND;
    else if (frequency > 280 && frequency < 320) return THIRD;
    else if (frequency > 380 && frequency < 420) return FOURTH;
    else if (frequency > 480 && frequency < 520) return FIFTH;
    else return MIDWAY;
} 

void reset_measurements(){
    frequency = 0;
    ir_t.stop();
    ir_t.reset();
}

void switch_row(){
    int round = tmp++ % 2;
        switch(round){
            case 0:
                key1 = 0; key8 = 1;
                break;
            case 1:
                key1 = 1; key8 = 0;
                break;
            default:
                break;
        }
}
     

int to_int(char x){
    return x - '0';
}

/* Rising edge interrupt handler for pin 3 of the keypad */
void rise_third_col(){
    keyPressed = true;
    if (key1 == 1){
        floor_number = 4;
    }
    else if (key8 == 1){
        floor_number = 1;
    } 
                        
}


/* Rising edge interrupt handler for pin 5 of the keypad */
void rise_fifth_col(){
    keyPressed = true;
    if (key1 == 1){
        floor_number = 5;
    }
    else if (key8 == 1){
        floor_number = 2;
    }
                           
}

/* Rising edge interrupt handler for pin 6 of the keypad */
void rise_sixth_col(){
    keyPressed = true;
    if (key1 == 1){
        floor_number = 6;
    }
    else if (key8 == 1){
        floor_number = 3;
    }
                           
}

/* Falling edge interrupt handler for pin 6 of the keypad */
void fall_col(){
    keyPressed = false;
}

/* Initialization */
void init(){
    keyPressed = false;
    key1 = 0; key8 = 0;
    tmp = 0;
  
    key3.rise(&rise_third_col);
    key3.fall(&fall_col);
    
    key5.rise(&rise_fifth_col);
    key5.fall(&fall_col);
    
    key6.rise(&rise_sixth_col);
    key6.fall(&fall_col);
    
    switch_row_ticker.attach_us(&switch_row,8000);
    ir_in.rise(&ir_rising_edge);
    ir_in.mode(PullNone);
    ir_t.reset();
    
}    
    

void elevator_thread(void const *args) {
    while(1){
        otis.process_next_request();
        //pc.printf("loop");
    }    

}
 
    
int main() {
    init();
    int req[5];

    int current_floor = 0;
    
    //Thread el_thread(elevator_thread);
    
    while(1) {
        current_floor = get_floor();
        
        if (keyPressed){
            otis.request(floor_number);
        }
        
        otis.update_current_floor(current_floor);
        otis.process_next_request();
        
        for (int i = 0; i < 5; i++){
            req[i] = otis.is_requested(i+1) ? 1: -1;
        }
         
        if (ir_t.read() > 0.2){
            reset_measurements();
        }   
        pc.printf("R: %2d %2d %2d %2d %2d, CF: %d, F: %f\n\r",req[0], req[1], req[2], req[3], req[4], current_floor, frequency);
        //pc.printf("C: %d, F: %.2f\n\r",current_floor, frequency);
    }    
}
      