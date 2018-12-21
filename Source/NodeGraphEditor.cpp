#include "NodeGraphEditor.h"

NodeGraphEditor::NodeGraphEditor() : Component()
{
	mode = Mode::Idle;
	mousePos = Point<int>(0, 0);
	rawMousePos = Point<int>(0, 0);
	lastMousePos = Point<int>(0, 0);
	currentModuleSize = Point<int>(4, 4);
	currentModuleType = ModuleType::Gain;
	currentMoveModuleID = -1;

	canPlaceNode = true;

	tooltips.setLookAndFeel(&laF_ToolbarButton);
	addAndMakeVisible(tooltips);

	splashScreen.addMouseListener(this, false);
	addAndMakeVisible(splashScreen);

	setSize(800, 575);
}

NodeGraphEditor::~NodeGraphEditor()
{
}

void NodeGraphEditor::paint(Graphics &g) {
	switch (mode) {
		case Mode::Idle:

			break;
		case Mode::PlaceNode:
			if(canPlaceNode)
				g.setColour(Colour((uint8)255, (uint8)255, (uint8)255, (uint8)64));
			else
				g.setColour(Colour((uint8)255, (uint8)0, (uint8)0, (uint8)64));
			g.fillRect(mousePos.x, mousePos.y, currentModuleSize.x * 25, currentModuleSize.y * 25);
			if (canPlaceNode)
				g.setColour(Colour(255, 255, 255));
			else
				g.setColour(Colour(255, 0, 0));
			g.drawRect(mousePos.x, mousePos.y, currentModuleSize.x * 25, currentModuleSize.y * 25, 5);
			break;
		case Mode::ConnectNodes:

			break;
		case Mode::MoveNode:
			if (canPlaceNode)
				g.setColour(Colour((uint8)0, (uint8)0, (uint8)255, (uint8)64));
			else
				g.setColour(Colour((uint8)255, (uint8)0, (uint8)0, (uint8)64));
			g.fillRect(mousePos.x, mousePos.y, currentModuleSize.x * 25, currentModuleSize.y * 25);
			if (canPlaceNode)
				g.setColour(Colour(0, 0, 255));
			else
				g.setColour(Colour(255, 0, 0));
			g.drawRect(mousePos.x, mousePos.y, currentModuleSize.x * 25, currentModuleSize.y * 25, 5);
			break;
		default:

			break;
	}
}

void NodeGraphEditor::paintOverChildren(Graphics &g) {
	//drawConnections
	for (int i = 0; i < ngp->modules.size(); i++) {
		if (ngp->modules[i] != nullptr) {
			for (int n = 0; n < ngp->modules[i]->inputs.size(); n++) {
				if (ngp->modules[i]->inputs[n].connectedModule >= 0) {
					Point<int> start = ngp->modules[i]->inputSocketButtons[n]->getPosition() + ngp->modules[i]->getPosition() + Point<int>(12, 12);
					Point<int> end = ngp->modules[ngp->modules[i]->inputs[n].connectedModule]->outputSocketButtons[ngp->modules[i]->inputs[n].connectedOutput]->getPosition() + ngp->modules[ngp->modules[i]->inputs[n].connectedModule]->getPosition() + Point<int>(12, 12);
					g.setGradientFill(ColourGradient(ngp->modules[i]->inputSocketButtons[n]->socketColor, (float)start.x, (float)start.y, ngp->modules[ngp->modules[i]->inputs[n].connectedModule]->outputSocketButtons[ngp->modules[i]->inputs[n].connectedOutput]->socketColor, (float)end.x, (float)end.y, false));
					Point<float> startF = Point<float>(start.x, start.y);
					Point<float> endF = Point<float>(end.x, end.y);
					DrawBezierCurve(g, startF, Point<float>(startF.x - abs(endF.x - startF.x) * 0.5f, startF.y), Point<float>(endF.x + abs(endF.x - startF.x) * 0.5f, endF.y), endF);
				}
			}
			for (int n = 0; n < ngp->modules[i]->controls.size(); n++) {
				if (ngp->modules[i]->controls[n].connectedModule >= 0) {
					Point<int> start = ngp->modules[i]->controlSocketButtons[n]->getPosition() + ngp->modules[i]->getPosition() + Point<int>(12, 12);
					Point<int> end = ngp->modules[ngp->modules[i]->controls[n].connectedModule]->outputSocketButtons[ngp->modules[i]->controls[n].connectedOutput]->getPosition() + ngp->modules[ngp->modules[i]->controls[n].connectedModule]->getPosition() + Point<int>(12, 12);
					g.setGradientFill(ColourGradient(ngp->modules[i]->controlSocketButtons[n]->socketColor, (float)start.x, (float)start.y, ngp->modules[ngp->modules[i]->controls[n].connectedModule]->outputSocketButtons[ngp->modules[i]->controls[n].connectedOutput]->socketColor, (float)end.x, (float)end.y, false));
					Point<float> startF = Point<float>(start.x, start.y);
					Point<float> endF = Point<float>(end.x, end.y);
					DrawBezierCurve(g, startF, Point<float>(startF.x, startF.y + abs(endF.x - startF.x)), Point<float>(endF.x + abs(endF.x - startF.x) * 0.5f, endF.y), endF);
				}
			}
		}
	}
	if (mode == Mode::ConnectNodes) {
		Point<int> start = ngp->modules[currentSourceModuleID]->outputSocketButtons[currentSourceOutputID]->getPosition() + ngp->modules[currentSourceModuleID]->getPosition() + Point<int>(12, 12);
		Point<int> end = rawMousePos;
		Point<float> startF = Point<float>(start.x, start.y);
		Point<float> endF = Point<float>(end.x, end.y);
		g.setColour(ngp->modules[currentSourceModuleID]->outputSocketButtons[currentSourceOutputID]->socketColor);
		DrawBezierCurve(g, startF, Point<float>(startF.x + abs(endF.x - startF.x) * 0.5f, startF.y), Point<float>(endF.x - abs(endF.x - startF.x) * 0.5f, endF.y), endF);
	}
}

void NodeGraphEditor::DrawBezierCurve(Graphics &g, Point<float> start, Point<float> control0, Point<float> control1, Point<float> end) {
	Path curve;
	for (float t = 0.0f; t <= 1.1f; t += 0.1f) {
		if (t == 0) {
			curve.startNewSubPath(start);
		}
		else {
			Point<float> pStartControl0 = start + (control0 - start) * t;
			Point<float> pControl0Control1 = control0 + (control1 - control0) * t;
			Point<float> pControl1End = control1 + (end - control1) * t;
			Point<float> pSC0C0C1 = pStartControl0 + (pControl0Control1 - pStartControl0) * t;
			Point<float> pC0C1C1E = pControl0Control1 + (pControl1End - pControl0Control1) * t;
			Point<float> pFinal = pSC0C0C1 + (pC0C1C1E - pSC0C0C1) * t;
			curve.lineTo(pFinal);
		}
	}
	g.strokePath(curve, PathStrokeType(4.0f, PathStrokeType::JointStyle::beveled, PathStrokeType::EndCapStyle::rounded));
}

void NodeGraphEditor::RedrawGUI() {
	for (int i = 0; i < ngp->modules.size(); i++) {
		if (ngp->modules[i] != nullptr) {
			addAndMakeVisible(ngp->modules[i]);
			ngp->modules[i]->setBounds(ngp->modules[i]->getBounds());
			ngp->modules[i]->nge = this;
		}
	}
	repaint(0, 0, 800, 575);
}

void NodeGraphEditor::resized() {
	splashScreen.setBounds(200, 125, 400, 300);
}

void NodeGraphEditor::mouseMove(const MouseEvent &event) {
	rawMousePos = event.getPosition();
	mousePos.x = (int)(floor(rawMousePos.x * 0.04f) / 0.04f);
	mousePos.y = (int)(floor(rawMousePos.y * 0.04f) / 0.04f);
	canPlaceNode = !CheckOverlap();
	if(mode == Mode::PlaceNode || mode == Mode::MoveNode)
		repaint(lastMousePos.x, lastMousePos.y, currentModuleSize.x * 25, currentModuleSize.y * 25);
	lastMousePos = mousePos;
	if(mode == Mode::ConnectNodes)
		repaint(0, 0, 800, 575);

}

void NodeGraphEditor::mouseUp(const MouseEvent &event) {
	if (canPlaceNode && mode == Mode::PlaceNode && event.mods.isLeftButtonDown()) {
		ngp->canProcess = false;
		switch(currentModuleType){
			case ModuleType::Gain:
				ngp->modules.push_back(new GainModule());
				break;
			case ModuleType::ADSR:
				ngp->modules.push_back(new ADSRModule());
				break;
			case ModuleType::Out:
				ngp->modules.push_back(new OutModule());
				ngp->outputModuleID = ngp->currentID;
				break;
			case ModuleType::MIDI:
				ngp->modules.push_back(new MIDIModule());
				break;
			case ModuleType::BasicGenerator:
				ngp->modules.push_back(new BasicGeneratorModule());
				break;
			case ModuleType::MtoF:
				ngp->modules.push_back(new MtoFModule());
				break;
			case ModuleType::Add:
				ngp->modules.push_back(new AddModule());
				break;
			case ModuleType::Subtract:
				ngp->modules.push_back(new SubtractModule());
				break;
			case ModuleType::Multiply:
				ngp->modules.push_back(new MultiplyModule());
				break;
			case ModuleType::Divide:
				ngp->modules.push_back(new DivideModule());
				break;
			case ModuleType::Value:
				ngp->modules.push_back(new ValueModule());
				break;
			case ModuleType::FM:
				ngp->modules.push_back(new FMModule());
				break;
			default:
				ngp->modules.push_back(new GainModule());
				break;
		}
		addAndMakeVisible(ngp->modules[ngp->currentID]);
		ngp->modules[ngp->currentID]->setBounds(mousePos.x, mousePos.y, currentModuleSize.x * 25, currentModuleSize.y * 25);
		ngp->modules[ngp->currentID]->id = ngp->currentID;
		ngp->modules[ngp->currentID]->nge = this;
		ngp->modules[ngp->currentID]->ngp = this->ngp;
		mode = Mode::Idle;
		ngp->currentID++;
		ngp->canProcess = true;
	}
	if (mode == Mode::MoveNode && event.mods.isLeftButtonDown()) {
		if (canPlaceNode) {
			ngp->modules[currentMoveModuleID]->setBounds(mousePos.x, mousePos.y, ngp->modules[currentMoveModuleID]->moduleSize.x * 25, ngp->modules[currentMoveModuleID]->moduleSize.y * 25);
			mode = Mode::Idle;
			currentMoveModuleID = -1;
			repaint(0, 0, 800, 575);
		}
	}
}

void NodeGraphEditor::mouseDown(const MouseEvent &event) {
	splashScreen.setVisible(false);
	splashScreen.setEnabled(false);
}

void NodeGraphEditor::AddGainModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(4, 4);
	currentModuleType = ModuleType::Gain;
}

void NodeGraphEditor::AddADSRModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(10, 4);
	currentModuleType = ModuleType::ADSR;
}

void NodeGraphEditor::AddOutModule() {
	if (!ngp->hasOutModule) {
		mode = Mode::PlaceNode;
		currentModuleSize = Point<int>(4, 3);
		currentModuleType = ModuleType::Out;
		ngp->hasOutModule = true;
	}
}

void NodeGraphEditor::AddMIDIModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(4, 4);
	currentModuleType = ModuleType::MIDI;
}

void NodeGraphEditor::AddBasicGeneratorModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(6, 4);
	currentModuleType = ModuleType::BasicGenerator;
}

void NodeGraphEditor::AddMtoFModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(4, 3);
	currentModuleType = ModuleType::MtoF;
}

void NodeGraphEditor::AddAddModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(4, 4);
	currentModuleType = ModuleType::Add;
}

void NodeGraphEditor::AddSubtractModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(4, 4);
	currentModuleType = ModuleType::Subtract;
}

void NodeGraphEditor::AddMultiplyModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(4, 4);
	currentModuleType = ModuleType::Multiply;
}

void NodeGraphEditor::AddDivideModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(4, 4);
	currentModuleType = ModuleType::Divide;
}

void NodeGraphEditor::AddValueModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(4, 4);
	currentModuleType = ModuleType::Value;
}

void NodeGraphEditor::AddFMModule() {
	mode = Mode::PlaceNode;
	currentModuleSize = Point<int>(4, 5);
	currentModuleType = ModuleType::FM;
}

void NodeGraphEditor::BeginConnectModule(unsigned int sourceModuleID, unsigned int sourceOutputID) {
	if (mode != Mode::ConnectNodes && mode != Mode::PlaceNode && mode != Mode::MoveNode) {
		currentSourceModuleID = sourceModuleID;
		currentSourceOutputID = sourceOutputID;
		mode = Mode::ConnectNodes;
	}
}

void NodeGraphEditor::EndConnectModule(unsigned int destModuleID, unsigned int destInputID) {
	if (mode == Mode::ConnectNodes) {
		bool canConnect = true;
		for (int i = 0; i < ngp->modules[currentSourceModuleID]->inputs.size(); i++) {
			if (ngp->modules[currentSourceModuleID]->inputs[i].connectedModule == destModuleID) {
				canConnect = false;
			}
		}
		if (canConnect && destModuleID != currentSourceModuleID) {
			ngp->modules[destModuleID]->inputs[destInputID].connectedModule = currentSourceModuleID;
			ngp->modules[destModuleID]->inputs[destInputID].connectedOutput = currentSourceOutputID;
			repaint(0, 0, 800, 575);
		}
		mode = Mode::Idle;
	}
	else if (mode == Mode::Idle) {
		ngp->canProcess = false;
		ngp->modules[destModuleID]->inputs[destInputID].connectedModule = -1;
		ngp->modules[destModuleID]->inputs[destInputID].connectedOutput = -1;
		ngp->canProcess = true;
		repaint(0, 0, 800, 575);
	}
}

void NodeGraphEditor::EndConnectControl(unsigned int destModuleID, unsigned int destInputID) {
	if (mode == Mode::ConnectNodes) {
		bool canConnect = true;
		for (int i = 0; i < ngp->modules[currentSourceModuleID]->inputs.size(); i++) {
			if (ngp->modules[currentSourceModuleID]->inputs[i].connectedModule == destModuleID) {
				canConnect = false;
			}
		}
		for (int i = 0; i < ngp->modules[currentSourceModuleID]->controls.size(); i++) {
			if (ngp->modules[currentSourceModuleID]->controls[i].connectedModule == destModuleID) {
				canConnect = false;
			}
		}
		if (canConnect && destModuleID != currentSourceModuleID) {
			ngp->modules[destModuleID]->controls[destInputID].connectedModule = currentSourceModuleID;
			ngp->modules[destModuleID]->controls[destInputID].connectedOutput = currentSourceOutputID;
			repaint(0, 0, 800, 575);
		}
		mode = Mode::Idle;
	}
	else if (mode == Mode::Idle) {
		ngp->canProcess = false;
		ngp->modules[destModuleID]->controls[destInputID].connectedModule = -1;
		ngp->modules[destModuleID]->controls[destInputID].connectedOutput = -1;
		ngp->canProcess = true;
		repaint(0, 0, 800, 575);
	}
}

void NodeGraphEditor::MoveModule(int moveModuleID, Point<int> size) {
	if (mode == Mode::Idle) {
		currentMoveModuleID = moveModuleID;
		currentModuleSize = size;
		mode = Mode::MoveNode;
	}
}

void NodeGraphEditor::DeleteModule(int moduleID) {
	if (mode == Mode::Idle && moduleID != ngp->outputModuleID) {
		//Remove connections first so nothing will access a potentially deleted module
		for (int i = 0; i < ngp->modules[moduleID]->inputs.size(); i++) {
			ngp->modules[moduleID]->inputs[i].connectedModule = -1;
			ngp->modules[moduleID]->inputs[i].connectedOutput = -1;
		}
		for (int i = 0; i < ngp->modules.size(); i++) {
			if (ngp->modules[i] != nullptr) {
				for (int n = 0; n < ngp->modules[i]->inputs.size(); n++) {
					if (ngp->modules[i]->inputs[n].connectedModule == moduleID) {
						ngp->modules[i]->inputs[n].connectedModule = -1;
						ngp->modules[i]->inputs[n].connectedOutput = -1;
					}
				}
				for (int n = 0; n < ngp->modules[i]->controls.size(); n++) {
					if (ngp->modules[i]->controls[n].connectedModule == moduleID) {
						ngp->modules[i]->controls[n].connectedModule = -1;
						ngp->modules[i]->controls[n].connectedOutput = -1;
					}
				}
			}
		}
		//Delete Module
		delete ngp->modules[moduleID];
		ngp->modules[moduleID] = nullptr;
		ngp->modules.erase(ngp->modules.begin() + moduleID);
		//Shift IDs
		for (int i = moduleID; i < ngp->modules.size(); i++) {
			ngp->modules[i]->id -= 1;
		}
		for (int i = 0; i < ngp->modules.size(); i++) {
			for (int n = 0; n < ngp->modules[i]->inputs.size(); n++) {
				if (ngp->modules[i]->inputs[n].connectedModule >= moduleID) {
					ngp->modules[i]->inputs[n].connectedModule -= 1;
				}
			}
			for (int n = 0; n < ngp->modules[i]->controls.size(); n++) {
				if (ngp->modules[i]->controls[n].connectedModule >= moduleID) {
					ngp->modules[i]->controls[n].connectedModule -= 1;
				}
			}
		}
		if (ngp->outputModuleID >= 0 && ngp->outputModuleID >= moduleID)
			ngp->outputModuleID -= 1;
		ngp->currentID -= 1;
		repaint(0, 0, 800, 575);
	}
}

bool NodeGraphEditor::CheckOverlap() {
	Rectangle<int> nodeRectangle = Rectangle<int>(mousePos, mousePos + Point<int>(currentModuleSize.x * 25, currentModuleSize.y * 25));
	bool overlaps = false;
	for (int i = 0; i < ngp->modules.size(); i++) {
		if (ngp->modules[i] != nullptr) {
			if (nodeRectangle.intersects(ngp->modules[i]->getBounds()) && i != currentMoveModuleID) {
				overlaps = true;
			}
		}
	}
	return overlaps;
}