#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Mindwave.h"

#if defined( CINDER_MAC )
	#define PORT "/Dev/tty.MindWave"
#else
	#define PORT "\\\\.\\COM5"
#endif

using namespace ci;
using namespace ci::app;
using namespace std;

class _TBOX_PREFIX_App : public AppNative {
  public:
	void setup();
	void update();
	void draw();
	
	Mindwave *mMindwave;
	
	int sig;
	int att;
	int med;
	
	int raw;
	int delta;
	int theta;
	int alpha1;
	int alpha2;
	int beta1;
	int beta2;
	int gamma1;
	int gamma2;
	
	int blink;
};

void _TBOX_PREFIX_App::setup()
{
	mMindwave = new Mindwave( PORT );
	console() << "ThinkGear driver version: " << mMindwave->getDriverVersion() << endl;
	
	mMindwave->enableBlinkDetection();
	
	if (! mMindwave->connect() ) {
		console() << "Unable to connect to the ThinkGear device." << endl;
		exit(1);
	}
}

void _TBOX_PREFIX_App::update()
{
	mMindwave->update();
	
	sig		= mMindwave->getSignalQuality();
	att		= mMindwave->getAttention();
	med		= mMindwave->getMeditation();
	raw		= mMindwave->getRaw();
	delta	= mMindwave->getDelta();
	theta	= mMindwave->getTheta();
	alpha1	= mMindwave->getAlpha1();
	alpha2	= mMindwave->getAlpha2();
	beta1	= mMindwave->getBeta1();
	beta2	= mMindwave->getBeta2();
	gamma1	= mMindwave->getGamma1();
	gamma2	= mMindwave->getGamma2();
	
	blink = mMindwave->getBlinkStrength();
	
	console() << "Signal: " << sig << "  Att: " << att << "  Med: " << med << "  Blink: " << blink << endl;
}

void _TBOX_PREFIX_App::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	
	int num_bars = 12;
	int curr_bar = 0;
	float bar_height = getWindowHeight() / num_bars;
	
	// draw a bar chart using mindwave values
	gl::color( Color( 1.0f, 0.0f, 0.0f ) );
	Rectf sigRect( 0, curr_bar * bar_height, sig * 3, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( sigRect );

	curr_bar++;

	Rectf blinkRect( 0, curr_bar * bar_height, blink * 3, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( blinkRect );
	
	curr_bar++;
	
	gl::color( Color( 0.8f, 0.8f, 1.0f ) );
	Rectf attRect( 0, curr_bar * bar_height, att * 3, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( attRect );
	
	curr_bar++;
	
	gl::color( Color( 0.7f, 0.7f, 1.0f ) );
	Rectf medRect( 0, curr_bar * bar_height, med * 3, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( medRect );
	
	curr_bar++;
	
	gl::color( Color( 0.6f, 0.6f, 1.0f ) );
	Rectf rawRect( 0, curr_bar * bar_height, raw * 3, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( rawRect );
	
	curr_bar++;
	
	gl::color( Color( 0.5f, 0.5f, 1.0f ) );
	Rectf deltaRect( 0, curr_bar * bar_height, delta/4, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( deltaRect );
	
	curr_bar++;
	
	gl::color( Color( 0.4f, 0.4f, 1.0f ) );
	Rectf alpha1Rect( 0, curr_bar * bar_height, alpha1/4, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( alpha1Rect );
	
	curr_bar++;
	
	gl::color( Color( 0.3f, 0.3f, 1.0f ) );
	Rectf alpha2Rect( 0, curr_bar * bar_height, alpha2/4, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( alpha2Rect );
	
	curr_bar++;
	
	gl::color( Color( 0.2f, 0.2f, 1.0f ) );
	Rectf beta1Rect( 0, curr_bar * bar_height, beta1/4, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( beta1Rect );
	
	curr_bar++;

	gl::color( Color( 0.15f, 0.15f, 1.0f ) );
	Rectf beta2Rect( 0, curr_bar * bar_height, beta2/4, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( beta2Rect );
	
	curr_bar++;
	
	gl::color( Color( 0.1f, 0.1f, 1.0f ) );
	Rectf gamma1Rect( 0, curr_bar * bar_height, gamma1/4, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( gamma1Rect );
	
	curr_bar++;
	
	gl::color( Color( 0.05f, 0.05f, 1.0f ) );
	Rectf gamma2Rect( 0, curr_bar * bar_height, gamma2/4, curr_bar * bar_height + bar_height );
	gl::drawSolidRect( gamma2Rect );

}


CINDER_APP_NATIVE( _TBOX_PREFIX_App, RendererGl )
