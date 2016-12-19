#include <math.h>

#ifndef Arduino_h
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	#include <string.h>
	#include <unistd.h>
#endif

#ifndef PI
#define PI (M_PI)
#endif

#define DEG2RAD (PI / 180)
#define RAD2DEG (180 / PI)
#define SECONDS_IN_DAY ((double) (24 * 3600))

struct direction {
	double azimuth;
	double altitude;
};

int J1970 = 2440588,
	J2000 = 2451545;


// coordinaten van remmers' huis (google maps)
double lat = 53.181634;
double lng = 6.541645;

double M0 = 357.5291 * DEG2RAD,
	M1 = 0.98560028 * DEG2RAD,
	J0 = 0.0009,
	J1 = 0.0053,
	J2 = -0.0069,
	C1 = 1.9148 * DEG2RAD,
	C2 = 0.0200 * DEG2RAD,
	C3 = 0.0003 * DEG2RAD,
	P = 102.9372 * DEG2RAD,
	e = 23.45 * DEG2RAD,
	th0 = 280.1600 * DEG2RAD,
	th1 = 360.9856235 * DEG2RAD,
	h0 = -0.83 * DEG2RAD, //sunset angle
	d0 = 0.53 * DEG2RAD, //sun diameter
	h1 = -6 * DEG2RAD, //nautical twilight angle
	h2 = -12 * DEG2RAD, //astronomical twilight angle
	h3 = -18 * DEG2RAD; //darkness angle


double timestampToJulianDate(int timestamp) {
	return timestamp / SECONDS_IN_DAY + J1970 - 0.5;
}

double getSolarMeanAnomaly(double Js) {
	return M0 + M1 * (Js - J2000);
}

double getEquationOfCenter(double M) {
	return C1 * sin(M) + C2 * sin(2 * M) + C3 * sin(3 * M);
}

double getEclipticLongtitude(double M, double C) {
	return M + P + C + PI;
}

double getSunDeclination(double Lsun) {
	return asin(sin(Lsun) * sin(e));
}

double getRightAscension(double Lsun) {
	return atan2(sin(Lsun) * cos(e), cos(Lsun));
}

double getSiderealTime(double J, double lw) {
	return th0 + th1 * (J - J2000) - lw;
}

double getAzimuth(double th, double a, double phi, double d) {
	double H = th - a;
	return atan2(sin(H), cos(H) * sin(phi) - tan(d) * cos(phi));
}

double getAltitude(double th, double a, double phi, double d) {
	double H = th - a;
	return asin(sin(phi) * sin(d) + cos(phi) * cos(d) * cos(H));
}

/**
 * Rekent de positie van de zon uit op de manier waarop suncalc dit doet.
 *
 * @see suncalc.net
 *
 * @param  pos        struct waarin de hoek komt
 * @param  timestamp  UNIX timestamp voor het moment waar de hoek wordt uitgerekend
 * @param  latitude   breedtegraad
 * @param  longtitude lengtegraad
 *
 * @return            -1 als er geen lege richtingsstruct mee is gegeven, 0 op success
 */
int getSunPosition(struct direction* pos, int timestamp, double latitude, double longtitude) {
	if (pos == 0) return -1;

	double J = timestampToJulianDate(timestamp);
	double phi = latitude * DEG2RAD;
	double lw = longtitude * -DEG2RAD;

	double M = getSolarMeanAnomaly(J);
	double C = getEquationOfCenter(M);
	double Lsun = getEclipticLongtitude(M, C);
	double d = getSunDeclination(Lsun);
	double a = getRightAscension(Lsun);
	double th = getSiderealTime(J, lw);

	// TODO
	pos->azimuth = getAzimuth(th, a, phi, d) * RAD2DEG + 180;
	pos->altitude = getAltitude(th, a, phi, d) * RAD2DEG;

	return 0;
}

/**
 * Rekent de positie uit die het paneel moet aannemen aan de hand van
 * een hoek waaronder de zon schijnt
 *
 * @param  panel lege "direction" struct waarin azimuth en altitude komen
 * @param  sun   struct met hoek waaronder de zon op een gegeven moment schijnt
 * @param  base  De hoek waaronder het paneel stationair staat, aangenomen dat
 *               de normaal precies in het noorden op de horizon ligt.
 *
 * @return       0 op success, -1 als er parameters ontbreken
 */
int getPanelPosition(struct direction* panel, struct direction* sun, struct direction* base) {

	if (sun == 0 || base == 0) return -1;

	// De hoek waaron de spiegel moet staan komt overeen met het gemiddelde
	// van de zon-hoek, en de hoek waarop de spiegel recht op de panelen schijnt.
	panel->azimuth = (sun->azimuth + base->azimuth) / 2;
	panel->altitude = (sun->altitude + base->altitude) / 2;

	return 0;
}

void setup() {
	//
}

void loop() {
	//
}

#ifndef Arduino_h

struct coordinates {
	double x, y, z;
};

struct direction basePanelPos;

/**
 * Rekent een azimuth, altitude richting om naar x, y, z coordinaten
 *
 * @param  coords lege struct waarin x, y, z coordinaten komen
 * @param  sun    direction struct met azimuth en altitude richting.
 *
 * @return        0 voor success, -1 als er geen richting is gegeven.
 */
int getCoordinatesForSun(struct coordinates *coords, struct direction *sun) {

	if (sun == 0) return -1;

	coords->x = sin(sun->azimuth * DEG2RAD) * cos(sun->altitude * DEG2RAD);
	coords->y = sin(sun->altitude * DEG2RAD);
	coords->z = cos(sun->azimuth * DEG2RAD) * cos(sun->altitude * DEG2RAD);

	return 0;
}

/**
 * Print een reeks met gegevens in het formaat
 * x, y, z, timestamp
 * vanaf de gegeven timestamp. Print alleen gegevens
 * zolang altitude > 0, dus als de zon op is.
 *
 * Geef een timestamp van het begin van een dag (12 uur 's nachts)
 * om de curve van een hele dag te krijgen.
 *
 * @param time timestamp waar vanaf geplot moet worden
 */
void printPlotStats(int time) {

	struct direction sun;
	struct coordinates coords;

	int rising = 1;
	int loops = 0;

	do {

		if (getSunPosition(&sun, time, lat, lng) != 0 || getCoordinatesForSun(&coords, &sun) != 0) break;

		if (sun.altitude > 0) {
			printf("%f\t%f\t%f\t%d\n", coords.x, coords.y, coords.z, time);
		}

		time += 100; // zelfde getal als in dat SECONDS_IN_DAY / <getal> hieronder..

		if (sun.altitude > 0) rising = 0;

		if (loops > SECONDS_IN_DAY / 100) break; // voor als de zon in die dag niet op gaat, geen inf loop

		loops++;

	} while (sun.altitude > 0 || rising);
}

/**
 * Print x, y, z coordinaten voor een gegeven timestamp.
 *
 * @param time timestamp waarvoor coordinaten geprint moeten worden
 */
void printCurrentPlotStats(int time) {
	struct direction sun;
	struct coordinates coords;

	if (getSunPosition(&sun, time, lat, lng) != 0 || getCoordinatesForSun(&coords, &sun) != 0) return;

	printf("%f\t%f\t%f\n", coords.x, coords.y, coords.z);
}

/**
 * Print informatie over de zon die kan worden gebruikt om
 * te testen of het script nog goed werkt. Zou hetzelfde
 * resultaat moeten leveren als suncalc-web.
 *
 * @see ../suncalc-web/
 */
void printSunInfo() {

	struct direction sunPos;

	time_t tijd = time(0);

	if (getSunPosition(&sunPos, tijd, lat, lng) != 0) return;

	printf("\nSun position\n");
	printf("Azimuth: %f, Altitude: %f\n", sunPos.azimuth, sunPos.altitude);

	struct direction panelPos;

	if (getPanelPosition(&panelPos, &sunPos, &basePanelPos) != 0) return;

	printf("\nPaneel stand:\n");
	printf("Azimuth: %f, Altitude: %f\n", panelPos.azimuth, panelPos.altitude);
}

int main(int argc, char *const *argv)
{
	// Hoek die je vanaf het noorden met de klok mee moet draaien
	// om in de richting van de panelen te kijken.
	basePanelPos.azimuth = 90;

	// Hoek die je omhoog vanaf de horizon moet kijken om op de
	// hoogte van de panelen te kijken.
	basePanelPos.altitude = 20;

	int opt, given_time;
	struct direction sun;
	struct direction panel;

	while ((opt = getopt(argc, argv, "t:c:p:")) != -1) {
		given_time = atoi(optarg);

		switch (opt) {
			case 't':
				// Print een lijst met x, y, z, timestamp gegevens, vanaf de
				// timestamp, alleen waneer altitude > 0 Geef dus een
				// timestamp 's nachts aan het begin van de dag, zodat de zon
				// nog op moet komen, en dan krijg je gegevens van het moment
				// dat de zon op komt, tot dat hij ondergaat.
				printPlotStats(given_time);

				return 0;

			case 'c':
				// Print alleen de x, y, z (geen timestamp) voor de gegeven timestamp
				// Bij -t wordt wel een timestamp gegeven, omdat python dit nodig
				// heeft om de datum voor een filmpje te maken (@see imggen.py)
				// Dit maakt het wel wat inconsistent met parsen, maar dat is
				// nou eenmaal niet anders..
				printCurrentPlotStats(given_time);
				return 0;

			case 'p':
				// Print alleen de azimuth, altitude met de standen die het
				// paneel aan moet nemen. Dit wordt uiteindelijk gebruikt in
				// het script dat de positie doorgeeft via een serial aan de
				// arduino.
				if (getSunPosition(&sun, given_time, lat, lng) == 0 && getPanelPosition(&panel, &sun, &basePanelPos) == 0) {
					printf("%f\t%f\n", panel.azimuth, panel.altitude);

					return 0;
				}

				return -1;

			default:
				// Hier zouden we usage kunnen printen, maar gezien dit geen
				// public code is, slaan we dit over omdat we onze tijd
				// beter kunnen gebruiken.
				return -1;
		}
	}

	// Geen speciale opties gegeven, dus dump gewoon de informatie over
	// de zon, om te controleren of het script nog goed werkt in vergelijking
	// met de originele suncalc
	// (@see suncalc-web/index.html)
	// (@see suncalc-web/suncalc.js)
	printSunInfo();

	return 0;
}

#endif // #ifndef Arduino_h
