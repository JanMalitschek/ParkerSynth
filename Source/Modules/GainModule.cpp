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
	gain = 1.0f;
	gainKnob.setLookAndFeel(&laF_Knob);
	gainKnob.setTooltip("Gain\n0.0 - 2.0");
	addAndMakeVisible(gainKnob);
	inputSocketButtons[0]->button.setTooltip("Signal");
	outputSocketButtons[0]->button.setTooltip("Signal");
}

GainModule::~GainModule(){
	gainKnob.removeListener(this);
	removeAllChildren();
}

void GainModule::PaintGUI(Graphics &g) {

}

void GainModule::ResizeGUI() {
	gainKnob.setBounds(UtPX(1), UtPY(1), UtPX(2), UtPY(2));
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
		gain = gainKnob.getValue();
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
			outputs[0] = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) * gain;
		else
			outputs[0] = 0.0;
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void GainModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	if (IS_INPUT_CONNECTED(0))
		outputs[0] = GET_INPUT(0) * gainKnob.getValue();
	else
		outputs[0] = 0.0;
}