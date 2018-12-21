#include "OutModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

OutModule::OutModule() : Module(ModuleColorScheme::Pink, "Out", 1, 0, 0, Point<int>(4, 3), 0) {
	inputSocketButtons[0]->button.setTooltip("Signal");
	inputSocketButtons[0]->SetValueType(ValueType::Signal);
}

OutModule::~OutModule(){

}

void OutModule::PaintGUI(Graphics &g) {
	/*g.setFont(boldFont);
	g.setColour(MODULE_FONT_COLOR);
	g.drawFittedText("Out", 50, 25, 50, 50, Justification::centred, 1);
	g.setColour(LINE_PINK);
	g.drawLine(25, 50, 50, 50, 2);*/
}

void OutModule::ResizeGUI() {
}

void OutModule::sliderValueChanged(Slider* slider) {

}

void OutModule::sliderDragStarted(Slider* slider) {
}

void OutModule::sliderDragEnded(Slider* slider) {
}

float OutModule::GetParameter(int id) {
	return 0.0f;
}

void OutModule::SetParameter(int id, float value) {

}

double OutModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (inputs[0].connectedModule >= 0)
		return ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
	else
		return 0.0f;
}