#pragma once
#include "Module.hpp"
#include "..\LookAndFeel\LaF_New.hpp"
#include <string>
#include "..\NumericTextFilter.h"

class RangeModule : public Module, public TextEditor::Listener, public Slider::Listener
{
public:
	RangeModule();
	~RangeModule();

	void PaintGUI(Graphics &g) override;
	void ResizeGUI() override;

	void sliderValueChanged(Slider* slider) override;
	void sliderDragStarted(Slider* slider) override;
	void sliderDragEnded(Slider* slider) override;

	void textEditorReturnKeyPressed(TextEditor &t);

	float GetParameter(int id) override;
	void SetParameter(int id, float value) override;

	double GetResult(int midiNote, float velocity, int outputID, int voiveID) override;
	inline void GetResultIteratively(int midiNote, float velocity, int voiveID) override;

private:
	LaF_TextEditor laF_TextEditor;
	LaF_RotarySlider laF_RotarySlider;
	TextEditor inputFieldA;
	Slider lerpAmountKnob;
	double lerp;
	TextEditor inputFieldB;
	NumericTextFilter filter;
	float valueA;
	float valueB;
};

