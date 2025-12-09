#include "ofApp.h"
#include "collision/Util.h"

//--------------------------------------------------------------
void ofApp::setup()

{

	// Sounds & Musics setup
	if (titleMusic.load("sounds/titleMusic.mp3"))
	{   
		titleMusic.setLoop(true);
		titleMusic.setVolume(0.3f);
		titleMusic.play();
	}

	if (bgMusic.load("sounds/gameBGMusic.mp3"))
	{
		bgMusic.setLoop(true);
		bgMusic.setVolume(0.2f);
	}

	if (goMusic.load("sounds/gameOverMusic.mp3"))
	{
		//goMusic.setLoop(true);
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

	if (ufoSound.load("sounds/ufo-noise.mp3"))
	{
		ufoSound.setLoop(true);
		ufoSound.setVolume(0.4);
	}

	if (menuHoverSound.load("sounds/menu-hover.mp3"))
	{
		menuHoverSound.setVolume(1.0);
	}

	if (menuSelectedSound.load("sounds/menu-select.mp3"))
	{
		menuSelectedSound.setVolume(1.5);
	}

	if (ufoBeamSound.load("sounds/beam-active.mp3"))
	{
		ufoBeamSound.setVolume(1.0);
	}

	if (ufoExplosionSound.load("sounds/explosion.mp3"))
	{
		ufoExplosionSound.setVolume(0.5);
	}

	if (cowMooSound.load("sounds/cow-moo.mp3"))
	{
		cowMooSound.setVolume(0.5);
	}

	if (cowTeleportSound.load("sounds/cow-teleport.mp3"))
	{
		cowTeleportSound.setVolume(0.5);
	}
	
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableDepthTest();
	
	skyBox.load("images/stars.png");
	titleBackground.load("images/titlebg.png");
	victoryBackground.load("images/victorybg.png");
	defeatBackground.load("images/defeatbg.png");
	star.load("images/Star.png");
	
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

	// Shader Setup
	#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/shader");
	#else
	shader.load("shaders/shader");
	#endif


	// UFO setup
	ufo.loadModel();

	// Terrain setup
	bTerrainLoaded = terrain.loadModel("geo/terrain-oct.obj");
	terrainColor.loadModel("geo/terrain.obj");
	terrain.setScaleNormalization(false);
	terrainColor.setScaleNormalization(false);
	terrainOctree.create(terrain.getMesh(0), 20);

	// Game Object
	//
	station1.loadModel();
	station1.createOctree();
	station2.loadModel();
	station2.createOctree();
	station2.position = ofVec3f(536.866, 165, -436.84);
	station3.loadModel();
	station3.createOctree();
	station3.position = ofVec3f(-442.068, 145, 470.853);

	mothership.loadModel();
	mothership.createOctree();
	mothership.position = ofVec3f(0, 500, 0);

	// Up to mothership
	speedRing1.bVertical = false;
	speedRing1.loadModel();
	speedRing1.createOctree();
	speedRing1.position = ofVec3f(50, 300, 50);

	// Down to ground from mothership
	speedRing2.bVertical = false;
	speedRing2.loadModel();
	speedRing2.createOctree();
	speedRing2.position = ofVec3f(50, 500, 50);

	// station 3 to cow platform
	speedRing3.bVertical = true;
	speedRing3.loadModel();
	speedRing3.createOctree();
	speedRing3.position = ofVec3f(-420, 175, 0);

	// station 2 to station 1
	speedRing4.bVertical = true;
	speedRing4.loadModel();
	speedRing4.createOctree();
	speedRing4.position = ofVec3f(260, 200, -250);
	speedRing4.rotation = 135;

	// cow platform to mothership
	speedRing5.bVertical = true;
	speedRing5.loadModel();
	speedRing5.createOctree();
	speedRing5.position = ofVec3f(-250, 200, -200);
	speedRing5.rotation = 90;
	
	// Create 10 cows object
	for (auto i = 0; i < 10; ++i)
	{
		cows.push_back(std::make_unique<Cow>());
	}

	// Load all 10 cows
	// Assign each cows in the list with the preassigned positions list
	for (auto i = 0; i < cows.size() && i < cowsPositions.size(); ++i)
	{
		cows[i]->loadModel();
		//cows[i]->position = cowsPositions[i];
	}



	cow1.loadModel();
	cow1.position.y += 50;
	cow1.position.x += 30;
	
	// Cow platform
	cowPlatform.loadModel();
	cowPlatform.createOctree();
	cowPlatform.position = ofVec3f(-400, 50, -216.059);

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

	// Particle Setup
	//

	// Particles when UFO is using its thruster
	ufoMoveV.speed = 20;
	ufoMoveV.setEmitterShape(DirectionalEmitter);
	ufoMoveV.setParticleShape(DISK);
	ufoMoveV.rate = 0.5;
	ufoMoveV.numParticles = 1;
	ufoMoveV.setLifespan(1);
	ufoMoveV.colors.push_back(ofColor(0, 255, 255, 50));
	ufoMoveV.direction = ofVec3f(0, -1, 0);
	ufoMoveV.radius = 4;
	ufoMoveV.start();
	ufoMoveV.scale = 1;
	ufoMoveV.scaleRate = 1.01;

	// Particles for any UFO movement
	ufoMoveL.speed = 0;
	ufoMoveL.setEmitterShape(DirectionalEmitter);
	ufoMoveL.setParticleShape(SPHERE);
	ufoMoveL.rate = 0.01;
	ufoMoveL.setLifespan(1);
	ufoMoveL.colors.push_back(ofColor(0, 255, 255));
	ufoMoveL.direction = ofVec3f(0, 0, 0);
	ufoMoveL.radius = 0.6;
	ufoMoveL.scaleRate = 0.9;

	ufoMoveR.speed = 0;
	ufoMoveR.setEmitterShape(DirectionalEmitter);
	ufoMoveR.setParticleShape(SPHERE);
	ufoMoveR.rate = 0.01;
	ufoMoveR.setLifespan(1);
	ufoMoveR.colors.push_back(ofColor(0, 255, 255));
	ufoMoveR.direction = ofVec3f(0, 0, 0);
	ufoMoveR.radius = 0.6;
	ufoMoveR.scaleRate = 0.9;
	
	// Particles when UFO crashes
	ufoExplosion.speed = 300;
	ufoExplosion.acceleration = 60;
	ufoExplosion.rotAccel = 100;
	ufoExplosion.setEmitterShape(SphereEmitter);
	ufoExplosion.setParticleShape(CUBE);
	ufoExplosion.numParticles = 500;
	ufoExplosion.setLifespan(1);
	ufoExplosion.colors.push_back(ofColor::orangeRed);
	ufoExplosion.colors.push_back(ofColor::yellow);
	ufoExplosion.colors.push_back(ofColor::black);
	ufoExplosion.colors.push_back(ofColor::red);
	ufoExplosion.scaleRate = 0.95;
	ufoExplosion.radius = 3;

	ufoFireTrail.speed = 20;
	ufoFireTrail.setEmitterShape(ConeEmitter);
	ufoFireTrail.setParticleShape(SPHERE);
	ufoFireTrail.numParticles = 10;
	ufoFireTrail.setLifespan(2);
	ufoFireTrail.scaleRate = 1.02;
	ufoFireTrail.radius = 0.2;
	ufoFireTrail.rate = 0.001;
	ufoFireTrail.direction = ofVec3f(0,-1,0);
	ufoFireTrail.colors.push_back(ofColor::orangeRed);
	ufoFireTrail.colors.push_back(ofColor::black);
	ufoFireTrail.colors.push_back(ofColor(45, 45, 45));
	ufoFireTrail.colors.push_back(ofColor(85, 85, 85));
	ufoFireTrail.stop();
}

//--------------------------------------------------------------
void ofApp::update()
{
	//cout << "Ufo's position: " << ufo.position << '\n';
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
					ufoSound.stop();
					if (bgMusic.isPlaying())
						bgMusic.stop();
					gameState = STATE_GAMEOVER;
					if (!goMusic.isPlaying())
						goMusic.play();
				}
				else if (deathElapsed >= 3.0f && !bSparkSoundPlayed)
				{
					cout << "Spark sound play\n";
					drawStar = true;
					sparkSound.play();
					bSparkSoundPlayed = true;
				}
				if (drawStar && starGrow) {
					starScale += 0.125;
					if (starScale >= 1)
						starGrow = false;
				}
				else if (drawStar) {
					starScale -= 0.125;
					if (starScale <= 0) {
						drawStar = false;
						starGrow = true;
					}
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
					if (!ufoExplosionSound.isPlaying() && !bVictory)
						ufoExplosionSound.play();
					ufoExplosion.emit();
					ufoFireTrail.start();
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

			// AGL
			ufo.calculateAltitude(terrainOctree);


			// Ufo physics movement acceleration
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

			if (ufo.velocity.length() >= 0.01f)
			{
				if (!ufoSound.isPlaying())
					ufoSound.play();
				ufoMoveL.start();
				ufoMoveR.start();
			}
			else
			{
				ufoSound.stop();
				ufoMoveL.stop();
				ufoMoveR.stop();
			}

			bool moving = false;

			if (keysMap[' '] && ufo.hasFuel())																// up (space)
			{
				ufo.handleTakeoff();
				ufo.force += THRUST_UP_ACCEL * ufo.getHeadingY();
				float deltaTime = 1.0 / ofGetFrameRate();
				ufo.fuelLeftTime = max(static_cast<float>(0.0), ufo.fuelLeftTime - deltaTime);
				moving = true;
			}

			if (keysMap['w']) ufo.force += FORWARD_ACCEL * ufo.getHeadingZ();				// forward (w)
			if (keysMap['s']) ufo.force += -FORWARD_ACCEL * ufo.getHeadingZ();				// backward (d)
			if (keysMap['a']) ufo.force += -STRAFE_ACCEL * ufo.getHeadingX();				// left (a)
			if (keysMap['d']) ufo.force += STRAFE_ACCEL * ufo.getHeadingX();				// right (d)
			if (keysMap['e']) ufo.rotationForce -= YAW_TORQUE;								// yaw right (q)
			if (keysMap['q']) ufo.rotationForce += YAW_TORQUE;								// yaw left (e)

			if (moving) ufoMoveV.start();
			else ufoMoveV.stop();

			glm::vec3 right = glm::normalize(glm::cross(ufo.getHeadingZ(), glm::vec3(0,1,0)));
			glm::vec3 left = -right;
			ufoMoveL.position = ufo.position + left*8.8 + glm::vec3(0,1,0);
			ufoMoveR.position = ufo.position + right*8.8 + glm::vec3(0,1,0);

			ufoMoveL.update();
			ufoMoveR.update();
			ufoMoveL.integrateParticles();
			ufoMoveR.integrateParticles();
			
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
				if (nCowAbducted >= nCowRequired)
				{
					cout << "Victory!\n";
					ufoSound.stop();
					bVictory = true;
					if (bgMusic.isPlaying())
						bgMusic.stop();
					gameState = STATE_GAMEOVER;
					if (!goMusic.isPlaying())
						goMusic.play();
				}
				else
				{ 
					cout << "Meet the requirement first!\n";
				}
			}
			mothership.updateLight(ufo);

			// UFO vs Charging Stations

			// Station 1:
			if (station1.octree.intersect(ufo.boundingBox, station1.octree.root, station1, colBoxList, colNodeList))
			{
				station1.handleCollision(ufo);
			}
			station1.updateLight(ufo);

			// Station2:
			if (station2.octree.intersect(ufo.boundingBox, station2.octree.root, station2, colBoxList, colNodeList))
			{
				station2.handleCollision(ufo);
			}
			station2.updateLight(ufo);

			// Station3:
			if (station3.octree.intersect(ufo.boundingBox, station3.octree.root, station3, colBoxList, colNodeList))
			{
				station3.handleCollision(ufo);
			}
			station3.updateLight(ufo);

			// SpeedRing1:
			if (speedRing1.octree.intersect(ufo.boundingBox, speedRing1.octree.root, speedRing1, colSpeedBoxList, colSpeedNodeList))
			{
				cout << "Colliding with speedRing\n";
				speedRing1.handleCollision(ufo);
			}

			// SpeedRing2:
			if (speedRing2.octree.intersect(ufo.boundingBox, speedRing2.octree.root, speedRing2, colSpeedBoxList, colSpeedNodeList))
			{
				cout << "Colliding with speedRing2\n";
				speedRing2.handleCollision(ufo);
			}

			// SpeedRing3:
			if (speedRing3.octree.intersect(ufo.boundingBox, speedRing3.octree.root, speedRing3, colSpeedBoxList, colSpeedNodeList))
			{
				cout << "Colliding with speedRing3\n";
				speedRing3.handleCollision(ufo);
			}

			// SpeedRing4:
			if (speedRing4.octree.intersect(ufo.boundingBox, speedRing4.octree.root, speedRing4, colSpeedBoxList, colSpeedNodeList))
			{
				cout << "Colliding with speedRing4\n";
				speedRing4.handleCollision(ufo);
			}

			// SpeedRing5:
			if (speedRing5.octree.intersect(ufo.boundingBox, speedRing5.octree.root, speedRing5, colSpeedBoxList, colSpeedNodeList))
			{
				cout << "Colliding with speedRing5\n";
				speedRing5.handleCollision(ufo);
			}

			//speedRing1.handleSpeedBoost(ufo);

			// Handle UFO vs octrees collision based on number of collided boxes
			if (colBoxList.size() >= 1)
			{
				if (ufo.velocity.length() >= 5.0)
				{
					if (bFuelDeathPending)
					{
						bFuelDeathPending = false;
						if (alarmSound.isPlaying())
							alarmSound.stop();
					}

					ofVec3f contactNormal = getNormalAtContactPoint();
					if (!ufoExplosionSound.isPlaying() && !bVictory)
						ufoExplosionSound.play();
					ufoExplosion.emit();
					ufoFireTrail.start();
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

			for (auto& cowPtr : cows)
			{
				cowPtr->updateBoundingBox();
				vector<TreeNode> cowNodeList;		// Store all collided (leaf) nodes
				vector<Box> cowBoxList;				// Store all collided (leaf) nodes's boxes
				terrainOctree.intersect(cowPtr->boundingBox, terrainOctree.root, cowBoxList, cowNodeList);
				station1.octree.intersect(cowPtr->boundingBox, station1.octree.root, station1, cowBoxList, cowNodeList);
				station2.octree.intersect(cowPtr->boundingBox, station2.octree.root, station2, cowBoxList, cowNodeList);
				station3.octree.intersect(cowPtr->boundingBox, station3.octree.root, station3, cowBoxList, cowNodeList);

				// Handle cows interaction with cow platform
				if (cowPlatform.octree.intersect(cowPtr->boundingBox, cowPlatform.octree.root, cowPlatform, cowBoxList, cowNodeList) && cowPtr->bHasBoundingBox)
				{
					cowPtr->destroy();
					cowPtr->free();
					nCowAbducted += 1;
					if (!cowTeleportSound.isPlaying())
						cowTeleportSound.play();
				}

				// Handle cows landing and physics (integrate)
				if (cowBoxList.size() >= 1)
				{
					cowPtr->handleLanding();
				}
				else
				{
					cowPtr->integrate();
				}

				// Beam Collision with cow
				// for every cow [TBD] make sure to free all other cows after finding 1st cow
				if (beam.checkInside(cowPtr->boundingBox) && !cowCaptured) {
					if (!cowMooSound.isPlaying())
						cowMooSound.play();
					cowPtr->follow(&beam.capturePoint);
					cowCaptured = true;
				}
				else if (!beam.active) {
					cowCaptured = false;
					cowPtr->free();
				}
			}

			// Handle collision of cow 
			cow1.updateBoundingBox();
			vector<TreeNode> cowNodeList;		// Store all collided (leaf) nodes
			vector<Box> cowBoxList;				// Store all collided (leaf) nodes's boxes
			terrainOctree.intersect(cow1.boundingBox, terrainOctree.root, cowBoxList, cowNodeList);
			station1.octree.intersect(cow1.boundingBox, station1.octree.root, station1, cowBoxList, cowNodeList);
			station2.octree.intersect(cow1.boundingBox, station2.octree.root, station2, cowBoxList, cowNodeList);
			station3.octree.intersect(cow1.boundingBox, station3.octree.root, station3, cowBoxList, cowNodeList);

			
			// Cow collide with cow platform
			if (cowPlatform.octree.intersect(cow1.boundingBox, cowPlatform.octree.root, cowPlatform, cowBoxList, cowNodeList) && cow1.bHasBoundingBox)
			{
				cow1.destroy();
				cow1.free();
				nCowAbducted += 1;
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
				if (!cowMooSound.isPlaying())
					cowMooSound.play();
				cow1.follow(&beam.capturePoint);
				cowCaptured = true;
			}
			else if (!beam.active) {
				cowCaptured = false;
				cow1.free();
			}

			ufoMoveV.position = ufo.position;
			ufoMoveV.integrateParticles();
			ufoMoveV.update();

			ufoFireTrail.position = ufo.position;
			ufoFireTrail.integrateParticles();
			ufoFireTrail.update();
			
			ufoExplosion.position = ufo.position + glm::vec3(0, 8, 0);
			ufoExplosion.integrateParticles();
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
			station2.draw();
			station3.draw();
			mothership.draw();
			speedRing1.draw();
			speedRing2.draw();
			speedRing3.draw();
			speedRing4.draw();
			speedRing5.draw();

			ofSetColor(ofColor::white);
			shader.begin();
			
			ufoMoveV.draw();
			ufoExplosion.draw();
			ufoMoveL.draw();
			ufoMoveR.draw();
			ufoFireTrail.draw();
			
			shader.end();

			ofSetColor(ofColor::white);

			// Draw the 10 cows
			for (auto& cowPtr : cows)
			{
				if (cowPtr->bAlive)
				{
					cowPtr->draw();
				}
			}

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
				ofPushStyle();
				ofNoFill();
				ofSetColor(ofColor::white);
				terrainOctree.drawLeafNodes(terrainOctree.root);
				Octree::drawBox(ufo.boundingBox);
				Octree::drawBox(cow1.boundingBox);
				ofPopStyle();
			}

			ofPopMatrix();
			activeCam->end();
			ofDisableDepthTest();
			ofDisableLighting();

			// UI draw: don't include it in the 3D stuff (within camera)
			glDepthMask(false);
			
			if (gameState == STATE_GAMESTART)
			{
				if (!bHideHUD)
				{
					// Fuel bar
					ofPushStyle();

					fontUI.drawString("Altitude: " + ofToString(ufo.altitude, 2), 20, 70);
					ofPushStyle();
					ofSetColor(ufo.velocity.length() >= 5.0f ? ofColor::red : ofColor::white);
					fontUI.drawString("Velocity: " + ofToString(ufo.velocity.length(), 2), 20, 140);
					ofPopStyle();
					fontUI.drawString("Fuel time left: " + (ufo.hasFuel() ? ofToString(ufo.fuelLeftTime, 2) + " s" : "Out of Fuel!"), 20, 210);
					float fuelPercent = ufo.fuelLeftTime / ufo.fuelTotalTime;

					ofPushStyle();
					ofFill();
					ofSetColor(ofColor::darkSlateGray);
					ofDrawRectangle(18, 228, 304, 34);
					ofSetColor(ofColor::black);
					ofDrawRectangle(20, 230, 300, 30);
					ofSetColor(ofColor::aqua);
					ofDrawRectangle(20, 230, 300 * fuelPercent, 30);
					ofSetColor(ofColor::darkSlateGray);
					ofDrawTriangle(glm::vec3(18, 228, 0), glm::vec3(18, 250, 0), glm::vec3(40, 228, 0));
					ofPopStyle();

					fontUI.drawString("Cows abducted: " + std::to_string(nCowAbducted) + " / " + std::to_string(nCowRequired), 20, 300);

					ofPopStyle();
				}
				if (drawStar)
					star.draw((ofGetWindowWidth() / 2) - (star.getWidth() / 16.0) * starScale, ofGetWindowHeight() / 2 - (star.getHeight() / 16.0) * starScale, (star.getWidth() / 8.0) * starScale, (star.getHeight() / 8.0) * starScale);

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
			std::string strGameOver = "";
			if (bVictory)
			{
				victoryBackground.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
				strGameOver = "YOU WIN!";
			}
			else
			{
				defeatBackground.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
				strGameOver = "GAME OVER!";
			}

			std::string strRestart = "Press R to Restart";
			std::string strMainMenu = "Press M to Return to Main Menu";

			// lambda to center each menu items text
			auto centerX = [&](const std::string str)
				{
					return (ofGetWindowWidth() - fontUI.stringWidth(str)) * 0.5f;
				};

			auto centerY = ofGetWindowHeight() * 0.5f;

			if (bVictory) 
			{
				ofPushStyle();
				ofSetColor(ofColor::lightCyan);

				fontTitle.drawString(strGameOver, (ofGetWindowWidth() - fontTitle.stringWidth(strGameOver)) * 0.5f, ofGetWindowHeight() / 2.0 - 50);

				float space = ofGetWindowHeight() / 12;

				fontUI.drawString(strRestart, centerX(strRestart), centerY + space * 4.5);
				fontUI.drawString(strMainMenu, centerX(strMainMenu), centerY + space * 5.5);
				ofPopStyle();
			} 
			else
			{
				ofPushStyle();
				ofSetColor(ofColor::lightCyan);

				fontTitle.drawString(strGameOver, (ofGetWindowWidth() - fontTitle.stringWidth(strGameOver)) * 0.5f, ofGetWindowHeight() / 2.0);

				float space = ofGetWindowHeight() / 12;

				fontUI.drawString(strRestart, centerX(strRestart), centerY + space * 1);
				fontUI.drawString(strMainMenu, centerX(strMainMenu), centerY + space * 2);
				ofPopStyle();
			}
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
				menuHoverSound.play();
				if (currentMenuItem == MENU_START)
					currentMenuItem = MENU_QUIT;
				else
					currentMenuItem = static_cast<MenuItem>(currentMenuItem - 1);
			}
			else if (key == 's' || key == 'S')
			{
				menuHoverSound.play();
				if (currentMenuItem == MENU_QUIT)
					currentMenuItem = MENU_START;
				else
					currentMenuItem = static_cast<MenuItem>(currentMenuItem + 1);
			}
			else if (key == 'r' || key == 'R' || key == ' ')
			{
				menuSelectedSound.play();
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
					
					if (!ufoBeamSound.isPlaying() && !beam.active)
						ufoBeamSound.play();
					else
						ufoBeamSound.stop();
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
				case 'h':
				case 'H':
					bHideHUD = !bHideHUD;
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
	nCowAbducted = 4;


	for (auto i = 0; i < cows.size() && i < cowsPositions.size(); ++i)
	{
		cows[i]->bAlive = true;
		cows[i]->free();
		cows[i]->position = cowsPositions[i];
	}

	cow1.bAlive = true;
	cow1.free();
	cow1.position = ofVec3f(30, 250, 0);

	// Other stuff
	ufoFireTrail.stop();
	bVictory = false;

}

/**
 * Update the gameplay camera based on the current view mode
 */
void ofApp::updateGameCamera()
{
	switch (camView)
	{
	case CAM_THIRD: // 3rd person: camera sits 6 up and 12 behind the ufo, looks 3 ahead
		gameCam.setPosition(ufo.position + ufo.getHeadingY() * 40 - ufo.getHeadingZ() * 45);
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
