/* 
Copyright (c) 2013, Keith Butters
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Mindwave.h"
#include "cinder/app/App.h"

using namespace ci;
using namespace ci::app;

// Max number of connections
#define 	TG_MAX_CONNECTION_HANDLES   128
#define 	TG_STREAM_PACKETS			0

// Data format for use with TG_Connect() and TG_SetDataFormat().
#define 	TG_STREAM_5VRAW				1
#define 	TG_STREAM_FILE_PACKETS		2
#define 	TG_DATA_BATTERY				0

// Data types that can be requested from TG_GetValue().
#define 	TG_DATA_POOR_SIGNAL			1
#define 	TG_DATA_ATTENTION			2
#define 	TG_DATA_MEDITATION			3
#define 	TG_DATA_RAW					4
#define 	TG_DATA_DELTA				5
#define 	TG_DATA_THETA				6
#define 	TG_DATA_ALPHA1				7
#define 	TG_DATA_ALPHA2				8
#define 	TG_DATA_BETA1				9
#define 	TG_DATA_BETA2				10
#define 	TG_DATA_GAMMA1				11
#define 	TG_DATA_GAMMA2				12
#define 	TG_DATA_BLINK_STRENGTH		37


std::unique_ptr<Mindwave> Mindwave::create( const char * port )
{
	return std::unique_ptr<Mindwave>( new Mindwave( port ) );
}


Mindwave::Mindwave( const char * port )
{
	mConnectionID	= -1;		// ThinkGear connection handle
	mBaudRate		= 57600;	// set default baud rate

	mPortname = port;
	
	#if defined( CINDER_MAC )
		// load and initialize the bundle
		CFBundleRef mainBundle	= CFBundleGetMainBundle();
		CFURLRef mainURL		= CFBundleCopyExecutableURL( mainBundle );
		CFURLRef bundleURL		= CFURLCreateCopyAppendingPathComponent( kCFAllocatorDefault, mainURL, CFSTR( "../ThinkGear.bundle" ), false );
		
		mThinkGearBundle = CFBundleCreate( kCFAllocatorDefault, bundleURL);
		
		CFRelease( mainURL );
		CFRelease( bundleURL );
		
		if( !mThinkGearBundle ) {
			console() << "Error: Could not load the ThinkGear.bundle. Does it exist in: " << cinder::cocoa::convertCfString( CFURLGetString( bundleURL ) ) << " ?" << std::endl;
			exit(1);
		}
		
		// set up function pointers 
		TG_GetDriverVersion		= (int(*)())CFBundleGetFunctionPointerForName(mThinkGearBundle, CFSTR("TG_GetDriverVersion"));
		TG_GetNewConnectionId	= (int(*)())CFBundleGetFunctionPointerForName(mThinkGearBundle,CFSTR("TG_GetNewConnectionId"));
		TG_Connect				= (int(*)(int, const char *, int, int))CFBundleGetFunctionPointerForName(mThinkGearBundle,CFSTR("TG_Connect"));
		TG_ReadPackets			= (int(*)(int, int))CFBundleGetFunctionPointerForName(mThinkGearBundle,CFSTR("TG_ReadPackets"));
		TG_GetValue				= (float(*)(int, int))CFBundleGetFunctionPointerForName(mThinkGearBundle,CFSTR("TG_GetValue"));
		TG_Disconnect			= (int(*)(int))CFBundleGetFunctionPointerForName(mThinkGearBundle,CFSTR("TG_Disconnect"));
		TG_FreeConnection		= (void(*)(int))CFBundleGetFunctionPointerForName(mThinkGearBundle,CFSTR("TG_FreeConnection"));
		TG_EnableBlinkDetection	= (int(*)(int, int))CFBundleGetFunctionPointerForName(mThinkGearBundle,CFSTR("TG_EnableBlinkDetection"));
		TG_EnableLowPassFilter	= (int(*)(int, int))CFBundleGetFunctionPointerForName(mThinkGearBundle,CFSTR("TG_EnableLowPassFilter"));
		
		if( !TG_GetDriverVersion ||
			!TG_GetNewConnectionId ||
			!TG_Connect ||
			!TG_ReadPackets ||
			!TG_GetValue ||
			!TG_Disconnect ||
			!TG_FreeConnection ||
			!TG_EnableBlinkDetection ||
			!TG_EnableLowPassFilter )
		{
			console() << "Error: Expected functions in ThinkGear.bundle were not found.";
			exit(1);
		}
	#endif
	
	// get a new connection ID
	mConnectionID = TG_GetNewConnectionId();
}

bool Mindwave::connect()
{
	if( !TG_Connect( mConnectionID, mPortname, mBaudRate, TG_STREAM_PACKETS) )
		return true;
		
	return false;
}

void Mindwave::update()
{
	// read the packets from the output stream
	int numPackets = TG_ReadPackets( mConnectionID, -1 );
			
	if ( numPackets > 0 ) {
		mSignalQuality	= TG_GetValue( mConnectionID, TG_DATA_POOR_SIGNAL );
		mAttention		= TG_GetValue( mConnectionID, TG_DATA_ATTENTION );
		mMeditation		= TG_GetValue( mConnectionID, TG_DATA_MEDITATION );
		mRaw			= TG_GetValue( mConnectionID, TG_DATA_RAW );
		mDelta			= TG_GetValue( mConnectionID, TG_DATA_DELTA );
		mTheta			= TG_GetValue( mConnectionID, TG_DATA_THETA );
		mAlpha1			= TG_GetValue( mConnectionID, TG_DATA_ALPHA1 );
		mAlpha2			= TG_GetValue( mConnectionID, TG_DATA_ALPHA2 );
		mBeta1			= TG_GetValue( mConnectionID, TG_DATA_BETA1 );
		mBeta2			= TG_GetValue( mConnectionID, TG_DATA_BETA2 );
		mGamma1			= TG_GetValue( mConnectionID, TG_DATA_GAMMA1 );
		mGamma2			= TG_GetValue( mConnectionID, TG_DATA_GAMMA2 );
		mBlinkStrength	= TG_GetValue( mConnectionID, TG_DATA_BLINK_STRENGTH );
	}
}

void Mindwave::enableBlinkDetection()
{
	TG_EnableBlinkDetection( mConnectionID, 1 );
}

void Mindwave::disableBlinkDetection()
{
	TG_EnableBlinkDetection( mConnectionID, 0 );
}

void Mindwave::enableLowPassFilter()
{
	TG_EnableLowPassFilter( mConnectionID, 512 );
}

void Mindwave::disableLowPassFilter()
{
	TG_EnableLowPassFilter( mConnectionID, 0 );
}

void Mindwave::disconnect()
{
	TG_Disconnect( mConnectionID );
}

void Mindwave::freeConnection()
{
	TG_FreeConnection( mConnectionID );
}


int Mindwave::getDriverVersion()
{
	return TG_GetDriverVersion();
}

Mindwave::~Mindwave()
{
	if( mConnectionID != -1 ){
		TG_FreeConnection( mConnectionID );
	}
	
	#if defined( CINDER_MAC )
		if( mThinkGearBundle )
			CFRelease( mThinkGearBundle );
	#endif
}
