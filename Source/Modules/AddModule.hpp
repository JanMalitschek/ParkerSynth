#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\Knob.hpp"

class AddModule : public Module {
public:
	AddModule();
	~AddModule();

	void PaintGUI(Graphics &g) override;
	void ResizeGUI() override;

	float GetParameter(int id) override;
	void SetParameter(int id, float value) override;

	double GetResult(int midiNote, float velocity, int outputID, int voiceID) override;
private:

};