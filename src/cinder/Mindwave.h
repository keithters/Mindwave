//
//  Mindwave.h
//
//  Created by Keith Butters on 6/7/13.
//
//


#include <CoreFoundation/CoreFoundation.h>

using namespace std;

class Mindwave {
  public:
	Mindwave( const char * port );
	
	const char * getPortName() const { return mPortname; };
	void setBaudRate( int rate ) { mBaudRate = rate; };
	
	bool connect();
	void update();
	
	void disconnect();
	void freeConnection();
	
	int getDriverVersion();
	int getConnectionID();
	
	float getSignalQuality() const { return mSignalQuality; };
	float getAttention() const { return mAttention; };
	float getMeditation() const { return mMeditation; };
	float getRaw() const { return mRaw; };
	float getDelta() const { return mDelta; }
	float getTheta() const { return mTheta; }
	float getAlpha1() const { return mAlpha1; };
	float getAlpha2() const { return mAlpha2; };
	float getBeta1() const { return mBeta1; };
	float getBeta2() const { return mBeta2; };
	float getGamma1() const { return mGamma1; };
	float getGamma2() const { return mGamma2; };
	int getBlinkStrength() const { return mBlinkStrength; };
	
	void enableBlinkDetection();
	void disableBlinkDetection();
	
	void enableLowPassFilter();
	void disableLowPassFilter();
	
	~Mindwave();
		
  private:
	const char * mPortname;			// Name of port to run on e.g. "/Dev/tty.MindWave"
	int mConnectionID	= -1;		// ThinkGear connection handle
	int mBaudRate		= 57600;	// set default baud rate
	
	
	float mSignalQuality;	// poor signal status
	float mAttention;		// eSense attention
	float mMeditation;		// eSense meditation
	float mRaw;
	float mDelta;
	float mTheta;
	float mAlpha1;
	float mAlpha2;
	float mBeta1;
	float mBeta2;
	float mGamma1;
	float mGamma2;
	int mBlinkStrength;

	// ThinkGear function pointers
	int (*TG_GetDriverVersion)()					= NULL;
	int (*TG_GetNewConnectionId)()					= NULL;
	int (*TG_Connect)(int, const char *, int, int)	= NULL;
	int (*TG_ReadPackets)(int, int)					= NULL;
	float (*TG_GetValue)(int, int)					= NULL;
	int (*TG_Disconnect)(int)						= NULL;
	void (*TG_FreeConnection)(int)					= NULL;
	int (*TG_EnableBlinkDetection)(int, int)		= NULL;
	int (*TG_EnableLowPassFilter)(int, int)			= NULL;
	
	CFBundleRef mThinkGearBundle;
};