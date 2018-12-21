#ifndef LAF_TOOLBAR_BUTTON
#define LAF_TOOLBAR_BUTTON

#include "../JuceLibraryCode/JuceHeader.h"
#include "Colors.hpp"

class LaF_ToolbarButton : public LookAndFeel_V4 {

	void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override {
		if (isMouseOverButton) {
			button.setMouseCursor(MouseCursor::PointingHandCursor);
			g.setColour(TOOLBAR_HIGHLIGHT);
			if (isButtonDown) {
				button.setMouseCursor(MouseCursor::DraggingHandCursor);
				g.setColour(TOOLBAR_CLICKED);
			}
		}
		else
		{
			button.setMouseCursor(MouseCursor::NoCursor);
			g.setColour(TOOLBAR_BG);
		}
		g.fillRect(button.getLocalBounds());
	}

	void drawButtonText(Graphics &g, TextButton &button, bool isMouseOverButton, bool isButtonDown) {
		g.setColour(TOOLBAR_TEXT);
		g.setFont(textFont);
		g.drawFittedText(button.getButtonText(), button.getLocalBounds(), Justification::centred, 2);
	}

	void drawTooltip(Graphics &g, const String &text, int width, int height) {
		g.setColour(TOOLBAR_BG);
		g.fillRect(0, 0, width, height);
		g.setColour(TOOLBAR_TEXT);
		g.setFont(textFont);
		g.drawFittedText(text, 0, 0, width, height, Justification::centred, 3);
	}

private:
	Font textFont = Font(Typeface::createSystemTypefaceFor(BinaryData::BAHNSCHRIFT_TTF, (size_t)BinaryData::BAHNSCHRIFT_TTFSize));
};

#endif