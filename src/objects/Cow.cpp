
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

void Cow::follow(glm::vec3 * Pos) {
	if (bAlive)
	{
		followedPoint = Pos;
		position.y += 1;
		state = ABDUCTED;
	}
};

void Cow::free() {
	if (bAlive)
	{
		followedPoint = nullptr;
		state = FREE;
	}
};

void Cow::destroy()
{
	bAlive = false;
}

void Cow::loadModel() {
    if (model.loadModel("geo/cow.obj", true))
	{
		model.getMesh(0).enableTextures();
		model.setScaleNormalization(false);
	}
};

void Cow::updateBoundingBox()
{
	if (bAlive)
	{
		ofVec3f min = model.getSceneMin() + position;
		ofVec3f max = model.getSceneMax() + position;
		boundingBox = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
	}
};

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
