#include "Module.hpp"
#include "../LookAndFeel/Colors.hpp"
#include "..\NodeGraphEditor.h"
#include "..\NodeGraphProcessor.h"

Module::Module() : Component() {
	this->colorScheme = ModuleColorScheme::Pink;
	boldFont = Font(Typeface::createSystemTypefaceFor(BinaryData::BAHNSCHRIFT_TTF, (size_t)BinaryData::BAHNSCHRIFT_TTFSize));
	this->title = "Module";
	inputs = std::vector<Input>(0);
	outputs = std::vector<double>(0);
	id = 0;
	moduleSize = Point<int>(4, 4);
	numParameters = 0;
}

Module::Module(ModuleColorScheme colorScheme, juce::String title, int numInputs, int numOutputs, int numControls, Point<int> moduleSize, int numParameters) : Component() {
	boldFont = Font(Typeface::createSystemTypefaceFor(BinaryData::BAHNSCHRIFT_TTF, (size_t)BinaryData::BAHNSCHRIFT_TTFSize));
	this->title = title;
	inputs = std::vector<Input>(numInputs);
	inputSocketButtons = std::vector<Socket*>();
	outputs = std::vector<double>(numOutputs);
	outputSocketButtons = std::vector<Socket*>();
	controls = std::vector<Control>(numControls);
	controlSocketButtons = std::vector<Socket*>();
	for (int i = 0; i < numInputs; i++) {
		inputSocketButtons.push_back(new Socket(false));
		addAndMakeVisible(inputSocketButtons[i]);
		float divisionHeight = (getLocalBounds().getHeight() - 50) / (numInputs);
		inputSocketButtons[i]->setBounds(0, (divisionHeight * i) + (divisionHeight / 2) + 13, 25, 25);
		inputSocketButtons[i]->button.addListener(this);
	}
	for (int i = 0; i < numOutputs; i++) {
		outputSocketButtons.push_back(new Socket(false));
		addAndMakeVisible(outputSocketButtons[i]);
		float divisionHeight = (getLocalBounds().getHeight() - 50) / (numOutputs);
		outputSocketButtons[i]->setBounds(getLocalBounds().getWidth() - 25, (divisionHeight * i) + (divisionHeight / 2) + 13, 25, 25);
		outputSocketButtons[i]->button.addListener(this);
	}
	for (int i = 0; i < numControls; i++) {
		controlSocketButtons.push_back(new Socket(true));
		addAndMakeVisible(controlSocketButtons[i]);
		float divisionHeight = (getLocalBounds().getWidth() - 50) / (numControls);
		controlSocketButtons[i]->setBounds((divisionHeight * i) + (divisionHeight / 2) + 13, getLocalBounds().getHeight() - 25, 25, 25);
		controlSocketButtons[i]->button.addListener(this);
	}
	id = 0;
	this->moduleSize = moduleSize;
	this->numParameters = numParameters;
}

Module::~Module() {

}

void Module::paint(Graphics &g) {
	Rectangle<int> bounds = getLocalBounds();
	
	//switch (this->colorScheme) {
	//case ModuleColorScheme::Pink:
	//	g.setColour(BG_PINK);
	//	g.fillRect(bounds);
	//	
	//	g.setColour(BG_PINK_LIGHT);
	//	DrawBackgroundHighlight(g, bounds);
	//	break;
	//case ModuleColorScheme::Blue:
	//	g.setColour(BG_BLUE);
	//	g.fillRect(bounds);

	//	g.setColour(BG_BLUE_LIGHT);
	//	DrawBackgroundHighlight(g, bounds);
	//	break;
	//case ModuleColorScheme::Grey:
	//	g.setColour(BG_GREY);
	//	g.fillRect(bounds);

	//	g.setColour(BG_GREY_LIGHT);
	//	DrawBackgroundHighlight(g, bounds);
	//	break;
	//default:
	//	break;
	//}
	//g.setColour(MODULE_SHADING);
	//g.drawRect(bounds, 2.0f);

	//g.setColour(Colour((uint8)255, (uint8)255, (uint8)255, (uint8)255));
	//g.drawImage(screw, 0, 0, 25, 25, 0, 0, 25, 25);
	//g.drawImage(screw, bounds.getRight() - 25, 0, 25, 25, 0, 0, 25, 25);
	//g.drawImage(screw, bounds.getRight() - 25, bounds.getBottom() - 25, 25, 25, 0, 0, 25, 25);
	//g.drawImage(screw, 0, bounds.getBottom() - 25, 25, 25, 0, 0, 25, 25);

	g.setColour(MODULE_BG);
	g.fillRoundedRectangle(12, 12, bounds.getWidth() - 25, bounds.getHeight() - 25, 10);
	Path corners;
	corners.addArc(12, 12, 15, 15, -1.5708f, 0.0f, true);
	corners.addArc(bounds.getWidth() - 28, 12, 15, 15, 0.0f, 1.5708f, true);
	corners.addArc(12, bounds.getHeight() - 28, 15, 15, -3.14159f, -1.5708f, true);
	corners.addArc(bounds.getWidth() - 28, bounds.getHeight() - 28, 15, 15, 1.5708f, 3.14159f, true);
	g.setColour(MODULE_RAILING);
	g.strokePath(corners, PathStrokeType(5.0f, PathStrokeType::JointStyle::beveled, PathStrokeType::EndCapStyle::rounded));

	g.setFont(boldFont);
	g.setFont(22.0f);
	g.drawText(this->title, 25, 0, bounds.getWidth() - 50, 25, Justification::centred, false);

	PaintGUI(g);
}

void Module::resized() {
	ResizeGUI();
	//Input Sockets
	for (int i = 0; i < inputSocketButtons.size(); i++) {
		float divisionHeight = (getLocalBounds().getHeight() - 50) / (inputSocketButtons.size());
		inputSocketButtons[i]->setBounds(0, (divisionHeight * i) + (divisionHeight / 2) + 13, 25, 25);
	}
	for (int i = 0; i < outputSocketButtons.size(); i++) {
		float divisionHeight = (getLocalBounds().getHeight() - 50) / (outputSocketButtons.size());
		outputSocketButtons[i]->setBounds(getLocalBounds().getWidth() - 25, (divisionHeight * i) + (divisionHeight / 2) + 13, 25, 25);
	}
	for (int i = 0; i < controlSocketButtons.size(); i++) {
		float divisionHeight = (getLocalBounds().getWidth() - 50) / (controlSocketButtons.size());
		controlSocketButtons[i]->setBounds((divisionHeight * i) + (divisionHeight / 2) + 13, getLocalBounds().getHeight() - 25, 25, 25);
	}
}

void Module::buttonClicked(Button* button) {
	for (int i = 0; i < outputSocketButtons.size(); i++) {
		if (button == &outputSocketButtons[i]->button) {
			nge->BeginConnectModule(this->id, i);
		}
	}
	for (int i = 0; i < inputSocketButtons.size(); i++) {
		if (button == &inputSocketButtons[i]->button) {
			nge->EndConnectModule(this->id, i);
		}
	}
	for (int i = 0; i < controlSocketButtons.size(); i++) {
		if (button == &controlSocketButtons[i]->button) {
			nge->EndConnectControl(this->id, i);
		}
	}
}

void Module::mouseUp(const MouseEvent &event) {
	if (event.mods.isRightButtonDown()) {
		nge->MoveModule(this->id, this->moduleSize);
	}
	else if (event.mods.isMiddleButtonDown()) {
		nge->DeleteModule(this->id);
	}
}

void Module::DrawBackgroundHighlight(Graphics &g, Rectangle<int> bounds) {
	Path highlight = Path();
	highlight.startNewSubPath(0, 0);
	highlight.lineTo(bounds.getRight(), 0);
	highlight.lineTo(bounds.getRight(), bounds.getBottom() * 0.3f);
	highlight.lineTo(0, bounds.getBottom() * 0.6f);
	highlight.lineTo(0, 0);
	g.fillPath(highlight);
}