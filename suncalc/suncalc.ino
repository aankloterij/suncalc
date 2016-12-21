// suncalc - YoloSwag

// The size of the buffer
#define BUFSIZE  128

// The pins on which the data pins of the servos are attached
#define SERVO_AZIMUTH 8
#define SERVO_ALTITUDE 9

// The initial position of the servos
#define AZ_INIT_POS 0
#define AL_INIT_POS 0

Servo az_servo, al_servo;
double azimuth, altitude;
char *strtokbuf, *buffer;
int cursor = 0;
bool is_reset = false;
char delim = '\t';

/**
 * @brief           Reset de servos langzaam in de init position
 */
void resetPos() {

	// Reset niet twee keer
	if(is_reset)
		return;

	for(int i = altitude; i > AZ_INIT_POS; i--)
		az_servo.write(i);

	for(int i = azimuth; i > AL_INIT_POS; i--)
		al_servo.write(i);
}

/**
 * @brief           Zet de zonnepanelen in het prototype in de juiste positie
 *
 * @todo            Stuur de motoren aan
 *
 * @param azimuth   De azimuth
 * @param altitude  De altitude
 */
void fixPanel() {

	// De zon is onder, reset de panels in de orginele positie
	if(altitude < 0) {
		resetPos();
		is_reset = true;
		return;
	}

	// Schrijf de posities naar de servo
	az_servo.write(azimuth);
	al_servo.write(altitude);

	is_reset = false;
}

void setup() {
	// Set the servo pins
	az_servo.attach(SERVO_AZIMUTH);
	al_servo.attach(SERVO_ALTITUDE);

	strtokbuf = (char *) malloc(BUFSIZE);
	buffer = (char *) malloc(BUFSIZE);

	Serial.begin(9600);

	resetPos();

	while(!Serial);
}

void loop() {
	if (Serial.available()) {
		byte incoming = Serial.read();

		if(incoming != '\n') {
			buffer[cursor++] = incoming;
		} else {
			char *first_part = strtok_r(buffer, &delim, &strtokbuf);
			char *second_part = strtok_r(0, &delim, &strtokbuf);

			azimuth = atof(first_part);
			altitude = atof(second_part);

			fixPanel();

			cursor = 0;

			memset(buffer, 0, sizeof(BUFSIZE));
			memset(strtokbuf, 0, sizeof(BUFSIZE));
		}
	}
}
