/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <string>

//==============================================================================
ParkerSynthAudioProcessor::ParkerSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
					#if ! JucePlugin_IsMidiEffect
					#if ! JucePlugin_IsSynth
							.withInput("Input", AudioChannelSet::stereo(), true)
					#endif
							.withOutput("Output", AudioChannelSet::stereo(), true)
					#endif
						),
	valueTree(*this, nullptr)
#endif
{
	for (int i = 0; i < 20; i++) {
		juce::String index = juce::String(std::to_string(i));
		valueTree.createAndAddParameter("MACRO_" + index, "Macro " + index, "Macro" + index, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f, nullptr, nullptr);
	}
	valueTree.state = ValueTree("SavedData");

	synth.clearVoices();
	for (int i = 0; i < NUM_VOICES; i++) {
		synth.addVoice(new SynthVoice());
	}
	synth.clearSounds();
	synth.addSound(new SynthSound());
	ngp = NodeGraphProcessor();
	ngp.sampleRate = getSampleRate();
	
	for (int i = 0; i < 20; i++) {
		ngp.macros.push_back(new Macro());
		ngp.macros[i]->ngp = &ngp;
		juce::String index = juce::String(std::to_string(i));
		ngp.macros[i]->attachment = new AudioProcessorValueTreeState::SliderAttachment(valueTree, "MACRO_" + index, ngp.macros[i]->valueKnob);
	}
	for (int i = 0; i < NUM_VOICES; i++) {
		SynthVoice* voice = dynamic_cast<SynthVoice*>(this->synth.getVoice(i));
		voice->ngp = &ngp;
		voice->id = i;
		DBG(juce::String(std::to_string(voice->id)));
	}
}

ParkerSynthAudioProcessor::~ParkerSynthAudioProcessor()
{
}

//==============================================================================
const String ParkerSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ParkerSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ParkerSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ParkerSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ParkerSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ParkerSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ParkerSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ParkerSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String ParkerSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void ParkerSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ParkerSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	ignoreUnused(samplesPerBlock);
	lastSampleRate = sampleRate;
	ngp.sampleRate = sampleRate;
	synth.setCurrentPlaybackSampleRate(lastSampleRate);
}

void ParkerSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ParkerSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ParkerSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages){
	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool ParkerSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ParkerSynthAudioProcessor::createEditor()
{
    return new ParkerSynthAudioProcessorEditor (*this);
}

//==============================================================================
void ParkerSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

	ngp.SavePreset(false);
	if (ngp.saveData.size() > 5) {
		for (int i = 0; i < ngp.saveData.size(); i++) {
			Identifier propName = "Line" + std::to_string(i);
			valueTree.state.setProperty(propName, var(ngp.saveData[i]), nullptr);
		}
		ngp.saveDataSize = valueTree.state.getNumProperties();
		auto state = valueTree.state.createCopy();
		std::unique_ptr<XmlElement> xml(state.createXml());
		copyXmlToBinary(*xml, destData);
	}
}

void ParkerSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState.get() != nullptr) {
		if (xmlState->hasTagName(valueTree.state.getType())) {
			valueTree.state = ValueTree::fromXml(*xmlState);
			ngp.saveData.clear();
			ngp.saveDataSize = 0;
			for (int i = 0; i < valueTree.state.getNumProperties(); i++) {
				String propName = "Line" + std::to_string(i);
				ngp.saveData.add(valueTree.state.getProperty(propName).toString());
			}
			ngp.saveDataSize = valueTree.state.getNumProperties();
			ngp.LoadPreset(false);
		}
	}

}

void ParkerSynthAudioProcessor::SetNodeGraphEditor(NodeGraphEditor* nge) {

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParkerSynthAudioProcessor();
}
