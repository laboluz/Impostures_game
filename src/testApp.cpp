#include "testApp.h"
#include "rfidCodes.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	//////////////////////////////////////////////
	// load settings from xml
	loadSettings();
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// init general openframeworks settings
	ofSeedRandom();
	ofSetFrameRate(25);
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofSetLogLevel(OF_LOG_VERBOSE);
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// init video box container
	nVideos			= videoDIR.listDir("videos");
	currentVideo	= 0;
	videoCvBox.allocate(VIDEOS_WIDTH, VIDEOS_HEIGHT);
	videoBox = new ofVideoPlayer();
	videoBox->loadMovie("videos/entrada00.mov");
	videoBox->stop();
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// init background images
	startImg.loadImage(START_IMAGE);
	backgroundImg.loadImage(RUNTIME_IMAGE);
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// init settings vars
	actualCode		 = 0;
	codeReceived	 = false;
	isPlaying		 = false;
	startActualVideo = false;
	videoON			 = false;
	isTheStart		 = true;
	
	videoW = (VIDEOS_WIDTH*mainScreenH)/VIDEOS_HEIGHT;
	videoH = mainScreenH;
	
	videoX = (mainScreenW-videoW)/2.0;
	videoY = 0.0f;
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// init serial port
	serial.enumerateDevices();
	serial.setup(portName,19200); // mac osx
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// set log level to error only
	ofSetLogLevel(OF_LOG_ERROR);
	//////////////////////////////////////////////

}

//--------------------------------------------------------------
void testApp::update(){
	
	///////////////////////////////////////////////////////////
	//// Load videos form hard drive into an ofxCvColorImage sruct
	if(videoON){
		actualCode = 0;
		videoBox->idleMovie();
		mP = videoBox->getPosition()*videoBox->getDuration();
		mD = videoBox->getDuration();
		if(mP < mD - 0.5){
			isPlaying = true;
		}else{
			videoBox->stop();
			isPlaying = false;
			videoON = false;
		}
		videoCvBox.setFromPixels(videoBox->getPixels(), VIDEOS_WIDTH,VIDEOS_HEIGHT);
	}
	///////////////////////////////////////////////////////////
	
	if(startActualVideo){
		videoBox->play();
		isPlaying = true;
		videoON = true;
		startActualVideo = false;
	}
	
	converUCharToINT(); // convert byte card code to int
	
	// trigger the video if tha card code exist in database
	if(codeReceived && !videoON && !isTheStart){
		
		string tempVideo;
		char temp[256];
		int	i = 0;
		float rr = ofRandomuf();
		
		if(rr < 0.33f){
			i = 1;
		}else if(rr >= 0.33f && rr < 0.66f){
			i = 2;
		}else{
			i = 3;
		}
		
		bool bLoadVideo = codeToMark(&tempVideo,actualCode);
		if(bLoadVideo){
			printf("Video File exist: %s\n",tempVideo.c_str());
			if(!isPlaying){
				videoBox->stop();
				videoBox->close();
				delete videoBox;
				videoBox = new ofVideoPlayer();
				sprintf(temp,"videos/%s_%i.mov",tempVideo.c_str(),i);
				videoBox->loadMovie(temp);
				if(videoBox->bLoaded){
					printf("Loaded Video %s\n",temp);
					startActualVideo = true;
				}
			}
		}else{
			printf("Video file don't exist\n");
		}
		
		codeReceived = false;
	}
		
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofBackground(0,0,0);
	
	if(videoON){
		ofSetColor(255, 255, 255);
		videoCvBox.draw(videoX,videoY,videoW,videoH);
	}else{
		ofSetColor(0, 0, 0);
		ofFill();
		ofRect(0,0, mainScreenW, mainScreenW);
		ofSetColor(255, 255, 255);
		if(isTheStart){
			startImg.draw(videoX,videoY,videoW,videoH);
		}else{
			backgroundImg.draw(videoX,videoY,videoW,videoH);
		}
	}

}


//--------------------------------------------------------------
void testApp::keyReleased(int key){
	switch (key) {
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 'p':
			startActualVideo = true;
			isTheStart		 = false;
			break;
			
		case 'b':
			videoBox->stop();
			isPlaying = false;
			videoON = false;
			startActualVideo = false;
			break;
			
		default:
			break;
	}
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::converUCharToINT(){
	
	if(serial.readBytes(rfidCode, RFID_CODE_BYTES_LENGTH) > RFID_CODE_BYTES_LENGTH-1){
		
		codeReceived = true;
		// left shifts & ORs
		cardCode = rfidCode[9] | (rfidCode[8] | (rfidCode[7] | (rfidCode[6] | (rfidCode[5] | (rfidCode[4] | (rfidCode[3] | (rfidCode[2] | (rfidCode[1] | (rfidCode[0] << CHAR_BIT) << CHAR_BIT) << CHAR_BIT) << CHAR_BIT) << CHAR_BIT) << CHAR_BIT) << CHAR_BIT) << CHAR_BIT) << CHAR_BIT) << CHAR_BIT;
	
		actualCode = (int)cardCode;
		//printf("%i\n",actualCode);
		
	}
	
}

//--------------------------------------------------------------
void testApp::loadSettings(){
	
	settings.loadFile(IG_XML_SETTINGS);
	
	//////////////////////////////////////////////
	// get SCREENS settings
	mainScreenW				= settings.getValue("ms_width",0,0);
	mainScreenH				= settings.getValue("ms_height",0,0);
	projectionScreenW		= settings.getValue("ps_width",0,0);
	projectionScreenH		= settings.getValue("ps_height",0,0);
	
	FPS						= settings.getValue("fps",0,0);
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	// get SCREENS settings
	portName				= settings.getValue("port_name"," ",0);
	//////////////////////////////////////////////
	
}
