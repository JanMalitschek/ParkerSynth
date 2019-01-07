#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\LaF_New.hpp"

struct UnisonVoice {
	UnisonVoice() {
		currentAngle = 0.0;
		angleDelta = 0.0;
	}
	double currentAngle;
	double angleDelta;
};

struct BasicGeneratorVoice{
	double currentAngle;
	double angleDelta;
	std::vector<UnisonVoice> uniVoices;
	double oldFrequency;
	bool recalculate;
};

class BasicGeneratorModule : public Module, public Slider::Listener {
public:
	BasicGeneratorModule();
	~BasicGeneratorModule();

	void PaintGUI(Graphics &g) override;
	void ResizeGUI() override;

	void sliderValueChanged(Slider* slider) override;
	void sliderDragStarted(Slider* slider) override;
	void sliderDragEnded(Slider* slider) override;

	float GetParameter(int id) override;
	void SetParameter(int id, float value) override;

	double GetResult(int midiNote, float velocity, int outputID, int voiceID) override;
private:
	Slider shapeKnob;
	LaF_RotarySlider laF_Knob;
	LaF_VerticalSlider laF_Slider;
	Slider voicesKnob;
	Slider detuneKnob;
	float oldDetune;

	BasicGeneratorVoice voices[9];

	double angleDelta;
	double currentAngle;

	void CalculateUnisonVoices(float freq, int voiceID);
};