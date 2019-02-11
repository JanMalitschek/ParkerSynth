#pragma once

#include "Module.hpp"
#include "..\LookAndFeel\Knob.hpp"

class MultiplyModule : public Module {
public:
	MultiplyModule();
	~MultiplyModule();

	void PaintGUI(Graphics &g) override;
	void ResizeGUI() override;

	float GetParameter(int id) override;
	void SetParameter(int id, float value) override;

	double GetResult(int midiNote, float velocity, int outputID, int voiveID) override;
	inline void GetResultIteratively(int midiNote, float velocity, int voiveID) override;
private:

};