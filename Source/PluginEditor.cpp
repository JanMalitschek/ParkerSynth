/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ParkerSynthAudioProcessorEditor::ParkerSynthAudioProcessorEditor(ParkerSynthAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p), toolbar(), nge()
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
	addAndMakeVisible(nge);
	addAndMakeVisible(toolbar);
	toolbar.nge = &nge;
	nge.ngp = &processor.ngp;
	nge.RedrawGUI();
	processor.ngp.nge = &nge;
	toolbar.DrawMacros();
}

ParkerSynthAudioProcessorEditor::~ParkerSynthAudioProcessorEditor()
{
}

//==============================================================================
void ParkerSynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colour(50, 50, 50));

	g.setColour(Colour(40, 40, 40));
	for (float i = 25; i < 800.0f; i += 25.0f) {
		g.drawLine(i, 0, i, 600, 2.0f);
	}
	for (float i = 25; i < 600.0f; i += 25.0f) {
		g.drawLine(0, i, 800, i, 2.0f);
	}
}

void ParkerSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	toolbar.setBounds(0, 0, 800, 25);
	nge.setBounds(0, 25, 800, 575);
}
