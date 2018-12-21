#include "../JuceLibraryCode/JuceHeader.h"
#include "Colors.hpp"

class LaF_LinkButton : public LookAndFeel_V4 {

public:
	enum Type {
		Link = 0,
		Unlink
	};
	Type type;
private:
	void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override {
		if (isMouseOverButton) {
			button.setMouseCursor(MouseCursor::PointingHandCursor);
			g.setColour(MODULE_RAILING.brighter(0.2f));
			if (isButtonDown) {
				button.setMouseCursor(MouseCursor::DraggingHandCursor);
				g.setColour(MODULE_RAILING.brighter(4.0f));
			}
		}
		else
		{
			button.setMouseCursor(MouseCursor::NoCursor);
			g.setColour(MODULE_RAILING);
		}
		Rectangle<int> bounds = button.getLocalBounds();
		Rectangle<float> fBounds = Rectangle<float>(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
		fBounds.reduce(2, 2);
		g.drawRoundedRectangle(fBounds, 3.0f, 4.0f);
		
	}

	void drawButtonText(Graphics &g, TextButton &button, bool isMouseOverButton, bool isButtonDown) {
		g.setColour(MAIN_COLOR);
		g.setFont(textFont);
		g.drawFittedText(button.getButtonText(), button.getLocalBounds(), Justification::centred, 2);
	}

private:
	Font textFont = Font(Typeface::createSystemTypefaceFor(BinaryData::BAHNSCHRIFT_TTF, (size_t)BinaryData::BAHNSCHRIFT_TTFSize));
};

