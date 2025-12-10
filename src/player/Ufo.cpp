/**
 * Alison Schonauer and Hoang Nguyen
 * CS 134: Computer Game Design and Programming
 * Professor Kevin Smith
 * Fall 2025
 */

#include "Ufo.h"

Ufo::Ufo()
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

	fuelLeftTime = fuelTotalTime;

}
void Ufo::draw()
{
	ofPushMatrix();
	ofMultMatrix(getTransform());

	// Draw the UFO model faces
	model.drawFaces();

	ofSetColor(0, 150, 150, 40);
	ofEnableAlphaBlending();
	window.drawFaces();
	ofDisableAlphaBlending();
	ofPopMatrix();

	// DEBUG: Draw the UFO bounding box
	//ofNoFill();
	//ofSetColor(ofColor::white);
	//Octree::drawBox(ufoBoundingBox);
}

// Integrate UFO's motion using Euler integration
//
void Ufo::integrate()
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

// Load the UFO model and setup its properties
//
void Ufo::loadModel()
{
	
	if (model.loadModel("geo/newUFO.obj"))
	{
		model.setScaleNormalization(false);
	}
	if (window.loadModel("geo/UFOwindow.obj"))
	{	
		window.disableMaterials();
		window.setScaleNormalization(false);
	}
}

// Update the UFO's bounding box based on its current position
//
void Ufo::updateBoundingBox()
{
	// Might be useful if want to rotate the bouding box
	// https://gamedev.stackexchange.com/questions/162819/how-do-axis-aligned-bounding-boxes-update-with-rotations
	ofVec3f min = model.getSceneMin() + position;
	ofVec3f max = model.getSceneMax() + position;
	boundingBox = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

// Handle landing collision. Apply impulse force once upon first contact.
// Stop all motion after first contact.
//
void Ufo::handleLanding(const ofVec3f contactNormal)
{

	if (bLandingImpulseDone) 
	{
		velocity.set(0, 0, 0);
		rotationSpeed = 0.0f;
		return;
	}

	glm::vec3 v = velocity;
	glm::vec3 n = glm::normalize(glm::vec3(contactNormal));
	float vDotn = glm::dot(v, n);
	float e = 0.1f;  // bounciness
	glm::vec3 impulse = (e + 1.0f) * (-vDotn) * n;
	velocity += 3 * impulse;   // apply bounce once
	bLandingImpulseDone = true; 
}

// Reset landing impulse flag on takeoff
//
void Ufo::handleTakeoff()
{
	bLandingImpulseDone = false;
}

// Handle death collision. Launch UFO away from contact point.
//
void Ufo::handleDeath(const ofVec3f contactNormal)
{
	//ufoState = UFO_DEAD;
	bDead = true;
	float launchSpeed = 2000;
	velocity = launchSpeed * contactNormal;
}

// Used ray casting to calculate altitude above terrain (AGL)
// If no intersection (out of bounds), set altitude to 0
//
void Ufo::calculateAltitude(Octree& terrain)
{
	// Origin is the UFO's position, Direction is downward (-Y)
	Ray rayAltutideSensor = Ray(Vector3(position.x, position.y, position.z), Vector3(0, -1,0));
	if (terrain.intersect(rayAltutideSensor, terrain.root, terrainHitNode))
	{
		terrainHitLocation = terrain.mesh.getVertex(terrainHitNode.points[0]);
		altitude = max(static_cast<float>(0.0), position.y - terrainHitLocation.y);
	}
	else if (!(position.x <= terrain.root.box.max().x() && position.x >= terrain.root.box.min().x() 
				&& position.z <= terrain.root.box.max().z() && position.z >= terrain.root.box.min().z())) 
	{
		altitude = 0;
	}
}

// Check if UFO has fuel left
//
bool Ufo::hasFuel()
{
	return fuelLeftTime > 0.0;
}

// Heading X, Y, and Z of the UFO in world coordinates
//
glm::vec3 Ufo::getHeadingX()
{
	return glm::normalize(glm::vec3(getTransform() * glm::vec4(1, 0, 0, 0)));
}

glm::vec3 Ufo::getHeadingY()
{
	return glm::normalize(glm::vec3(getTransform() * glm::vec4(0, 1, 0, 0)));
}

glm::vec3 Ufo::getHeadingZ()
{
	return glm::normalize(glm::vec3(getTransform() * glm::vec4(0, 0, -1, 0)));
}
