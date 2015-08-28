#include <stdio.h>
#include <wandboard_hd44780_gpio.h>

FILE *rs_pin, *e_pin, *db4_pin, *db5_pin, *db6_pin, *db7_pin;
int init_disp();
int send_4bit_command(int command);
int set_bit(FILE *gpio_file);

int main (void)
{
	
	init_disp();
	return 0;
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

	int function_set = 0b00101000;
	send_4bit_command(function_set);
	int display_clear = 0b00000001;
	send_4bit_command(display_clear);
	return 0;
}

int send_4bit_command(int command){
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
