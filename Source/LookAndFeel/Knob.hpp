#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Colors.hpp"

class LaF_Knob : public LookAndFeel_V4 {
	//void drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override {
	//	float diameter = jmin(width, height) * 0.8f;
	//	float radius = diameter / 2;
	//	float px = x + (width / 2) - radius;
	//	float py = y + (height / 2) - radius;

	//	//draw the value ellipse
	//	Path valueArc;
	//	float angleDifference = rotaryEndAngle - rotaryStartAngle;
	//	valueArc.addCentredArc(width / 2, height / 2, radius - diameter * 0.05f, radius - diameter * 0.05f, 0.0f, rotaryStartAngle, rotaryStartAngle + angleDifference * sliderPos, true);
	//	Path railArc;
	//	railArc.addCentredArc(width / 2, height / 2, radius - diameter * 0.05f, radius - diameter * 0.05f, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
	//	PathStrokeType valueStroke = PathStrokeType(diameter * 0.1);

	//	g.setColour(juce::Colour(52, 52, 52));
	//	g.strokePath(railArc, valueStroke);

	//	Colour knobColor = slider.findColour(slider.backgroundColourId);

	//	g.setColour(knobColor);
	//	g.strokePath(valueArc, valueStroke);

	//	if (knobColor == KNOB_PINK)
	//	{
	//		g.drawImage(knob_pink, 0, 0, width, height, 0, 0, 50, 50);
	//	}
	//	else if (knobColor == KNOB_BLUE) {
	//		g.drawImage(knob_blue, 0, 0, width, height, 0, 0, 50, 50);
	//	}
	//	else if (knobColor == KNOB_GREY) {
	//		g.drawImage(knob_grey, 0, 0, width, height, 0, 0, 50, 50);
	//	}
	//	else {
	//		g.drawImage(knob_pink, 0, 0, width, height, 0, 0, 50, 50);
	//	}

	//	//draw handle
	//	float diameter5 = diameter * 0.1f;
	//	float px5 = px + diameter * 0.45f;
	//	float py5 = py + diameter * 0.45f;

	//	Path handleTick;
	//	handleTick.addEllipse(-diameter5 * 0.5f, -radius * 0.4f, diameter5, diameter5);
	//	g.setColour(juce::Colour(52, 52, 52));
	//	g.fillPath(handleTick, AffineTransform::rotation(rotaryStartAngle + angleDifference * sliderPos).translated(width / 2, height / 2));
	//}

	void fillTextEditorBackground(Graphics &g, int width, int height, TextEditor &t) override {
		g.setColour(MODULE_FONT_COLOR);
		g.fillRect(0, 0, width, height);
	}

	void drawTextEditorOutline(Graphics &g, int width, int height, TextEditor &t) override {
		g.setColour(MODULE_FONT_COLOR);
		g.drawRect(0, 0, width, height);
	}

private:

};