#include "TransposeMIDIModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

TransposeMIDIModule::TransposeMIDIModule() : Module(ModuleColorScheme::Grey, "TransM", 1, 1, 0, Point<int>(4, 4), 1) {
	transKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	transKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	transKnob.addListener(this);
	transKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	transKnob.setRange(-12.0f, 12.0f, 1.0f);
	transKnob.setValue(0.0f);
	transKnob.setLookAndFeel(&laF_Knob);
	transKnob.setTooltip("Transposition\n-12.0st - 12.0st");
	addAndMakeVisible(transKnob);
	inputSocketButtons[0]->button.setTooltip("MIDI Note Number");
	inputSocketButtons[0]->SetValueType(ValueType::MIDINoteNumber);
	outputSocketButtons[0]->button.setTooltip("MIDI Note Number");
	outputSocketButtons[0]->SetValueType(ValueType::MIDINoteNumber);

	SetTitleFontSize(16.0f);

}

TransposeMIDIModule::~TransposeMIDIModule(){

}

void TransposeMIDIModule::PaintGUI(Graphics &g) {

}

void TransposeMIDIModule::ResizeGUI() {
	transKnob.setBounds(25, 25, 50, 50);
}

void TransposeMIDIModule::sliderValueChanged(Slider* slider) {
	if (slider == &transKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Offset";
		ngp->lastTweakedParameter = 0;
		ngp->lastTweakedParameterMin = transKnob.getMinimum();
		ngp->lastTweakedParameterMax = transKnob.getMaximum();
		ngp->lastTweakedParameterInc = transKnob.getInterval();
		ngp->lastTweakedParameterValue = transKnob.getValue();
	}
}

void TransposeMIDIModule::sliderDragStarted(Slider* slider) {
	if (slider == &transKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void TransposeMIDIModule::sliderDragEnded(Slider* slider) {
	if (slider == &transKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float TransposeMIDIModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return transKnob.getValue();
	default:
		return 0.0;
	}
}

void TransposeMIDIModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		transKnob.setValue(value);
		break;
	default:
		break;
	}
}

double TransposeMIDIModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		if (inputs[0].connectedModule >= 0)
			outputs[0] = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) + transKnob.getValue();
		else
			outputs[0] = 69;
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void TransposeMIDIModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	if (inputs[0].connectedModule >= 0)
		outputs[0] = ngp->modules[inputs[0].connectedModule]->outputs[inputs[0].connectedOutput] + transKnob.getValue();
	else
		outputs[0] = 69;
}