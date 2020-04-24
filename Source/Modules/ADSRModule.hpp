#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\LaF_New.hpp"

struct ADSRVoice {
	bool reset;
	double time;
	double vel;
};

class ADSRModule : public Module, public Slider::Listener {
public:
	ADSRModule();
	~ADSRModule();

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
	Slider aKnob;
	double attack;
	Slider dKnob;
	double decay;
	Slider sKnob;
	double sustain;
	Slider rKnob;
	double release;
	LaF_RotarySlider laF_Knob;

	ADSRVoice voices[9];

	float lastGain;

	inline double moveTowards(double& value, double target, double maxDelta);

};