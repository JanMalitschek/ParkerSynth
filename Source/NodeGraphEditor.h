#ifndef NODE_GRAPH_EDITOR
#define NODE_GRAPH_EDITOR
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
#include "NodeGraphProcessor.h"
#include "LookAndFeel\ToolbarButton.hpp"
#include "Splashscreen.h"

enum Mode {
	Idle,
	PlaceNode,
	ConnectNodes,
	MoveNode
};

class NodeGraphEditor : public Component, public MouseListener
{
public:
	NodeGraphEditor();
	~NodeGraphEditor();

	void paint(Graphics &g) override;
	void paintOverChildren(Graphics &g);
	void resized() override;

	void mouseMove(const MouseEvent &event);
	void mouseUp(const MouseEvent &event);
	void mouseDown(const MouseEvent &event);

	Mode mode;
	void BeginConnectModule(unsigned int sourceModuleID, unsigned int sourceOutputID);
	void EndConnectModule(unsigned int destModuleID, unsigned int destInputID);
	void EndConnectControl(unsigned int destModuleID, unsigned int destInputID);
	void MoveModule(int moveModuleID, Point<int> size);
	void DeleteModule(int moduleID);

	void AddGainModule();
	void AddADSRModule();
	void AddOutModule();
	void AddMIDIModule();
	void AddBasicGeneratorModule();
	void AddMtoFModule();
	void AddAddModule();
	void AddSubtractModule();
	void AddMultiplyModule();
	void AddDivideModule();
	void AddValueModule();
	void AddFMModule();

	NodeGraphProcessor* ngp;

	void RedrawGUI();

private:
	Point<int> mousePos;
	Point<int> rawMousePos;
	Point<int> lastMousePos;
	Point<int> currentModuleSize;
	ModuleType currentModuleType;

	unsigned int currentSourceModuleID;
	unsigned int currentSourceOutputID;
	int currentMoveModuleID;

	bool canPlaceNode;
	bool CheckOverlap();

	LaF_ToolbarButton laF_ToolbarButton;
	TooltipWindow tooltips;

	CustomSplashscreen splashScreen;

	void DrawBezierCurve(Graphics &g, Point<float> start, Point<float> control0, Point<float> control1, Point<float> end);
};

#endif