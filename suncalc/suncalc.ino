char buffer[128];
int cursor;

/**
 * @brief           Zet de zonnepanelen in het prototype in de juiste positie
 *
 * @todo            Stuur de motoren aan
 *
 * @param azimuth   De azimuth
 * @param altitude  De altitude
 */
void fixPanel(double azimuth, double altitude) {
  Serial.print(azimuth, DEC);
  Serial.print("\t");
  Serial.print(altitude, DEC);
  Serial.print('\n');
}

char delim = '\t';

char *strtokbuf;

void setup() {
        cursor = 0;
        
        strtokbuf = (char *) malloc(128);

	Serial.begin(9600);

	while(!Serial); // Volgensmij is dit niet nodig
}

void loop() {
  if (Serial.available()) {
    byte incoming = Serial.read();

    if(incoming != '\n') {
      buffer[cursor++] = incoming;
    } else {
      double azimuth, altitude;
      
      char *first_part = strtok_r(buffer, &delim, &strtokbuf);
      char *second_part = strtok_r(0, &delim, &strtokbuf);
      
      azimuth = atof(first_part);
      altitude = atof(second_part);
      
      fixPanel(azimuth, altitude);

      cursor = 0;

      memset(buffer, 0, sizeof(buffer));
    }
  }
}
