#include <avr/io.h>
#include <util/delay.h>
#include "lcd_16x2_lib.h"

uint16_t P_int=0, T_int=0, H_int=0;


int main(void) { 
    _delay_ms(10); 
    lcd_init(); 

    _delay_ms(10);
    empty_display();

    populate_cg_ram_with_segments();

    T_int = 24; 
    H_int = 53; 
    P_int = 1017;

    while (1) {
        set_cursor_at(0, 0); 
        print_string_upto_16_char("Weather Monitor"); 
        set_cursor_at(1, 2); 
        print_string_upto_16_char("Version 1.0"); 

        _delay_ms(2000); 
        empty_display();

        print_bold_int(0, T_int);

        set_cursor_at(1, 6); 
        print_a_char('C');
        
        print_bold_int(9, H_int);

        set_cursor_at(1, 15); 
        print_a_char('%');
        
        _delay_ms(2000); 
        empty_display(); 

        print_bold_int(0, P_int);
        
        set_cursor_at(1, 13); 
        print_string_upto_16_char("hPa"); 
        
        _delay_ms(2000); 
        empty_display();
    }
}

