/*
  ==============================================================================

    SynthVoice.h
    Created: 2 Jun 2018 11:26:20am
    Author:  Aprikosis

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "NodeGraphProcessor.h"

class SynthVoice : public SynthesiserVoice {
public:
	bool canPlaySound(SynthesiserSound* sound) {
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
		frequency = 440.0f * pow(2.0, ((float)midiNoteNumber - 69) / 12);
		currentMidiNote = midiNoteNumber;
		currentVelocity = velocity;
	}

	void stopNote(float velocity, bool allowTailOff) {
		allowTailOff = true;
		if(velocity == 0)
			clearCurrentNote();
		currentVelocity = 0.0f;
	}

	void pitchWheelMoved(int newPitchWheelValue) {

	}

	void controllerMoved(int controllerNumber, int newControllerValue) {
		 
	}

	void renderNextBlock(AudioBuffer<float> &outputBuffer, int sampleStart, int numSamples) {
		for (int sample = 0; sample < numSamples; ++sample) {
			double result = 0.0;
			if (currentVelocity > 0.0 || volume > 0.0) {
				result = ngp->GetResult(currentMidiNote, currentVelocity, this->id);
				if (abs(result) > volume) {
					volume = abs(result);
				}
			}
			volume -= timeStep;
			volume = jmax(volume, 0.0);
			outputBuffer.addSample(0, sample, result);
			outputBuffer.addSample(1, sample, result);
			++sampleStart;
		}
	}
	NodeGraphProcessor* ngp;

	int id;

private:
	double frequency;
	int currentMidiNote;
	float currentVelocity;
	double volume = 0.0;
	double timeStep = 1.0 / 44100;
};