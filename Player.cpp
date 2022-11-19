#include"Player.h"

double Player::tempFromAltitude(double altitude) {
	if (altitude <= 0) {
		// TODO: vary this temp based on current pos and other conditions of the surrounding area
		return 15.04;
	}
	else if (altitude < 11000) {
		// Troposphere
		return 15.04 - 0.00649 * altitude;
	}
	else if (altitude < 25000) {
		// Tropopause
		return -56.46;
	}
	else if (altitude < 44000) {
		// Stratosphere
		return -131.21 + 0.00299 * altitude;
	}
	// Mostly estimates from this point on
	else if (altitude < 52000) {
		// Stratopause
		return 0.35;
	}
	else if (altitude < 80000) {
		// Mesosphere
		return 162.59 - 0.00312 * altitude;
	}
	else if (altitude < 85000) {
		// Mesopause
		return -87.01;
	}
	else if (altitude < 600000) {
		// TODO: Thermosphere, which idk what to do about because of wild fluctuations :twimerp:
		return 0;
	}
	else {
		// Exosphere / deep space
		return -273.15;
	}
}

double Player::pressureFromAltitude(double altitude) {
	// TODO: Just can't be bothered right now 22/11/19
	return 100000;
}

