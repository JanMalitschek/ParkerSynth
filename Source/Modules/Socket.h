#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../LookAndFeel/LaF_New.hpp"

enum ValueType {
	Signal,
	Velocity,
	Frequency,
	MIDINoteNumber,
	SimpleValue
};

class Socket : public Component, public SettableTooltipClient
{
public:
	Socket(bool isControl);
	~Socket();

	void paint(Graphics &g) override;
	void resized() override;
	TextButton button;
	LaF_IOSocket laF_IOSocket;
	LaF_ControlSocket laF_ControlSocket;

	bool isControl;

	void SetValueType(ValueType type);

	Colour socketColor;
private:
	ValueType valueType;
};

