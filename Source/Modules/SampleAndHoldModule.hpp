#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\Knob.hpp"

struct SaHVoice {
	bool holding = false;
	double value = 0.0f;
};

class SampleAndHoldModule : public Module, public Slider::Listener {
public:
	SampleAndHoldModule();
	~SampleAndHoldModule();

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
	bool holding;
	double heldValue;
};