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

	// Draw the UFO model faces
	ufoModel.drawFaces();

	ofPopMatrix();

	// Draw the UFO bounding box
	ofNoFill();
	ofSetColor(ofColor::white);
	Octree::drawBox(ufoBoundingBox);
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

		// Euler integration for rotation motion
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
	
	if (ufoModel.loadModel("geo/mars-low-5x-v2.obj"))
	{
		ufoModel.setScaleNormalization(false);
	}
}

void Lander::updateBoundingBox()
{
	// Might be useful if want to rotate the bouding box
	// https://gamedev.stackexchange.com/questions/162819/how-do-axis-aligned-bounding-boxes-update-with-rotations
	ofVec3f min = ufoModel.getSceneMin() + position;
	ofVec3f max = ufoModel.getSceneMax() + position;
	ufoBoundingBox = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
} 

glm::mat4 Lander::getTransform()
{
	glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(position));
	glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 1, 0));	// rotate around y-axis
	glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(scale));
	return T * R * S;
}

glm::vec3 Lander::getHeadingX()
{
	return glm::normalize(glm::vec3(getTransform() * glm::vec4(1, 0, 0, 0)));
}

glm::vec3 Lander::getHeadingY()
{
	return glm::normalize(glm::vec3(getTransform() * glm::vec4(0, 1, 0, 0)));
}

glm::vec3 Lander::getHeadingZ()
{
	return glm::normalize(glm::vec3(getTransform() * glm::vec4(0, 0, -1, 0)));
}
