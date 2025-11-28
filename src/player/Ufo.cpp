#include "Ufo.h"

Ufo::Ufo()
{
	position.set(0, 200, 0);
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

	ofPopMatrix();

	// DEBUG: Draw the UFO bounding box
	//ofNoFill();
	//ofSetColor(ofColor::white);
	//Octree::drawBox(ufoBoundingBox);
}
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
void Ufo::loadModel()
{
	
	if (model.loadModel("geo/newUFO.obj"))
	{
		model.setScaleNormalization(false);
	}
}
void Ufo::updateBoundingBox()
{
	// Might be useful if want to rotate the bouding box
	// https://gamedev.stackexchange.com/questions/162819/how-do-axis-aligned-bounding-boxes-update-with-rotations
	ofVec3f min = model.getSceneMin() + position;
	ofVec3f max = model.getSceneMax() + position;
	boundingBox = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

void Ufo::handleLanding(const ofVec3f contactNormal)
{

	if (bLandingImpulseDone) 
	{
		velocity.set(0, 0, 0);
		rotationSpeed = 0.0f;
		return;
	}

	glm::vec3 v = velocity;
	glm::vec3 n = glm::normalize(glm::vec3(contactNormal));  // make sure it's unit length

	float vDotn = glm::dot(v, n);
	if (vDotn >= 0.0f) return; 
	float e = 0.1f;  // bounciness
	glm::vec3 impulse = (e + 1.0f) * (-vDotn) * n;
	velocity += 3 * impulse;   // apply bounce once
	bLandingImpulseDone = true;
}

void Ufo::handleTakeoff()
{
	bLandingImpulseDone = false;
}

void Ufo::calculateAltitude(Octree& terrain)
{
	// Origin is the UFO's position, Direction is downward (-Y)
	Ray rayAltutideSensor = Ray(Vector3(position.x, position.y, position.z), Vector3(0, -1,0));
	if (terrain.intersect(rayAltutideSensor, terrain.root, terrainHitNode))
	{
		terrainHitLocation = terrain.mesh.getVertex(terrainHitNode.points[0]);
		altitude = max(static_cast<float>(0.0), position.y - terrainHitLocation.y);
	}
	else
	{
		altitude = 0.0;
	}
}
bool Ufo::hasFuel()
{
	return fuelLeftTime > 0.0;
}

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
