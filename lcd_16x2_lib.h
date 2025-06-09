/**************************************************************************************************

      -----
  PC6|     |PC5                       |   
  PD0|  A  |PC4                       |		LCD HEADER FILE
  PD1|  T  |PC3                       |		Jawad T. Bin Taufik   
  PD2|  M  |PC2                       | 	Version: 1.0
  PD3|  E  |PC1                       |		09th June, 2025
  PD4|  G  |PC0                       | 
  VCC|  A  |GND                       |--------------------------------- 
  GND|  8  |AREF                      |		
  PB6|     |AVCC                      |		CONTROL SHORTCUTS
  PB7|  M  |PB5                       |		FUNCTION PROTOTYPES
  PD5|  C  |PB4                       |		LCD INSTRUCTIONS
  PD6|  U  |PB3                       |		MACROS AND
  PD7|     |PB2                       |		I/O DEFINITONS
  PB0|     |PB1                       |
      -----

  ---------------------------------------------------------------------
 |                                                                     |
 |                              16X2 LCD                               |
 |                                                                     |
  ---------------------------------------------------------------------
                         |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
                         L- L+ D7 D6 D5 D4 D3 D2 D1 D0 E  RW RS CC =5 G
 
 
**************************************************************************************************/

/* Header Guard*/
#ifndef LCD_16X2_LIB_H_
#define LCD_16X2_LIB_H_


#include <avr/io.h>
#include <util/delay.h>


/* I/O Connections, Register Naming */

//rs - instruction/data select
//rw - read/write select
//en - write enable signal
//pwr - feeds power to lcd
//d4-d7 - actual connected data lines
//bf - busy flag location in read register

// control and data line registers
#define ctrl_ddr	DDRD
#define ctrl_port	PORTD

#define data_ddr	DDRC
#define data_port	PORTC
#define input_reg	PINC

// control pins
#define rs	0
#define rw	1
#define en 	2

#define pwr 3

// data pins
#define d4	0
#define d5	1
#define d6	2
#define d7	3

#define bf  7


/* Command Types */
#define _inst	0
#define _data 	1


/* LCD Line Address */
#define line_0_start_addr	0x00
#define line_1_start_addr	0x40


/* Control line shortcuts */
#define rs_hi	(ctrl_port |=  (1<<rs))
#define rs_lo	(ctrl_port &= ~(1<<rs))

#define rw_hi	(ctrl_port |=  (1<<rw))
#define rw_lo	(ctrl_port &= ~(1<<rw))

#define en_hi	(ctrl_port |=  (1<<en))
#define en_lo	(ctrl_port &= ~(1<<en))


/* Useful Macros */
#define clear_data_lines() \
    (data_port &= ~((1<<d4)|(1<<d5)|(1<<d6)|(1<<d7)))

#define wait_ms(x) _delay_ms(x)
#define wait_us(x) _delay_us(x)


/* LCD instructions */
#define __clr_disp		0b00000001		// Replace all characters with ASCII 'space'
#define __ret_home		0b00000010		// Return cursor to first position on first line

#define __em_nds_inc	0b00000110		// no display shift, incrementing cursor >>
#define __em_nds_dec	0b00000100		// no display shift, decrementing cursor <<
#define __em_ds_left	0b00000111		// display shifts to left on data write - scrolling
#define __em_ds_rite	0b00000101		// display shifts to right on data write - scrtolling

#define __d_off			0b00001000		// Turn display off, no cursor, no blink
#define __d_on			0b00001100		// Display on, cursor off, don't blink character
#define __d_c_on		0b00001110		// Display and cursor on, no blink
#define __dcb_on		0b00001111		// Display on with a blinking cursor

#define __shft_c_r		0b00010100		//single shift command, shifts cursor only, to right
#define __shft_c_l		0b00010000		//single shift command, shifts cursor only, to left
#define __shft_d_r		0b00011100		//single shift command, shifts whole display, to right
#define __shft_d_l		0b00011000		//single shift command, shifts whole display, to left

#define __func_8b_00	0b00110000		// 8-bit interface, N=0,F=0, 1-line, 5x7 font 
#define __func_4b_10	0b00101000		// 4-bit interface, 2-line, 5 x 7 font

#define __set_cg_addr	0b01000000		// Set CGRAM address (this is the command)
#define __set_cursor	0b10000000		// Set cursor position(i.e setting DDRAM addr.)


/* Function Prototypes */
void configure_io_lines();						// setup i/o lines and registers
void push_nibble(uint8_t);						// write a 4 bit nibble into the lcd
void push_command(uint8_t, uint8_t);			// write a 8 bit command into lcd
void check_and_wait_for_busy_flag();			// check and handle the busy flag

void lcd_init();								// initialize lcd module
void empty_display();							// empty the whole display
void set_cursor_at(uint8_t, uint8_t);			// set the cursor position
void print_a_char(uint8_t);						// print a single ascii character
void print_string_upto_16_char(const char*);	// print a string upto 16 characters
void print_integer_i32(int32_t);				// print a signed 32 bit integer

void populate_cg_ram_with_segments();			// load custom font information
void write_a_bold_digit(uint8_t, uint8_t);		// write one bold font digit 
void print_bold_int(uint8_t, uint16_t);			// write bold integer upto 9999


#endif /* LCD_16X2_LIB_H_ */















