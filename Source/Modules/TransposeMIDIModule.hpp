#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\LaF_New.hpp"

class TransposeMIDIModule : public Module, public Slider::Listener {
public:
	TransposeMIDIModule();
	~TransposeMIDIModule();

	void PaintGUI(Graphics &g) override;
	void ResizeGUI() override;

	void sliderValueChanged(Slider* slider) override;
	void sliderDragStarted(Slider* slider) override;
	void sliderDragEnded(Slider* slider) override;

	float GetParameter(int id) override;
	void SetParameter(int id, float value) override;

	double GetResult(int midiNote, float velocity, int outputID, int voiceID) override;
	inline void GetResultIteratively(int midiNote, float velocity, int voiceID) override;
private:
	Slider transKnob;
	double trans;
	LaF_RotarySlider laF_Knob;
};