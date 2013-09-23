//
//  Mindwave.h
//
//  Created by Keith Butters on 6/7/13.
//
//
#pragma once

#include "cinder/Cinder.h"

#if defined( CINDER_MAC )
	#include <CoreFoundation/CoreFoundation.h>
#elif defined ( CINDER_MSW )
	#include "thinkgear.h"
#endif


typedef std::shared_ptr<class Mindwave> MindwaveRef;

class Mindwave {
  public:
	
	static std::unique_ptr<Mindwave> create( const char * port );
	
	const char * getPortName() const { return mPortname; };
	void setBaudRate( int rate ) { mBaudRate = rate; };
	
	bool connect();
	void update();
	
	void disconnect();
	void freeConnection();
	
	int getDriverVersion();
	int getConnectionID() const { return mConnectionID; };
	
	int getSignalQuality() const { return mSignalQuality; };
	int getAttention() const { return mAttention; };
	int getMeditation() const { return mMeditation; };
	int getRaw() const { return mRaw; };
	int getDelta() const { return mDelta; }
	int getTheta() const { return mTheta; }
	int getAlpha1() const { return mAlpha1; };
	int getAlpha2() const { return mAlpha2; };
	int getBeta1() const { return mBeta1; };
	int getBeta2() const { return mBeta2; };
	int getGamma1() const { return mGamma1; };
	int getGamma2() const { return mGamma2; };
	int getBlinkStrength() const { return mBlinkStrength; };
	
	void enableBlinkDetection();
	void disableBlinkDetection();
	
	void enableLowPassFilter();
	void disableLowPassFilter();
	
	~Mindwave();
		
  private:
  
  	Mindwave( const char * port );
	
	#if defined( CINDER_MAC )
		CFBundleRef mThinkGearBundle;
	#endif
  
	const char * mPortname;			// Name of port to run on e.g. "/Dev/tty.MindWave"
	int mConnectionID;		// ThinkGear connection handle
	int mBaudRate;	// set default baud rate
	
	
	int mSignalQuality;	// poor signal status
	int mAttention;		// eSense attention
	int mMeditation;		// eSense meditation
	int mRaw;
	int mDelta;
	int mTheta;
	int mAlpha1;
	int mAlpha2;
	int mBeta1;
	int mBeta2;
	int mGamma1;
	int mGamma2;
	int mBlinkStrength;

	#if defined( CINDER_MAC )
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
	#endif
};