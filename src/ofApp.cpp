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
	titleBackground.load("images/titlebg.png");

	// Fonts setup
	if (fontUI.load("font/Stardock.ttf", 18, true, true))
	{
		ofSetLineWidth(5);
	}
	else
	{
		cout << "Failed to load font\n";
	}

	if (fontTitle.load("font/Stardock.ttf",50, true, true))
	{
		ofSetLineWidth(10);
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
	cowPlatform.loadModel();
	cowPlatform.createOctree();

	// Debug Camera setup
	debugCam.setDistance(10);
	debugCam.setNearClip(.1);
	debugCam.setFov(65.5);
	debugCam.disableMouseInput();

	// Beam Setup
	//
	beam.radius = 5;
	beam.maxHeight = 50;
	beam.extendRate = 2;
	beam.retractRate = 5;

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
	sunLight.setSpecularColor(ofFloatColor(0.5, 0.65, 0.7));
	sunLight.setDiffuseColor(ofFloatColor(0.6, 0.65, 0.7));				
	sunLight.setAmbientColor(ofFloatColor(0.56, 0.6, 0.6));
	
	sunLight2.setup();
	sunLight2.enable();
	//sunLight2.setDirectional();
	sunLight2.setPosition(-500, -400, 500);
	sunLight2.setAreaLight(1000, 1000);
	sunLight2.setSpecularColor(ofFloatColor(0.4, 0.45, 0.5));
	sunLight2.setDiffuseColor(ofFloatColor(0.3, 0.4, 0.5));				
	sunLight2.setAmbientColor(ofFloatColor(0.3, 0.3, 0.25)); 

	sunLight3.setup();
	sunLight3.enable();
	//sunLight3.setDirectional();
	sunLight3.setPosition(-500, 400, -500);
	sunLight3.setAreaLight(1000, 1000);
	sunLight3.setSpecularColor(ofFloatColor(0.4, 0.45, 0.5));
	sunLight3.setDiffuseColor(ofFloatColor(0.3, 0.4, 0.5));				
	sunLight3.setAmbientColor(ofFloatColor(0.25, 0.3, 0.3)); 

}

//--------------------------------------------------------------
void ofApp::update()
{
	float scaleX = ofGetWindowWidth() / (ORIGINAL_WIDTH*1.0);
	float scaleY = ofGetWindowHeight() / (ORIGINAL_HEIGHT*1.0);

	if (scaleX != lastScaleX || scaleY != lastScaleY) {
		float scale = std::min(scaleX, scaleY);
		fontUI.load("font/Stardock.ttf", 18*scale, true, true);
		ofSetLineWidth(5);
		if (gameState == STATE_TITLE) {
			fontTitle.load("font/Stardock.ttf",50*scale, true, true);
			ofSetLineWidth(10);
		}
		lastScaleX = scaleX;
		lastScaleY = scaleY;
	}
	switch (gameState)
	{
		case STATE_TITLE:
		{
			break;
		}
		case STATE_GAMESTART:
		{
			if (!ufo.hasFuel())
			{
				;
			}
			
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

			// will move these variables outside later
			constexpr float THRUST_ACCEL = 15.0f;
			constexpr float FORWARD_ACCEL = 15.0f;
			constexpr float STRAFE_ACCEL = 15.0f;
			constexpr float YAW_TORQUE = 50.0f;

			// Beam
			beam.update();
			beam.pos.x = ufo.position.x;
			beam.pos.z = ufo.position.z;
			beam.pos.y = ufo.position.y - (beam.height/2);
		
			// Thrust Force
			if (keysMap[' '] && ufo.hasFuel())												// up (space)
			{
				ufo.handleTakeoff();
				ufo.force += THRUST_ACCEL * ufo.getHeadingY();
				float deltaTime = 1.0 / ofGetFrameRate();
				ufo.fuelLeftTime = max(static_cast<float>(0.0), ufo.fuelLeftTime - deltaTime);
			}

			if (keysMap['w']) ufo.force += FORWARD_ACCEL * ufo.getHeadingZ();			// forward (w)
			if (keysMap['s']) ufo.force += -FORWARD_ACCEL * ufo.getHeadingZ();			// backward (d)
			if (keysMap['a']) ufo.force += -STRAFE_ACCEL * ufo.getHeadingX();			// left (a)
			if (keysMap['d']) ufo.force += STRAFE_ACCEL * ufo.getHeadingX();			// right (d)
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

			if (colBoxList.size() >= 1)
			{
				if (!keysMap[' '])
				{
					// Handle Impulse Force and Landing
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
			else
			{
				cow1.integrate();
			}

			// Beam Collision with cow
			// for every cow [TBD] make sure to free all other cows after finding 1st cow
			if (beam.checkInside(cow1.boundingBox) && !cowCaptured) {
				cow1.follow(&beam.capturePoint);
				cowCaptured = true;
			}
			else if (!beam.active) {
				cowCaptured = false;
				cow1.free();
			}		

		}
	}
	
}

//--------------------------------------------------------------
void ofApp::draw()
{
	switch (gameState)
	{
		case STATE_TITLE:
		{
			ofDisableDepthTest();
			ofDisableLighting();

			ofSetColor(ofColor::white);
			titleBackground.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

			std::string strGameTitle = "Area 1: Low Fat";
			std::string strStart = "Start Game";
			std::string strInstr = "Instructions";
			std::string strSand = "Diagnostic Mode";
			std::string strQuit = "Quit Game";
		
			// lambda to center each menu items text
			auto centerX = [&](const std::string str)
				{
					return (ofGetWindowWidth() - fontUI.stringWidth(str)) * 0.5f;
				};

			auto centerY = ofGetWindowHeight() * 0.5f;

			// lambda to draw and check whether or not the item is currently the selected menu item
			auto drawMenuItem = [&](MenuItem item, const std::string& label, float y)
				{
					if (currentMenuItem == item)
						ofSetColor(ofColor::lightBlue);   // selected
					else
						ofSetColor(ofColor::white);       // normal

					fontUI.drawString(label, centerX(label), y);
				};

			ofSetColor(ofColor::lightCyan);

			fontTitle.drawString(strGameTitle, (ofGetWindowWidth() - fontTitle.stringWidth(strGameTitle)) * 0.5f, ofGetWindowHeight() / 2.5);
		
			float space = ofGetWindowHeight() / 12;

			drawMenuItem(MENU_START, strStart, centerY);
			drawMenuItem(MENU_INSTR, strInstr, centerY + space);
			drawMenuItem(MENU_DIAG, strSand, centerY + space*2);
			drawMenuItem(MENU_QUIT, strQuit, centerY + space*3);

			break;
		}
		case STATE_GAMESTART:
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
			cowPlatform.draw();
			beam.draw();

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
			ofSetColor(ofColor::lightCyan);
			fontUI.drawString("Altitude: " + ofToString(ufo.altitude, 2), 20, 70);
			fontUI.drawString("Velocity: " + ofToString(ufo.velocity.length(), 2), 20, 140);
			fontUI.drawString("Fuel time left: " + (ufo.hasFuel() ? ofToString(ufo.fuelLeftTime, 2) + " s" : "Out of Fuel!"), 20, 210);
			float fuelPercent = ufo.fuelLeftTime / ufo.fuelTotalTime;
			
			// Fuel bar
			ofFill();
			ofSetColor(ofColor::darkSlateGray);
			ofDrawRectangle(18, 228, 154, 19);
			ofSetColor(ofColor::black);
			ofDrawRectangle(20, 230, 150, 15);
			ofSetColor(ofColor::aqua);
			ofDrawRectangle(20, 230, 150*fuelPercent, 15);
			ofSetColor(ofColor::darkSlateGray);
			ofDrawTriangle(glm::vec3(18, 228, 0), glm::vec3(18, 240, 0), glm::vec3(30, 228, 0));

			glDepthMask(true);
			break;
		}
		case STATE_GAMEOVER:
		{
			break;
		}
	}
	

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch (gameState)
	{
		case STATE_TITLE:
		{
			if (key == OF_KEY_UP)
			{
				if (currentMenuItem == MENU_START)
					currentMenuItem = MENU_QUIT;
				else
					currentMenuItem = static_cast<MenuItem>(currentMenuItem - 1);
			}
			else if (key == OF_KEY_DOWN)
			{
				if (currentMenuItem == MENU_QUIT)
					currentMenuItem = MENU_START;
				else
					currentMenuItem = static_cast<MenuItem>(currentMenuItem + 1);
			}
			else if (key == OF_KEY_RETURN)
			{
				switch (currentMenuItem)
				{
				case MENU_START:
					gameState = STATE_GAMESTART;
					break;
				case MENU_INSTR:
					break;
				case MENU_DIAG:
					break;
				case MENU_QUIT:
					ofExit();
					break;
				}
			}
			break;
		}
		case STATE_GAMESTART:
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
			case 'F':
			case 'f':
				beam.toggle();
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
			break;
		}
	}

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
