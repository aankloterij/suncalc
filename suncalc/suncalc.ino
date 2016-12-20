char buffer[128];

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

	Serial.begin(9600);

	while(!Serial); // Volgensmij is dit niet nodig
}

int cursor = 0;

void loop() {
  if (Serial.available()) {
    byte incoming = Serial.read();

    if(incoming != '\n') {
      
      buffer[cursor++] = incoming;
    } else {
      double azimuth, altitude;

      sscanf("%f\t%f", buffer, &azimuth, &altitude);

      fixPanel(azimuth, altitude);

      cursor = 0;

      memset(buffer, 0, sizeof(buffer));
    }
  }
}
