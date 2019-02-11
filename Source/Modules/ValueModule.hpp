#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\LaF_New.hpp"
#include <string>
#include "..\NumericTextFilter.h"

class ValueModule : public Module, public TextEditor::Listener {
public:
	ValueModule();
	~ValueModule();

	void PaintGUI(Graphics &g) override;
	void ResizeGUI() override;

	void textEditorReturnKeyPressed(TextEditor &t);

	float GetParameter(int id) override;
	void SetParameter(int id, float value) override;

	double GetResult(int midiNote, float velocity, int outputID, int voiveID) override;
	inline void GetResultIteratively(int midiNote, float velocity, int voiveID) override;
private:
	LaF_TextEditor laF_TextEditor;
	TextEditor inputField;
	NumericTextFilter filter;
	float value;
};