#include "Socket.h"

Socket::Socket(bool isControl) : Component()
{
	this->valueType = ValueType::Signal;
	//button.setImages(false, false, true, socket, 1.0f, Colour((uint8)255, (uint8)255, (uint8)255, (uint8)0), socket, 1.0f, Colour((uint8)200, (uint8)200, (uint8)200, (uint8)32), socket, 1.0f, Colour((uint8)150, (uint8)150, (uint8)150, (uint8)32));
	this->isControl = isControl;
	button.setTooltip("Socket");
	button.setButtonText(" ");
	this->socketColor = MAIN_COLOR;
	button.setColour(TextButton::ColourIds::buttonColourId, this->socketColor);
	if (isControl) {
		button.setLookAndFeel(&laF_ControlSocket);
	}
	else {
		button.setLookAndFeel(&laF_IOSocket);
	}
	addAndMakeVisible(button);
	setSize(25, 25);
}

Socket::~Socket()
{
	removeAllChildren();
}

void Socket::paint(Graphics &g) {

}

void Socket::resized() {
	button.setBounds(0, 0, 25, 25);
}

void Socket::SetValueType(ValueType type) {
	switch (type) {
	case ValueType::Signal:
		this->socketColor = MAIN_COLOR;
		break;
	case ValueType::Velocity:
		this->socketColor = VALUE_VELOCITY;
		break;
	case ValueType::Frequency:
		this->socketColor = VALUE_FREQUENCY;
		break;
	case ValueType::MIDINoteNumber:
		this->socketColor = VALUE_MIDI;
		break;
	case ValueType::SimpleValue:
		this->socketColor = VALUE_VALUE;
		break;
	default:
		this->socketColor = MAIN_COLOR;
		break;
	}
	this->valueType = type;
	button.setColour(TextButton::ColourIds::buttonColourId, this->socketColor);
}