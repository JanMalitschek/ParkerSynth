#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\Knob.hpp"
#include <string>
#include "..\NumericTextFilter.h"

class ValueModule : public Module, public TextEditorListener {
public:
	ValueModule();
	~ValueModule();

	void PaintGUI(Graphics &g) override;
	void ResizeGUI() override;

	void textEditorReturnKeyPressed(TextEditor &t);

	float GetParameter(int id) override;
	void SetParameter(int id, float value) override;

	double GetResult(int midiNote, float velocity, int outputID, int voiveID) override;
private:
	LaF_Knob laF_Knob;
	TextEditor inputField;
	NumericTextFilter filter;
	float value;
};