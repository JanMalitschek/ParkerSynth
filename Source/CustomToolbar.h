#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "LookAndFeel\ToolbarButton.hpp"
#include "NodeGraphEditor.h"

class CustomToolbar : public Component, public Button::Listener
{
public:
	CustomToolbar();
	~CustomToolbar();
	void paint(Graphics &g) override;
	void resized() override;
	void buttonClicked(Button* button) override;

	NodeGraphEditor* nge;
	bool canDrawMacros;

	void DrawMacros();
	
private:
	Font boldFont;
	TextButton button_Preset;
	bool presetTabActive;
	TextButton button_Node;
	bool moduleTabActive;
	TextButton button_Info;
	bool infoTabActive;
	TextButton button_Macro;
	bool macroTabActive;
	TextButton button_SwitchConnections;
	LaF_ToolbarButton laF_ToolbarButton;
	Colour dbg;
	
	//Preset Buttons
	std::vector<TextButton*> group_preset;
	TextButton button_preset_Save;
	TextButton button_preset_Load;
	TextButton button_preset_Init;


	//Module Buttons
	std::vector<TextButton*> group_module;
	TextButton button_module_Gain;
	TextButton button_module_ADSR;
	TextButton button_module_Out;
	TextButton button_module_MIDI;
	TextButton button_module_BasicGenerator;
	TextButton button_module_MtoF;
	TextButton button_module_Add;
	TextButton button_module_Subtract;
	TextButton button_module_Multiply;
	TextButton button_module_Divide;
	TextButton button_module_Value;
	TextButton button_module_FM;
	TextButton button_module_Range;
	TextButton button_module_TransposeMIDI;
	TextButton button_module_TransposeFrequency;
	TextButton button_module_WhiteNoise;
	TextButton button_module_Mix;
	TextButton button_module_SampleAndHold;
	TextButton button_module_Saturation;
	TextButton button_module_Bitcrush;
	TextButton button_module_AMRM;
	TextButton button_module_Filter;
};

