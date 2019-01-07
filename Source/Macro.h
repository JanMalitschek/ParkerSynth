#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "LookAndFeel\Colors.hpp"
#include "LookAndFeel\LaF_New.hpp"
#include "LookAndFeel\MacroLinkButton.hpp"

class NodeGraphProcessor;
class Macro : public Component, public Slider::Listener, public Button::Listener
{
public:
	Macro();
	~Macro();
	void paint(Graphics &g) override;
	void resized() override;

	void sliderValueChanged(Slider* slider) override;
	void sliderDragStarted(Slider* slider) override;
	void sliderDragEnded(Slider* slider) override;

	void buttonClicked(Button* button) override;

	NodeGraphProcessor* ngp;

	AudioProcessorValueTreeState::SliderAttachment* attachment;
	Slider valueKnob;

	int moduleID;
	int parameterID;
	float mappingFactor;
	float mappingOffset;

	TextEditor macroTitle;

	void Reset();

private:
	
	LaF_RotarySlider laF_Knob;
	String moduleTitle;

	LaF_TextEditor laF_TextEditor;

	TextButton linkButton;
	LaF_LinkButton laF_LinkButton;
	TextButton unlinkButton;
	LaF_LinkButton laF_unlinkButton;

	Font boldFont;
};

