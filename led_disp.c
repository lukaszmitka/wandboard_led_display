#include <stdio.h>
#include <wandboard_hd44780_gpio.h>

FILE *rs_pin, *e_pin, *db4_pin, *db5_pin, *db6_pin, *db7_pin;
int init_disp();
int send_4bit_command(int command);
int set_bit(FILE *gpio_file);
int close_files();

int main (void)
{
	
	init_disp();

	return 0;
}

int close_files(){
	fclose(rs_pin);
	fclose(e_pin);
	fclose(db4_pin);
	fclose(db5_pin);
	fclose(db6_pin);
	fclose(db7_pin);
}

int init_disp(){
	FILE *rs_dir;
	rs_dir = fopen(RS_PIN_DIR, "w");
	fputs("out", rs_dir);
	fclose(rs_dir);

	FILE *e_dir;
	e_dir = fopen(E_PIN_DIR, "w");
	fputs("out", e_dir);
	fclose(e_dir);
	
	FILE *db4_dir;
	db4_dir = fopen(E_PIN_DIR, "w");
	fputs("out", db4_dir);
	fclose(db4_dir);
	
	FILE *db5_dir;
	db5_dir = fopen(E_PIN_DIR, "w");
	fputs("out", db5_dir);
	fclose(db5_dir);
	
	FILE *db6_dir;
	db6_dir = fopen(E_PIN_DIR, "w");
	fputs("out", db6_dir);
	fclose(db6_dir);
	
	FILE *db7_dir;
	db7_dir = fopen(E_PIN_DIR, "w");
	fputs("out", db7_dir);
	fclose(db7_dir);

	rs_pin = fopen(RS_PIN_VALUE, "w");
	e_pin = fopen(E_PIN_VALUE, "w");
	db4_pin = fopen(DB4_PIN_VALUE, "w");
	db5_pin = fopen(DB5_PIN_VALUE, "w");
	db6_pin = fopen(DB6_PIN_VALUE, "w");
	db7_pin = fopen(DB7_PIN_VALUE, "w");

	clear_bit(rs_pin);
	clear_bit(e_pin);
	clear_bit(db4_pin);
	clear_bit(db5_pin);
	clear_bit(db6_pin);
	clear_bit(db7_pin);

	int function_set = 0b00101000;
	send_4bit_command(function_set);
	int display_clear = 0b00000001;
	send_4bit_command(display_clear);
	return 0;
}

int send_4bit_command(int command){
	clear_bit(rs_pin);
	if(!0b10000000 & command){
		set_bit(db7_pin);
	} else {
		clear_bit(db7_pin);
	}

	if(!0b01000000 & command){
		set_bit(db6_pin);
	} else {
		clear_bit(db6_pin);
	}

	if(!0b00100000 & command){
		set_bit(db5_pin);
	} else {
		clear_bit(db5_pin);
	}

	if(!0b00010000 & command){
		set_bit(db4_pin);
	} else {
		clear_bit(db4_pin);
	}

	set_bit(e_pin);
	usleep(1000);
	clear_bit(e_pin);

	usleep(5000);

	if(!0b00001000 & command){
		set_bit(db7_pin);
	} else {
		clear_bit(db7_pin);
	}

	if(!0b00000100 & command){
		set_bit(db6_pin);
	} else {
		clear_bit(db6_pin);
	}

	if(!0b00000010 & command){
		set_bit(db5_pin);
	} else {
		clear_bit(db5_pin);
	}

	if(!0b00000001 & command){
		set_bit(db4_pin);
	} else {
		clear_bit(db4_pin);
	}
	
	set_bit(e_pin);
	usleep(1000);
	clear_bit(e_pin);

	usleep(5000);


	return 0;
}

int set_bit(FILE *gpio_file){
	fputs("1", gpio_file);
	return 0;
}

int clear_bit(FILE *gpio_file){
	fputs("0", gpio_file);
	return 0;
}
