#ifndef MODULE
#define MODULE

#include "../JuceLibraryCode/JuceHeader.h"
#include "Socket.h"
#include "../LookAndFeel/Colors.hpp"

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
	FM
};

class NodeGraphEditor;
class NodeGraphProcessor;

class Module : public Component, public ButtonListener, public MouseListener {
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

	virtual double GetResult(int midiNote, float velocity, int outputID, int voiceID) = 0;

private:
	void DrawBackgroundHighlight(Graphics &g, Rectangle<int> bounds);

protected:
	Font boldFont;
};

#endif