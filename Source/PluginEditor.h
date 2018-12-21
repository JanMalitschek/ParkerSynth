/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Modules\GainModule.hpp"
#include "CustomToolbar.h"
#include "NodeGraphEditor.h"
#include "ToolbarFactory.h"

//==============================================================================
/**
*/
class ParkerSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ParkerSynthAudioProcessorEditor (ParkerSynthAudioProcessor&);
    ~ParkerSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ParkerSynthAudioProcessor& processor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParkerSynthAudioProcessorEditor)

	//CustomToolbar toolbar;
	NodeGraphEditor nge;
	CustomToolbar toolbar;

};
