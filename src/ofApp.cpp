#include "ofApp.h"
#include "collision/Util.h"

//--------------------------------------------------------------
void ofApp::setup()

{

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
	lander.loadModel();
	ofSetColor(255);

	// Terrain setup
	terrain.loadModel("geo/mars-low-5x-v2.obj");
	terrain.setScaleNormalization(false);
	terrainOctree.create(terrain.getMesh(0), 20);

	// Game Object
	//
	landingPad.loadModel();
	landingPad.createOctree();

	// Debug Camera setup
	debugCam.setDistance(10);
	debugCam.setNearClip(.1);
	debugCam.setFov(65.5);
	debugCam.disableMouseInput();

	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableDepthTest();
	initLightingAndMaterials();

}

//--------------------------------------------------------------
void ofApp::update()
{

	lander.calculateAltitude(terrainOctree);
	//cout << "Altitude: " << lander.altitude;

	// will move these variables outside later
	constexpr float THRUST_ACCEL = 15.0f;   
	constexpr float FORWARD_ACCEL = 15.0f;   
	constexpr float STRAFE_ACCEL = 15.0f;  
	constexpr float YAW_TORQUE = 50.0f;   

	// Thrust Force
	if (keysMap[OF_KEY_CONTROL]) lander.force += -THRUST_ACCEL * lander.getHeadingY();	// down (ctrl)
	if (keysMap[' '] && lander.hasFuel())
	{
		lander.force +=  THRUST_ACCEL  * lander.getHeadingY();			// up (space)
		//lander.handleTakeOff();
		
		float deltaTime = 1.0 / ofGetFrameRate();
		lander.fuelLeftTime = max(static_cast<float>(0,0), lander.fuelLeftTime - deltaTime);
	}
	
	if (keysMap['w']) lander.force +=  FORWARD_ACCEL * lander.getHeadingZ();			// forward (w)
	if (keysMap['s']) lander.force += -FORWARD_ACCEL * lander.getHeadingZ();			// backward (d)
	if (keysMap['a']) lander.force += -STRAFE_ACCEL  * lander.getHeadingX();			// left (a)
	if (keysMap['d']) lander.force +=  STRAFE_ACCEL  * lander.getHeadingX();			// right (d)
	if (keysMap['e']) lander.rotationForce -= YAW_TORQUE;								// yaw right (q)
	if (keysMap['q']) lander.rotationForce += YAW_TORQUE;								// yaw left (e)
	
	// Gravity Force
	const glm::vec3 gravity = glm::vec3(0.0f, -1.68f, 0.0f);
	lander.force += lander.mass * gravity;

	// Turbulence Force
	lander.force.x += ofRandom(-5, 5);
	lander.force.y += ofRandom(-5, 5);
	lander.force.z += ofRandom(-5, 5);

	lander.integrate();

	// Gameplay Camera setup
	updateGameCamera();

	// Handle Collision Terrain vs UFO
	lander.updateBoundingBox();
	colBoxList.clear();
	colNodeList.clear();
	terrainOctree.intersect(lander.ufoBoundingBox, terrainOctree.root, colBoxList, colNodeList);

	//cout << "Number of collided nodes/boxes: " << colBoxList.size() << '\n';
	if (colBoxList.size() >= 1000 && lander.altitude <= 0.2)
	{
		//lander.handleLanding();
	}

}

//--------------------------------------------------------------
void ofApp::draw()
{

	ofBackground(ofColor::black);

	ofDisableDepthTest();
	skyBox.draw(0, 0, ofGetWidth(), ofGetHeight());
	ofEnableDepthTest();

	glDepthMask(false);
	fontUI.drawString("Altitude: " + ofToString(lander.altitude, 2), 20, 70);
	fontUI.drawString("Velocity: " + ofToString(lander.velocity.length(), 2), 20, 140);
	fontUI.drawString("Fuel time left: " + (lander.hasFuel() ? ofToString(lander.fuelLeftTime, 2) + " s" : "Out of Fuel!"), 20, 210);
	glDepthMask(true);

	activeCam->begin();
	ofPushMatrix();
	
	ofEnableLighting();
	lander.draw();
	terrain.drawFaces();
	landingPad.draw();
	ofDisableLighting();

	ofPopMatrix();
	activeCam->end();

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
		camGroundPosition.set(lander.terrainHitLocation);
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

/**
 * Update the gameplay camera based on the current view mode
 */
void ofApp::updateGameCamera()
{
	
	switch (camView)
	{
	case CAM_THIRD: // 3rd person: camera sits 6 up and 12 behind the lander, looks 3 ahead
		gameCam.setPosition(lander.position + lander.getHeadingY() * 20 - lander.getHeadingZ() * 25);
		gameCam.lookAt(lander.position + lander.getHeadingZ() * 3);
		break;
	case CAM_FIRST: //1st person: camera at lander position, looks straight forward
		gameCam.setPosition(lander.position.x, lander.position.y + 10, lander.position.z);
		gameCam.lookAt(gameCam.getPosition() + lander.getHeadingZ() * 3);
		break;
	case CAM_TOP:	// Top-down: camera 20 up above lander, looks straight down
		gameCam.setPosition(lander.position.x, lander.position.y + 50, lander.position.z);
		gameCam.lookAt(lander.position);
		break;
	case CAM_GROUND:
		gameCam.setPosition(camGroundPosition);
		gameCam.lookAt(lander.position);
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
