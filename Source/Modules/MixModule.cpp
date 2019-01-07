#include "MixModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

MixModule::MixModule() : Module(ModuleColorScheme::Grey, "Mix", 2, 1, 1, Point<int>(4, 4), 1) {
	mixKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mixKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	mixKnob.addListener(this);
	mixKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	mixKnob.setRange(0.0f, 1.0f, 0.0f);
	mixKnob.setValue(0.0f);
	mixKnob.setLookAndFeel(&laF_Knob);
	addAndMakeVisible(mixKnob);
	inputSocketButtons[0]->button.setTooltip("SignalA");
	inputSocketButtons[1]->button.setTooltip("SignalB");
	outputSocketButtons[0]->button.setTooltip("Signal");
	controlSocketButtons[0]->SetValueType(ValueType::SimpleValue);
	controlSocketButtons[0]->button.setTooltip("Mix Factor\n0.0 - 1.0");
}

MixModule::~MixModule(){

}

void MixModule::PaintGUI(Graphics &g) {

}

void MixModule::ResizeGUI() {
	mixKnob.setBounds(25, 25, 50, 50);
}

void MixModule::sliderValueChanged(Slider* slider) {
	if (slider == &mixKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Mix Factor";
		ngp->lastTweakedParameter = 0;
		ngp->lastTweakedParameterMin = mixKnob.getMinimum();
		ngp->lastTweakedParameterMax = mixKnob.getMaximum();
		ngp->lastTweakedParameterInc = mixKnob.getInterval();
		ngp->lastTweakedParameterValue = mixKnob.getValue();
	}
}

void MixModule::sliderDragStarted(Slider* slider) {
	if (slider == &mixKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void MixModule::sliderDragEnded(Slider* slider) {
	if (slider == &mixKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float MixModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return mixKnob.getValue();
	default:
		return 0.0;
	}
}

void MixModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		mixKnob.setValue(value);
		break;
	default:
		break;
	}
}

double MixModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	float mixFactor = mixKnob.getValue();
	if (controls[0].connectedModule >= 0)
		mixFactor = ngp->modules[controls[0].connectedModule]->GetResult(midiNote, velocity, controls[0].connectedOutput, voiceID);
	double signalA = 0.0f;
	double signalB = 0.0f;
	if (inputs[0].connectedModule >= 0)
		signalA = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
	if (inputs[1].connectedModule >= 0)
		signalB = ngp->modules[inputs[1].connectedModule]->GetResult(midiNote, velocity, inputs[1].connectedOutput, voiceID);

	outputs[0] = signalA + (signalB - signalA) * mixFactor;

	return outputs[outputID];
}