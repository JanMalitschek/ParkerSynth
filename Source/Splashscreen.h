#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LookAndFeel\Colors.hpp"

class CustomSplashscreen : public Component
{
public:
	CustomSplashscreen();
	~CustomSplashscreen();

	void paint(Graphics &g) override;
	void resized() override;
private:
	Image splashImage;
	Font textFont;
};