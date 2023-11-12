#pragma once

// Perlin noise by Ken Perlin
// Basically written by Github Copilot (which probably trained on a lot of this code)

// TODO: make the permutation take a seed
inline const int p[] = {
    151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
};

inline const int perm(int x) {
	return p[x & 0xFF];
}

inline const double grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	float u = h < 8 ? x : y;
	float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

inline const double lerp(double t, double a, double b) {
	return a + t * (b - a);
}

inline const double fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

inline const double perlinNoise(double x, double y, double z) {
	int X = int(floor(x)) & 255;
	int Y = int(floor(y)) & 255;
	int Z = int(floor(z)) & 255;

	x -= int(floor(x));
	y -= int(floor(y));
	z -= int(floor(z));

	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	int AAA = perm(perm(perm(X  ) + Y  ) + Z  );
	int ABA = perm(perm(perm(X  ) + Y+1) + Z  );
	int AAB = perm(perm(perm(X  ) + Y  ) + Z+1);
	int ABB = perm(perm(perm(X  ) + Y+1) + Z+1);
	int BAA = perm(perm(perm(X+1) + Y  ) + Z  );
	int BBA = perm(perm(perm(X+1) + Y+1) + Z  );
	int BAB = perm(perm(perm(X+1) + Y  ) + Z+1);
	int BBB = perm(perm(perm(X+1) + Y+1) + Z+1);

	double x1, x2, y1, y2;
	x1 = lerp(u, grad(AAA, x, y, z), grad(BAA, x-1, y, z));
	x2 = lerp(u, grad(ABA, x, y-1, z), grad(BBA, x-1, y-1, z));
	y1 = lerp(v, x1, x2);

	x1 = lerp(u, grad(AAB, x, y, z-1), grad(BAB, x-1, y, z-1));
	x2 = lerp(u, grad(ABB, x, y-1, z-1), grad(BBB, x-1, y-1, z-1));
	y2 = lerp(v, x1, x2);

	return lerp(w, y1, y2);
}

inline const double octavePerlin(double x, double y, double z, int octaves, double persistence, double scaling = 2) {
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	double maxValue = 0;
	for (int i = 0; i < octaves; i++) {
		total += perlinNoise(x * frequency, y * frequency, z * frequency) * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;
		frequency *= scaling;
	}
	return total / maxValue;
}

const int offsetMax = 10000;

class PerlinNoiseGenerator {
public:
	int seed;
	int octaves;
	double persistence;
	double scaling;
	double baseFreq;
	std::vector<glm::vec3> offsets;

	PerlinNoiseGenerator(int seed, 
		int octaves, 
		double persistence = 0.7, 
		double freqScaling = 2, double baseFreq = 1) {
		this->seed = seed;
		this->octaves = octaves;
		this->persistence = persistence;
		this->scaling = freqScaling;
		this->baseFreq = baseFreq;
		// Generate offsets based on seed
		std::mt19937_64 gen(seed);
		std::uniform_real_distribution<double> dist(-offsetMax, offsetMax);
		for (int i = 0; i < octaves; i++) {
			offsets.push_back(glm::vec3(dist(gen), dist(gen), dist(gen)));
		}
	}
	double generate(double x, double y, double z) {
		double total = 0;
		double frequency = baseFreq;
		double amplitude = 1;
		double maxValue = 0;
		for (int i = 0; i < octaves; i++) {
			total += perlinNoise((x + offsets[i].x) * frequency, (y + offsets[i].y) * frequency, (z + offsets[i].z) * frequency) * amplitude;

			maxValue += amplitude;

			amplitude *= persistence;
			frequency *= scaling;
		}
		return total / maxValue;
	}
};