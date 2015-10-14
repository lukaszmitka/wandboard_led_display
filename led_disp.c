#include <stdio.h>
#include <wandboard_hd44780_gpio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

const char *myarg = NULL;
FILE *rs_pin, *e_pin, *db4_pin, *db5_pin, *db6_pin, *db7_pin;
int init_disp();
int send_4bit_command(int command);
int set_bit(FILE *gpio_file);
int close_files();
int switch_to_4bit();
int init_files();
bool verbose = false;
bool init_display = false;
int opt;
int arg_count;
int i;
int pin_e_duration=50;
int delay_after_high_half=100;
int delay_after_command=4500;
int delay_after_data=100;
int delay_for_state_propagation=10;
int main (int argc, char *argv[])
{
    while ((opt = getopt(argc, argv, "vi")) != -1) { // i: oznacza że wymaga argumentu
        switch (opt) {
        case 'v':
            verbose = true;
            break;
        case 'i':
            //nsecs = atoi(optarg);
            init_display = true;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-v] [-i] firstline [secondline]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    init_files();
    if(init_display){
        init_disp();
        if(verbose){
            printf("Initialising display\n");
        }
    }
    if(verbose){ printf("Start init_disp.\n");}
    
    send_4bit_command(1); // clear display

    arg_count = argc - optind;
    for(i = 0; i<arg_count; i++){
        if(verbose){ printf("Received argument: %s\n", argv[i+optind]);}
        int j = 0;
        myarg = argv[i+optind];
        if(verbose){ printf("My arg: %s \n",myarg);}
        char *zero = "\n";
        while(myarg[j]!= 0){
            if(verbose){ printf("Char %d: %c\n", j, myarg[j]);}
            send_4bit_data(myarg[j]);
            j++;
        }
        send_4bit_command(0b10000000+0x40);
    }

    if(verbose){ printf("Close files.\n");}
    close_files();
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

int init_files(){
    if(verbose){ printf("Opening directory files\n");}
    FILE *rs_dir;
    rs_dir = fopen(RS_PIN_DIR, "w");
    fputs("out", rs_dir);
    fclose(rs_dir);
    if(verbose){ printf("RS set to out\n");}
    FILE *e_dir;
    e_dir = fopen(E_PIN_DIR, "w");
    fputs("out", e_dir);
    fclose(e_dir);
    FILE *db4_dir;
    db4_dir = fopen(DB4_PIN_DIR, "w");
    fputs("out", db4_dir);
    fclose(db4_dir);
    FILE *db5_dir;
    db5_dir = fopen(DB5_PIN_DIR, "w");
    fputs("out", db5_dir);
    fclose(db5_dir);
    FILE *db6_dir;
    db6_dir = fopen(DB6_PIN_DIR, "w");
    fputs("out", db6_dir);
    fclose(db6_dir);
    FILE *db7_dir;
    db7_dir = fopen(DB7_PIN_DIR, "w");
    fputs("out", db7_dir);
    fclose(db7_dir);
    if(verbose){ printf("Open 'value' files\n");}
    rs_pin = fopen(RS_PIN_VALUE, "w");
    e_pin = fopen(E_PIN_VALUE, "w");
    db4_pin = fopen(DB4_PIN_VALUE, "w");
    db5_pin = fopen(DB5_PIN_VALUE, "w");
    db6_pin = fopen(DB6_PIN_VALUE, "w");
    db7_pin = fopen(DB7_PIN_VALUE, "w");

    if(verbose){ printf("Clear all bits.\n");}
    clear_bit(rs_pin);
    clear_bit(e_pin);
    clear_bit(db4_pin);
    clear_bit(db5_pin);
    clear_bit(db6_pin);
    clear_bit(db7_pin);
}

int init_disp(){
    if(verbose){ printf("Switch to 4bit mode\n");}
    switch_to_4bit();
    if(verbose){ printf("Function set, 4bit, 2 lines, 5x7 font\n");}
    send_4bit_command(0b00101000);
    // BLINKING CURSOR
    //if(verbose){ printf("Turn on display, cursor, blinking\n");}
    //send_4bit_command(0b00001111);
    // NO CURSOR
    if(verbose){ printf("Turn on display, cursor off, blinking off\n");}
    send_4bit_command(0b00001100);
    
    if(verbose){ printf("Display initialised\n");}
    return 0;
}

int switch_to_4bit(){
    if(verbose){ printf("Set only 4 bits for switching\n");}
    clear_bit(rs_pin);
    clear_bit(db7_pin);
    clear_bit(db6_pin);
    set_bit(db5_pin);
    clear_bit(db4_pin);
    if(verbose){ printf("Signal\n");}
    usleep(delay_for_state_propagation);
    set_bit(e_pin);
    usleep(pin_e_duration);
    clear_bit(e_pin);
    usleep(delay_after_command);
    if(verbose){ printf("4 bit mode set\n");}
}

int send_4bit_command(int command){
    if(verbose){ printf("Received command: %d\n", command);}
    if(verbose){ printf("Set higher 4 bits\n");}
    clear_bit(rs_pin);
    if((command >> 7) & 1){
        set_bit(db7_pin);
        if(verbose){ printf("1");}
    } else {
        clear_bit(db7_pin);
        if(verbose){ printf("0");}
    }

    if((command >> 6) & 1){
        set_bit(db6_pin);
        if(verbose){ printf("1");}
    } else {
        clear_bit(db6_pin);
        if(verbose){ printf("0");}
    }

    if((command >> 5) & 1){
        set_bit(db5_pin);
        if(verbose){ printf("1");}
    } else {
        clear_bit(db5_pin);
        if(verbose){ printf("0");}
    }

    if((command >> 4) & 1){
        set_bit(db4_pin);
        if(verbose){ printf("1\n");}
    } else {
        clear_bit(db4_pin);
        if(verbose){ printf("0\n");}
    }
    if(verbose){ printf("Signal\n");}
    usleep(delay_for_state_propagation);
    set_bit(e_pin);
    usleep(pin_e_duration);
    clear_bit(e_pin);
    usleep(delay_after_high_half);
    if(verbose){ printf("Set lower 4 bits\n");}
    if((command >> 3) & 1){
        set_bit(db7_pin);
        if(verbose){ printf("1");}
    } else {
        clear_bit(db7_pin);
        if(verbose){ printf("0");}
    }

    if((command >> 2) & 1){
        set_bit(db6_pin);
        if(verbose){ printf("1");}
    } else {
        clear_bit(db6_pin);
        if(verbose){ printf("0");}
    }

    if((command >> 1) & 1){
        set_bit(db5_pin);
        if(verbose){ printf("1");}
    } else {
        clear_bit(db5_pin);
        if(verbose){ printf("0");}
    }

    if(command & 1){
        set_bit(db4_pin);
        if(verbose){ printf("1\n");}
    } else {
        clear_bit(db4_pin);
        if(verbose){ printf("0\n");}
    }
    if(verbose){ printf("Signal\n");}
    usleep(delay_for_state_propagation);
    set_bit(e_pin);
    usleep(pin_e_duration);
    clear_bit(e_pin);
    usleep(delay_after_command);
    return 0;
}

int send_4bit_data(int data){
    set_bit(rs_pin);
    if((data >> 7) & 1){
        set_bit(db7_pin);
    } else {
        clear_bit(db7_pin);
    }

    if((data >> 6) & 1){
        set_bit(db6_pin);
    } else {
        clear_bit(db6_pin);
    }

    if((data >> 5) & 1){
        set_bit(db5_pin);
    } else {
        clear_bit(db5_pin);
    }

    if((data >> 4) & 1){
        set_bit(db4_pin);
    } else {
        clear_bit(db4_pin);
    }

    usleep(delay_for_state_propagation);
    set_bit(e_pin);
    usleep(pin_e_duration);
    clear_bit(e_pin);
    usleep(delay_after_high_half);
    if((data >> 3) & 1){
    set_bit(db7_pin);
    } else {
        clear_bit(db7_pin);
    }

    if((data >> 2) & 1){
        set_bit(db6_pin);
    } else {
        clear_bit(db6_pin);
    }

    if((data >> 1) & 1){
        set_bit(db5_pin);
    } else {
        clear_bit(db5_pin);
    }

    if(1 & data){
        set_bit(db4_pin);
    } else {
        clear_bit(db4_pin);
    }

    usleep(delay_for_state_propagation);
    set_bit(e_pin);
    usleep(pin_e_duration);
    clear_bit(e_pin);
    usleep(delay_after_data);
    return 0;
}

int set_bit(FILE *gpio_file){
    fputs("1", gpio_file);
    rewind(gpio_file);
    return 0;
}

int clear_bit(FILE *gpio_file){
    fputs("0", gpio_file);
    rewind(gpio_file);
    return 0;
}
