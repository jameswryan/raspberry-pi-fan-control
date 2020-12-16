#include <stdio.h>
#include <pigpiod_if2.h>
#include <stdlib.h>
#include <unistd.h>

#define CPU_TEMP_THRESH 40
#define FAN_OFF 0
#define FAN_ON 1

double get_cpu_temp(void)
{
	const char* PATH = "/sys/class/thermal/thermal_zone0/temp";
	const char* MODES = "r";

	FILE* fp = fopen(PATH, MODES);
	if(!fp) {
		fprintf(stderr, "Unable to open file!");
		exit(EXIT_FAILURE);
	}
	
	// Get line from file, store in buffer
	char* line = NULL;
	size_t bufsize = 0;
	if(getline(&line, &bufsize, fp) == -1) {
		if(feof(fp)) {
		} else {
			perror("Error reading from file!");
			exit(EXIT_FAILURE);
		}
	}

	//get numerical value, scale to degrees celsius
	double temp_raw = atof(line);
	free(line);
	double temp_c = temp_raw / 1000;

	return temp_c;
}

int main() 
{
	// Connect to pigpio daemon
	int pi = pigpio_start(NULL, NULL);
	if(pi < 0) {
		fprintf(stderr, "Failed to connect to daemon!");
		exit(EXIT_FAILURE);
	}

	// Set GPIO 17 to output
	if(set_mode(pi, 17, PI_OUTPUT) != 0) {
		fprintf(stderr, "Unable to set GPIO to output!\n");
		exit(EXIT_FAILURE);
	}

	// Get cpu temp
	double cpu_temp = get_cpu_temp();
	
	// Decide whether to turn fan on
	int fan_mode = 0;
	if(cpu_temp > CPU_TEMP_THRESH)
		fan_mode = FAN_ON;
	else
		fan_mode = FAN_OFF;
	
	// Set GPIO pin level
	if(gpio_write(pi, 17, fan_mode) != 0) {
		fprintf(stderr, "Unable to write GPIO pin!");
		exit(EXIT_FAILURE);
	}

	// Disconnect from pigpio daemon
	pigpio_stop(pi);

	exit(EXIT_SUCCESS);
}
