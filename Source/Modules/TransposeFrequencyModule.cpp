#include "TransposeFrequencyModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

TransposeFrequencyModule::TransposeFrequencyModule() : Module(ModuleColorScheme::Grey, "TransF", 1, 1, 0, Point<int>(4, 4), 1) {
	transKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	transKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	transKnob.addListener(this);
	transKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	transKnob.setRange(-12.0f, 12.0f, 1.0f);
	transKnob.setValue(0.0f);
	trans = 0.0f;
	transKnob.setLookAndFeel(&laF_Knob);
	transKnob.setTooltip("Transposition\n-12.0st - 12.0st");
	addAndMakeVisible(transKnob);
	inputSocketButtons[0]->button.setTooltip("Frequency");
	inputSocketButtons[0]->SetValueType(ValueType::Frequency);
	outputSocketButtons[0]->button.setTooltip("Frequency");
	outputSocketButtons[0]->SetValueType(ValueType::Frequency);

	SetTitleFontSize(16.0f);
}

TransposeFrequencyModule::~TransposeFrequencyModule(){

}

void TransposeFrequencyModule::PaintGUI(Graphics &g) {

}

void TransposeFrequencyModule::ResizeGUI() {
	transKnob.setBounds(UtPX(1), UtPY(1), UtPX(2), UtPY(2));
}

void TransposeFrequencyModule::sliderValueChanged(Slider* slider) {
	if (slider == &transKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Offset";
		ngp->lastTweakedParameter = 0;
		ngp->lastTweakedParameterMin = transKnob.getMinimum();
		ngp->lastTweakedParameterMax = transKnob.getMaximum();
		ngp->lastTweakedParameterInc = transKnob.getInterval();
		ngp->lastTweakedParameterValue = transKnob.getValue();
		trans = transKnob.getValue();
	}
}

void TransposeFrequencyModule::sliderDragStarted(Slider* slider) {
	if (slider == &transKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void TransposeFrequencyModule::sliderDragEnded(Slider* slider) {
	if (slider == &transKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float TransposeFrequencyModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return transKnob.getValue();
	default:
		return 0.0;
	}
}

void TransposeFrequencyModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		transKnob.setValue(value);
		break;
	default:
		break;
	}
}

double TransposeFrequencyModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		if (inputs[0].connectedModule >= 0)
			outputs[0] = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) * pow(1.059463f, trans);
		else
			outputs[0] = 440.0f;
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void TransposeFrequencyModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	if (IS_INPUT_CONNECTED(0))
		outputs[0] = GET_INPUT(0) * pow(1.059463f, transKnob.getValue());
	else
		outputs[0] = 440.0f;
}