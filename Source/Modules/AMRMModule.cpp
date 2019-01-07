#include "AMRMModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

AMRMModule::AMRMModule() : Module(ModuleColorScheme::Grey, "AM RM", 2, 1, 1, Point<int>(4, 5), 2) {
	amountKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	amountKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	amountKnob.addListener(this);
	amountKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	amountKnob.setRange(0.0f, 1.0f);
	amountKnob.setValue(0.0f);
	amountKnob.setLookAndFeel(&laF_Knob);
	addAndMakeVisible(amountKnob);

	modeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	modeSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	modeSlider.addListener(this);
	modeSlider.setRange(0.0f, 1.0f, 1.0f);
	modeSlider.setValue(0.0f);
	modeSlider.setLookAndFeel(&laF_Slider);
	addAndMakeVisible(modeSlider);

	inputSocketButtons[0]->button.setTooltip("Carrier Signal");
	inputSocketButtons[1]->button.setTooltip("Modulator Signal");
	outputSocketButtons[0]->button.setTooltip("Signal");
	controlSocketButtons[0]->button.setTooltip("Modulation Amount\n0.0 - 1.0");
	controlSocketButtons[0]->SetValueType(ValueType::SimpleValue);

	SetTitleFontSize(20.0f);
}

AMRMModule::~AMRMModule(){

}

void AMRMModule::PaintGUI(Graphics &g) {

}

void AMRMModule::ResizeGUI() {
	amountKnob.setBounds(25, 50, 50, 50);
	modeSlider.setBounds(25, 25, 50, 25);
}

void AMRMModule::sliderValueChanged(Slider* slider) {
	if (slider == &amountKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Modulation Amount";
		ngp->lastTweakedParameter = 0;
		ngp->lastTweakedParameterMin = amountKnob.getMinimum();
		ngp->lastTweakedParameterMax = amountKnob.getMaximum();
		ngp->lastTweakedParameterInc = amountKnob.getInterval();
		ngp->lastTweakedParameterValue = amountKnob.getValue();
	}
}

void AMRMModule::sliderDragStarted(Slider* slider) {
	if (slider == &amountKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
	else if (slider == &modeSlider) {
		slider->setMouseCursor(MouseCursor::LeftRightResizeCursor);
	}
}

void AMRMModule::sliderDragEnded(Slider* slider) {
	if (slider == &amountKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
	else if (slider == &modeSlider) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float AMRMModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return amountKnob.getValue();
	case 1:
		return modeSlider.getValue();
	default:
		return 0.0;
	}
}

void AMRMModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		amountKnob.setValue(value);
		break;
	case 1:
		modeSlider.setValue(value);
		break;
	default:
		break;
	}
}

double AMRMModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		double carrSignal = 0.0;
		double modSignal = 0.0;
		if (inputs[0].connectedModule >= 0)
			carrSignal = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
		if (inputs[1].connectedModule >= 0)
			modSignal = ngp->modules[inputs[1].connectedModule]->GetResult(midiNote, velocity, inputs[1].connectedOutput, voiceID);

		double modFactor = 0.0;
		if (controls[0].connectedModule >= 0) {
			modFactor = ngp->modules[controls[0].connectedModule]->GetResult(midiNote, velocity, controls[0].connectedOutput, voiceID);
		}
		else {
			modFactor = amountKnob.getValue();
		}

		if (modeSlider.getValue() == 0.0) {
			outputs[0] = carrSignal + (carrSignal * abs(modSignal) - carrSignal) * modFactor;
		}
		else {
			outputs[0] = carrSignal + (carrSignal * modSignal - carrSignal) * modFactor;
		}
		canBeEvaluated = false;
	}
	return outputs[outputID];
}