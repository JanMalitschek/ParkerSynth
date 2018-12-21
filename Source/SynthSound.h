/*
  ==============================================================================

    SynthSound.h
    Created: 2 Jun 2018 11:26:14am
    Author:  Aprikosis

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SynthSound : public SynthesiserSound {
public:
	bool appliesToNote(int midiNoteNumber) {
		return true;
	}
	bool appliesToChannel(int midiChannel) {
		return true;
	}
};