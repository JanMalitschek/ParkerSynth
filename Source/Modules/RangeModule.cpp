#include "RangeModule.h"
#include "../NodeGraphProcessor.h"

RangeModule::RangeModule() : Module(ModuleColorScheme::Grey, "Range", 0, 1, 1, Point<int>(4, 6), 3)
{
	inputFieldA.setMultiLine(false);
	inputFieldA.setCaretVisible(true);
	inputFieldA.setScrollbarsShown(false);
	inputFieldA.setPopupMenuEnabled(false);
	inputFieldA.setFont(boldFont);
	inputFieldA.setColour(TextEditor::textColourId, LINE_GREY);
	inputFieldA.setText("0.0", true);
	inputFieldA.setLookAndFeel(&laF_TextEditor);
	inputFieldA.addListener(this);
	inputFieldA.setInputFilter(&filter, false);
	addAndMakeVisible(inputFieldA);

	inputFieldB.setMultiLine(false);
	inputFieldB.setCaretVisible(true);
	inputFieldB.setScrollbarsShown(false);
	inputFieldB.setPopupMenuEnabled(false);
	inputFieldB.setFont(boldFont);
	inputFieldB.setColour(TextEditor::textColourId, LINE_GREY);
	inputFieldB.setText("1.0", true);
	inputFieldB.setLookAndFeel(&laF_TextEditor);
	inputFieldB.addListener(this);
	inputFieldB.setInputFilter(&filter, false);
	addAndMakeVisible(inputFieldB);

	lerpAmountKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	lerpAmountKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	lerpAmountKnob.addListener(this);
	lerpAmountKnob.setRotaryParameters(0.0f, 3.1415f, true);
	lerpAmountKnob.setRange(0.0f, 1.0f, 0.0f);
	lerpAmountKnob.setValue(0.0f);
	lerpAmountKnob.setLookAndFeel(&laF_RotarySlider);
	addAndMakeVisible(lerpAmountKnob);

	valueA = 0.0f;
	valueB = 1.0f;

	outputSocketButtons[0]->SetValueType(ValueType::SimpleValue);
	outputSocketButtons[0]->button.setTooltip("Value");
	controlSocketButtons[0]->SetValueType(ValueType::SimpleValue);
	controlSocketButtons[0]->button.setTooltip("Lerp Control\n0.0 - 1.0");

	SetTitleFontSize(16.0f);

}

RangeModule::~RangeModule()
{
}

void RangeModule::PaintGUI(Graphics &g) {

}

void RangeModule::ResizeGUI() {
	inputFieldA.setBounds(25, 25, 50, 25);
	lerpAmountKnob.setBounds(25, 50, 50, 50);
	inputFieldB.setBounds(25, 100, 50, 25);
}

void RangeModule::sliderValueChanged(Slider* slider) {
	if (slider == &lerpAmountKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Lerp Amount";
		ngp->lastTweakedParameter = 2;
		ngp->lastTweakedParameterMin = lerpAmountKnob.getMinimum();
		ngp->lastTweakedParameterMax = lerpAmountKnob.getMaximum();
		ngp->lastTweakedParameterInc = lerpAmountKnob.getInterval();
		ngp->lastTweakedParameterValue = lerpAmountKnob.getValue();
	}
}

void RangeModule::sliderDragStarted(Slider* slider) {
	if (slider == &lerpAmountKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void RangeModule::sliderDragEnded(Slider* slider) {
	if (slider == &lerpAmountKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float RangeModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return valueA;
	case 1:
		return valueB;
	case 2:
		return lerpAmountKnob.getValue();
	default:
		return 0.0;
	}
}

void RangeModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		this->valueA = value;
		inputFieldA.setText(juce::String(value));
		break;
	case 1:
		this->valueB = value;
		inputFieldB.setText(juce::String(value));
		break;
	case 2:
		lerpAmountKnob.setValue(value);
	default:
		break;
	}
}

void RangeModule::textEditorReturnKeyPressed(TextEditor &t) {
	if (inputFieldA.getText().isEmpty()) {
		valueA = 0.0f;
	}
	else {
		std::string input = inputFieldA.getText().toStdString();
		std::string::size_type sz;
		valueA = std::stof(input.c_str(), &sz);
	}
	if (inputFieldB.getText().isEmpty()) {
		valueB = 0.0f;
	}
	else {
		std::string input = inputFieldB.getText().toStdString();
		std::string::size_type sz;
		valueB = std::stof(input.c_str(), &sz);
	}
}

double RangeModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		double lerpAmount = 0.0f;
		if (controls[0].connectedModule == -1)
			lerpAmount = lerpAmountKnob.getValue();
		else
			lerpAmount = ngp->modules[controls[0].connectedModule]->GetResult(midiNote, velocity, controls[0].connectedOutput, voiceID);

		outputs[0] = valueA + (valueB - valueA) * lerpAmount;
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void RangeModule::GetResultIteratively(int midiNote, float velocity, int voiveID) {
	double lerpAmount = 0.0f;
	if (controls[0].connectedModule == -1)
		lerpAmount = lerpAmountKnob.getValue();
	else
		lerpAmount = ngp->modules[controls[0].connectedModule]->outputs[controls[0].connectedOutput];

	outputs[0] = valueA + (valueB - valueA) * lerpAmount;
}
