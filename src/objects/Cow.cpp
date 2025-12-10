/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Fall 2025
 */

#include "Cow.h"

Cow::Cow() {
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	force.set(0, 0, 0);

	rotSpeed = 0.0;
	rotAcceleration = 0.0;
	rotForce = 0.0;

	damping = 0.99;
	mass = 1;
};

// Integrate function to update position and rotation based on forces and state
// If the cow is alive, apply forces based on its state (FREE or ABDUCTED)
// If the cow is free, apply gravity force downwards
// If the cow is abducted, apply force towards the followed point and rotational acceleration
// 
void Cow::integrate() {
	if (bAlive)
	{
		if (ofGetFrameRate() == 0 || state == LANDED) return;
		switch (state) {
		case FREE:
			force.set(0, -10, 0);
			rotAcceleration = 0;
			break;
		case ABDUCTED:
			glm::vec3 direction = glm::vec3(followedPoint->x, followedPoint->y, followedPoint->z) - position;
			force.set(direction * forceStrength);
			rotAcceleration = 8;
			break;
		}

		// Euler integration for linear motion
		float dt = 1.0 / ofGetFrameRate();
		ofVec3f accel = acceleration;
		accel += force / mass;
		position += velocity * dt;
		velocity += accel * dt;
		velocity *= damping;

		// Euler integration for rotation motion
		float rotAccel = rotAcceleration;
		rotAccel += rotForce / mass;
		rotation += rotSpeed * dt;
		rotSpeed += rotAccel * dt;
		rotSpeed *= damping;

		// Reset force after integration
		force.set(0, 0, 0);
		rotForce = 0;
	}	
};

// Make the cow follow a given position pointer
//
void Cow::follow(glm::vec3 * Pos) {
	if (bAlive)
	{
		followedPoint = Pos;
		position.y += 1;
		state = ABDUCTED;
	}
};


// Make the cow free from being abducted
//
void Cow::free() {
	if (bAlive)
	{
		followedPoint = nullptr;
		state = FREE;
	}
};

// Destroy the cow (set alive to false and remove bounding box)
//
void Cow::destroy()
{
	bAlive = false;
	bHasBoundingBox = false;
}

// Load the cow model
//
void Cow::loadModel() {
    if (model.loadModel("geo/cow.obj", true))
	{
		model.getMesh(0).enableTextures();
		model.setScaleNormalization(false);
	}
};


// Update the bounding box of the cow based on its position and model's scene min/max
//
void Cow::updateBoundingBox()
{
	if (bAlive)
	{
		bHasBoundingBox = true;
		ofVec3f min = model.getSceneMin() + position;
		ofVec3f max = model.getSceneMax() + position;
		boundingBox = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
	}
};

// Handle landing of the cow
//
void Cow::handleLanding()
{
	if (bAlive)
	{
		if (state == ABDUCTED)
			position.y += 1;
		else
			state = LANDED;
	}
};
