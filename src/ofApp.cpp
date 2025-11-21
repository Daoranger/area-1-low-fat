#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// Camera setup
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();
	initLightingAndMaterials();

	// UFO setup
	lander.loadModel();
	ofSetColor(255);

	// Terrain setup
	mars.loadModel("geo/ha.obj");
	mars.setScaleNormalization(false);


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

	if (keysMap[' ']) lander.force +=  THRUST_ACCEL  * lander.getHeadingY();		// up
	if (keysMap['w']) lander.force +=  FORWARD_ACCEL * lander.getHeadingZ();		// forward
	if (keysMap['s']) lander.force += -FORWARD_ACCEL * lander.getHeadingZ();	    // backward
	if (keysMap['a']) lander.force += -STRAFE_ACCEL  * lander.getHeadingX();		// left
	if (keysMap['d']) lander.force +=  STRAFE_ACCEL  * lander.getHeadingX();		// right
	if (keysMap['e']) lander.rotationForce -= YAW_TORQUE;							// yaw right
	if (keysMap['q']) lander.rotationForce += YAW_TORQUE;							// yaw left

	lander.force += lander.mass * moonGravity;

	lander.integrate();
}

//--------------------------------------------------------------
void ofApp::draw()
{

	cam.begin();
		

	lander.draw();
	mars.drawFaces();
	cam.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch (key)
	{
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
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
