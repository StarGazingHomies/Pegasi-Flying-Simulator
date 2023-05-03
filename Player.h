#pragma once

#include <format>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"Phys_Object.h"
#include"Font.h"

enum class WingMotion {
	IDLE,
	DOWN,
	UP,
};

enum class ControlState {
	// Basic ground states
	GROUND_WALKING,
	GROUND_RUNNING,
	// Some alternatives if I have time for them
	GROUND_TIPTOE,   // Tiptoeing around
	GROUND_OBJAVOID, // Avoiding an object (going over a fence, through a small hole in the wall, etc.)
	// Swimming
	WATER_SURFACE,
	WATER_DIVING,
	// Core of pegasi FLIGHT simulator :3
	FLIGHT,
	FREEFALL,  // Jumping or falling off cliffs. Mostly for non-pegasi
	LEVITATION,
	FREECAM // Debug
};

enum class CameraState {
	FIRST_PERSON,
	THIRD_PERSON
};

struct keyStatus {
	int status;
	double lastPress;
	unsigned int consecutiveClicks;
};

class Player {
public:
	// Things that settings change
	// Time it takes for wings to return to resting pos
	double autoReturnTime = 1.0;
	// Time it takes to double click
	double doubleClickTime = 0.15f;

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

	double wingStrokeMagnitude = 15.0f;
	double minWingStrokeLength = 0.20f;
	double maxWingStrokeLength = 0.30f;
	double wingStrokeUpDelay = 0.05f;
	double wingStrokeAccTime = 0.02f;
	// Temporary
	double primaryF = 700.0f;
	double secondaryF = 200.0f;

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
	double wingSpan = 5.5;      // Very beeg wings lol. That's what meter-tall horsies need to fly, apparently. Tiny MLP wings just don't cut it.
	double primaryFeatherLength = 2.55; // Will also happen to be the primary feather width used for Coefficient of Lift...
	double humurusLength = 1.0; // The humurus doesn't really move when flapping to provide lift.
	double secondaryWidth = 1.95; // Width of secondary feathers


	// Not really Consts (changes very little over the game, but it does and should change!)
	double mass = 40;
	glm::mat3 momentOfInertia = glm::mat3(1.0f); // Inverse of moment of inertia
	float jumpVelocity = 4.0, walkingSpeed = 4.0;
	glm::vec3 ponyHalfSize = glm::vec3(0.5f, 0.5f, 0.25f);
	glm::vec3 ponyLoafHalfSize = glm::vec3(0.5f, 0.5f, 0.25f);
	float forehoofLengths[3] = { 0.2f, 0.18f, 0.22f };
	float backhoofLengths[3] = { 0.25f, 0.05f, 0.3f };
	
	// Stamina
	double staminaRegen = 0.1;
	double burstStaminaRegen = 0.5;
	double maxStamina = 100;
	double maxBurstStamina = 10;

	// Walking (and standing still)
	float walkingMaxAcceleration = 12.0;		// Range: (0,+inf), technically maxWalkingSpeed = walkingMaxAcceleration / (1-walkingRetention)
	float walkingStopAcceleration = 18.0;		// Rnage: [0, +inf), maximum stopping accr
	float walkingRetention = 0.35;				// Range: [0,1) (Change maxAcc accordingly)
	float walkingTargetSpeed = 5.0;				// Range: [0,maxWalkingSpeed]
	float walkingLookImportance = 0.18f;		// Range: [0,0.5] (0 --> Look dir no impact, 0.5 --> Going back velocity 0)
	float walkingBodyImportance = 0.14f;		// Range: [0,0.5] (0 --> Look dir no impact, 0.5 --> Going back velocity 0)
	float walkingDirectionStaminaImpact = 1.9f;	// Range: [0,+inf) (0 --> Look dir no additional impact)
	float walkingInstantStop = 0.15f;			// Range: [0,+inf) (0 --> never completely stop; inf --> instantly completely stop)
	float walkingDirWeight = 1.2f;				// Range: [0,+inf) (0 --> Old direction has no weight, inf --> very important)
	float walkingTargetDirWeight = 0.4f;		// Range: [0,+inf) (0 --> Target direction has no weight, inf --> very important)
	float walkingVelocityDirWeight = 1.0f;		// Range: [0,+inf) (0 --> Velocity direction has no weight, inf --> very important)
	float walkingDirAccelerationWeight = 2.5f;  // Range: [0,+inf) (0 --> Body direction has no impact on accr, inf --> body direction determine accr)
	float walkingMaxTurnSpeed = 8.0f;			// Range: [0,+inf) (0 --> No turning, inf --> Instant turn)
	float walkingMaxAccStaminaCost = 0.44f;		// Range: [0,+inf) stamina cost of maximum acceleration

	bool walkingAutoTurn = true;

	// Variables

	// Store pos/velocity.
	// Acceleration is calculated per frame
	glm::vec3 position, velocity;
	glm::quat rot, rotVelocity;
	// Need more variables for player's exact state, such as the 4 hooves
	glm::vec3 camPos, camOrientation, camUp;
	glm::vec3 camRelPos = glm::vec3(0.0f, 1.0f, 0.0f);

	// Stores each wing's status
	WingMotion rightWingMotion, leftWingMotion;
	double rightWingVelocity, leftWingVelocity;
	double rightWingPosition, leftWingPosition;

	// Status of player
	ControlState controlState = ControlState::GROUND_WALKING;
	ControlState prevControlState = ControlState::GROUND_WALKING;
	CameraState camState = CameraState::FIRST_PERSON;
	float camThirdPersonDistance = 5;
	double firstClick = false;

	// Ground movement
	double stamina = 100, burstStamina = 10;

	// Window stuff
	int width, height;
	double sensitivity = 100.0;
	glm::mat4 projMatrix, viewMatrix;
	double FOVdeg = 90.0f, nearPlane = 0.1f, farPlane = 10000.0f;

	// Debug display
	bool debugGraphics;
	VAO debugCamVAO, debugBodyVAO, debugVectorsVAO;
	VBO debugCamVBO, debugBodyVBO, debugVectorsVBO;
	EBO debugCamEBO, debugBodyEBO;
	// Debug vector display additional stuff
	// Fmt: vec3 pos, vec3 col;
	std::vector<float> debugVecData;
	// Temp debug vars
	glm::vec3 debugTempVec1, debugTempVec2;

	// Initializer
	Player();

	// Debug helper functions
	std::string getControlStateString();

	// Inputs
	std::vector<keyStatus> keymap;
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void keyClear(int key);
	void windowResizeCallback(int width, int height);

	// Camera helper functions
	void updateMatrix();
	glm::mat4 getProjMatrix();
	glm::mat4 getViewMatrix();
	std::vector<glm::vec3> getAxes();

	// Earth Pony mode helper functions


	// Birdie mode helper functions

	// Get the (EARTH) temperature and pressure at a given altitude
	// Based on nasa website: https://www.grc.nasa.gov/www/k-12/rocket/atmosmet.html
	double tempFromAltitude(double altitude);
	double pressureFromAltitude(double altitude);

	double RHSignedAngle(glm::vec3 a, glm::vec3 b, glm::vec3 n);
	double LHSignedAngle(glm::vec3 a, glm::vec3 b, glm::vec3 n);
	//double wingEfficiency(float angleOfAttack);
	double wingDragCoefficient(float angleOfAttack);
	double wingLiftCoefficient(float angleOfAttack);
	glm::vec3 wingLiftForce();
	glm::vec3 wingStrokeForce();
	glm::vec3 wingAcceleration(double time);
	glm::vec3 wingAngularAcceleration(double time);

	// Main functions
	void Draw(Shader& shader);
	void debugText(Font& font, Shader& fontShader);
	void debugVectors(Shader& vecDebugShader);
	void addDebugVector(glm::vec3 src, glm::vec3 dir, glm::vec3 colour);
	void Tick(GLFWwindow* window, float time);
};