#include "ValueModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

ValueModule::ValueModule() : Module(ModuleColorScheme::Grey, "Value", 0, 1, 0, Point<int>(4, 3), 1) {
	inputField.setMultiLine(false);
	inputField.setCaretVisible(true);
	inputField.setScrollbarsShown(false);
	inputField.setPopupMenuEnabled(false);
	inputField.setFont(boldFont);
	inputField.setColour(TextEditor::textColourId, LINE_GREY);
	inputField.setText("0.0", true);
	inputField.setLookAndFeel(&laF_TextEditor);
	inputField.addListener(this);
	inputField.setInputFilter(&filter, false);
	addAndMakeVisible(inputField);

	value = 0.0f;

	outputSocketButtons[0]->button.setTooltip("Value");
	outputSocketButtons[0]->SetValueType(ValueType::SimpleValue);
}

ValueModule::~ValueModule(){

}

void ValueModule::PaintGUI(Graphics &g) {

}

void ValueModule::ResizeGUI() {
	inputField.setBounds(25, 25, 50, 25);
}

float ValueModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return value;
	default:
		return 0.0;
	}
}

void ValueModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		this->value = value;
		inputField.setText(juce::String(value));
		break;
	default:
		break;
	}
}

void ValueModule::textEditorReturnKeyPressed(TextEditor &t) {
	if (inputField.getText().isEmpty()) {
		value = 0.0f;
	}
	else {
		std::string input = inputField.getText().toStdString();
		std::string::size_type sz;
		value = std::stof(input.c_str(), &sz);
	}
}

double ValueModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	return value;
}