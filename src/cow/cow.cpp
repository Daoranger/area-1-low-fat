
#include "cow.h"


Cow::Cow() {
    position.set(0, 0, 0);
	rotation = 0.0;
	scale.set(1, 1, 1);

	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	force.set(0, 0, 0);

	rotationSpeed = 0.0;
	rotationAcceleration = 0.0;
	rotationForce = 0.0;

	damping = 0.99;
	mass = 1;
};


void Cow::integrate() {
    if (ofGetFrameRate() == 0) return;
    switch (state) {
        case FREE:
            force.set(0, -10, 0);
            rotationAcceleration = 0;
        case ABDUCTED:
            glm::vec3 direction = glm::vec3(followedPoint->x, followedPoint->y, followedPoint->z) - position;
            direction = glm::normalize(direction);
            force.set(direction*forceStrength);
            rotationAcceleration = 5;
    }

	// Euler integration for linear motion
	float dt = 1.0 / ofGetFrameRate();
	ofVec3f accel = acceleration;
	accel += force / mass;
	position += velocity * dt;
	velocity += accel * dt;
	velocity *= damping;

    // Euler integration for rotation motion
	float rotAccel = rotationAcceleration;
	rotAccel += rotationForce / mass;
	rotation += rotationSpeed * dt;
	rotationSpeed += rotAccel * dt;
	rotationSpeed *= damping;

    // Reset force after integration
	force.set(0, 0, 0);
	rotationForce = 0;
	
};

void Cow::draw() {
    ofPushMatrix();
	ofMultMatrix(getTransform());

	cowModel.drawFaces();

	ofPopMatrix();
};

void Cow::follow(glm::vec3 * Pos) {
    followedPoint = Pos;
    state = ABDUCTED;
};

void Cow::free() {
    followedPoint = nullptr;
    state = FREE;
};

void Cow::loadModel() {
    if (cowModel.loadModel("geo/cow.obj"))
	{
		cowModel.setScaleNormalization(false);
	}
};

glm::mat4 Cow::getTransform() {
    glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(position));
	glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 1, 0));	// rotate around y-axis
	glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(scale));
	return T * R * S;
};