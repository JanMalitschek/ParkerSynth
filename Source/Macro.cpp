#include "Macro.h"
#include "NodeGraphProcessor.h"

Macro::Macro()
{
	setSize(400, 50);

	linkButton.setButtonText("L");
	laF_LinkButton.type = LaF_LinkButton::Type::Link;
	linkButton.setLookAndFeel(&laF_LinkButton);
	linkButton.addListener(this);
	addAndMakeVisible(linkButton);

	valueKnob.setColour(valueKnob.backgroundColourId, KNOB_GREY);
	valueKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	valueKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	valueKnob.addListener(this);
	valueKnob.setRotaryParameters(-2.79253f, 2.79253f, true);
	valueKnob.setRange(0.0f, 1.0f, 0.01f);
	valueKnob.setValue(0.0f);
	valueKnob.setLookAndFeel(&laF_Knob);
	valueKnob.setEnabled(false);
	addAndMakeVisible(valueKnob);

	this->moduleID = -1;
	this->parameterID = -1;

	boldFont = Font(Typeface::createSystemTypefaceFor(BinaryData::BAHNSCHRIFT_TTF, (size_t)BinaryData::BAHNSCHRIFT_TTFSize));
	moduleTitle = "[Empty]";
	macroTitle.setFont(boldFont);
	macroTitle.setText(moduleTitle);
	macroTitle.setLookAndFeel(&laF_TextEditor);
	addAndMakeVisible(macroTitle);

	unlinkButton.setButtonText("U");
	laF_unlinkButton.type = LaF_LinkButton::Type::Unlink;
	unlinkButton.setLookAndFeel(&laF_unlinkButton);
	unlinkButton.addListener(this);
	addAndMakeVisible(unlinkButton);

}

Macro::~Macro()
{
}

void Macro::paint(Graphics &g) {
	g.setColour(TOOLBAR_BG);
	g.fillRect(0, 0, 400, 100);
	/*g.setFont(boldFont);
	g.setColour(TOOLBAR_TEXT);
	g.drawFittedText(moduleTitle, 75, 0, 275, 50, Justification::centredLeft, 2);*/
}

void Macro::resized() {
	linkButton.setBounds(0, 12, 25, 25);
	valueKnob.setBounds(25, 0, 50, 50);
	macroTitle.setBounds(75, 2, 273, 46);
	unlinkButton.setBounds(350, 12, 25, 25);
}

void Macro::sliderValueChanged(Slider* slider) {
	if (slider == &valueKnob) {
		if (this->moduleID >= 0) {
			ngp->modules[moduleID]->SetParameter(parameterID, mappingOffset + valueKnob.getValue() * mappingFactor);
		}
	}
}

void Macro::sliderDragStarted(Slider* slider) {
	if (slider == &valueKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void Macro::sliderDragEnded(Slider* slider) {
	if (slider == &valueKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

void Macro::buttonClicked(Button* button) {
	if (button == &linkButton) {
		if (ngp->lastTweakedModule >= 0 && this->moduleID == -1) {
			this->moduleID = ngp->lastTweakedModule;
			this->macroTitle.setText(ngp->lastTweakedModuleTitle);
			this->parameterID = ngp->lastTweakedParameter;
			mappingFactor = ngp->lastTweakedParameterMax - ngp->lastTweakedParameterMin;
			mappingOffset = ngp->lastTweakedParameterMin;
			//this->valueKnob.setRange(ngp->lastTweakedParameterMin, ngp->lastTweakedParameterMax, ngp->lastTweakedParameterInc);
			this->valueKnob.setValue(ngp->lastTweakedParameterValue / mappingFactor - mappingOffset);
			valueKnob.setEnabled(true);
		}
	}
	else if (button == &unlinkButton) {
		this->moduleID = -1;
		this->macroTitle.setText("[Empty]");
		this->parameterID = -1;
		//this->valueKnob.setRange(0.0f, 1.0f, 0.01f); 
		this->valueKnob.setValue(0.0f);
		this->valueKnob.setRange(0.0f, 1.0f, 0.01f);
		mappingFactor = 1.0f;
		mappingOffset = 0.0f;
		valueKnob.setEnabled(false);
	}
}

void Macro::Reset() {
	this->moduleID = -1;
	this->macroTitle.setText("[Empty]");
	this->parameterID = -1;
	//this->valueKnob.setRange(0.0f, 1.0f, 0.01f); 
	this->valueKnob.setValue(0.0f);
	this->valueKnob.setRange(0.0f, 1.0f, 0.01f);
	mappingFactor = 1.0f;
	mappingOffset = 0.0f;
	valueKnob.setEnabled(false);
}
