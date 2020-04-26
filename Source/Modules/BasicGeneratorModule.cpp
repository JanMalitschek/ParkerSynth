#include "BasicGeneratorModule.hpp"
#include "..\LookAndFeel\Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

BasicGeneratorModule::BasicGeneratorModule() : Module(ModuleColorScheme::Blue, "Generator", 2, 1, 1, Point<int>(6, 4), 3) {
	shapeKnob.setSliderStyle(Slider::SliderStyle::LinearVertical);
	shapeKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	shapeKnob.addListener(this);
	shapeKnob.setRange(0.0f, 3.0f, 1.0f);
	shapeKnob.setValue(0.0f);
	shape = 0.0f;
	shapeKnob.setLookAndFeel(&laF_Slider);
	shapeKnob.setTooltip("Shape\nSine - Triangle - Saw - Square");
	addAndMakeVisible(shapeKnob);

	voicesKnob.setSliderStyle(Slider::SliderStyle::LinearVertical);
	voicesKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	voicesKnob.addListener(this);
	voicesKnob.setRange(1.0f, 5.0f, 1.0f);
	voicesKnob.setValue(1.0f);
	voicesParam = 1.0f;
	voicesKnob.setLookAndFeel(&laF_Slider);
	voicesKnob.setTooltip("Voices\n1 - 5");
	addAndMakeVisible(voicesKnob);

	detuneKnob.setColour(detuneKnob.backgroundColourId, KNOB_BLUE);
	detuneKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	detuneKnob.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	detuneKnob.addListener(this);
	detuneKnob.setRotaryParameters(-2.35619f, 2.35619f, true);
	detuneKnob.setRange(0.0f, 2.0f, 0.01f);
	detuneKnob.setValue(0.0f);
	detune = 0.0f;
	detuneKnob.setLookAndFeel(&laF_Knob);
	detuneKnob.setTooltip("Detune\n0.0 - 2.0");
	addAndMakeVisible(detuneKnob);

	oldDetune = 0.0f;

	for (int i = 0; i < 9; i++) {
		/*voices[i].currentAngle = 0.0f;
		voices[i].angleDelta = (440.0 / ngp->sampleRate) * 2.0 * MathConstants<double>::pi;*/
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
	controlSocketButtons[0]->button.setTooltip("Detune Control\n0.0 - 2.0");
}

BasicGeneratorModule::~BasicGeneratorModule(){
	shapeKnob.removeListener(this);
	voicesKnob.removeListener(this);
	detuneKnob.removeListener(this);
	removeAllChildren();
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
	shapeKnob.setBounds(UtPX(1), UtPY(1), UtPX(1), UtPY(2));
	voicesKnob.setBounds(UtPX(2), UtPY(1), UtPX(1), UtPY(2));
	detuneKnob.setBounds(UtPX(3), UtPY(1), UtPX(2), UtPY(2));
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
		shape = shapeKnob.getValue();
	}
	else if (slider == &voicesKnob) {
		voicesParam = voicesKnob.getValue();
	}
	else if (slider == &detuneKnob) {
		detune = detuneKnob.getValue();
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
	else if (slider == &detuneKnob) {
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
	else if (slider == &detuneKnob) {
		slider->setMouseCursor(MouseCursor::NormalCursor);
	}
}

float BasicGeneratorModule::GetParameter(int id) {
	switch (id) {
	case 0:
		return shapeKnob.getValue();
		break;
	case 1:
		return voicesKnob.getValue();
		break;
	case 2:
		return detuneKnob.getValue();
		break;
	default:
		return 0.0;
	}
}

void BasicGeneratorModule::SetParameter(int id, float value) {
	switch (id) {
	case 0:
		shapeKnob.setValue(value);
		break;
	case 1:
		voicesKnob.setValue(value);
		break;
	case 2:
		detuneKnob.setValue(value);
		break;
	default:
		break;
	}
}

void BasicGeneratorModule::CalculateUnisonVoices(float freq, int voiceID) {
	int numUniVoices = (int)voicesParam;
	float uniOffsetCenter = (float)numUniVoices / 2.0 - 0.5f;
	for (int i = 0; i < numUniVoices; i++) {
		float midiOffset = ((float)i - uniOffsetCenter) * detune;
		float uniFrequency = freq * pow(2, midiOffset / 12.0);
		voices[voiceID].uniVoices[i].angleDelta = (uniFrequency / ngp->sampleRate) * 2.0 * MathConstants<double>::pi;
	}
}

double BasicGeneratorModule::GetResult(int midiNote, float velocity, int outputID, int voiceID) {
	if (canBeEvaluated) {
		float freq = 0.0f;
		if (inputs[1].connectedModule >= 0)
			freq = ngp->modules[inputs[1].connectedModule]->GetResult(midiNote, velocity, inputs[1].connectedOutput, voiceID);
		else
			freq = 440.0f;

		BasicGeneratorVoice& currentVoice = voices[voiceID];
		if (currentVoice.oldFrequency != freq || currentVoice.recalculate == true) {
			CalculateUnisonVoices(freq, voiceID);
			currentVoice.oldFrequency = freq;
			currentVoice.recalculate = false;
		}

		if (controls[0].connectedModule >= 0) {
			float currentDetune = ngp->modules[controls[0].connectedModule]->GetResult(midiNote, velocity, controls[0].connectedOutput, voiceID);
			if (oldDetune != currentDetune) {
				detuneKnob.setValue(currentDetune);
				oldDetune = currentDetune;
			}
		}

		if (inputs[0].connectedModule >= 0) {
			/*double cyclesPerSecond = freq / 44100;
			voices[voiceID].angleDelta = cyclesPerSecond * 2.0 * MathConstants<double>::pi;*/
			int numUniVoices = (int)voicesParam;
			float uniVolumeFactor = 1.0 / (float)numUniVoices;
			switch ((int)(shape)) {
			case 0:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += std::sin(currentVoice.uniVoices[i].currentAngle) * ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) * uniVolumeFactor;
					currentVoice.uniVoices[i].currentAngle += currentVoice.uniVoices[i].angleDelta;
				}
				break;
			case 1:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += std::asin(std::sin(currentVoice.uniVoices[i].currentAngle)) * ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) * uniVolumeFactor;
					currentVoice.uniVoices[i].currentAngle += currentVoice.uniVoices[i].angleDelta;
				}
				break;
			case 2:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += (fmod(currentVoice.uniVoices[i].currentAngle, 2.0 * MathConstants<double>::pi) / (2.0 * MathConstants<double>::pi) * 2.0 - 1.0) * ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) * uniVolumeFactor;
					currentVoice.uniVoices[i].currentAngle += currentVoice.uniVoices[i].angleDelta;
				}
				break;
			case 3:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += (std::sin(currentVoice.uniVoices[i].currentAngle) > 0.0f ? 1.0f : -1.0f) * ngp->modules[inputs[0].connectedModule]->GetResult(midiNote, velocity, inputs[0].connectedOutput, voiceID) * uniVolumeFactor;
					currentVoice.uniVoices[i].currentAngle += currentVoice.uniVoices[i].angleDelta;
				}
				break;
			default:

				break;
			}
		}
		else
			outputs[0] = 0.0;

		canBeEvaluated = false;
	}
	return outputs[outputID];
}

void BasicGeneratorModule::GetResultIteratively(int midiNote, float velocity, int voiceID) {
		READ_INPUT_FLBK(freq, 1, 440.0)

		if (voices[voiceID].oldFrequency != freq || voices[voiceID].recalculate == true) {
			CalculateUnisonVoices(freq, voiceID);
			voices[voiceID].oldFrequency = freq;
			voices[voiceID].recalculate = false;
		}

		if (IS_CTRL_CONNECTED(0)) {
			float currentDetune = ngp->modules[controls[0].connectedModule]->outputs[controls[0].connectedOutput];
			if (oldDetune != currentDetune) {
				detuneKnob.setValue(currentDetune);
				oldDetune = currentDetune;
			}
		}

		if (IS_INPUT_CONNECTED(0)) {
			/*double cyclesPerSecond = freq / 44100;
			voices[voiceID].angleDelta = cyclesPerSecond * 2.0 * MathConstants<double>::pi;*/
			int numUniVoices = (int)voicesKnob.getValue();
			float uniVolumeFactor = 1.0 / (float)numUniVoices;
			switch ((int)(shapeKnob.getValue())) {
			case 0:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += std::sin(voices[voiceID].uniVoices[i].currentAngle) * ngp->modules[inputs[0].connectedModule]->outputs[inputs[0].connectedOutput] * uniVolumeFactor;
					voices[voiceID].uniVoices[i].currentAngle += voices[voiceID].uniVoices[i].angleDelta;
				}
				break;
			case 1:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += std::asin(std::sin(voices[voiceID].uniVoices[i].currentAngle)) * ngp->modules[inputs[0].connectedModule]->outputs[inputs[0].connectedOutput] * uniVolumeFactor;
					voices[voiceID].uniVoices[i].currentAngle += voices[voiceID].uniVoices[i].angleDelta;
				}
				break;
			case 2:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += (fmod(voices[voiceID].uniVoices[i].currentAngle, 2.0 * MathConstants<double>::pi) / (2.0 * MathConstants<double>::pi) * 2.0 - 1.0) * ngp->modules[inputs[0].connectedModule]->outputs[inputs[0].connectedOutput] * uniVolumeFactor;
					voices[voiceID].uniVoices[i].currentAngle += voices[voiceID].uniVoices[i].angleDelta;
				}
				break;
			case 3:
				outputs[0] = 0.0;
				for (int i = 0; i < numUniVoices; i++) {
					outputs[0] += (std::sin(voices[voiceID].uniVoices[i].currentAngle) > 0.0f ? 1.0f : -1.0f) * ngp->modules[inputs[0].connectedModule]->outputs[inputs[0].connectedOutput] * uniVolumeFactor;
					voices[voiceID].uniVoices[i].currentAngle += voices[voiceID].uniVoices[i].angleDelta;
				}
				break;
			default:

				break;
			}
		}
		else
			outputs[0] = 0.0;
}