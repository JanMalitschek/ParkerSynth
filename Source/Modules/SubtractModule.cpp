#include "SubtractModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

SubtractModule::SubtractModule() : Module(ModuleColorScheme::Grey, "Sub", 2, 1, 0, Point<int>(4, 4), 0) {
	inputSocketButtons[0]->button.setTooltip("Value A");
	inputSocketButtons[0]->SetValueType(ValueType::SimpleValue);
	inputSocketButtons[1]->button.setTooltip("Value B");
	inputSocketButtons[1]->SetValueType(ValueType::SimpleValue);
	outputSocketButtons[0]->button.setTooltip("Result");
	outputSocketButtons[0]->SetValueType(ValueType::SimpleValue);
}

SubtractModule::~SubtractModule(){

}

void SubtractModule::PaintGUI(Graphics &g) {
}

void SubtractModule::ResizeGUI() {

}

float SubtractModule::GetParameter(int id) {
	return 0.0f;
}

void SubtractModule::SetParameter(int id, float value) {

}

double SubtractModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		double a = 0.0f;
		double b = 0.0f;
		if (inputs[0].connectedModule >= 0)
			a = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
		if (inputs[1].connectedModule >= 0)
			b = ngp->modules[inputs[1].connectedModule]->GetResult(midiNote, velocity, inputs[1].connectedOutput, voiceID);

		outputs[0] = a - b;
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void SubtractModule::GetResultIteratively(int midiNote, float velocity, int voiveID) {
	READ_INPUT(a, 0)
	READ_INPUT(b, 1)
	outputs[0] = a - b;
}