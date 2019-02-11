#include "GainModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

GainModule::GainModule() : Module(ModuleColorScheme::Grey, "Gain", 1, 1, 0, Point<int>(4, 4), 1) {
	gainKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	gainKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	gainKnob.addListener(this);
	gainKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	gainKnob.setRange(0.0f, 2.0f, 0.01f);
	gainKnob.setValue(1.0f);
	gainKnob.setLookAndFeel(&laF_Knob);
	addAndMakeVisible(gainKnob);
	inputSocketButtons[0]->button.setTooltip("Signal");
	outputSocketButtons[0]->button.setTooltip("Signal");
}

GainModule::~GainModule(){

}

void GainModule::PaintGUI(Graphics &g) {

}

void GainModule::ResizeGUI() {
	gainKnob.setBounds(25, 25, 50, 50);
}

void GainModule::sliderValueChanged(Slider* slider) {
	if (slider == &gainKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Gain";
		ngp->lastTweakedParameter = 0;
		ngp->lastTweakedParameterMin = gainKnob.getMinimum();
		ngp->lastTweakedParameterMax = gainKnob.getMaximum();
		ngp->lastTweakedParameterInc = gainKnob.getInterval();
		ngp->lastTweakedParameterValue = gainKnob.getValue();
	}
}

void GainModule::sliderDragStarted(Slider* slider) {
	if (slider == &gainKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void GainModule::sliderDragEnded(Slider* slider) {
	if (slider == &gainKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float GainModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return gainKnob.getValue();
	default:
		return 0.0;
	}
}

void GainModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		gainKnob.setValue(value);
		break;
	default:
		break;
	}
}

double GainModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		if (inputs[0].connectedModule >= 0)
			outputs[0] = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) * gainKnob.getValue();
		else
			outputs[0] = 0.0;
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void GainModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	if (inputs[0].connectedModule >= 0)
		outputs[0] = ngp->modules[inputs[0].connectedModule]->outputs[inputs[0].connectedOutput] * gainKnob.getValue();
	else
		outputs[0] = 0.0;
}