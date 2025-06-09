///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
//                  C LIBRARY | HD44780 IC BASED 16X2 LCD | TARGET AVR ATMEGA8                   //
//               ----------------------------------------------------------------                //
//                 AUTHOR: JAWAD T. BIN TAUFIK | JUNE 09TH, 2025 | VERSION: 1.0                  //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "lcd_16x2_lib.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// custom bold digits


#define MT 32  //ascii code for empty space


uint8_t segments[8][8] = {
    {0b00011100, 0b00011110, 0b00011110, 0b00011110, 0b00011110, 0b00011110, 0b00011110, 0b00011100}, // seg_1
    {0b00000111, 0b00001111, 0b00001111, 0b00001111, 0b00001111, 0b00001111, 0b00001111, 0b00000111}, // seg_2
    {0b00011111, 0b00011111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00011111}, // seg_3
    {0b00011110, 0b00011100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011000, 0b00011100}, // seg_4
    {0b00001111, 0b00000111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b00000111}, // seg_5
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00011111}, // seg_6
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b00001111}, // seg_7
    {0b00011111, 0b00011111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}  // seg_8
};


uint8_t digits[10][6] = {
    { 1, 7, 0, 1, 5, 0 },     // 0
    {MT,MT, 0,MT,MT, 0 },     // 1
    { 4, 2, 0, 1, 5, 5 },     // 2
    { 4, 2, 0, 6, 5, 0 },     // 3
    { 1, 5, 0,MT,MT, 0 },     // 4
    { 1, 2, 3, 6, 5, 0 },     // 5
    { 1, 2, 3, 1, 5, 0 },     // 6
    { 1, 7, 0,MT,MT, 0 },     // 7
    { 1, 2, 0, 1, 5, 0 },     // 8
    { 1, 2, 0, 6, 5, 0 }      // 9
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// lcd functions - hardware driver


void configure_io_lines() {
	ctrl_ddr |= ((1<<rs) | (1<<rw) | (1<<en) | (1<<pwr));
	data_ddr |= ((1<<d4) | (1<<d5) | (1<<d6) | (1<<d7));
}


void push_nibble(uint8_t nib) {
	clear_data_lines(); //func-like macro

	if(nib & 0x10) data_port |= (1 << d4);
	if(nib & 0x20) data_port |= (1 << d5);
	if(nib & 0x40) data_port |= (1 << d6);
	if(nib & 0x80) data_port |= (1 << d7);

    en_hi;          // Enable pin high
    wait_us(1);     // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
    en_lo;          // Enable pin low
    wait_us(1);		// implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
}


void push_command(uint8_t cmd_type, uint8_t cmd_byte) {
	en_lo; rw_lo; //writing

	switch(cmd_type) {
	case _inst: 
		rs_lo; break;
	case _data: 
		rs_hi; break;
	default: 
		break;
	}

	wait_us(1);
	push_nibble(cmd_byte); 
	push_nibble(cmd_byte<<4); //msb to lsb in 2 nibbles
}


void check_and_wait_for_busy_flag() {
	data_ddr &= ~(1<<d7); 
	uint8_t busy_flag = 0;
	en_lo;
	rs_lo;
	rw_hi;
	wait_us(1);

	do {
		busy_flag = 0;
		en_hi; 
		wait_us(1); 
		busy_flag |= input_reg & (1<<d7); // could be ORed
		en_lo; 
		wait_us(1);

		// dummy read for bits 3~0 - discard
		en_hi; 
		wait_us(1); 
		en_lo; 
		wait_us(1);
	} while(busy_flag);

	//come here when lcd has released busy state

	data_ddr |= (1<<d7);
	rw_lo; //writing mode
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// lcd functions - application level


void lcd_init() {
	configure_io_lines(); 
	wait_ms(100); 
	ctrl_port |= (1<<pwr); // feed power voltage to lcd

	// intialize by instruction - page 45 ===========================
	wait_ms(50); 
	en_lo; 
	rs_lo; 
	rw_lo; 
	push_nibble(__func_8b_00);
	wait_ms(10);  
	push_nibble(__func_8b_00);
	wait_us(200); 
	push_nibble(__func_8b_00);
	wait_us(100); 
	push_nibble(__func_4b_10); //after this, bf can be checked

	check_and_wait_for_busy_flag(); 
	push_command(_inst, __func_4b_10);
	check_and_wait_for_busy_flag(); 
	push_command(_inst, __d_off);
	check_and_wait_for_busy_flag(); 
	push_command(_inst, __clr_disp);
	check_and_wait_for_busy_flag(); 
	push_command(_inst, __em_nds_inc);
	//================================================================

	wait_us(100);
	check_and_wait_for_busy_flag(); 
	push_command(_inst, __d_on);
	wait_ms(100);
}


void empty_display() {
	check_and_wait_for_busy_flag(); 
	push_command(_inst, __clr_disp);
	check_and_wait_for_busy_flag(); 
	push_command(_inst, __ret_home);
}


void set_cursor_at(uint8_t line_num, uint8_t col_num) {
	check_and_wait_for_busy_flag();
	switch(line_num) {
	case 0: 
		push_command(_inst, __set_cursor|(line_0_start_addr + col_num));
		break;
	case 1: 
		push_command(_inst, __set_cursor|(line_1_start_addr + col_num));
		break;
	 default: 
	 	break; 
	}
}


void print_a_char(uint8_t in_char) {
	push_command(_data, in_char);
}


void print_string_upto_16_char(const char *in_str) {
	//after the 16th, characters are discarded
	uint8_t count = 0;
	while(*in_str && count < 16) { //until *in_str='\0'=0x00
		check_and_wait_for_busy_flag();
		print_a_char(*in_str++);
		count++;
	}
}


void print_integer_i32(int32_t in_int) { //prints upto +- 2billion
	// % 10 gets the last digit of a number
	// '0' + digit turns the digit into its ASCII character (e.g., 3 → '3').
	// buf[i++] stores that character in the buffer.
	// num /= 10 removes the last digit.

    char buf[11]; // Max 10 digits for u32 type + '\0', max"+-2147483648"
    uint8_t i = 0;
    uint32_t val;

    if (in_int == 0) {
        print_a_char('0');
        return;
    }

    // If negative, print '-' and convert to absolute value using unsigned
    if (in_int < 0) {
        print_a_char('-');
        val = (uint32_t)(-in_int);  // Safe: -2147483648 → 2147483648
    } 
    else {
        val = (uint32_t)in_int;
    }

    // Convert number to string in reverse
    while (val > 0 && i < sizeof(buf) - 1) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }

    // Print in correct (reverse again) order
    while (i--) {
        print_a_char(buf[i]);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// lcd functions - custom bold fonts for digits


void populate_cg_ram_with_segments() {
    uint8_t cg_addr=0, row=0;
    
    for(cg_addr = 0; cg_addr < 8; cg_addr++) {
        check_and_wait_for_busy_flag();
        push_command(_inst, __set_cg_addr + (cg_addr*8)); // Set CGRAM address
        for (row = 0; row < 8; row++) {
            print_a_char(segments[cg_addr][row]);
            _delay_us(100);
        }
    }
}


void write_a_bold_digit(uint8_t col, uint8_t dgt) {
    uint8_t i=0;
    for(i=0;i<3;i++) {
        set_cursor_at(0, col+i); 
        print_a_char(digits[dgt][i]);
        set_cursor_at(1, col+i); 
        print_a_char(digits[dgt][3+i]);
    }
}


//always starts printing from line 0
void print_bold_int(uint8_t column, uint16_t val) { //from 0 to 9999

    uint8_t buf[4] = {0,0,0,0};
    uint8_t i = 0, more_than_2_digits = 0;
	if(val>99) more_than_2_digits = 1; 

    if (val < 0) {return;} 
    while (val > 0) {
        buf[i++] = val % 10;
        val /= 10;
    } 

    if(more_than_2_digits) {
    	write_a_bold_digit(column+0, buf[3]);
	    write_a_bold_digit(column+3, buf[2]);
	    write_a_bold_digit(column+6, buf[1]);
	    write_a_bold_digit(column+9, buf[0]);
    }
    else {
     	write_a_bold_digit(column+0, buf[1]);
    	write_a_bold_digit(column+3, buf[0]);
    }
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
//           ____  ____  ________  ____  __________   ___________  ______________  _   __        //
//          / __ \/ __ \/ ____/ / / /  |/  / ____/ | / /_  __/   |/_  __/  _/ __ \/ | / /        //
//         / / / / / / / /   / / / / /|_/ / __/ /  |/ / / / / /| | / /  / // / / /  |/ /         //
//        / /_/ / /_/ / /___/ /_/ / /  / / /___/ /|  / / / / ___ |/ / _/ // /_/ / /|  /          //
//       /_____/\____/\____/\____/_/  /_/_____/_/ |_/ /_/ /_/  |_/_/ /___/\____/_/ |_/           //
//                                                                                               //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////


/**************************************************************************************************

IR - isnt code, dis clr, cursor shift, addr for ddram, cgram, only can be written from mpu
DR - temp sto for data to be written in ddram cgram, to read from ddram cgram
aft data read, data in ddram ang cgram at next addr is sent to dr for the next read.

busy flag - 1, internal op mode, no instruction will be accepted. 
rs=0 && r/w = 1, busy flag set to DB7, write info only when busy flag =0 is ensured 

addr counter - assigns addr to both ddram and cgram
write into ddram n cgram AC++ and reading AC-- wutomatically
rs-rw = 00-ir write, 01-read flags and ac, 10-dr write ddram/cgram, 11-dr read ddram/cgram

display shift operation, shifts ddram address - page 11
cgrom - predefined 208 5x8 chars, cgram - 8 programmable 5x8 chars

The HD44780U can send data in either two 4-bit operations or one 8-bit operation, thus allowing
interfacing with 4- or 8-bit MPUs.

for 4 bit op, to send a 8 bit byte, the four high order bits (for 8-bit operation,
DB4 to DB7) are transferred before the four low order bits (for 8-bit operation, DB0 to DB3).

The busy flag must be checked (one instruction) after the 4-bit data has been transferred twice. Two
more 4-bit operations then transfer the busy flag and address counter data.

disp resets on power boot cycle, starts with: 10ms BusyFlag kept busy then BF=0
dsp clr, funcset,DL=1,8bit interface, N=0,1 line dsp, F=0; 5x8font (for N=1,f=*) 
DCB=0,dsp,cur,bnk=off, entrymode:I/D=1,inc by1,S=0,no shift

MPU only controls IR and DR. instructions falls into 4 categories: page 24***
• Designate HD44780U functions, such as display format, data length, etc.
• Set internal RAM addresses
• Perform data transfer with internal RAM
• Perform miscellaneous functions

Instructions in Table 6*************
0x20 = space

Control the cursor or character position by setting the DDRAM address using the 
Set DDRAM Address command (0b1xxxxxxx, or 0x80 | address).
lcd_command(0x80 | 0x40);  // Set DDRAM address to 0x40

When N is 1 (2-line display),
AAAAAAA can be 00H to 27H for the first line, and 40H to 67H for the second line.

S/C=0,shifts cursor,1,shifts entire display R/L=0,to left,1,to right

To write into CG or DDRAM, first set the appropriate address.  After a write, 
the address is automatically incremented or decremented by 1 according to the entry mode. 
The entry mode also determines the display shift

**************************************************************************************************/
