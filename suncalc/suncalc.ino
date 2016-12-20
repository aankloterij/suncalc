char buffer[128];
int read;

/**
 * @brief           Zet de zonnepanelen in het prototype in de juiste positie
 *
 * @todo            Stuur de motoren aan
 *
 * @param azimuth   De azimuth
 * @param altitude  De altitude
 */
void fixPanel(double azimuth, double altitude) {

  Serial.println("This is temporary");

  Serial.print(azimuth, DEC);
  Serial.print(altitude, DEC);
  Serial.print('\n');
}

void setup() {
  read = 0;

	Serial.begin(9600);

	while(!Serial); // Volgensmij is dit niet nodig
}

void loop() {
	if (Serial.available()) {
    if ((*(buffer + read++) = Serial.read()) == '\0') { // prop shit in buffer tot er een 0-byte is

       double azimuth, altitude;

       // Parse de serial output en sla ze op in azimuth en altitude
       sscanf("%f\t%f", buffer, &azimuth, &altitude);

       // Stuur de panelen aan
       fixPanel(azimuth, altitude);

       // Empty de buffer
       memset(buffer, 0, sizeof(buffer));

       read = 0;
    }
	}
}
