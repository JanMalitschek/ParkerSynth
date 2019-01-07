#include "Splashscreen.h"

CustomSplashscreen::CustomSplashscreen() : Component()
{
	//splashImage = ImageFileFormat::loadFrom(BinaryData::ParkerSynth_Splashscreen_png, (size_t)BinaryData::ParkerSynth_Splashscreen_pngSize);
	textFont = Font(Typeface::createSystemTypefaceFor(BinaryData::BAHNSCHRIFT_TTF, (size_t)BinaryData::BAHNSCHRIFT_TTFSize));
	setSize(400, 300);
}

CustomSplashscreen::~CustomSplashscreen()
{
}

void CustomSplashscreen::paint(Graphics &g) {
	//g.drawImage(splashImage, 0, 0, 400, 300, 0, 0, 400, 300);
	g.setColour(MODULE_FONT_COLOR);
	g.setFont(textFont);
	g.setFont(16.0f);
	g.drawText("Version " + juce::String(ProjectInfo::versionString), 12, 225, 200, 25, Justification::centredLeft);
}

void CustomSplashscreen::resized() {

}