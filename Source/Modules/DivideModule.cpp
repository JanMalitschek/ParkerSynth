#include "DivideModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

DivideModule::DivideModule() : Module(ModuleColorScheme::Grey, "Div", 2, 1, 0, Point<int>(4, 4), 0) {

	inputSocketButtons[0]->button.setTooltip("Value A");
	inputSocketButtons[0]->SetValueType(ValueType::SimpleValue);
	inputSocketButtons[1]->button.setTooltip("Value B");
	inputSocketButtons[1]->SetValueType(ValueType::SimpleValue);
	outputSocketButtons[0]->button.setTooltip("Result");
	outputSocketButtons[0]->SetValueType(ValueType::SimpleValue);
}

DivideModule::~DivideModule(){
	removeAllChildren();
}

void DivideModule::PaintGUI(Graphics &g) {
}

void DivideModule::ResizeGUI() {

}

float DivideModule::GetParameter(int id) {
	return 0;
}

void DivideModule::SetParameter(int id, float value) {

}

double DivideModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		double a = 0.0f;
		double b = 0.0f;
		if (inputs[0].connectedModule >= 0)
			a = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
		if (inputs[1].connectedModule >= 0)
			b = ngp->modules[inputs[1].connectedModule]->GetResult(midiNote, velocity, inputs[1].connectedOutput, voiceID);

		if (b != 0.0)
			outputs[0] = a / b;
		else
			outputs[0] = 0.0;
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void DivideModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	READ_INPUT(a, 0)
	READ_INPUT(b, 1)

	if (b != 0.0)
		outputs[0] = a / b;
	else
		outputs[0] = 0.0;
}