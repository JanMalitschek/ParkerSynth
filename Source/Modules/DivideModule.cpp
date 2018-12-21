#include "DivideModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

DivideModule::DivideModule() : Module(ModuleColorScheme::Grey, "Div", 2, 1, 0, Point<int>(4, 4), 0) {
	div_Image = ImageFileFormat::loadFrom(BinaryData::Math_Divide_png, (size_t)BinaryData::Math_Divide_png);
	inputSocketButtons[0]->button.setTooltip("Value A");
	inputSocketButtons[0]->SetValueType(ValueType::SimpleValue);
	inputSocketButtons[1]->button.setTooltip("Value B");
	inputSocketButtons[1]->SetValueType(ValueType::SimpleValue);
	outputSocketButtons[0]->button.setTooltip("Result");
	outputSocketButtons[0]->SetValueType(ValueType::SimpleValue);
}

DivideModule::~DivideModule(){

}

void DivideModule::PaintGUI(Graphics &g) {
	g.drawImage(div_Image, 0, 0, 100, 100, 0, 0, 100, 100);
}

void DivideModule::ResizeGUI() {

}

float DivideModule::GetParameter(int id) {
	return 0;
}

void DivideModule::SetParameter(int id, float value) {

}

double DivideModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	double a = 0.0f;
	double b = 0.0f;
	if (inputs[0].connectedModule >= 0)
		a = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
	if (inputs[1].connectedModule >= 0)
		b = ngp->modules[inputs[1].connectedModule]->GetResult(midiNote, velocity, inputs[1].connectedOutput, voiceID);

	if (b != 0.0)
		return a / b;
	else
		return 0.0;
}