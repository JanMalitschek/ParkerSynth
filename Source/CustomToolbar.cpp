#include "CustomToolbar.h"
#include "LookAndFeel\Colors.hpp"

CustomToolbar::CustomToolbar() : Component(){
	boldFont = Font(Typeface::createSystemTypefaceFor(BinaryData::BAHNSCHRIFT_TTF, (size_t)BinaryData::BAHNSCHRIFT_TTFSize));
	setSize(800, 25);
	button_Preset.setButtonText("Preset");
	button_Preset.setLookAndFeel(&laF_ToolbarButton);
	button_Preset.addListener(this);
	addAndMakeVisible(button_Preset);
	button_Node.setButtonText("Node");
	button_Node.setLookAndFeel(&laF_ToolbarButton);
	button_Node.addListener(this);
	addAndMakeVisible(button_Node);
	button_Info.setButtonText("Info");
	button_Info.setLookAndFeel(&laF_ToolbarButton);
	button_Info.addListener(this);
	addAndMakeVisible(button_Info);
	button_Macro.setButtonText("Macros");
	button_Macro.setLookAndFeel(&laF_ToolbarButton);
	button_Macro.addListener(this);
	addAndMakeVisible(button_Macro);
	button_SwitchConnections.setButtonText("T.C.");
	button_SwitchConnections.setLookAndFeel(&laF_ToolbarButton);
	button_SwitchConnections.addListener(this);
	addAndMakeVisible(button_SwitchConnections);
	button_Recompile.setButtonText("Compile");
	button_Recompile.setLookAndFeel(&laF_ToolbarButton);
	button_Recompile.addListener(this);
	addAndMakeVisible(button_Recompile);
	dbg = Colour(0, 0, 0);
	presetTabActive = false;
	moduleTabActive = false;
	infoTabActive = false;
	macroTabActive = false;

	group_preset = std::vector<TextButton*>();
	button_preset_Save.setButtonText("Save");
	button_preset_Save.setLookAndFeel(&laF_ToolbarButton);
	button_preset_Save.addListener(this);
	group_preset.push_back(&button_preset_Save);
	addAndMakeVisible(button_preset_Save);
	button_preset_Load.setButtonText("Load");
	button_preset_Load.setLookAndFeel(&laF_ToolbarButton);
	button_preset_Load.addListener(this);
	group_preset.push_back(&button_preset_Load);
	addAndMakeVisible(button_preset_Load);
	button_preset_Init.setButtonText("Init");
	button_preset_Init.setLookAndFeel(&laF_ToolbarButton);
	button_preset_Init.addListener(this);
	group_preset.push_back(&button_preset_Init);
	addAndMakeVisible(button_preset_Init);

	group_module = std::vector<TextButton*>();
	button_module_Gain.setButtonText("Gain");
	button_module_Gain.setLookAndFeel(&laF_ToolbarButton);
	button_module_Gain.addListener(this);
	group_module.push_back(&button_module_Gain);
	addAndMakeVisible(button_module_Gain);
	button_module_ADSR.setButtonText("ADSR");
	button_module_ADSR.setLookAndFeel(&laF_ToolbarButton);
	button_module_ADSR.addListener(this);
	group_module.push_back(&button_module_ADSR);
	addAndMakeVisible(button_module_ADSR);
	button_module_Out.setButtonText("Out");
	button_module_Out.setLookAndFeel(&laF_ToolbarButton);
	button_module_Out.addListener(this);
	group_module.push_back(&button_module_Out);
	addAndMakeVisible(button_module_Out);
	button_module_MIDI.setButtonText("MIDI");
	button_module_MIDI.setLookAndFeel(&laF_ToolbarButton);
	button_module_MIDI.addListener(this);
	group_module.push_back(&button_module_MIDI);
	addAndMakeVisible(button_module_MIDI);
	button_module_BasicGenerator.setButtonText("Basic Generator");
	button_module_BasicGenerator.setLookAndFeel(&laF_ToolbarButton);
	button_module_BasicGenerator.addListener(this);
	group_module.push_back(&button_module_BasicGenerator);
	addAndMakeVisible(button_module_BasicGenerator);
	button_module_MtoF.setButtonText("MIDI to Freq");
	button_module_MtoF.setLookAndFeel(&laF_ToolbarButton);
	button_module_MtoF.addListener(this);
	group_module.push_back(&button_module_MtoF);
	addAndMakeVisible(button_module_MtoF);
	button_module_Add.setButtonText("Add");
	button_module_Add.setLookAndFeel(&laF_ToolbarButton);
	button_module_Add.addListener(this);
	group_module.push_back(&button_module_Add);
	addAndMakeVisible(button_module_Add);
	button_module_Subtract.setButtonText("Subtract");
	button_module_Subtract.setLookAndFeel(&laF_ToolbarButton);
	button_module_Subtract.addListener(this);
	group_module.push_back(&button_module_Subtract);
	addAndMakeVisible(button_module_Subtract);
	button_module_Multiply.setButtonText("Multiply");
	button_module_Multiply.setLookAndFeel(&laF_ToolbarButton);
	button_module_Multiply.addListener(this);
	group_module.push_back(&button_module_Multiply);
	addAndMakeVisible(button_module_Multiply);
	button_module_Divide.setButtonText("Divide");
	button_module_Divide.setLookAndFeel(&laF_ToolbarButton);
	button_module_Divide.addListener(this);
	group_module.push_back(&button_module_Divide);
	addAndMakeVisible(button_module_Divide);
	button_module_Value.setButtonText("Value");
	button_module_Value.setLookAndFeel(&laF_ToolbarButton);
	button_module_Value.addListener(this);
	group_module.push_back(&button_module_Value);
	addAndMakeVisible(button_module_Value);
	button_module_FM.setButtonText("FM");
	button_module_FM.setLookAndFeel(&laF_ToolbarButton);
	button_module_FM.addListener(this);
	group_module.push_back(&button_module_FM);
	addAndMakeVisible(button_module_FM);
	button_module_Range.setButtonText("Range");
	button_module_Range.setLookAndFeel(&laF_ToolbarButton);
	button_module_Range.addListener(this);
	group_module.push_back(&button_module_Range);
	addAndMakeVisible(button_module_Range);
	button_module_TransposeMIDI.setButtonText("TransM");
	button_module_TransposeMIDI.setLookAndFeel(&laF_ToolbarButton);
	button_module_TransposeMIDI.addListener(this);
	group_module.push_back(&button_module_TransposeMIDI);
	addAndMakeVisible(button_module_TransposeMIDI);
	button_module_TransposeFrequency.setButtonText("TransF");
	button_module_TransposeFrequency.setLookAndFeel(&laF_ToolbarButton);
	button_module_TransposeFrequency.addListener(this);
	group_module.push_back(&button_module_TransposeFrequency);
	addAndMakeVisible(button_module_TransposeFrequency);
	button_module_WhiteNoise.setButtonText("Noise");
	button_module_WhiteNoise.setLookAndFeel(&laF_ToolbarButton);
	button_module_WhiteNoise.addListener(this);
	group_module.push_back(&button_module_WhiteNoise);
	addAndMakeVisible(button_module_WhiteNoise);
	button_module_Mix.setButtonText("Mix");
	button_module_Mix.setLookAndFeel(&laF_ToolbarButton);
	button_module_Mix.addListener(this);
	group_module.push_back(&button_module_Mix);
	addAndMakeVisible(button_module_Mix);
	button_module_SampleAndHold.setButtonText("SaH");
	button_module_SampleAndHold.setLookAndFeel(&laF_ToolbarButton);
	button_module_SampleAndHold.addListener(this);
	group_module.push_back(&button_module_SampleAndHold);
	addAndMakeVisible(button_module_SampleAndHold);
	button_module_Saturation.setButtonText("Saturation");
	button_module_Saturation.setLookAndFeel(&laF_ToolbarButton);
	button_module_Saturation.addListener(this);
	group_module.push_back(&button_module_Saturation);
	addAndMakeVisible(button_module_Saturation);
	button_module_Bitcrush.setButtonText("Bitcrush");
	button_module_Bitcrush.setLookAndFeel(&laF_ToolbarButton);
	button_module_Bitcrush.addListener(this);
	group_module.push_back(&button_module_Bitcrush);
	addAndMakeVisible(button_module_Bitcrush);
	button_module_AMRM.setButtonText("AM/RM");
	button_module_AMRM.setLookAndFeel(&laF_ToolbarButton);
	button_module_AMRM.addListener(this);
	group_module.push_back(&button_module_AMRM);
	addAndMakeVisible(button_module_AMRM);
	button_module_Filter.setButtonText("Filter");
	button_module_Filter.setLookAndFeel(&laF_ToolbarButton);
	button_module_Filter.addListener(this);
	group_module.push_back(&button_module_Filter);
	addAndMakeVisible(button_module_Filter);

	nge = nullptr;
	canDrawMacros = false;
}

CustomToolbar::~CustomToolbar(){
	/*for (auto it : group_preset)
		delete it;*/
	group_preset.clear();
	/*for (auto it : group_module)
		delete it;*/
	group_module.clear();
}

void CustomToolbar::paint(Graphics &g) {
	g.setColour(TOOLBAR_BG);
	g.fillRect(0, 0, 800, 25);
	if (presetTabActive || moduleTabActive || infoTabActive || macroTabActive) {
		g.setColour(Colour(27, 27, 27));
		g.fillRect(0, 25, 800, 575);
	}
}

void CustomToolbar::resized() {
	button_Preset.setBounds(0, 0, 100, 25);
	button_Node.setBounds(100, 0, 100, 25);
	button_Macro.setBounds(200, 0, 100, 25);
	button_Info.setBounds(600, 0, 100, 25);
	button_SwitchConnections.setBounds(700, 0, 100, 25);
	button_Recompile.setBounds(500, 0, 100, 25);

	button_preset_Save.setBounds(0, 25, 100, 25);

	for (int i = 0; i < group_preset.size(); i++) {
		group_preset[i]->setBounds(0, i * 25 + 25, 100, 25);
	}

	for (int i = 0; i < group_module.size(); i++) {
		group_module[i]->setBounds(0, i * 25 + 25, 100, 25);
	}

	if (canDrawMacros == true) {
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 2; x++) {
				nge->ngp->macros[x * 10 + y]->setBounds(x * 400, 25 + y * 50, 400, 50);
			}
		}
	}
}

void CustomToolbar::buttonClicked(Button* button) {
	if (button == &button_Node) {
		presetTabActive = false;
		moduleTabActive = !moduleTabActive;
		macroTabActive = false;
		for (int i = 0; i < group_preset.size(); i++) {
			group_preset[i]->setVisible(false);
		}
		for (int i = 0; i < group_module.size(); i++) {
			group_module[i]->setVisible(true);
		}
		for (int i = 0; i < 20; i++) {
			nge->ngp->macros[i]->setVisible(false);
		}
		if (moduleTabActive) {
			this->setBounds(0, 0, 800, 600);
			repaint(0, 25, 800, 575);
		}
		else {
			this->setBounds(0, 0, 800, 25);
			repaint(0, 25, 800, 575);
		}
	}
	else if (button == &button_Preset) {
		presetTabActive = !presetTabActive;
		moduleTabActive = false;
		macroTabActive = false;
		for (int i = 0; i < group_preset.size(); i++) {
			group_preset[i]->setVisible(true);
		}
		for (int i = 0; i < group_module.size(); i++) {
			group_module[i]->setVisible(false);
		}
		for (int i = 0; i < 20; i++) {
			nge->ngp->macros[i]->setVisible(false);
		}
		if (presetTabActive) {
			this->setBounds(0, 0, 800, 600);
			repaint(0, 25, 800, 575);
		}
		else {
			this->setBounds(0, 0, 800, 25);
			repaint(0, 25, 800, 575);
		}
	}
	else if (button == &button_Macro) {
		presetTabActive = false;
		moduleTabActive = false;
		macroTabActive = !macroTabActive;
		for (int i = 0; i < group_preset.size(); i++) {
			group_preset[i]->setVisible(false);
		}
		for (int i = 0; i < group_module.size(); i++) {
			group_module[i]->setVisible(false);
		}
		for (int i = 0; i < 20; i++) {
			nge->ngp->macros[i]->setVisible(true);
		}
		if (macroTabActive) {
			this->setBounds(0, 0, 800, 600);
			repaint(0, 25, 800, 575);
		}
		else {
			this->setBounds(0, 0, 800, 25);
			repaint(0, 25, 800, 575);
		}
	}
	else if (button == &button_preset_Save) {
		presetTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->ngp->SavePreset();
	}
	else if (button == &button_preset_Load) {
		presetTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->ngp->LoadPreset();
	}
	else if (button == &button_preset_Init) {
		presetTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->ngp->InitPreset();
	}
	else if (button == &button_Info) {
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::NoIcon, "Save Data Size", nge->ngp->saveData[75]);
	}
	else if (button == &button_SwitchConnections) {
		nge->SwitchConnections();
	}
	else if (button == &button_Recompile) {
		nge->ngp->RecompileNodeTree();
	}
	else if (button == &button_module_Gain) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddGainModule();
	}
	else if (button == &button_module_ADSR) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddADSRModule();
	}
	else if (button == &button_module_Out) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddOutModule();
	}
	else if (button == &button_module_MIDI) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddMIDIModule();
	}
	else if (button == &button_module_BasicGenerator) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddBasicGeneratorModule();
	}
	else if (button == &button_module_MtoF) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddMtoFModule();
	}
	else if (button == &button_module_Add) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddAddModule();
	}
	else if (button == &button_module_Subtract) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddSubtractModule();
	}
	else if (button == &button_module_Multiply) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddMultiplyModule();
	}
	else if (button == &button_module_Divide) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddDivideModule();
	}
	else if (button == &button_module_Value) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddValueModule();
	}
	else if (button == &button_module_FM) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddFMModule();
	}
	else if (button == &button_module_Range) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddRangeModule();
	}
	else if (button == &button_module_TransposeMIDI) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddTransposeMIDIModule();
	}
	else if (button == &button_module_TransposeFrequency) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddTransposeFrequencyModule();
	}
	else if (button == &button_module_WhiteNoise) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddWhiteNoiseModule();
	}
	else if (button == &button_module_Mix) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddMixModule();
	}
	else if (button == &button_module_SampleAndHold) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddSampleAndHoldModule();
	}
	else if (button == &button_module_Saturation) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddSaturationModule();
	}
	else if (button == &button_module_Bitcrush) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddBitcrushModule();
	}
	else if (button == &button_module_AMRM) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddAMRMModule();
	}
	else if (button == &button_module_Filter) {
		moduleTabActive = false;
		this->setBounds(0, 0, 800, 25);
		repaint(0, 25, 800, 575);
		nge->AddFilterModule();
	}
}

void CustomToolbar::DrawMacros() {
	for (int i = 0; i < nge->ngp->macros.size(); i++) {
		addAndMakeVisible(nge->ngp->macros[i]);
		nge->ngp->macros[i]->setVisible(false);
		nge->ngp->macros[i]->ngp = nge->ngp;
	}
	canDrawMacros = true;
}