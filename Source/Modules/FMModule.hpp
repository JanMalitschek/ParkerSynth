#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\LaF_New.hpp"

class FMModule : public Module, public Slider::Listener {
public:
	FMModule();
	~FMModule();

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
	Slider fmKnob;
	LaF_RotarySlider laF_Knob;
};