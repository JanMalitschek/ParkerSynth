#include "FMModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

FMModule::FMModule() : Module(ModuleColorScheme::Grey, "FM", 3, 1, 1, Point<int>(4, 5), 1) {
	fmKnob.setColour(fmKnob.backgroundColourId, KNOB_GREY);
	fmKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	fmKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	fmKnob.addListener(this);
	fmKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	fmKnob.setRange(0.0f, 32.0f, 0.01f);
	fmKnob.setValue(1.0f);
	fm = 1.0f;
	fmKnob.setLookAndFeel(&laF_Knob);
	fmKnob.setTooltip("FM\n0.0 - 32.0");
	addAndMakeVisible(fmKnob);

	inputSocketButtons[0]->button.setTooltip("Carrier Frequency");
	inputSocketButtons[0]->SetValueType(ValueType::Frequency);
	inputSocketButtons[1]->button.setTooltip("Modulator Signal");
	inputSocketButtons[2]->button.setTooltip("Modulator Frequency");
	inputSocketButtons[2]->SetValueType(ValueType::Frequency);
	outputSocketButtons[0]->button.setTooltip("Frequency");
	outputSocketButtons[0]->SetValueType(ValueType::Frequency);
	controlSocketButtons[0]->button.setTooltip("Amount Control\n0.0 - 32.0");
	controlSocketButtons[0]->SetValueType(ValueType::SimpleValue);
}

FMModule::~FMModule(){
	fmKnob.removeListener(this);
	removeAllChildren();
}

void FMModule::PaintGUI(Graphics &g) {

}

void FMModule::ResizeGUI() {
	fmKnob.setBounds(UtPX(1), UtPY(1), UtPX(2), UtPY(2));
}

void FMModule::sliderValueChanged(Slider* slider) {
	if (slider == &fmKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Amount";
		ngp->lastTweakedParameter = 0;
		ngp->lastTweakedParameterMin = fmKnob.getMinimum();
		ngp->lastTweakedParameterMax = fmKnob.getMaximum();
		ngp->lastTweakedParameterInc = fmKnob.getInterval();
		ngp->lastTweakedParameterValue = fmKnob.getValue();
		fm = fmKnob.getValue();
	}
}

void FMModule::sliderDragStarted(Slider* slider) {
	if (slider == &fmKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void FMModule::sliderDragEnded(Slider* slider) {
	if (slider == &fmKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float FMModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return fmKnob.getValue();
	default:
		return 0.0;
	}
}

void FMModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		fmKnob.setValue(value);
		break;
	default:
		break;
	}
}

double FMModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		double a = 0.0; //frequency
		double b = 0.0; //modulator signal
		double c = 0.0; //modulator frequency
		if (inputs[0].connectedModule >= 0)
			a = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
		else
			a = 0.0;
		if (inputs[1].connectedModule >= 0)
			b = ngp->modules[inputs[1].connectedModule]->GetResult(midiNote, velocity, inputs[1].connectedOutput, voiceID);
		else
			b = 0.0;
		if (inputs[2].connectedModule >= 0)
			c = ngp->modules[inputs[2].connectedModule]->GetResult(midiNote, velocity, inputs[2].connectedOutput, voiceID);
		else
			b = 1.0;

		double amount = fm;
		if (controls[0].connectedModule >= 0)
			amount = ngp->modules[controls[0].connectedModule]->GetResult(midiNote, velocity, controls[0].connectedOutput, voiceID) * fm;

		outputs[0] = a + b * c * amount;
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void FMModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	READ_INPUT(a, 0)
	READ_INPUT(b, 1)
	READ_INPUT_FLBK(c, 2, 1.0)

	double amount = 0.0;
	if (IS_CTRL_CONNECTED(0))
		amount = GET_CTRL(0) * fmKnob.getValue();
	else
		amount = fmKnob.getValue();

	outputs[0] = a + b * c * amount;
}