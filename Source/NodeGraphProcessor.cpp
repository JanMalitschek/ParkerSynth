#include "NodeGraphProcessor.h"
#include "Modules\GainModule.hpp"
#include "Modules\ADSRModule.hpp"
#include "Modules\OutModule.hpp"
#include "Modules\MIDIModule.hpp"
#include "Modules\BasicGeneratorModule.hpp"
#include "Modules\MtoFModule.hpp"
#include "Modules\AddModule.hpp"
#include "Modules\SubtractModule.hpp"
#include "Modules\MultiplyModule.hpp"
#include "Modules\DivideModule.hpp"
#include "Modules\ValueModule.hpp"
#include "Modules\FMModule.hpp"

#include "NodeGraphEditor.h"

NodeGraphProcessor::NodeGraphProcessor()
{
	modules = std::vector<Module*>();
	currentID = 0;
	outputModuleID = -1;
	hasOutModule = false;
	canProcess = true;

	branches = std::vector<std::pair<unsigned int, unsigned int>>();
	history = std::vector<unsigned int>();

	evaluationList = std::vector<Module*>();

	macros = std::vector<Macro*>();

	lastTweakedModule = -1;
	lastTweakedParameter = -1;
	lastTweakedModuleTitle = "[Empty]";
	lastTweakedParameterMin = 0.0f;
	lastTweakedParameterMax = 1.0f;
	lastTweakedParameterInc = 0.01f;
	lastTweakedParameterValue = 0.0;

	sampleRate = 44100.0f;

	saveData = StringArray();
	saveDataSize = 0;

	nge = nullptr;
}

NodeGraphProcessor::~NodeGraphProcessor()
{
	for (int i = 0; i < modules.size(); i++) {
		delete modules[i];
	}
	for (int i = 0; i < macros.size(); i++) {
		delete macros[i];
	}
}

void NodeGraphProcessor::SavePreset(bool toPresetFile) {
	File file;
	if (toPresetFile) {
		FileChooser dlg("Save Preset", File::nonexistent, "*.pspr");
		if (dlg.browseForFileToSave(true)) {
			file = dlg.getResult();
		}
		else
			return;
	}
	else {
		file.create();
	}
	saveData.clear();
		file.replaceWithText("[" + file.getFileNameWithoutExtension() + "]\n");
		saveData.add("[" + file.getFileNameWithoutExtension() + "]");
		file.appendText(std::to_string(modules.size()) + "\n");
		saveData.add(std::to_string(modules.size()));
		for (int i = 0; i < modules.size(); i++) {
			file.appendText("[" + modules[i]->title + "]{\n");
			saveData.add("[" + modules[i]->title + "]{");
			file.appendText("ID:\n");
			saveData.add("ID:");
			file.appendText(std::to_string(modules[i]->id) + "\n");
			saveData.add(std::to_string(modules[i]->id));
			file.appendText("Position:\n");
			saveData.add("Position:");
			file.appendText(std::to_string(modules[i]->getPosition().x) + "\n");
			saveData.add(std::to_string(modules[i]->getPosition().x));
			file.appendText(std::to_string(modules[i]->getPosition().y) + "\n");
			saveData.add(std::to_string(modules[i]->getPosition().y));
			file.appendText("Num Outputs:\n");
			saveData.add("Num Outputs:");
			file.appendText(std::to_string(modules[i]->outputs.size()) + "\n");
			saveData.add(std::to_string(modules[i]->outputs.size()));
			file.appendText("Connected Outputs:\n");
			saveData.add("Connected Outputs:");
			file.appendText(std::to_string(modules[i]->inputs.size()) + "\n");
			saveData.add(std::to_string(modules[i]->inputs.size()));
			for (int n = 0; n < modules[i]->inputs.size(); n++) {
				file.appendText(std::to_string(modules[i]->inputs[n].connectedModule) + "\n");
				saveData.add(std::to_string(modules[i]->inputs[n].connectedModule));
				file.appendText(std::to_string(modules[i]->inputs[n].connectedOutput) + "\n");
				saveData.add(std::to_string(modules[i]->inputs[n].connectedOutput));
			}
			file.appendText("Connected Outputs to Controls:\n");
			saveData.add("Connected Outputs to Controls:");
			file.appendText(std::to_string(modules[i]->controls.size()) + "\n");
			saveData.add(std::to_string(modules[i]->controls.size()));
			for (int n = 0; n < modules[i]->controls.size(); n++) {
				file.appendText(std::to_string(modules[i]->controls[n].connectedModule) + "\n");
				saveData.add(std::to_string(modules[i]->controls[n].connectedModule));
				file.appendText(std::to_string(modules[i]->controls[n].connectedOutput) + "\n");
				saveData.add(std::to_string(modules[i]->controls[n].connectedOutput));
			}
			file.appendText("Parameters:\n");
			saveData.add("Parameters:");
			file.appendText(std::to_string(modules[i]->numParameters) + "\n");
			saveData.add(std::to_string(modules[i]->numParameters));
			for (int n = 0; n < modules[i]->numParameters; n++) {
				file.appendText(std::to_string(modules[i]->GetParameter(n)) + "\n");
				saveData.add(std::to_string(modules[i]->GetParameter(n)));
			}
			file.appendText("}\n");
			saveData.add("}");
		}
		file.appendText("Macros{\n");
		saveData.add("Macros{");
		int numUsedMacros = 0;
		for (int i = 0; i < macros.size(); i++) {
			if (macros[i]->moduleID >= 0) {
				numUsedMacros++;
			}
		}
		file.appendText(std::to_string(numUsedMacros) + "\n");
		saveData.add(std::to_string(numUsedMacros));
		for (int i = 0; i < numUsedMacros; i++) {
			if (macros[i]->moduleID >= 0) {
				file.appendText(std::to_string(i) + "\n");
				saveData.add(std::to_string(i));
				file.appendText(std::to_string(macros[i]->moduleID) + "\n");
				saveData.add(std::to_string(macros[i]->moduleID));
				file.appendText(std::to_string(macros[i]->parameterID) + "\n");
				saveData.add(std::to_string(macros[i]->parameterID));
				file.appendText(std::to_string(macros[i]->valueKnob.getValue()) + "\n");
				saveData.add(std::to_string(macros[i]->valueKnob.getValue()));
				file.appendText(std::to_string(macros[i]->mappingOffset) + "\n");
				saveData.add(std::to_string(macros[i]->mappingOffset));
				file.appendText(std::to_string(macros[i]->mappingFactor) + "\n");
				saveData.add(std::to_string(macros[i]->mappingFactor));
				file.appendText(macros[i]->macroTitle.getText() + "\n");
				saveData.add(macros[i]->macroTitle.getText());
			}
		}
		file.appendText("}\n");
		saveData.add("}");
		//saveDataSize = saveData.size();
}

void NodeGraphProcessor::LoadPreset(bool fromPresetFile) {
	StringArray lines = StringArray();
	if (fromPresetFile) {
		FileChooser dlg("Load Preset", File::nonexistent, "*.pspr");
		if (dlg.browseForFileToOpen()) {
			auto file = dlg.getResult();
			file.readLines(lines);
		}
		else
			return;
	}
	else {
		lines = StringArray(saveData);
		if (lines.size() <= 0)
			return;
	}
	canProcess = false;
	currentID = 0;
	outputModuleID = -1;
	hasOutModule = false;
	for (int i = 0; i < modules.size(); i++) {
		delete modules[i];
		modules[i] = nullptr;
	}
	modules.clear();
	int numModules = std::stoi(lines[1].toStdString());
	int currentLine = 2;
	for (int i = 0; i < numModules; i++) {
		String moduleType = lines[currentLine];
		//Add Module
		if (moduleType == "[Add]{") {
			modules.push_back(new AddModule());
		}
		else if (moduleType == "[ADSR]{") {
			modules.push_back(new ADSRModule());
		}
		else if (moduleType == "[Generator]{") {
			modules.push_back(new BasicGeneratorModule());
		}
		else if (moduleType == "[Div]{") {
			modules.push_back(new DivideModule());
		}
		else if (moduleType == "[FM]{") {
			modules.push_back(new FMModule());
		}
		else if (moduleType == "[Gain]{") {
			modules.push_back(new GainModule());
		}
		else if (moduleType == "[MIDI]{") {
			modules.push_back(new MIDIModule());
		}
		else if (moduleType == "[MtoF]{") {
			modules.push_back(new MtoFModule());
		}
		else if (moduleType == "[Mul]{") {
			modules.push_back(new MultiplyModule());
		}
		else if (moduleType == "[Out]{") {
			modules.push_back(new OutModule());
			this->outputModuleID = currentID;
			this->hasOutModule = true;
		}
		else if (moduleType == "[Sub]{") {
			modules.push_back(new SubtractModule());
		}
		else if (moduleType == "[Value]{") {
			modules.push_back(new ValueModule());
		}
		else if (moduleType == "[Range]{") {
			modules.push_back(new RangeModule());
		}
		else if (moduleType == "[TransM]{") {
			modules.push_back(new TransposeMIDIModule());
		}
		else if (moduleType == "[TransF]{") {
			modules.push_back(new TransposeFrequencyModule());
		}
		else if (moduleType == "[Noise]{") {
			modules.push_back(new WhiteNoiseModule());
		}
		else if (moduleType == "[Mix]{") {
			modules.push_back(new MixModule());
		}
		else if (moduleType == "[SaH]{") {
			modules.push_back(new SampleAndHoldModule());
		}
		else if (moduleType == "[Saturate]{") {
			modules.push_back(new SaturationModule());
		}
		else if (moduleType == "[Bitcrush]{") {
			modules.push_back(new BitcrushModule());
		}
		else if (moduleType == "[AM RM]{") {
			modules.push_back(new AMRMModule());
		}
		else if (moduleType == "[Filter]{") {
			modules.push_back(new FilterModule());
		}
		//Set Size
		modules[currentID]->id = currentID;
		modules[currentID]->setBounds(std::stoi(lines[currentLine + 4].toStdString()), std::stoi(lines[currentLine + 5].toStdString()), modules[currentID]->moduleSize.x * 25, modules[currentID]->moduleSize.y * 25);
		if (this->nge != nullptr) {
			modules[currentID]->nge = this->nge;
		}
		modules[currentID]->ngp = this;

		int numOutputs = std::stoi(lines[currentLine + 7].toStdString());
		for (int n = 0; n < numOutputs; n++) {
			modules[currentID]->outputs.push_back(0.0f);
		}

		//Set Connected Outputs
		int numInputs = std::stoi(lines[currentLine + 9].toStdString());
		int outputsOffset = numInputs * 2;
		for (int n = 0; n < numInputs; n++) {
			//modules[currentID]->inputs.push_back(Input());
			modules[currentID]->inputs[n].connectedModule = std::stoi(lines[currentLine + 10 + n * 2].toStdString());
			modules[currentID]->inputs[n].connectedOutput = std::stoi(lines[currentLine + 11 + n * 2].toStdString());
		}

		//Set Connected Outputs
		int numControlInputs = std::stoi(lines[currentLine + outputsOffset + 11].toStdString());
		int outputsControlOffset = numControlInputs * 2;
		for (int n = 0; n < numControlInputs; n++) {
			//modules[currentID]->controls.push_back(Control());
			modules[currentID]->controls[n].connectedModule = std::stoi(lines[currentLine + outputsOffset + 12 + n * 2].toStdString());
			modules[currentID]->controls[n].connectedOutput = std::stoi(lines[currentLine + outputsOffset + 13 + n * 2].toStdString());
		}

		int numParameters = std::stoi(lines[currentLine + outputsOffset + outputsControlOffset + 13].toStdString());
		int parametersOffset = numParameters;
		for (int n = 0; n < numParameters; n++) {
			modules[i]->SetParameter(n, std::stof(lines[currentLine + outputsOffset + outputsControlOffset + 14 + n].toStdString()));
		}

		currentLine += 15 + outputsOffset + outputsControlOffset + parametersOffset;
		currentID++;
	}
	currentLine += 1;
	for (int i = 0; i < macros.size(); i++) {
		macros[i]->Reset();
	}
	int numMacros = std::stoi(lines[currentLine].toStdString());
	currentLine += 1;
	for (int i = 0; i < numMacros; i++) {
		int currentMacro = std::stoi(lines[currentLine + i * 6].toStdString());
		macros[currentMacro]->valueKnob.setEnabled(true);
		macros[currentMacro]->moduleID = std::stoi(lines[currentLine + i * 6 + 1].toStdString());
		macros[currentMacro]->parameterID = std::stoi(lines[currentLine + i * 6 + 2].toStdString());
		macros[currentMacro]->valueKnob.setValue(std::stof(lines[currentLine + i * 6 + 3].toStdString()));
		macros[currentMacro]->mappingOffset = std::stof(lines[currentLine + i * 6 + 4].toStdString());
		macros[currentMacro]->mappingFactor = std::stof(lines[currentLine + i * 6 + 5].toStdString());
		macros[currentMacro]->macroTitle.setText(lines[currentLine + i * 6 + 6]);
	}

	canProcess = true;
	if (this->nge != nullptr) {
		this->nge->RedrawGUI();
		this->nge->CalculatePatchBounds();
	}
	RecompileNodeTree();
}

void NodeGraphProcessor::InitPreset() {
	canProcess = false;
	currentID = 0;
	outputModuleID = -1;
	hasOutModule = false;
	for (int i = 0; i < modules.size(); i++) {
		delete modules[i];
		modules[i] = nullptr;
	}
	modules.clear();
	for (int i = 0; i < macros.size(); i++) {
		macros[i]->Reset();
	}
	canProcess = true;
}

double NodeGraphProcessor::GetResult(int midiNote, float velocity, int voiceID) {
	if (canProcess && outputModuleID >= 0) {
		for (auto it : this->modules) {
			it->canBeEvaluated = true;
		}
		return modules[outputModuleID]->GetResult(midiNote, velocity, 0, voiceID);
	}
	else
		return 0.0;
}

double NodeGraphProcessor::GetResultIteratively(int midiNote, float velocity, int voiceID) {
	if (!canProcess || outputModuleID < 0)
		return 0.0;
	for (int i = 0; i < evaluationList.size(); i++) {
		evaluationList[i]->GetResultIteratively(midiNote, velocity, voiceID);
	}
	return modules[modules[outputModuleID]->inputs[0].connectedModule]->outputs[modules[outputModuleID]->inputs[0].connectedOutput];
}

void NodeGraphProcessor::RecompileNodeTree() {
	canProcess = false;
	evaluationList.clear();
	if (outputModuleID < 0) {
		canProcess = true;
		return;
	}
	//Create Lookups
	bool* flattenedLookup = (bool*)malloc(modules.size() * sizeof(bool));
	for (int m = 0; m < modules.size(); m++)
		flattenedLookup[m] = false;
	int numFlattenedModules = 0;

	while (numFlattenedModules < modules.size()) {
		for (int m = 0; m < modules.size(); m++) {
			if (modules[m]->inputs.size() == 0) {
				evaluationList.push_back(modules[m]);
				flattenedLookup[modules[m]->id] = true;
				numFlattenedModules++;
			}
			else {
				bool isRoot = true;
				for (auto it : modules[m]->inputs)
					if (it.connectedModule >= 0)
						if (!flattenedLookup[modules[it.connectedModule]->id]) {
							isRoot = false;
							break;
						}
				if (isRoot) {
					evaluationList.push_back(modules[m]);
					flattenedLookup[modules[m]->id] = true;
					numFlattenedModules++;
				}
			}
		}
	}

	free(flattenedLookup);
	canProcess = true;
}

void NodeGraphProcessor::EnableEvaluation() {
	for (auto it : this->modules) {
		it->canBeEvaluated = true;
		//it->branchID = -1;
	}
}