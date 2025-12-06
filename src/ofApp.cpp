#include "ofApp.h"
#include "collision/Util.h"

//--------------------------------------------------------------
void ofApp::setup()

{

	// Sounds & Musics setup
	if (titleMusic.load("sounds/titleMusic.mp3"))
	{   
		titleMusic.setLoop(true);
		titleMusic.setVolume(0.5f);
		titleMusic.play();
	}

	if (bgMusic.load("sounds/bgMusic.mp3"))
	{
		bgMusic.setLoop(true);
		bgMusic.setVolume(0.5f);
	}

	if (goMusic.load("sounds/goMusic.mp3"))
	{
		goMusic.setLoop(true);
		goMusic.setVolume(0.5f);
	}

	if (sparkSound.load("sounds/spark.mp3"))
	{
		sparkSound.setVolume(1.0f);
	}


	if (alarmSound.load("sounds/alarm.mp3"))
	{
		alarmSound.setVolume(1.0f);
	}

	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableDepthTest();
	
	skyBox.load("images/stars.png");
	titleBackground.load("images/titlebg.png");
	
	ofPushStyle();
	// Fonts setup
	if (fontUI.load("font/Stardock.ttf", 18, true, true))
	{
		ofSetLineWidth(5);
	}
	else
	{
		cout << "Failed to load font\n";
	}
	ofPopStyle();

	ofPushStyle();
	if (fontTitle.load("font/Stardock.ttf",50, true, true))
	{
		ofSetLineWidth(10);
	}
	else
	{
		cout << "Failed to load font\n";
	}
	ofPopStyle();

	ofPushStyle();
	if (fontInstruction.load("font/Stardock.ttf", 20, true, true))
	{
		ofSetLineWidth(5);
	}
	ofPopStyle();

	ofPushStyle();
	if (fontSmallText.load("font/Stardock.ttf", 12, true, true))
	{
		ofSetLineWidth(5);
	}
	ofPopStyle();


	// UFO setup
	ufo.loadModel();

	// Terrain setup
	bTerrainLoaded = terrain.loadModel("geo/terrain-oct.obj");
	terrainColor.loadModel("geo/terrain.obj");
	terrain.setScaleNormalization(false);
	terrainColor.setScaleNormalization(false);
	terrainOctree.create(terrain.getMesh(1), 20);

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
	debugCam.setTarget(mothership.position);
	debugCam.setPosition(mothership.position + ofVec3f(0, 50, 0));
	debugCam.setDistance(50.0f);
	debugCam.setNearClip(0.1f);
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
			resetGame();
			break;
		}
		case STATE_GAMESTART:
		{			
			// Calculating time since dead
			if (ufo.bDead)
			{
				float deathElapsed = ofGetElapsedTimef() - deathStartTime;
				if (deathElapsed >= 5.0f)
				{
					if (bgMusic.isPlaying())
						bgMusic.stop();
					gameState = STATE_GAMEOVER;
					if (!goMusic.isPlaying())
						goMusic.play();
				}
				else if (deathElapsed >= 3.0f && !bSparkSoundPlayed)
				{
					cout << "Spark sound play\n";
					sparkSound.play();
					bSparkSoundPlayed = true;
				}
			}

			if (!ufo.hasFuel() && !ufo.bDead && !bFuelDeathPending)          
			{
				bFuelDeathPending = true;
				fuelDeathStartTime = ofGetElapsedTimef();

				// player alarm here
				if (!alarmSound.isPlaying())
					alarmSound.play();
			}
			
			// Play alarm for 5 seconds before death
			if (bFuelDeathPending && !ufo.bDead)
			{
				float elapsed = ofGetElapsedTimef() - fuelDeathStartTime;

				if (elapsed >= 5.0f)
				{
					if (alarmSound.isPlaying())
					{
						alarmSound.stop();
					}

					camTrackPosition = mothership.position + ofVec3f(0, 50, 0);
					camView = CAM_DEATH;
					ufo.handleDeath(ufo.getHeadingY());
					deathStartTime = ofGetElapsedTimef();
					bFuelDeathPending = false;
				}
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
			constexpr float THRUST_UP_ACCEL = 20.0f;
			constexpr float THRUST_DOWN_ACCEL = 15.0f;
			constexpr float FORWARD_ACCEL = 20.0f;
			constexpr float STRAFE_ACCEL = 20.0f;
			constexpr float YAW_TORQUE = 50.0f;

			// Beam
			beam.update();
			beam.pos.x = ufo.position.x;
			beam.pos.z = ufo.position.z;
			beam.pos.y = ufo.position.y - (beam.height / 2);


			// Thrust Forces
			if (keysMap[OF_KEY_SHIFT] && ufo.hasFuel())													// down (shift)
			{
				ufo.force += -THRUST_DOWN_ACCEL * ufo.getHeadingY();
				float deltaTime = 1.0 / ofGetFrameRate();
				ufo.fuelLeftTime = max(static_cast<float>(0.0), ufo.fuelLeftTime - deltaTime);
			}

			if (keysMap[' '] && ufo.hasFuel())																// up (space)
			{
				ufo.handleTakeoff();
				ufo.force += THRUST_UP_ACCEL * ufo.getHeadingY();
				float deltaTime = 1.0 / ofGetFrameRate();
				ufo.fuelLeftTime = max(static_cast<float>(0.0), ufo.fuelLeftTime - deltaTime);
			}

			if (keysMap['w']) ufo.force += FORWARD_ACCEL * ufo.getHeadingZ();				// forward (w)
			if (keysMap['s']) ufo.force += -FORWARD_ACCEL * ufo.getHeadingZ();				// backward (d)
			if (keysMap['a']) ufo.force += -STRAFE_ACCEL * ufo.getHeadingX();				// left (a)
			if (keysMap['d']) ufo.force += STRAFE_ACCEL * ufo.getHeadingX();				// right (d)
			if (keysMap['e']) ufo.rotationForce -= YAW_TORQUE;								// yaw right (q)
			if (keysMap['q']) ufo.rotationForce += YAW_TORQUE;								// yaw left (e)
			
			// Gravity Force: did not use moon gravity because it feel to low for gameplay
			const glm::vec3 gravity = glm::vec3(0.0f, -5.0f, 0.0f);
			ufo.force += ufo.mass * gravity;

			// Turbulence Force
			ufo.force.x += ofRandom(-5, 5);
			ufo.force.y += ofRandom(-5, 5);
			ufo.force.z += ofRandom(-5, 5);

			ufo.integrate();

			// Gameplay Camera update
			updateGameCamera();
			// Tracking Camerea update
			updateTrackCamera();

			// Handle UFO Collision
			ufo.updateBoundingBox();
			colBoxList.clear();
			colNodeList.clear();

			// UFO vs Terrain
			terrainOctree.intersect(ufo.boundingBox, terrainOctree.root, colBoxList, colNodeList);

			// UFO vs Mothership
			if (mothership.octree.intersect(ufo.boundingBox, mothership.octree.root, mothership, colBoxList, colNodeList))
			{
				mothership.handleCollision(ufo);
			}

			// UFO vs Charging Stations

			// Station 1:
			if (station1.octree.intersect(ufo.boundingBox, station1.octree.root, station1, colBoxList, colNodeList))
			{
				station1.handleCollision(ufo);
			}


			// Handle UFO vs octrees collision based on number of collided boxes
			if (colBoxList.size() >= 1)
			{
				if (ufo.velocity.length() >= 4.0)
				{
					if (bFuelDeathPending)
					{
						bFuelDeathPending = false;
						if (alarmSound.isPlaying())
							alarmSound.stop();
					}

					ofVec3f contactNormal = getNormalAtContactPoint();
					ufo.handleDeath(contactNormal);
					camTrackPosition = mothership.position + ofVec3f(0, 50, 0);
					camView = CAM_DEATH;
					deathStartTime = ofGetElapsedTimef();			// save current death time in seconds
				}
				else
				{
					if (!keysMap[' '])
					{
						// Handle Impulse Force and Landing
						ofVec3f contactNormal = getNormalAtContactPoint();
						ufo.handleLanding(contactNormal);
					}
				}
			}

			// Handle collision of cow 
			cow1.updateBoundingBox();
			vector<TreeNode> cowNodeList;		// Store all collided (leaf) nodes
			vector<Box> cowBoxList;				// Store all collided (leaf) nodes's boxes
			terrainOctree.intersect(cow1.boundingBox, terrainOctree.root, cowBoxList, cowNodeList);
			station1.octree.intersect(cow1.boundingBox, station1.octree.root, station1, cowBoxList, cowNodeList);
			if (cowPlatform.octree.intersect(cow1.boundingBox, cowPlatform.octree.root, cowPlatform, cowBoxList, cowNodeList))
			{
				cout << "Cow touched platform\n";
				cow1.destroy();
				cow1.free();
			}

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
			break;
		}
		case STATE_GAMEOVER:
		{
			break;
		}
		case STATE_INSTRUCTION:
		{
			break;
		}
		case STATE_DIAGNOSTIC:
		{
			ufo.updateBoundingBox();
			debugCam.setTarget(mothership.position);
			activeCam = &debugCam;
			break;
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

			ofPushStyle();
			ofSetColor(ofColor::lightCyan);

			fontTitle.drawString(strGameTitle, (ofGetWindowWidth() - fontTitle.stringWidth(strGameTitle)) * 0.5f, ofGetWindowHeight() / 2.5);
		
			float space = ofGetWindowHeight() / 12;

			drawMenuItem(MENU_START, strStart, centerY);
			drawMenuItem(MENU_INSTR, strInstr, centerY + space);
			drawMenuItem(MENU_DIAG, strSand, centerY + space*2);
			drawMenuItem(MENU_QUIT, strQuit, centerY + space*3);
			ofPopStyle();

			break;
		}
		case STATE_GAMESTART:
		case STATE_DIAGNOSTIC:
		{
			ofDisableDepthTest();
			ofDisableLighting();
			ofBackground(ofColor::black);
			// The 2D sky box image (don't draw it in 3D)
			skyBox.draw(0, 0, ofGetWidth(), ofGetHeight());
			ofEnableDepthTest();

			ofEnableLighting();
			activeCam->begin();
			ofPushMatrix();

			ufo.draw();
			terrainColor.drawFaces();
			station1.draw();
			mothership.draw();
			speedRing1.draw();
			if (cow1.bAlive)
			{
				cow1.draw();
			}
			cowPlatform.draw();
			beam.draw();

			if (bUfoSelected)
			{
				ofPushStyle();
				ofNoFill();
				ofSetColor(ofColor::white);
				Octree::drawBox(ufo.boundingBox);
				ofPopStyle();
			}

			// might remove this later
			if (bDrawOctree)
			{
				ofNoFill();
				ofSetColor(ofColor::white);
				terrainOctree.drawLeafNodes(terrainOctree.root);
				Octree::drawBox(ufo.boundingBox);
				Octree::drawBox(cow1.boundingBox);
			}

			ofPopMatrix();
			activeCam->end();
			ofDisableDepthTest();
			ofDisableLighting();

			// UI draw: don't include it in the 3D stuff (within camera)
			glDepthMask(false);
			
			if (gameState == STATE_GAMESTART)
			{
				// Fuel bar
				ofPushStyle();
				fontUI.drawString("Altitude: " + ofToString(ufo.altitude, 2), 20, 70);
				ofPushStyle();
				ofSetColor(ufo.velocity.length() >= 4.0f ? ofColor::red : ofColor::white);
				fontUI.drawString("Velocity: " + ofToString(ufo.velocity.length(), 2), 20, 140);
				ofPopStyle();
				fontUI.drawString("Fuel time left: " + (ufo.hasFuel() ? ofToString(ufo.fuelLeftTime, 2) + " s" : "Out of Fuel!"), 20, 210);
				float fuelPercent = ufo.fuelLeftTime / ufo.fuelTotalTime;

				ofFill();
				ofSetColor(ofColor::darkSlateGray);
				ofDrawRectangle(18, 228, 154, 19);
				ofSetColor(ofColor::black);
				ofDrawRectangle(20, 230, 150, 15);
				ofSetColor(ofColor::aqua);
				ofDrawRectangle(20, 230, 150 * fuelPercent, 15);
				ofSetColor(ofColor::darkSlateGray);
				ofDrawTriangle(glm::vec3(18, 228, 0), glm::vec3(18, 240, 0), glm::vec3(30, 228, 0));

				ofPopStyle();
			}
			else if (gameState == STATE_DIAGNOSTIC)
			{
				// Draw the press R to start
				ofPushStyle();
				ofSetColor(ofColor::white);
				std::string strReturn = "[Press R to Start]";
				ofRectangle boundReturn = fontSmallText.getStringBoundingBox(strReturn, 0, 0);
				fontSmallText.drawString(strReturn, (ofGetWidth() - boundReturn.getWidth()) * 0.5f, (ofGetHeight() - boundReturn.getHeight()) * 0.5f + 500);

				std::string strCamera = "[Press C to Enable Debug Camera]";
				ofRectangle boundCamera = fontSmallText.getStringBoundingBox(strCamera, 0, 0);
				fontSmallText.drawString(strCamera, (ofGetWidth() - boundCamera.getWidth()) * 0.5f, (ofGetHeight() - boundCamera.getHeight()) * 0.5f + 600);
				ofPopStyle();
			}

			glDepthMask(true);
			break;
		}
		case STATE_GAMEOVER:
		{
			ofDisableDepthTest();
			ofDisableLighting();

			ofSetColor(ofColor::white);
			titleBackground.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

			std::string strGameOver = "GAME OVER";
			std::string strRestart = "Press R to Restart";
			std::string strMainMenu = "Press M to Return to Main Menu";

			// lambda to center each menu items text
			auto centerX = [&](const std::string str)
				{
					return (ofGetWindowWidth() - fontUI.stringWidth(str)) * 0.5f;
				};

			auto centerY = ofGetWindowHeight() * 0.5f;

			ofPushStyle();
			ofSetColor(ofColor::lightCyan);

			fontTitle.drawString(strGameOver, (ofGetWindowWidth() - fontTitle.stringWidth(strGameOver)) * 0.5f, ofGetWindowHeight() / 2.0);

			float space = ofGetWindowHeight() / 12;

			fontUI.drawString(strRestart, centerX(strRestart), centerY + space * 1);
			fontUI.drawString(strMainMenu, centerX(strMainMenu), centerY + space * 2);
			ofPopStyle();
			break;
		}
		case STATE_INSTRUCTION:
		{
			ofDisableDepthTest();
			ofDisableLighting();

			ofPushStyle();
			ofBackground(ofColor::black);
			ofSetColor(ofColor::white);

			// Draw the itro and keybinds text
			std::string strInstr =
				"Hi Mr. Green Gremlin,\n"
				"We will invade Earth in a few years. Our species is small compared to humans.\n"
				"However, we discovered that there is an animal from Earth called a “cow” that\n"
				"produces something called “milk” and it makes us grow bigger and stronger.\n"
				"You are assigned a very important mission: abduct cows from Earth.\n"
				"Please deliver each cow to the transport platform and they will be automatically\n"
				"transferred to the mothership.After you finish, return to the mothership.\n"
				"Good luck on your mission! Try not to blow up!\n"
				"\n\n\n"
				"Space: go up\n"
				"Shift: go down\n"
				"W: move forward\n"
				"S: move backward\n"
				"A: move left\n"
				"D: move right\n"
				"\n"
				"E: yaw right\n"
				"Q: yaw left\n"
				"\n"
				"F: use beam (abduct the cow!)\n"
				"R: turn on light (just for aesthetics)\n";
			
			ofRectangle boundInstr = fontInstruction.getStringBoundingBox(strInstr, 0, 0); // Bouding box of the instruction string
			float x = (ofGetWidth() - boundInstr.getWidth()) * 0.5f - boundInstr.x;
			float y = (ofGetHeight() - boundInstr.getHeight()) * 0.5f - boundInstr.y;
			fontInstruction.drawString(strInstr, x, y);

			// Draw the return to menu text
			std::string strReturn = "[Press R to return]";
			ofRectangle boundReturn = fontSmallText.getStringBoundingBox(strReturn, 0, 0);
			fontSmallText.drawString(strReturn, (ofGetWidth() - boundReturn.getWidth()) * 0.5f, (ofGetHeight() - boundReturn.getHeight()) * 0.5f + 500);



			ofPopStyle();
			break;
		}
	}
	

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// Normalize all key to lowercase (fix the bug where holding shift cause keys to beocome uppercase)
	unsigned char c = static_cast<unsigned char>(key);

	if (std::isalpha(c)) {
		c = static_cast<unsigned char>(std::tolower(c));
	}
	keysMap[c] = true;

	switch (gameState)
	{
		case STATE_TITLE:
		{
			if (key == 'w' || key == 'W')
			{
				if (currentMenuItem == MENU_START)
					currentMenuItem = MENU_QUIT;
				else
					currentMenuItem = static_cast<MenuItem>(currentMenuItem - 1);
			}
			else if (key == 's' || key == 'S')
			{
				if (currentMenuItem == MENU_QUIT)
					currentMenuItem = MENU_START;
				else
					currentMenuItem = static_cast<MenuItem>(currentMenuItem + 1);
			}
			else if (key == 'r' || key == 'R')
			{
				switch (currentMenuItem)
				{
				case MENU_START:
					titleMusic.stop();
					gameState = STATE_GAMESTART;
					bgMusic.play();
					break;
				case MENU_INSTR:
					gameState = STATE_INSTRUCTION;
					break;
				case MENU_DIAG:
					titleMusic.stop();
					gameState = STATE_DIAGNOSTIC;
					bgMusic.play();
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
			switch (ufo.ufoState)
			{
			case ufo.UFO_ALIVE:
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
					if (activeCam == &trackCam) nextTrackCameraView();
					else activeCam = &trackCam;
					break;
				case '2':
					if (activeCam == &gameCam) nextGameCameraView();
					else activeCam = &gameCam;
					break;
				}

				break;
			}
			case ufo.UFO_DEAD:
				break;
			}
			break;
		}
		case STATE_GAMEOVER:
		{
			// reset all keys pressed when game over (to fix the key not being released when switching game mode)
			for (auto& entry : keysMap)
			{
				entry.second = false;
			}
			if (key == 'r' || key == 'R')
			{
				goMusic.stop();
				gameState = STATE_GAMESTART;
				resetGame();
			}
			if (key == 'm' || key == 'M')
			{
				goMusic.stop();
				gameState = STATE_TITLE;
				resetGame();
			}
			break;
		}
		case STATE_INSTRUCTION:
		{
			if (key == 'r' || key == 'R')
			{
				gameState = STATE_TITLE;
			}
			break;
		}
		case STATE_DIAGNOSTIC:
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
				bUfoSelected = false;
				bInDrag = false;
				activeCam = &gameCam;
				gameState = STATE_GAMESTART;
			}
			break;
		}
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	unsigned char c = static_cast<unsigned char>(key);

	if (std::isalpha(c)) {
		c = static_cast<unsigned char>(std::tolower(c));
	}
	keysMap[c] = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
	switch (gameState)
	{
		case STATE_TITLE:
			break;
		case STATE_GAMESTART:
			break;
		case STATE_GAMEOVER:
			break;
		case STATE_INSTRUCTION:
			break;
		case STATE_DIAGNOSTIC:
		{
			if (debugCam.getMouseInputEnabled()) return;

			if (bInDrag)
			{
				glm::vec3 ufoPos = ufo.position;

				glm::vec3 mousePos = getMousePointOnPlane(ufoPos, debugCam.getZAxis());
				glm::vec3 delta = mousePos - mouseLastPos;

				ufoPos += delta;
				ufo.position = ofVec3f(ufoPos.x, ufoPos.y, ufoPos.z);
				mouseLastPos = mousePos;
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	switch (gameState)
	{
	case STATE_TITLE:
		break;
	case STATE_GAMESTART:
		break;
	case STATE_GAMEOVER:
		break;
	case STATE_INSTRUCTION:
		break;
	case STATE_DIAGNOSTIC:
		
		// If debug camera not enable, don't allow mouse interaction
		if (debugCam.getMouseInputEnabled()) return;

		cout << "Can Press in Diagnostic Mode\n";

		glm::vec3 origin = debugCam.getPosition();
		glm::vec3 mouseWorld = debugCam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

		Ray mouseRay(
			Vector3(origin.x, origin.y, origin.z),
			Vector3(mouseDir.x, mouseDir.y, mouseDir.z)
		);

		bool hit = ufo.boundingBox.intersect(mouseRay, 0.0f, 10000.0f);

		if (hit) {
			bUfoSelected = true;
			mouseDownPos = getMousePointOnPlane(ufo.position, debugCam.getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
		}
		else
		{
			bUfoSelected = false;
		}

	}
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
	bInDrag = false;
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
	ofMesh mesh = terrain.getMesh(1);
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

void ofApp::resetGame()
{
	if (gameState == STATE_GAMESTART)
		bgMusic.play();
	else if (gameState == STATE_TITLE && !titleMusic.isPlaying())
		titleMusic.play();


	ufo.bDead = false;
	deathStartTime = 0.0f;
	camView = CAM_THIRD;
	activeCam = &gameCam;

	ufo.velocity = glm::vec3(0, 0, 0);
	ufo.force = glm::vec3(0, 0, 0);
	ufo.rotationForce = 0.0f;
	ufo.rotationSpeed = 0.0f;
	ufo.position = startUfoPos;    
	ufo.rotation = 0.0f;
	ufo.ufoState = ufo.UFO_ALIVE; 
	ufo.fuelLeftTime = ufo.fuelTotalTime;
	ufo.bLandingImpulseDone = false;

	// Misc
	bSparkSoundPlayed = false;

	// Cow reset
	cow1.bAlive = true;
	cow1.position = ofVec3f(30, 250, 0);
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
	case CAM_DEATH:
		gameCam.setPosition(camTrackPosition);
		gameCam.lookAt(ufo.position);
		break;
	}
}

void ofApp::updateTrackCamera()
{
	switch (trackView)
	{
	case TRACK_MOTHERSHIP:
		trackCam.setPosition(mothership.position + ofVec3f(0, 30, 0));
		trackCam.lookAt(ufo.position);
		break;
	case TRACK_MOUNTAIN: 
		trackCam.setPosition(ofVec3f(935.0f, 693.0f, 248.0f));
		trackCam.lookAt(ufo.position);
		break;
	case TRACK_TREE:
		trackCam.setPosition(ofVec3f(255.0f, 148.0f, 105.0f));
		trackCam.lookAt(ufo.position);
		break;
	case TRACK_LAKE:
		trackCam.setPosition(ofVec3f(-600.0f, 21.0f, -171.0f));
		trackCam.lookAt(ufo.position);
		break;
	case TRACK_SKY:
		trackCam.setPosition(ofVec3f(0.0f, 1700.0f, 0.0f));
		trackCam.lookAt(mothership.position);
		break;
	case TRACK_COW:
		trackCam.setPosition(cow1.position);
		trackCam.lookAt(ufo.position);
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

/**
 * Get next tracking view mode
 */
void ofApp::nextTrackCameraView()
{
	trackView = static_cast<TrackView>((trackView + 1) % 6);
}

glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm)
{
	// Setup our rays
	//
	glm::vec3 origin = debugCam.getPosition();
	glm::vec3 camAxis = debugCam.getZAxis();
	glm::vec3 mouseWorld = debugCam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}
