
#include <math.h>

#define PI (M_PI)
#define DEG2RAD (PI / 180)
#define SECONDS_IN_DAY ((double) (24 * 3600))

typedef struct {
	double azimuth;
	double altitude;
} sun_pos;

int J1970 = 2440588,
	J2000 = 2451545;

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
	return timestamp / SECONDS_IN_DAY;
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

int getSunPosition(sun_pos* pos, int timestamp, double latitude, double longtitude) {
	if (pos == NULL) return -1;

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
	pos.azimuth


	return 0;
}

void setup() {
	//
}

void loop() {
	//
}
