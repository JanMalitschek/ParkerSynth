#include "BasicGeneratorModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

BasicGeneratorModule::BasicGeneratorModule() : Module(ModuleColorScheme::Blue, "Generator", 2, 1, 1, Point<int>(6, 4), 1) {
	shapeKnob.setSliderStyle(Slider::SliderStyle::LinearVertical);
	shapeKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	shapeKnob.addListener(this);
	shapeKnob.setRange(0.0f, 3.0f, 1.0f);
	shapeKnob.setValue(0.0f);
	shapeKnob.setLookAndFeel(&laF_Slider);
	addAndMakeVisible(shapeKnob);
	shapeSymbols = ImageFileFormat::loadFrom(BinaryData::Shapes_png, (size_t)BinaryData::Shapes_pngSize);

	voicesKnob.setSliderStyle(Slider::SliderStyle::LinearVertical);
	voicesKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	voicesKnob.addListener(this);
	voicesKnob.setRange(1.0f, 5.0f, 1.0f);
	voicesKnob.setValue(1.0f);
	voicesKnob.setLookAndFeel(&laF_Slider);
	addAndMakeVisible(voicesKnob);

	detuneKnob.setColour(detuneKnob.backgroundColourId, KNOB_BLUE);
	detuneKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	detuneKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	detuneKnob.addListener(this);
	detuneKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	detuneKnob.setRange(0.0f, 2.0f, 0.01f);
	detuneKnob.setValue(0.0f);
	detuneKnob.setLookAndFeel(&laF_Knob);
	addAndMakeVisible(detuneKnob);

	currentAngle = 0.0f;
	angleDelta = (440.0 / 44100) * 2.0 * MathConstants<double>::pi;

	for (int i = 0; i < 9; i++) {
		voices[i].currentAngle = 0.0f;
		voices[i].angleDelta = (440.0 / 44100) * 2.0 * MathConstants<double>::pi;
		for (int n = 0; n < 5; n++) {
			voices[i].uniVoices.push_back(UnisonVoice());
		}
		voices[i].recalculate = true;
	}
	inputSocketButtons[0]->button.setTooltip("Velocity");
	inputSocketButtons[0]->SetValueType(ValueType::Velocity);
	inputSocketButtons[1]->button.setTooltip("Frequency");
	inputSocketButtons[1]->SetValueType(ValueType::Frequency);
	outputSocketButtons[0]->button.setTooltip("Signal");
}

BasicGeneratorModule::~BasicGeneratorModule(){

}

void BasicGeneratorModule::PaintGUI(Graphics &g) {
	/*g.drawImage(shapeSymbols, 0, 0, 250, 175, 0, 0, 250, 175);
	g.setFont(boldFont);
	g.setColour(MODULE_FONT_COLOR);
	g.drawFittedText("Vel", 0, inputSocketButtons[0]->getBounds().getBottom() - 5, 50, 25, Justification::centred, 1);
	g.drawFittedText("Freq", 0, inputSocketButtons[1]->getBounds().getBottom() - 5, 50, 25, Justification::centred, 1);
	g.drawFittedText("Shape", 75, 88, 50, 25, Justification::centred, 1);*/
}

void BasicGeneratorModule::ResizeGUI() {
	shapeKnob.setBounds(25, 25, 25, 50);
	voicesKnob.setBounds(50, 25, 25, 50);
	detuneKnob.setBounds(75, 25, 50, 50);
}

void BasicGeneratorModule::sliderValueChanged(Slider* slider) {
	if (slider == &shapeKnob) {
		ngp->lastTweakedModule = this->id;
		ngp->lastTweakedModuleTitle = this->title + "->Shape";
		ngp->lastTweakedParameter = 0;
		ngp->lastTweakedParameterMin = shapeKnob.getMinimum();
		ngp->lastTweakedParameterMax = shapeKnob.getMaximum();
		ngp->lastTweakedParameterInc = shapeKnob.getInterval();
		ngp->lastTweakedParameterValue = shapeKnob.getValue();
	}
	if (slider == &voicesKnob || slider == &detuneKnob) {
		for (int i = 0; i < 9; i++) {
			voices[i].recalculate = true;
		}
	}
}

void BasicGeneratorModule::sliderDragStarted(Slider* slider) {
	if (slider == &shapeKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
	else if (slider == &voicesKnob) {
		slider->setMouseCursor(MouseCursor::UpDownResizeCursor);
	}
}

void BasicGeneratorModule::sliderDragEnded(Slider* slider) {
	if (slider == &shapeKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
	else if (slider == &voicesKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float BasicGeneratorModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return shapeKnob.getValue();
	default:
		return 0.0;
	}
}

void BasicGeneratorModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		shapeKnob.setValue(value);
		break;
	default:
		break;
	}
}

void BasicGeneratorModule::CalculateUnisonVoices(float freq, int voiceID) {
	int numUniVoices = (int)voicesKnob.getValue();
	float uniOffsetCenter = (float)numUniVoices / 2.0 - 0.5f;
	float detune = detuneKnob.getValue();
	for (int i = 0; i < numUniVoices; i++) {
		float midiOffset = ((float)i - uniOffsetCenter) * detune;
		float uniFrequency = freq * pow(2, midiOffset / 12.0);
		voices[voiceID].uniVoices[i].angleDelta = (uniFrequency / 44100) * 2.0 * MathConstants<double>::pi;
	}
}

double BasicGeneratorModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	float freq = 0.0f;
	if (inputs[1].connectedModule >= 0)
		freq = ngp->modules[inputs[1].connectedModule]->GetResult(midiNote, velocity, inputs[1].connectedOutput, voiceID);
	else
		freq = 440.0f;

	if (voices[voiceID].oldFrequency != freq || voices[voiceID].recalculate == true) {
		CalculateUnisonVoices(freq, voiceID);
		voices[voiceID].oldFrequency = freq;
		voices[voiceID].recalculate = false;
	}

	if (inputs[0].connectedModule >= 0) {
		/*double cyclesPerSecond = freq / 44100;
		voices[voiceID].angleDelta = cyclesPerSecond * 2.0 * MathConstants<double>::pi;*/
		int numUniVoices = (int)voicesKnob.getValue();
		float uniVolumeFactor = 1.0 / (float)numUniVoices;
		switch ((int)(shapeKnob.getValue())) {
			case 0:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += std::sin(voices[voiceID].uniVoices[i].currentAngle) * ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) * uniVolumeFactor;
					voices[voiceID].uniVoices[i].currentAngle += voices[voiceID].uniVoices[i].angleDelta;
				}
				break;
			case 1:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += std::asin(std::sin(voices[voiceID].uniVoices[i].currentAngle)) * ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) * uniVolumeFactor;
					voices[voiceID].uniVoices[i].currentAngle += voices[voiceID].uniVoices[i].angleDelta;
				}
				break;
			case 2:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += (fmod(voices[voiceID].uniVoices[i].currentAngle, 2.0 * MathConstants<double>::pi) / (2.0 * MathConstants<double>::pi) * 2.0 - 1.0) * ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) * uniVolumeFactor;
					voices[voiceID].uniVoices[i].currentAngle += voices[voiceID].uniVoices[i].angleDelta;
				}
				break;
			case 3:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += std::sin(voices[voiceID].currentAngle) * (ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) > 0.0 ? 1.0 : -1.0) * uniVolumeFactor;
					voices[voiceID].uniVoices[i].currentAngle += voices[voiceID].uniVoices[i].angleDelta;
				}
				break;
			default:

				break;
		}
	}
	else
		outputs[0] = 0.0;
	voices[voiceID].currentAngle += voices[voiceID].angleDelta;
	return outputs[outputID];
}