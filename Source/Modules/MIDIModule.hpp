#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\Knob.hpp"

class MIDIModule : public Module, public SliderListener {
public:
	MIDIModule();
	~MIDIModule();

	void PaintGUI(Graphics &g) override;
	void ResizeGUI() override;

	void sliderValueChanged(Slider* slider) override;
	void sliderDragStarted(Slider* slider) override;
	void sliderDragEnded(Slider* slider) override;

	float GetParameter(int id) override;
	void SetParameter(int id, float value) override;

	double GetResult(int midiNote, float velocity, int outputID, int voiceID) override;
private:
	Image midiIcon;
	LinearSmoothedValue<float> velocity;
};