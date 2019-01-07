#ifndef MODULE
#define MODULE

#include "../JuceLibraryCode/JuceHeader.h"
#include "Socket.h"
#include "../LookAndFeel/Colors.hpp"

/*
PLANNED MODULES:
Biquad Filter
Inputs:
Signal
Outputs:
Signal
Controls:
Frequency

Sample and Hold
Inputs:
Signal
TriggerVelocity
Ouputs:
Value
Controls:
None

*/

/*
ADDING A NEW MODULE
1. Create New Module Class
2. Add module to ModuleType in Module.hpp
3. Add module to the mouseUp Callback in NGE
4. Add AddModule function to NGE
5. Add Button+fuctionality to the custom toolbar
6. Add module to the LoadPreset function in NGP
*/

enum ModuleColorScheme {
	Pink,
	Blue,
	Grey
};

struct Input {
	int connectedModule = -1;
	int connectedOutput = -1;
};

struct Control {
	int connectedModule = -1;
	int connectedOutput = -1;
};

enum ModuleType {
	Gain,
	ADSR,
	Out,
	MIDI,
	BasicGenerator,
	MtoF,
	Add,
	Subtract,
	Multiply,
	Divide,
	Value,
	FM,
	Range,
	TransposeMIDI,
	TransposeFreqency,
	WhiteNoise,
	Mix,
	SampleAndHold,
	Saturation,
	Bitcrush,
	AMRM
};

class NodeGraphEditor;
class NodeGraphProcessor;

class Module : public Component, public Button::Listener, public MouseListener {
public:
	Module();
	Module(ModuleColorScheme colorScheme, juce::String title, int numInputs, int numOutputs, int numControls, Point<int> moduleSize, int numParameters);
	~Module();
	void paint(Graphics &g) override;
	void resized() override;
	void buttonClicked(Button* button) override;
	virtual void PaintGUI(Graphics &g) = 0;
	virtual void ResizeGUI() = 0;
	void mouseUp(const MouseEvent &event);
	unsigned int id;
	juce::String title;

	std::vector<Input> inputs;
	std::vector<Socket*> inputSocketButtons;
	std::vector<double> outputs;
	std::vector<Socket*> outputSocketButtons;
	std::vector<Control> controls;
	std::vector<Socket*> controlSocketButtons;
	int numParameters;
	virtual float GetParameter(int id) = 0;
	virtual void SetParameter(int id, float value) = 0;

	NodeGraphEditor* nge;
	NodeGraphProcessor* ngp;

	ModuleColorScheme colorScheme;

	Point<int> moduleSize;

	float titleFontSize = 22.0f;

	float sampleRate = 44100.0f;
	virtual double GetResult(int midiNote, float velocity, int outputID, int voiceID) = 0;

private:
	void DrawBackgroundHighlight(Graphics &g, Rectangle<int> bounds);

protected:
	Font boldFont;
	void SetTitleFontSize(float fontSize = 22.0f);
};

#endif