#include "SampleAndHoldModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

SampleAndHoldModule::SampleAndHoldModule() : Module(ModuleColorScheme::Grey, "SaH", 2, 1, 0, Point<int>(4, 4), 0) {
	inputSocketButtons[0]->button.setTooltip("Signal");
	inputSocketButtons[1]->button.setTooltip("Trigger Value");
	inputSocketButtons[1]->SetValueType(ValueType::SimpleValue);
	outputSocketButtons[0]->button.setTooltip("Held Value");
	outputSocketButtons[0]->SetValueType(ValueType::SimpleValue);
	holding = false;
	heldValue = 0.0f;
}

SampleAndHoldModule::~SampleAndHoldModule(){

}

void SampleAndHoldModule::PaintGUI(Graphics &g) {

}

void SampleAndHoldModule::ResizeGUI() {
}

void SampleAndHoldModule::sliderValueChanged(Slider* slider) {

}

void SampleAndHoldModule::sliderDragStarted(Slider* slider) {
}

void SampleAndHoldModule::sliderDragEnded(Slider* slider) {
}

float SampleAndHoldModule::GetParameter(int id) {
	return 0.0f;
}

void SampleAndHoldModule::SetParameter(int id, float value) {

}

double SampleAndHoldModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		double signal = 0.0f;
		float triggerValue = 0.0f;
		if (inputs[0].connectedModule >= 0) {
			signal = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
		}
		if (inputs[1].connectedModule >= 0) {
			triggerValue = ngp->modules[inputs[1].connectedModule]->GetResult(midiNote, velocity, inputs[1].connectedOutput, voiceID);
		}
		if (triggerValue > 0.0f && !holding) {
			holding = true;
			heldValue = signal;
		}
		if (triggerValue <= 0.0f) {
			holding = false;
			heldValue = 0.0f;
		}
		outputs[0] = heldValue;
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void SampleAndHoldModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	READ_INPUT(signal, 0)
	READ_INPUT(triggerValue, 1)
	if (triggerValue > 0.0 && !holding) {
		holding = true;
		heldValue = signal;
	}
	if (triggerValue <= 0.0) {
		holding = false;
		heldValue = 0.0;
	}
	outputs[0] = heldValue;
}