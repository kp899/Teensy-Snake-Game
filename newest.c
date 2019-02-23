#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sprite.h"
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"

/*
* Useful defines you can use in your system time calculations
*/
#define FREQUENCY 8000000.0
#define PRESCALER 1024.0

Sprite snake[25];
Sprite food;
int collision_check;
int lives = 5;
int score = 0;
float x = LCD_X/2,y = LCD_Y/2;//X and Y values which will be changed when the joystick is moved
float dx,dy; // Displacement vector for the x and y position
int snake_index = 0;//
int snake_length = 2;//Starting length is 2 adn is incremented when food is eaten
int snake_speed = 3;//speed to add to dx and dy values
float fx = LCD_X/2 -4,fy = LCD_Y/2 -8;

    static unsigned char snakepart [] = {
        0b11111111,
        0b11111111,
        0b11111111,
    };
    static unsigned char foodimage [] ={
        0b11111111,
        0b11111111,
        0b11111111,
    };

void display_name(){
     draw_string(20,15,"n9643885");
     draw_string(15,25,"Komalpreet S");
     show_screen();
     _delay_ms(2000);
     clear_screen();
}
void initialise_snake(){
    for(int i =0;i<snake_length;i++){
    init_sprite(&snake[i],x,y,3,3,snakepart);
    }
}
void draw_snake(){
    for(int i =0;i<snake_length;i++){
        draw_sprite(&snake[i]);
    }
}
// int collision(int sanke_length){
    
    // for(int i=0;i<snake_length; i++){
        // if (round(fx) -2 >= round(snake[i].x) && round(fy) -2 >= round(snake[i].y)){
            // collision_check = 1;
        // }
        // else{
            // collision_check = 0;
        // }
    // }
    // return collision_check;
// }
void initialise_food(){

    if (round(x) >= round(food.x - 2) && round(x) <= round(food.x + 2) &&
        round(y) >= round(food.y - 2) && round(y) <= round(food.y + 2)){
        collision_check ++;
        score += 1;
        //snake_length +=1;
        food.x = rand()%LCD_X-1;
        food.y = rand()%LCD_Y-1;
        
    }
}
void setup(void){
    char sco[80];
    char liv[80];
    char coll[80];
    sprintf(coll, "%02d",collision_check);
    draw_string(24,35, coll);
    sprintf(sco, "%02d", score);
    draw_string(0,0, sco);
    draw_string(12,0,"(");
    draw_string(28,0,")");
    sprintf(liv,"%02d", lives);
    draw_string(18,0,liv);
    }
int main(void){
    set_clock_speed(CPU_8MHz);    
    //initialise the LCD screen
    lcd_init(LCD_DEFAULT_CONTRAST); 
    PORTC |= (1 << PC7);
    display_name();
    initialise_snake();
    init_sprite(&food,fx,fy,3,3,foodimage);
    
    //draw_sprite(&food);
    while(1){
        clear_screen();
        setup();
    
        if (snake_index >= snake_length){
            snake_index = 0;
        }
        //button configuration for movement
            //Make the snake move left
            if(PINB & 0b00000010){
                dx = -snake_speed;
                dy = 0;
            }
            if(PIND & 0b00000001){
                dx = snake_speed;
                dy = 0;
            }
            if(PIND & 0b00000010){
                dx = 0;
                dy = -snake_speed;
            }
            if(PINB & 0b10000000){
                dx = 0;
                dy = snake_speed;
            }
            // move the x and y on every process of the loop
            x += dx;    
            y += dy;

            initialise_food();

            // moving one piece at a time in the train of snake pieces
            // to the location of x and y
            
            snake[snake_index].x = x;
            snake[snake_index].y = y;

            snake_index += 1; // step through the array of snake pieces

            _delay_ms(100);
            
            //collision(snake_length);
            draw_sprite(&food);
            draw_snake();
            show_screen();
        
    }
}