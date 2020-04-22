#include "WhiteNoiseModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"
#include <time.h>

WhiteNoiseModule::WhiteNoiseModule() : Module(ModuleColorScheme::Blue, "Noise", 1, 1, 0, Point<int>(4, 3), 0) {

	inputSocketButtons[0]->button.setTooltip("Velocity");
	inputSocketButtons[0]->SetValueType(ValueType::Velocity);
	outputSocketButtons[0]->button.setTooltip("Signal");

	srand(time(0));
}

WhiteNoiseModule::~WhiteNoiseModule(){

}

void WhiteNoiseModule::PaintGUI(Graphics &g) {

}

void WhiteNoiseModule::ResizeGUI() {

}

float WhiteNoiseModule::GetParameter(int id) {
	switch (id) {
	default:
		return 0.0;
	}
}

void WhiteNoiseModule::SetParameter(int id, float value) {
	switch (id) {
	default:
		break;
	}
}

double WhiteNoiseModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		float velocityIn = 0.0f;
		if (inputs[0].connectedModule >= 0)
			velocityIn = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
		if (velocityIn > 0.0f) {
			outputs[0] = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * velocityIn;
		}
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void WhiteNoiseModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	float velocityIn = 0.0f;
	if (IS_INPUT_CONNECTED(0))
		velocityIn = GET_INPUT(0);
	if (velocityIn > 0.0f) {
		outputs[0] = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * velocityIn;
	}
}