#include "MIDIModule.hpp"
#include "..\LookAndFeel\Colors.hpp"

MIDIModule::MIDIModule() : Module(ModuleColorScheme::Pink, "MIDI", 0, 2, 0, Point<int>(4, 4), 0) {
	velocity.reset(44100, 0.01);
	outputSocketButtons[0]->button.setTooltip("Velocity");
	outputSocketButtons[0]->SetValueType(ValueType::Velocity);
	outputSocketButtons[1]->button.setTooltip("MIDI Note Number");
	outputSocketButtons[1]->SetValueType(ValueType::MIDINoteNumber);
}

MIDIModule::~MIDIModule(){

}

void MIDIModule::PaintGUI(Graphics &g) {
	/*g.drawImage(midiIcon, 0, 25, 50, 50, 0, 0, 50, 50);
	g.setFont(boldFont);
	g.setColour(MODULE_FONT_COLOR);
	g.drawFittedText("Vel", 73, 57, 25, 25, Justification::centred, 1);
	g.drawFittedText("Note", 73, 107, 25, 25, Justification::centred, 1);
	g.setColour(LINE_PINK);
	g.drawLine(50, 50, 75, 50, 2);
	g.drawLine(25, 75, 25, 100, 2);
	g.drawLine(25, 100, 75, 100, 2);*/
}

void MIDIModule::ResizeGUI() {
}

void MIDIModule::sliderValueChanged(Slider* slider) {

}

void MIDIModule::sliderDragStarted(Slider* slider) {
}

void MIDIModule::sliderDragEnded(Slider* slider) {
}

float MIDIModule::GetParameter(int id) {
	return 0.0f;
}

void MIDIModule::SetParameter(int id, float value) {

}

double MIDIModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	this->velocity.setValue(velocity);
	outputs[0] = velocity;
	outputs[1] = midiNote;

	return outputs[outputID];
}

void MIDIModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	outputs[0] = velocity;
	outputs[1] = midiNote;
}