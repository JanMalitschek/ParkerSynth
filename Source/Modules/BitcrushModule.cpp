#include "BitcrushModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

BitcrushModule::BitcrushModule() : Module(ModuleColorScheme::Grey, "Bitcrush", 1, 1, 2, Point<int>(6, 4), 2) {
	depthKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	depthKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	depthKnob.addListener(this);
	depthKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	depthKnob.setRange(0.0f, 0.995f);
	depthKnob.setValue(0.0f);
	depth = 0.0f;
	depthKnob.setSkewFactor(2.0f);
	depthKnob.setLookAndFeel(&laF_Knob);
	depthKnob.setTooltip("Bit Depth\n100% - 0.005%");
	addAndMakeVisible(depthKnob);

	downsampleKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	downsampleKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	downsampleKnob.addListener(this);
	downsampleKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	downsampleKnob.setRange(0.0f, 1.0f);
	downsampleKnob.setValue(0.0f);
	downsample = 0.0f;
	downsampleKnob.setSkewFactor(0.1f);
	downsampleKnob.setLookAndFeel(&laF_Knob);
	downsampleKnob.setTooltip("Samplerate\n100% - 0.001%");
	addAndMakeVisible(downsampleKnob);

	inputSocketButtons[0]->button.setTooltip("Signal");
	outputSocketButtons[0]->button.setTooltip("Signal");
	controlSocketButtons[0]->SetValueType(ValueType::SimpleValue);
	controlSocketButtons[0]->button.setTooltip("Depth Control\n0.0 - 1.0");
	controlSocketButtons[1]->SetValueType(ValueType::SimpleValue);
	controlSocketButtons[1]->button.setTooltip("Samplerate Control\n0.0 - 1.0");

	sampleRate = 44100;

	for (int i = 0; i < 9; i++) {
		voices[i].currentSample = 0.0;
		voices[i].sampleCounter = 0;
	}
}

BitcrushModule::~BitcrushModule(){
	depthKnob.removeListener(this);
	downsampleKnob.removeListener(this);
	removeAllChildren();
}

void BitcrushModule::PaintGUI(Graphics &g) {

}

void BitcrushModule::ResizeGUI() {
	depthKnob.setBounds(UtPX(1), UtPY(1), UtPX(2), UtPY(2));
	downsampleKnob.setBounds(UtPX(3), UtPY(1), UtPX(2), UtPY(2));
}

void BitcrushModule::sliderValueChanged(Slider* slider) {
	if (slider == &depthKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Depth Reduction Amount";
		ngp->lastTweakedParameter = 0;
		ngp->lastTweakedParameterMin = depthKnob.getMinimum();
		ngp->lastTweakedParameterMax = depthKnob.getMaximum();
		ngp->lastTweakedParameterInc = depthKnob.getInterval();
		ngp->lastTweakedParameterValue = depthKnob.getValue();
		depth = depthKnob.getValue();
	}
	else if (slider == &downsampleKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Downsample Amount";
		ngp->lastTweakedParameter = 1;
		ngp->lastTweakedParameterMin = downsampleKnob.getMinimum();
		ngp->lastTweakedParameterMax = downsampleKnob.getMaximum();
		ngp->lastTweakedParameterInc = downsampleKnob.getInterval();
		ngp->lastTweakedParameterValue = downsampleKnob.getValue();
		downsample = downsampleKnob.getValue();
	}
}

void BitcrushModule::sliderDragStarted(Slider* slider) {
	if (slider == &depthKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
	else if (slider == &downsampleKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void BitcrushModule::sliderDragEnded(Slider* slider) {
	if (slider == &depthKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
	else if (slider == &downsampleKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float BitcrushModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return depthKnob.getValue();
	case 1:
		return downsampleKnob.getValue();
	default:
		return 0.0;
	}
}

void BitcrushModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		depthKnob.setValue(value);
		break;
	case 1:
		downsampleKnob.setValue(value);
		break;
	default:
		break;
	}
}

double BitcrushModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		double input = 0.0;
		if (inputs[0].connectedModule >= 0) {
			input = ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID);
		}

		double downsampleFactor = downsample;
		if (controls[0].connectedModule >= 0) {
			downsampleFactor = ngp->modules[controls[0].connectedModule]->GetResult(midiNote, velocity, controls[0].connectedOutput, voiceID);
		}

		BitcrushVoice& currentVoice = voices[voiceID];
		float maxHeldSamples = sampleRate * downsampleFactor;
		currentVoice.sampleCounter++;
		if (currentVoice.sampleCounter > maxHeldSamples) {
			currentVoice.currentSample = input;
			currentVoice.sampleCounter = 0;
		}

		double depthReductionFactor = depth;
		if (controls[1].connectedModule >= 0) {
			depthReductionFactor = ngp->modules[controls[1].connectedModule]->GetResult(midiNote, velocity, controls[1].connectedOutput, voiceID);
		}

		float resolution = (1.0f - depthReductionFactor) * 1024.0f;

		outputs[0] = floor(currentVoice.currentSample * resolution) / resolution;
		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void BitcrushModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	READ_INPUT(input, 0)

	READ_CTRL(downsampleFactor, 0, downsampleKnob.getValue())

	float maxHeldSamples = sampleRate * downsampleFactor;
	voices[voiceID].sampleCounter++;
	if (voices[voiceID].sampleCounter > maxHeldSamples) {
		voices[voiceID].currentSample = input;
		voices[voiceID].sampleCounter = 0;
	}

	READ_CTRL(depthReductionFactor, 1, depthKnob.getValue())

	float resolution = (1.0f - depthReductionFactor) * 1024.0f;

	outputs[0] = floor(voices[voiceID].currentSample * resolution) / resolution;
}