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
#include <chrono>

class SynthVoice : public SynthesiserVoice {
public:
	bool canPlaySound(SynthesiserSound* sound) {
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
		frequency = 440.0f * pow(2.0, ((float)midiNoteNumber - 69) / 12);
		currentMidiNote = midiNoteNumber;
		currentVelocity = velocity;
		volume = 1.0;
	}

	void stopNote(float velocity, bool allowTailOff) {
		currentVelocity = 0.0f;
	}

	void pitchWheelMoved(int newPitchWheelValue) {

	}

	void controllerMoved(int controllerNumber, int newControllerValue) {
		 
	}

	void renderNextBlock(AudioBuffer<float> &outputBuffer, int sampleStart, int numSamples) {
		/*auto now1 = std::chrono::high_resolution_clock::now();*/
		if (currentVelocity > 0.0) {
			while (--numSamples >= 0) {
				double result = 0.0;
				result = ngp->GetResultIteratively(currentMidiNote, currentVelocity, this->id);
				outputBuffer.addSample(0, sampleStart, result);
				outputBuffer.addSample(1, sampleStart, result);
				++sampleStart;
			}
		}
		else {
			while (--numSamples >= 0) {
				double result = 0.0;
				if (volume > 0.0) {
					result = ngp->GetResultIteratively(currentMidiNote, 0.0f, this->id);
					if (abs(result) > volume)
						volume = abs(result);
				}
				volume -= timeStep;
				volume = jmax(volume, 0.0);
				outputBuffer.addSample(0, sampleStart, result);
				outputBuffer.addSample(1, sampleStart, result);
				++sampleStart;
				if (currentVelocity <= 0.0 && volume <= 0.005)
					clearCurrentNote();
			}
		}
		//auto now2 = std::chrono::high_resolution_clock::now();
		//auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(now2 - now1);
		//DBG(sampleStart);
		/*lastTime = now;*/
	}
	NodeGraphProcessor* ngp;

	int id;

private:
	double frequency;
	int currentMidiNote;
	float currentVelocity = 0.0;
	double volume = 0.0;
	double timeStep = 1.0 / 44100;
	std::chrono::system_clock::time_point lastTime;
};