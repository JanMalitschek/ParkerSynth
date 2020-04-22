#include "FilterModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

#define _USE_MATH_DEFINES
#include <math.h>

FilterModule::FilterModule() : Module(ModuleColorScheme::Grey, "Filter", 1, 1, 1, Point<int>(8, 5), 4) {
	cutoffKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	cutoffKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	cutoffKnob.addListener(this);
	cutoffKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	cutoffKnob.setRange(0.01f, 0.5f);
	cutoffKnob.setValue(0.01f);
	cutoff = 0.01f;
	cutoffKnob.setLookAndFeel(&laF_Knob);
	cutoffKnob.setTooltip("Cutoff\n1hz - 20khz");
	addAndMakeVisible(cutoffKnob);
	
	gainKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	gainKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	gainKnob.addListener(this);
	gainKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	gainKnob.setRange(-10.0f, 10.0f);
	gainKnob.setValue(0.0f);
	gain = 0.0f;
	gainKnob.setLookAndFeel(&laF_Knob);
	gainKnob.setTooltip("Gain\n-10.0 - 10.0");
	addAndMakeVisible(gainKnob);

	qKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	qKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	qKnob.addListener(this);
	qKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	qKnob.setRange(0.1f, 10.0f);
	qKnob.setValue(1.0f);
	q = 1.0f;
	qKnob.setLookAndFeel(&laF_Knob);
	qKnob.setTooltip("Q\n0.1 - 10.0");
	addAndMakeVisible(qKnob);

	filterTypeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	filterTypeSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	filterTypeSlider.addListener(this);
	filterTypeSlider.setRange(1.0f, 3.0f, 1.0f);
	filterTypeSlider.setValue(1.0f);
	type = 1.0f;
	filterTypeSlider.setLookAndFeel(&laF_Slider);
	filterTypeSlider.setTooltip("Type\nLP - BP - HP");
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
	cutoffKnob.setBounds(UtPX(1), UtPY(1), UtPX(2), UtPY(2));
	gainKnob.setBounds(UtPX(3), UtPY(1), UtPX(2), UtPY(2));
	qKnob.setBounds(UtPX(5), UtPY(1), UtPX(2), UtPY(2));
	filterTypeSlider.setBounds(UtPX(1), UtPY(3), UtPX(6), UtPY(1));
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
		cutoff = cutoffKnob.getValue();
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
		gain = gainKnob.getValue();
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
		q = qKnob.getValue();
	}
	else if (slider == &filterTypeSlider) {
		for (int i = 0; i < 9; i++) {
			voices[i].mustBeRecalculated = true;
		}
		type = filterTypeSlider.getValue();
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
	else if (slider == &filterTypeSlider) {
		slider->setMouseCursor(MouseCursor::LeftRightResizeCursor);
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
	else if (slider == &filterTypeSlider) {
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
		BiquadVoice& currentVoice = voices[voiceID];
		if(currentVoice.mustBeRecalculated){
			double newValue = type;
			double norm;
			double V = pow(10, fabs(gain) / 20.0);
			double K = tan(M_PI * cutoff);
			double Q = q;
			if (newValue <= 1.0) { // LowPass
				norm = 1 / (1 + K / Q + K * K);
				currentVoice.a0 = K * K * norm;
				currentVoice.a1 = 2 * currentVoice.a0;
				currentVoice.a2 = currentVoice.a0;
				currentVoice.b1 = 2 * (K * K - 1) * norm;
				currentVoice.b2 = (1 - K / Q + K * K) * norm;
			}
			else if (newValue <= 2.0) { // BandPass
				norm = 1 / (1 + K / Q + K * K);
				currentVoice.a0 = K / Q * norm;
				currentVoice.a1 = 0;
				currentVoice.a2 = -currentVoice.a0;
				currentVoice.b1 = 2 * (K * K - 1) * norm;
				currentVoice.b2 = (1 - K / Q + K * K) * norm;
			}
			else if (newValue > 2.0) { // HighPass
				norm = 1 / (1 + K / Q + K * K);
				currentVoice.a0 = 1 * norm;
				currentVoice.a1 = -2 * currentVoice.a0;
				currentVoice.a2 = currentVoice.a0;
				currentVoice.b1 = 2 * (K * K - 1) * norm;
				currentVoice.b2 = (1 - K / Q + K * K) * norm;
			}
			currentVoice.z1 = currentVoice.z2 = 0.0;
			currentVoice.mustBeRecalculated = false;
		}

		double input = 0.0;
		if (inputs[0].connectedModule >= 0)
			input = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
		outputs[0] = input * currentVoice.a0 + currentVoice.z1;
		currentVoice.z1 = input * currentVoice.a1 + currentVoice.z2 - currentVoice.b1 * outputs[0];
		currentVoice.z2 = input * currentVoice.a2 - currentVoice.b2 * outputs[0];
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void FilterModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	//Recalculate Voice
	if (voices[voiceID].mustBeRecalculated) {
		double newValue = filterTypeSlider.getValue();
		double norm;
		double V = pow(10, fabs(gainKnob.getValue()) / 20.0);
		double K = tan(M_PI * cutoffKnob.getValue());
		double Q = qKnob.getValue();
		if (newValue <= 1.0) { // LowPass
			norm = 1 / (1 + K / Q + K * K);
			voices[voiceID].a0 = K * K * norm;
			voices[voiceID].a1 = 2 * voices[voiceID].a0;
			voices[voiceID].a2 = voices[voiceID].a0;
			voices[voiceID].b1 = 2 * (K * K - 1) * norm;
			voices[voiceID].b2 = (1 - K / Q + K * K) * norm;
		}
		else if (newValue <= 2.0) { // BandPass
			norm = 1 / (1 + K / Q + K * K);
			voices[voiceID].a0 = K / Q * norm;
			voices[voiceID].a1 = 0;
			voices[voiceID].a2 = -voices[voiceID].a0;
			voices[voiceID].b1 = 2 * (K * K - 1) * norm;
			voices[voiceID].b2 = (1 - K / Q + K * K) * norm;
		}
		else if (newValue > 2.0) { // HighPass
			norm = 1 / (1 + K / Q + K * K);
			voices[voiceID].a0 = 1 * norm;
			voices[voiceID].a1 = -2 * voices[voiceID].a0;
			voices[voiceID].a2 = voices[voiceID].a0;
			voices[voiceID].b1 = 2 * (K * K - 1) * norm;
			voices[voiceID].b2 = (1 - K / Q + K * K) * norm;
		}
		voices[voiceID].z1 = voices[voiceID].z2 = 0.0;
		voices[voiceID].mustBeRecalculated = false;
	}

	READ_INPUT(input, 0)
	outputs[0] = input * voices[voiceID].a0 + voices[voiceID].z1;
	voices[voiceID].z1 = input * voices[voiceID].a1 + voices[voiceID].z2 - voices[voiceID].b1 * outputs[0];
	voices[voiceID].z2 = input * voices[voiceID].a2 - voices[voiceID].b2 * outputs[0];
}