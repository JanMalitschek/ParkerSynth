#ifndef LAF_NEW
#define LAF_NEW

#include "../JuceLibraryCode/JuceHeader.h"
#include "Colors.hpp"
#include <string>

class LaF_IOSocket : public LookAndFeel_V4 {

	void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override {
		g.setColour(backgroundColour);
		Path p;
		p.startNewSubPath(9, 6);
		p.lineTo(15, 12);
		p.lineTo(9, 18);
		if (isMouseOverButton) {
			button.setMouseCursor(MouseCursor::PointingHandCursor);
			g.strokePath(p, PathStrokeType(6.0f, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded));
			if (isButtonDown) {
				button.setMouseCursor(MouseCursor::DraggingHandCursor);
			}
		}
		else
		{
			button.setMouseCursor(MouseCursor::NoCursor);
			g.strokePath(p, PathStrokeType(4.0f, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded));
		}
	}

	void drawButtonText(Graphics &g, TextButton &button, bool isMouseOverButton, bool isButtonDown) {

	}

};

class LaF_ControlSocket : public LookAndFeel_V4 {

	void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override {
		g.setColour(backgroundColour);
		Path p;
		p.startNewSubPath(6, 16);
		p.lineTo(12, 10);
		p.lineTo(18, 16);
		if (isMouseOverButton) {
			button.setMouseCursor(MouseCursor::PointingHandCursor);
			g.strokePath(p, PathStrokeType(6.0f, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded));
			if (isButtonDown) {
				button.setMouseCursor(MouseCursor::DraggingHandCursor);
			}
		}
		else
		{
			button.setMouseCursor(MouseCursor::NoCursor);
			g.strokePath(p, PathStrokeType(4.0f, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded));
		}
	}

	void drawButtonText(Graphics &g, TextButton &button, bool isMouseOverButton, bool isButtonDown) {

	}

};

class LaF_RotarySlider : public LookAndFeel_V4 {
	void drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override {
		float diameter = jmin(width, height) * 0.8f;
		float radius = diameter / 2;
		float px = x + (width / 2) - radius;
		float py = y + (height / 2) - radius;

		//draw the value ellipse
		Path valueArc;
		float angleDifference = rotaryEndAngle - rotaryStartAngle;
		valueArc.addCentredArc(width / 2, height / 2, radius - diameter * 0.05f, radius - diameter * 0.05f, 0.0f, rotaryStartAngle, rotaryStartAngle + angleDifference * sliderPos, true);
		Path subRailArc;
		subRailArc.addCentredArc(width / 2, height / 2, radius - diameter * 0.05f, radius - diameter * 0.05f, 0.0f, -3.1415, 3.1415, true);
		Path railArc;
		railArc.addCentredArc(width / 2, height / 2, radius - diameter * 0.05f, radius - diameter * 0.05f, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
		PathStrokeType valueStroke = PathStrokeType(diameter * 0.1, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded);

		g.setColour(SLIDER_SUBRAILING);
		g.strokePath(subRailArc, valueStroke);

		g.setColour(MODULE_RAILING);
		g.strokePath(railArc, valueStroke);

		g.setColour(MAIN_COLOR);
		g.strokePath(valueArc, valueStroke);

		Path handleTick;
		handleTick.startNewSubPath(0, 0);
		handleTick.lineTo(0, -(radius - diameter * 0.05f));
		g.setColour(SLIDER_THUMB);
		g.strokePath(handleTick, valueStroke, AffineTransform::rotation(rotaryStartAngle + angleDifference * sliderPos).translated(width / 2, height / 2));
	}
};

class LaF_VerticalSlider : public LookAndFeel_V4 {
	void drawLinearSlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider &s) override {
		Point<float> startPos;
		Point<float> endPos;
		Point<float> thumbPos;
		float lerpAmount = s.getValue() / (s.getMaximum() - s.getMinimum()) - s.getMinimum() / (s.getMaximum() - s.getMinimum());
		PathStrokeType valueStroke = PathStrokeType(6.0f, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded);
		Path rail;
		Path value;
		switch (style) {
		case Slider::SliderStyle::LinearVertical:
			startPos = Point<float>(width / 2, s.getHeight() - 5);
			endPos = Point<float>(width / 2, 5);
			break;
		case Slider::SliderStyle::LinearHorizontal:
			startPos = Point<float>(5, s.getHeight() / 2);
			endPos = Point<float>(s.getWidth() - 5, s.getHeight() / 2);
			break;
		default:
			break;
		}
		thumbPos = startPos + (endPos - startPos) * lerpAmount;
		rail.startNewSubPath(startPos);
		rail.lineTo(endPos);
		value.startNewSubPath(startPos);
		value.lineTo(thumbPos);
		g.setColour(MODULE_RAILING);
		g.strokePath(rail, valueStroke);
		g.setColour(MAIN_COLOR);
		g.strokePath(value, valueStroke);
		g.setColour(SLIDER_THUMB);
		g.fillEllipse(thumbPos.x - 5, thumbPos.y - 5, 10, 10);
	}
};

class LaF_TextEditor : public LookAndFeel_V4 {
	void fillTextEditorBackground(Graphics &g, int width, int height, TextEditor &t) override {
		g.setColour(MODULE_FONT_COLOR);
		g.fillRoundedRectangle(0, 0, width, height, 5);
	}

	void drawTextEditorOutline(Graphics &g, int width, int height, TextEditor &t) override {

	}
};

#endif