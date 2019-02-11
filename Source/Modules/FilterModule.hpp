#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\LaF_New.hpp"

struct BiquadVoice {
	double a0, a1, a2, b1, b2;
	double z1, z2;
	bool mustBeRecalculated;
	BiquadVoice() {
		a0 = 1.0;
		a1 = a2 = b1 = b2 = 0.0;
		z1 = z2 = 0.0;
		mustBeRecalculated = true;
	}
};

class FilterModule : public Module, public Slider::Listener {
public:
	FilterModule();
	~FilterModule();

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
	Slider cutoffKnob;
	Slider gainKnob;
	Slider qKnob;
	Slider filterTypeSlider;
	LaF_RotarySlider laF_Knob;
	LaF_VerticalSlider laF_Slider;

	BiquadVoice voices[9];

};