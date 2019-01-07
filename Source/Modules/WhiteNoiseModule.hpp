#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\LaF_New.hpp"

class WhiteNoiseModule : public Module {
public:
	WhiteNoiseModule();
	~WhiteNoiseModule();

	void PaintGUI(Graphics &g) override;
	void ResizeGUI() override;

	float GetParameter(int id) override;
	void SetParameter(int id, float value) override;

	double GetResult(int midiNote, float velocity, int outputID, int voiceID) override;
private:

};