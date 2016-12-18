char buffer[128];
int read = 0;


void fixPanel(double azimuth, double altitude) {
  // fix
  Serial.println("Fixing.. lol");

  Serial.print(azimuth, DEC);
  Serial.print(altitude, DEC);
  Serial.print('\n');
}

void setup() {

	Serial.begin(9600);
	while (!Serial) {
		// wait
	}
}

void loop() {
	if (Serial.available()) {
    if ((*(buffer + read++) = Serial.read()) == '\0') { // prop shit in buffer tot er een 0-byte is

       double azimuth, altitude;

       sscanf("%f\t%f", buffer, &azimuth, &altitude);

       fixPanel(azimuth, altitude);

       memset(buffer, 0, sizeof(buffer)); // clean buffer
       read = 0;
    }
	}
}
