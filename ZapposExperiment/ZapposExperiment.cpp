//------------------------------------------------------------------------------
// <copyright file="SkeletonBasics.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <strsafe.h>
#include <vector>
#include "resource.h"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Surface.h"
#include "cinder/Capture.h"
#include "cinder/Text.h"
#include "cinder/Log.h"

#include "Skeleton.h"
#include "ParticleController.h"

using namespace ci;
using namespace ci::app;

class ZapposApp : public App {
	public:
		void	setup() override;
		void	resize() override;
		void	update() override;
		void	draw() override;
		void	cleanup() override;

		CSkeleton * skeleton;
		ParticleController * particles;
};

void ZapposApp::setup()
{
	int width = getWindowWidth();
	int height = getWindowHeight();
	try {
		skeleton = new CSkeleton(width, height);
		skeleton->CreateFirstConnected();
	}
	catch (CaptureExc &exc) {
		CI_LOG_EXCEPTION("failed to initialize the Capture: ", exc);
	}

	try {
		particles = new ParticleController(1000);
		particles->Setup();
	}
	catch (CaptureExc &exc) {
		CI_LOG_EXCEPTION("failed to initialize the Capture: ", exc);
	}

	gl::enableDepthRead();
	gl::enableDepthWrite();
}

void ZapposApp::cleanup()
{
	if (skeleton) {
		delete skeleton;
	}
	if (particles) {
		delete particles;
	}
}

void ZapposApp::resize()
{
	int width = getWindowWidth();
	int height = getWindowHeight();
	skeleton->updateWidthHeight(width, height);
}

void ZapposApp::update()
{
	// Detect people with kinect and update their skeleton position
	if (skeleton) {
		skeleton->Update();
	}
	// Update particle positions
	std::vector<_NUI_SKELETON_DATA *> skeletons = skeleton->getSkeletons();
	for (int i=0; i < skeletons.size(); i++) 
	{
		particles->Update(skeleton->getLHandPos(skeletons[i]), skeleton->getRHandPos(skeletons[i]));
	}
}

void ZapposApp::draw()
{
	skeleton->Draw();
	particles->Draw();
}

void prepareSettings(App::Settings *settings) {
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60.0f);
	settings->setTitle("Zappos Experiment");
}

CINDER_APP(ZapposApp, RendererGl(RendererGl::Options().msaa(4)), &prepareSettings)


