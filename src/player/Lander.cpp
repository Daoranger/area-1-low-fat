#include "Lander.h"

Lander::Lander()
{
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
}

void Lander::draw()
{
	ofPushMatrix();
	ofMultMatrix(getTransform());
	ufoModel.drawFaces();
	ofPopMatrix();
}

void Lander::integrate()
{
	if (ofGetFrameRate() == 0) return;

	if (ofGetFrameRate() != 0)
	{
		// Euler integration for linear motion
		float dt = 1.0 / ofGetFrameRate();
		ofVec3f accel = acceleration;
		accel += force / mass;
		position += velocity * dt;
		velocity += accel * dt;
		velocity *= damping;

		// Eulter integration for rotation motion
		float rotAccel = rotationAcceleration;
		rotAccel += rotationForce / mass;
		rotation += rotationSpeed * dt;
		rotationSpeed += rotAccel * dt;
		rotationSpeed *= damping;

		// Reset force after integration
		force.set(0, 0, 0);
		rotationForce = 0;
	}
}

void Lander::loadModel()
{
	ufoModel.loadModel("geo/LEM-combined.obj");
	ufoModel.setPosition(position.x, position.y, position.z);
}

glm::mat4 Lander::getTransform()
{
	glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(position));
	glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
	glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(scale));
	return T * R * S;
}

glm::vec3 Lander::getHeadingY()
{
	return glm::normalize(glm::vec3(getTransform() * glm::vec4(0, 1, 0, 0)));
}
