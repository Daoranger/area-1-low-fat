#include "ofApp.h"
#include "collision/Util.h"

//--------------------------------------------------------------
void ofApp::setup()

{
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableDepthTest();
	//ofEnableLighting();
	//initLightingAndMaterials();

	skyBox.load("images/stars.png");

	// Fonts setup
	if (fontUI.load("font/Stardock.ttf", 20, true, true))
	{
		ofSetLineWidth(5);
	}
	else
	{
		cout << "Failed to load font\n";
	}

	// UFO setup
	ufo.loadModel();

	// Terrain setup
	bTerrainLoaded = terrain.loadModel("geo/terrain.obj");
	terrain.setScaleNormalization(false);
	terrainOctree.create(terrain.getMesh(0), 20);

	// Game Object
	//
	station1.loadModel();
	station1.createOctree();
	mothership.loadModel();
	mothership.createOctree();
	speedRing1.bVertical = true;
	speedRing1.loadModel();
	speedRing1.createOctree();
	cow1.loadModel();

	cow1.position.y += 50;
	cow1.position.x += 30;

	// Debug Camera setup
	debugCam.setDistance(10);
	debugCam.setNearClip(.1);
	debugCam.setFov(65.5);
	debugCam.disableMouseInput();

	// Lightnings Setup
	//

	// 
	ufoLight.setup();
	ufoLight.setSpotlight();
	ufoLight.setSpotlightCutOff(25);
	ufoLight.setSpotConcentration(10);
	ufoLight.setDiffuseColor(ofColor::red);
	ufoLight.setAmbientColor(ofColor::red);
	ufoLight.setAttenuation(0.001f, 0.0001f, 0.00001f);

	// World light (sun)
	sunLight.setup();
	sunLight.enable();
	sunLight.setDirectional();
	sunLight.setPosition(500, 400, 0);
	sunLight.setAreaLight(10000, 10000);
	sunLight.setDiffuseColor(ofColor::whiteSmoke);				
	sunLight.setAmbientColor(ofFloatColor(0.2, 0.2, 0.2));    

}

//--------------------------------------------------------------
void ofApp::update()
{

	// UFO lightning update

	if (bToggleUFOLight)
	{
		ufoLight.setPosition(ufo.position - ufo.getHeadingY() * 5);
		ufoLight.lookAt(ufo.position - ufo.getHeadingY() * 50);
		ufoLight.enable();
	}
	else
	{
		ufoLight.disable();
	}

	ufo.calculateAltitude(terrainOctree);
	//cout << "Altitude: " << ufo.altitude;

	// will move these variables outside later
	constexpr float THRUST_ACCEL = 15.0f;   
	constexpr float FORWARD_ACCEL = 15.0f;   
	constexpr float STRAFE_ACCEL = 15.0f;  
	constexpr float YAW_TORQUE = 50.0f;   

	// Thrust Force
	//if (keysMap[OF_KEY_CONTROL] && ufo.hasFuel())														// down (ctrl)
	//{
	//	ufo.force += -THRUST_ACCEL * ufo.getHeadingY();
	//	float deltaTime = 1.0 / ofGetFrameRate();
	//	ufo.fuelLeftTime = max(static_cast<float>(0.0), ufo.fuelLeftTime - deltaTime);
	//}

	if (keysMap[' '] && ufo.hasFuel())												// up (space)
	{
		ufo.handleTakeoff();
		ufo.force +=  THRUST_ACCEL * ufo.getHeadingY();
		float deltaTime = 1.0 / ofGetFrameRate();
		ufo.fuelLeftTime = max(static_cast<float>(0.0), ufo.fuelLeftTime - deltaTime);
	}
	
	if (keysMap['w']) ufo.force +=  FORWARD_ACCEL * ufo.getHeadingZ();			// forward (w)
	if (keysMap['s']) ufo.force += -FORWARD_ACCEL * ufo.getHeadingZ();			// backward (d)
	if (keysMap['a']) ufo.force += -STRAFE_ACCEL  * ufo.getHeadingX();			// left (a)
	if (keysMap['d']) ufo.force +=  STRAFE_ACCEL  * ufo.getHeadingX();			// right (d)
	if (keysMap['e']) ufo.rotationForce -= YAW_TORQUE;								// yaw right (q)
	if (keysMap['q']) ufo.rotationForce += YAW_TORQUE;								// yaw left (e)
	
	// Gravity Force
	const glm::vec3 gravity = glm::vec3(0.0f, -5.0f, 0.0f);
	ufo.force += ufo.mass * gravity;

	// Turbulence Force
	ufo.force.x += ofRandom(-5, 5);
	ufo.force.y += ofRandom(-5, 5);
	ufo.force.z += ofRandom(-5, 5);

	ufo.integrate();

	// Gameplay Camera update
	updateGameCamera();

	// Handle Collision Terrain vs UFO
	ufo.updateBoundingBox();
	colBoxList.clear();
	colNodeList.clear();
	terrainOctree.intersect(ufo.boundingBox, terrainOctree.root, colBoxList, colNodeList);
	if (station1.octree.intersect(ufo.boundingBox, station1.octree.root, station1, colBoxList, colNodeList))
	{
		station1.handleCollision(ufo);
	}

	cout << "Number of collided nodes/boxes: " << colBoxList.size() << '\n';
	if (colBoxList.size() >= 1)
	{
		if (!keysMap[' '])
		{
			ofVec3f contactNormal = getNormalAtContactPoint();
			ufo.handleLanding(contactNormal);
		}
	}

	// Handle collision of cow 
	cow1.updateBoundingBox();
	vector<TreeNode> cowNodeList;		// Store all collided (leaf) nodes
	vector<Box> cowBoxList;				// Store all collided (leaf) nodes's boxes
	terrainOctree.intersect(cow1.boundingBox, terrainOctree.root, cowBoxList, cowNodeList);
	station1.octree.intersect(cow1.boundingBox, station1.octree.root, station1, cowBoxList, cowNodeList);
	if (cowBoxList.size() >= 1)
	{
		cow1.handleLanding();
	}

	cow1.integrate();

}

//--------------------------------------------------------------
void ofApp::draw()
{

	ofBackground(ofColor::black);

	ofDisableDepthTest();
	ofDisableLighting();
	// The 2D sky box image (don't draw it in 3D)
	skyBox.draw(0, 0, ofGetWidth(), ofGetHeight());
	ofEnableDepthTest();

	ofEnableLighting();
	activeCam->begin();
	ofPushMatrix();

	ufo.draw();
	terrain.drawFaces();
	station1.draw();
	mothership.draw();
	speedRing1.draw();
	cow1.draw();

	if (bDrawOctree)
	{
		ofNoFill();
		ofSetColor(ofColor::white);
		terrainOctree.drawLeafNodes(terrainOctree.root);
		Octree::drawBox(ufo.boundingBox);
	}

	ofPopMatrix();
	activeCam->end();
	ofDisableDepthTest();
	ofDisableLighting();


	// UI draw: don't include it in the 3D stuff (within camera)
	glDepthMask(false);
	fontUI.drawString("Altitude: " + ofToString(ufo.altitude, 2), 20, 70);
	fontUI.drawString("Velocity: " + ofToString(ufo.velocity.length(), 2), 20, 140);
	fontUI.drawString("Fuel time left: " + (ufo.hasFuel() ? ofToString(ufo.fuelLeftTime, 2) + " s" : "Out of Fuel!"), 20, 210);
	glDepthMask(true);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch (key)
	{
	case 'C':
	case 'c':
		if (debugCam.getMouseInputEnabled()) debugCam.disableMouseInput();
		else debugCam.enableMouseInput();
		break;
	case 'R':
	case 'r':
		bToggleUFOLight = !bToggleUFOLight;
		break;
	case 'L':
	case 'l':
		bDrawOctree = !bDrawOctree;
		break;
	case '1':
		activeCam = &debugCam;
		break;
	case '2':
		if (activeCam == &gameCam) nextGameCameraView();
		else activeCam = &gameCam;
		break;
	}

	keysMap[key] = true;

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	keysMap[key] = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

// Need this function or else everything is white
void ofApp::initLightingAndMaterials()
{
	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{ 5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
}

ofVec3f ofApp::getNormalAtContactPoint()
{
	ofMesh mesh = terrain.getMesh(0);
	const auto numNormals = mesh.getNumNormals();

	std::unordered_set<int> collidedVerts;
	for (auto& node : colNodeList)
	{
		for (auto vi : node.points)
		{
			collidedVerts.insert(vi);
		}
	}

	ofVec3f avgVertexNormal(0, 0, 0);

	for (auto vi : collidedVerts)
	{
		// if the index not in range, skip it
		if (vi < 0 || vi >= static_cast<int>(numNormals))
		{
			continue;
		}
		avgVertexNormal += mesh.getNormals().at(vi);
	}

	avgVertexNormal.normalize();

	if (avgVertexNormal.length() == 0.0f) {
		return ofVec3f(0, 1, 0);
	}
	avgVertexNormal.normalize();
	return avgVertexNormal;
}

/**
 * Update the gameplay camera based on the current view mode
 */
void ofApp::updateGameCamera()
{
	
	switch (camView)
	{
	case CAM_THIRD: // 3rd person: camera sits 6 up and 12 behind the ufo, looks 3 ahead
		gameCam.setPosition(ufo.position + ufo.getHeadingY() * 20 - ufo.getHeadingZ() * 25);
		gameCam.lookAt(ufo.position + ufo.getHeadingZ() * 3);
		break;
	case CAM_FIRST: //1st person: camera at ufo position, looks straight forward
		gameCam.setPosition(ufo.position.x, ufo.position.y + 10, ufo.position.z);
		gameCam.lookAt(gameCam.getPosition() + ufo.getHeadingZ() * 3);
		break;
	case CAM_TOP:	// Top-down: camera 20 up above ufo, looks straight down
		gameCam.setPosition(ufo.position.x, ufo.position.y + 50, ufo.position.z);
		gameCam.lookAt(ufo.position, ufo.getHeadingZ());
		break;
	case CAM_GROUND:
		gameCam.setPosition(ufo.position + ufo.getHeadingY() * 5.5);
		gameCam.lookAt(ufo.position - ufo.getHeadingY(), ufo.getHeadingZ());
		break;
	}
}

/**
 * Get next camera view mode
 */
void ofApp::nextGameCameraView()
{
	camView = static_cast<CamView>((camView + 1) % 4);
}
