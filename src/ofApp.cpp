#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()

{
	// UFO setup
	lander.loadModel();
	ofSetColor(255);

	// Terrain setup
	mars.loadModel("geo/ha.obj");
	mars.setScaleNormalization(false);

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
	// gravity force apply to lander, I used the moon gravity as default but we can play with it
	const glm::vec3 moonGravity = glm::vec3(0.0f, -1.68f, 0.0f);

	// will move these variables outside later
	const float THRUST_ACCEL = 15.0f;   
	const float FORWARD_ACCEL = 15.0f;   
	const float STRAFE_ACCEL = 15.0f;  
	const float YAW_TORQUE = 50.0f;   

	if (keysMap[' ']) lander.force +=  THRUST_ACCEL  * lander.getHeadingY();		// up (space)
	if (keysMap['w']) lander.force +=  FORWARD_ACCEL * lander.getHeadingZ();		// forward (w)
	if (keysMap['s']) lander.force += -FORWARD_ACCEL * lander.getHeadingZ();	    // backward (d)
	if (keysMap['a']) lander.force += -STRAFE_ACCEL  * lander.getHeadingX();		// left (a)
	if (keysMap['d']) lander.force +=  STRAFE_ACCEL  * lander.getHeadingX();		// right (d)
	if (keysMap['e']) lander.rotationForce -= YAW_TORQUE;							// yaw right (q)
	if (keysMap['q']) lander.rotationForce += YAW_TORQUE;							// yaw left (e)
	 
	lander.force += lander.mass * moonGravity;
	lander.integrate();

	// Gameplay Camera setup
	updateGameCamera();

}

//--------------------------------------------------------------
void ofApp::draw()
{

	activeCam->begin();
		
	lander.draw();
	mars.drawFaces();

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
	
	// cout << "Heading Z : " << lander.getHeadingZ() << '\n';
	switch (camView)
	{
	case CAM_THIRD: // 3rd person: camera sits 6 up and 12 behind the lander, looks 3 ahead
		gameCam.setPosition(lander.position + lander.getHeadingY() * 7 - lander.getHeadingZ() * 12);	
		gameCam.lookAt(lander.position + lander.getHeadingZ() * 3);				
		break;
	case CAM_FIRST: //1st person: camera at lander position, looks straight forward
		gameCam.setPosition(lander.position);												
		gameCam.lookAt(lander.position + lander.getHeadingZ());								
		break;
	case CAM_TOP:	// Top-down: camera 20 up above lander, looks straight down
		gameCam.setPosition(lander.position.x, lander.position.y + 20, lander.position.z);	
		gameCam.lookAt(lander.position);													
		break;
	}
}

/**
 * Get next camera view mode
 */
void ofApp::nextGameCameraView()
{
	camView = static_cast<CamView>((camView + 1) % 3);
}
