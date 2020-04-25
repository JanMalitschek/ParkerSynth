#ifndef NODE_GRAPH_PROCESSOR
#define NODE_GRAPH_PROCESSOR

#include "Modules\Module.hpp"
#include "Macro.h"
#include <string>

#define NUM_VOICES 9

class NodeGraphEditor;
class ParkerSynthAudioProcessor;
class NodeGraphProcessor
{
public:
	NodeGraphProcessor();
	~NodeGraphProcessor();

	int currentID;
	int outputModuleID;
	std::vector<Module*> modules;
	bool hasOutModule;

	std::vector<Macro*> macros;

	void SavePreset(bool toPresetFile = true);
	void LoadPreset(bool fromPresetFile = true);
	void InitPreset();

	float sampleRate = 44100.0f;

	NodeGraphEditor *nge;

	//Macro Linking
	int lastTweakedModule;
	String lastTweakedModuleTitle;
	int lastTweakedParameter;
	float lastTweakedParameterMin;
	float lastTweakedParameterMax;
	float lastTweakedParameterValue;
	float lastTweakedParameterInc;

	//Recursive evaluation
	double GetResult(int midiNote, float velocity, int voiceID);
	//Iterative evaluation
	void RecompileNodeTree();
	double GetResultIteratively(int midiNote, float velocity, int voiceID);
	std::vector<std::pair<unsigned int, unsigned int>> branches;
	std::vector<unsigned int> history;

	void EnableEvaluation();

	volatile bool canProcess;

	ParkerSynthAudioProcessor* processor;

	StringArray saveData;
	int saveDataSize;

	std::vector<Module*> evaluationList;
};

#endif