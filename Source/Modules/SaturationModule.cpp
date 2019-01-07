#include "SaturationModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

SaturationModule::SaturationModule() : Module(ModuleColorScheme::Grey, "Saturate", 1, 1, 0, Point<int>(6, 4), 2) {
	slopeKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	slopeKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	slopeKnob.addListener(this);
	slopeKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	slopeKnob.setRange(1.0f, 100.0f);
	slopeKnob.setValue(1.0f);
	slopeKnob.setLookAndFeel(&laF_Knob);
	addAndMakeVisible(slopeKnob);

	kneeKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	kneeKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	kneeKnob.addListener(this);
	kneeKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	kneeKnob.setRange(0.0f, 1.0f);
	kneeKnob.setValue(0.0f);
	kneeKnob.setLookAndFeel(&laF_Knob);
	addAndMakeVisible(kneeKnob);

	inputSocketButtons[0]->button.setTooltip("Signal");
	outputSocketButtons[0]->button.setTooltip("Signal");
}

SaturationModule::~SaturationModule(){

}

void SaturationModule::PaintGUI(Graphics &g) {

}

void SaturationModule::ResizeGUI() {
	slopeKnob.setBounds(25, 25, 50, 50);
	kneeKnob.setBounds(75, 25, 50, 50);
}

void SaturationModule::sliderValueChanged(Slider* slider) {
	if (slider == &slopeKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Slope";
		ngp->lastTweakedParameter = 0;
		ngp->lastTweakedParameterMin = slopeKnob.getMinimum();
		ngp->lastTweakedParameterMax = slopeKnob.getMaximum();
		ngp->lastTweakedParameterInc = slopeKnob.getInterval();
		ngp->lastTweakedParameterValue = slopeKnob.getValue();
	}
	else if (slider == &kneeKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Knee";
		ngp->lastTweakedParameter = 1;
		ngp->lastTweakedParameterMin = kneeKnob.getMinimum();
		ngp->lastTweakedParameterMax = kneeKnob.getMaximum();
		ngp->lastTweakedParameterInc = kneeKnob.getInterval();
		ngp->lastTweakedParameterValue = kneeKnob.getValue();
	}
}

void SaturationModule::sliderDragStarted(Slider* slider) {
	if (slider == &slopeKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
	else if (slider == &kneeKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void SaturationModule::sliderDragEnded(Slider* slider) {
	if (slider == &slopeKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
	else if (slider == &kneeKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float SaturationModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return slopeKnob.getValue();
	case 1:
		return kneeKnob.getValue();
	default:
		return 0.0;
	}
}

void SaturationModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		slopeKnob.setValue(value);
		break;
	case 1:
		kneeKnob.setValue(value);
		break;
	default:
		break;
	}
}

double SaturationModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	double inputSignal = 0.0f;
	if (inputs[0].connectedModule >= 0)
		inputSignal = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);

	float slope = slopeKnob.getValue();
	double clipped = jlimit<double>(-1.0, 1.0, inputSignal * slope);
	double soft = (slope + 1) * (inputSignal / (1 + abs(slope * inputSignal)));
	outputs[0] = clipped + (soft - clipped) * kneeKnob.getValue();

	return outputs[outputID];
}