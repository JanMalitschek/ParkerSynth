#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\Knob.hpp"

class SubtractModule : public Module {
public:
	SubtractModule();
	~SubtractModule();

	void PaintGUI(Graphics &g) override;
	void ResizeGUI() override;

	float GetParameter(int id) override;
	void SetParameter(int id, float value) override;

	double GetResult(int midiNote, float velocity, int outputID, int voiveID) override;
private:
	Image sub_Image;
};