#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

// official addons
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"
#include "ofxDirList.h"

#include "constants.h"

class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
		void loadSettings();
		
		void converUCharToINT();
		bool codeToMark(string *s,int code);

		unsigned char	rfidCode[RFID_CODE_BYTES_LENGTH];	// data from serial, RFID_CODE_BYTES_LENGTH bytes
		unsigned		cardCode;
		int				actualCode;
	
	private:
		ofxDirList		videoDIR;
		ofVideoPlayer	*videoBox;
		ofxCvColorImage	videoCvBox;
		ofImage			startImg;
		ofImage			backgroundImg;
	
		int				nVideos;
		int				currentVideo;
		float			mP,mD;
		
		float			videoX;
		float			videoY;
		float			videoW;
		float			videoH;
		bool			startActualVideo;
		bool			videoON;
		bool			isTheStart;
	
		ofSerial		serial;
		ofxXmlSettings	settings;
		
		string			portName;
		int				mainScreenW;
		int				mainScreenH;
		int				projectionScreenW;
		int				projectionScreenH;
		int				FPS;
	
		bool			codeReceived;
		bool			isPlaying;
	
};

#endif	

