#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"Phys_Object.h"

enum class wingMotion {
	IDLE,
	DOWN,
	UP,

};

class Player {
public:
	// Things that settings change
	double autoReturnTime = 1.0;

	// Birdie "consts for now"

	/*
	* For this section, a lot needs to be considered.
	* Wing stroke times depend heavily on the size of the pegasus.
	* Smaller birds tend to flap faster, so maybe a pegasus filly should flap much, much faster than an adult.
	* Like how scootaloo buzzes her wings on the scooter
	* No idea if biology allows for this type of muscle development though.
	* 
	* That's something to worry about later, for now, estimate the adult consts.
	* Times are for a full stroke (up and down).
	*/

	double minWingStrokeLength = 0.20f;
	double maxWingStrokeLength = 0.30f;

	/*
	* Next is the size of the wings.
	* 
	* Note: Humurus/Ulna/Manus are the three sections (name of bones) of a bird's wing
	* but the wingspan also includes primary feather length!
	* 
	* Again, no idea how big a pegasus is relative to a human.
	* If we use IRL pony size as a reference... well, ponies don't fly
	* But the wandering albatross has the biggest wingspan of 2.51~3.5 (mean 3.1)m
	* And weighs 6.35-11.91 (mean depending on place, but prolly 9.1)kg
	* (Source: Wikipedia. Yeah, I'm not proud of it)
	* 
	* However, wingspan also has a relationship with mass to consider
	* 
	* Total arm length related to weight^0.42 approximately
	* Primary feather length related to arm length^0.82 ish
	* https://journals.plos.org/plosone/article?id=10.1371/journal.pone.0015665
	* https://www.researchgate.net/profile/Robert-Nudds/publication/272152474_Wing_bone_length_allometry_in_birds/links/5cd41defa6fdccc9dd98075e/Wing-bone-length-allometry-in-birds.pdf
	* 
	* Note: Cruising velocity goes by wing area ^0.25
	* https://www.science.org/doi/10.1126/sciadv.aat4269
	*/

	// Very crude estimates, for now
	double wingSpan = 5.5;      // Very beeg wings lol
	double primaryFeatherLength = 2.55;
	double humurusLength = 1.0; // The humurus doesn't really move when flapping to provide lift, which is where this is used.



	// Not really Consts (changes very little over the game, but it does and should change!)
	double mass = 40;

	// Variables

	// Store pos/velocity.
	// Acceleration is calculated per frame
	glm::vec3 position, velocity;
	glm::quat rot, rotVelocity;

	// Stores each wing's status
	wingMotion rightWingMotion, leftWingMotion;
	double rightWingVelocity, leftWingVelocity;
	double rightWingPosition, leftWingPosition;

	// Inputs
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// Birdie helper functions

	// Get the (EARTH) temperature and pressure at a given altitude
	// Based on nasa website: https://www.grc.nasa.gov/www/k-12/rocket/atmosmet.html
	double tempFromAltitude(double altitude);
	double pressureFromAltitude(double altitude);

	glm::vec3 wingLiftForce();
	glm::vec3 wingStrokeForce();
	glm::vec3 wingAcceleration(double time);
	glm::vec3 wingAngularAcceleration(double time);

	// Main functions
	void Draw();
	void Tick();
};