#include "FilterModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

FilterModule::FilterModule() : Module(ModuleColorScheme::Grey, "Filter", 1, 1, 1, Point<int>(8, 5), 4) {
	cutoffKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	cutoffKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	cutoffKnob.addListener(this);
	cutoffKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	cutoffKnob.setRange(20.0f, 20000.0f);
	cutoffKnob.setValue(440.0f);
	cutoffKnob.setLookAndFeel(&laF_Knob);
	addAndMakeVisible(cutoffKnob);
	
	gainKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	gainKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	gainKnob.addListener(this);
	gainKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	gainKnob.setRange(-10.0f, 10.0f);
	gainKnob.setValue(0.0f);
	gainKnob.setLookAndFeel(&laF_Knob);
	addAndMakeVisible(gainKnob);

	qKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	qKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	qKnob.addListener(this);
	qKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	qKnob.setRange(0.1f, 10.0f);
	qKnob.setValue(1.0f);
	qKnob.setLookAndFeel(&laF_Knob);
	addAndMakeVisible(qKnob);

	filterTypeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	filterTypeSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	filterTypeSlider.addListener(this);
	filterTypeSlider.setRange(1.0f, 3.0f, 1.0f);
	filterTypeSlider.setValue(1.0f);
	filterTypeSlider.setLookAndFeel(&laF_Slider);
	addAndMakeVisible(filterTypeSlider);

	inputSocketButtons[0]->button.setTooltip("Signal");
	outputSocketButtons[0]->button.setTooltip("Signal");
	controlSocketButtons[0]->SetValueType(ValueType::SimpleValue);
	controlSocketButtons[0]->button.setTooltip("Cutoff Control\n0.0 - 1.0");

	for (int i = 0; i < 9; i++) {
		voices[i] = BiquadVoice();
	}

}

FilterModule::~FilterModule(){

}

void FilterModule::PaintGUI(Graphics &g) {

}

void FilterModule::ResizeGUI() {
	cutoffKnob.setBounds(25, 25, 50, 50);
	gainKnob.setBounds(75, 25, 50, 50);
	qKnob.setBounds(125, 25, 50, 50);
	filterTypeSlider.setBounds(25, 75, 150, 25);
}

void FilterModule::sliderValueChanged(Slider* slider) {
	if (slider == &cutoffKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Cutoff";
		ngp->lastTweakedParameter = 0;
		ngp->lastTweakedParameterMin = cutoffKnob.getMinimum();
		ngp->lastTweakedParameterMax = cutoffKnob.getMaximum();
		ngp->lastTweakedParameterInc = cutoffKnob.getInterval();
		ngp->lastTweakedParameterValue = cutoffKnob.getValue();
		for (int i = 0; i < 9; i++) {
			voices[i].mustBeRecalculated = true;
		}
	}
	else if (slider == &gainKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Gain";
		ngp->lastTweakedParameter = 1;
		ngp->lastTweakedParameterMin = gainKnob.getMinimum();
		ngp->lastTweakedParameterMax = gainKnob.getMaximum();
		ngp->lastTweakedParameterInc = gainKnob.getInterval();
		ngp->lastTweakedParameterValue = gainKnob.getValue();
		for (int i = 0; i < 9; i++) {
			voices[i].mustBeRecalculated = true;
		}
	}
	else if (slider == &qKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Q";
		ngp->lastTweakedParameter = 2;
		ngp->lastTweakedParameterMin = qKnob.getMinimum();
		ngp->lastTweakedParameterMax = qKnob.getMaximum();
		ngp->lastTweakedParameterInc = qKnob.getInterval();
		ngp->lastTweakedParameterValue = qKnob.getValue();
		for (int i = 0; i < 9; i++) {
			voices[i].mustBeRecalculated = true;
		}
	}
	else if (slider == &filterTypeSlider) {
		for (int i = 0; i < 9; i++) {
			voices[i].mustBeRecalculated = true;
		}
	}
}

void FilterModule::sliderDragStarted(Slider* slider) {
	if (slider == &cutoffKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
	else if (slider == &gainKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
	else if (slider == &qKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void FilterModule::sliderDragEnded(Slider* slider) {
	if (slider == &cutoffKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
	else if (slider == &gainKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
	else if (slider == &qKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float FilterModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return cutoffKnob.getValue();
	case 1:
		return gainKnob.getValue();
	case 2:
		return qKnob.getValue();
	default:
		return 0.0;
	}
}

void FilterModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		cutoffKnob.setValue(value);
		break;
	case 1:
		gainKnob.setValue(value);
		break;
	case 2:
		qKnob.setValue(value);
		break;
	default:
		break;
	}
}

double FilterModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {

		//Recalculate Voice
		if(voices[voiceID].mustBeRecalculated){
			double newValue = filterTypeSlider.getValue();
			double norm;
			double V = pow(10, fabs(gainKnob.getValue()) / 20.0);
			double K = tan(MathConstants<double>::pi * cutoffKnob.getValue());
			double Q = qKnob.getValue();
			if (newValue < 1.0) { // LowPass
				norm = 1 / (1 + K / Q + K * K);
				voices[voiceID].a0 = K * K * norm;
				voices[voiceID].a1 = 2 * voices[voiceID].a0;
				voices[voiceID].a2 = voices[voiceID].a0;
				voices[voiceID].b1 = 2 * (K * K - 1) * norm;
				voices[voiceID].b2 = (1 - K / Q + K * K) * norm;
			}
			else if (newValue < 2.0) { // BandPass
				norm = 1 / (1 + K / Q + K * K);
				voices[voiceID].a0 = K / Q * norm;
				voices[voiceID].a1 = 0;
				voices[voiceID].a2 = -voices[voiceID].a0;
				voices[voiceID].b1 = 2 * (K * K - 1) * norm;
				voices[voiceID].b2 = (1 - K / Q + K * K) * norm;
			}
			else if (newValue >= 2.0) { // HighPass
				norm = 1 / (1 + K / Q + K * K);
				voices[voiceID].a0 = 1 * norm;
				voices[voiceID].a1 = -2 * voices[voiceID].a0;
				voices[voiceID].a2 = voices[voiceID].a0;
				voices[voiceID].b1 = 2 * (K * K - 1) * norm;
				voices[voiceID].b2 = (1 - K / Q + K * K) * norm;
			}
			voices[voiceID].mustBeRecalculated = false;
		}

		double input = 0.0;
		if (inputs[0].connectedModule >= 0)
			input = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
		outputs[0] = input * voices[voiceID].a0 * voices[voiceID].z1;
		voices[voiceID].z1 = input * voices[voiceID].a1 + voices[voiceID].z2 - voices[voiceID].b1 * outputs[0];
		voices[voiceID].z2 = input * voices[voiceID].a2 - voices[voiceID].b2 * outputs[0];
		canBeEvaluated = false;
	}
	return outputs[outputID];
}