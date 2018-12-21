#include "MtoFModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

MtoFModule::MtoFModule() : Module(ModuleColorScheme::Grey, "MtoF", 1, 1, 0, Point<int>(4, 3), 0) {
	inputSocketButtons[0]->button.setTooltip("MIDI Note Number");
	inputSocketButtons[0]->SetValueType(ValueType::MIDINoteNumber);
	outputSocketButtons[0]->button.setTooltip("Frequency");
	outputSocketButtons[0]->SetValueType(ValueType::Frequency);
}

MtoFModule::~MtoFModule(){

}

void MtoFModule::PaintGUI(Graphics &g) {
	/*g.setFont(boldFont);
	g.setColour(MODULE_FONT_COLOR);
	g.drawFittedText("MIDI", 25, 25, 50, 25, Justification::centred, 1);
	g.drawFittedText("Freq", 25, 50, 50, 50, Justification::centred, 1);
	g.setColour(LINE_GREY);
	g.drawLine(25, 50, 75, 50, 2);*/
}

void MtoFModule::ResizeGUI() {
}

void MtoFModule::sliderValueChanged(Slider* slider) {

}

void MtoFModule::sliderDragStarted(Slider* slider) {
}

void MtoFModule::sliderDragEnded(Slider* slider) {
}

float MtoFModule::GetParameter(int id) {
	return 0.0f;
}

void MtoFModule::SetParameter(int id, float value) {

}

double MtoFModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (inputs[0].connectedModule >= 0) {
		outputs[0] = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
		outputs[0] = 440.0 * pow(2.0, ((float)outputs[0] - 69) / 12);
	}
	else
		outputs[0] = 69;
	return outputs[outputID];
}