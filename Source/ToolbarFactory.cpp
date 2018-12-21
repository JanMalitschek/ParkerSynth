#include "ToolbarFactory.h"

ToolbarFactory::ToolbarFactory()
{
}

ToolbarFactory::~ToolbarFactory()
{
}

void ToolbarFactory::getAllToolbarItemIds(Array< int > &ids) {
	ids.add(Preset);
	ids.add(Module);
	ids.add(Info);
}

void ToolbarFactory::getDefaultItemSet(Array< int > &ids) {
	ids.add(Preset);
	ids.add(Module);
	ids.add(Info);
}

ToolbarItemComponent* ToolbarFactory::createItem(int itemId) {
	switch (itemId) {
		case Preset:
			button_preset = new ToolbarButton(Preset, "P", Drawable::createFromImageData(BinaryData::Screw_01_png, BinaryData::Screw_01_pngSize), 0);
			return button_preset;
			break;
		case Module:
			button_module = new ToolbarButton(Module, "Module", 0, 0);
			return button_module;
			break;
		case Info:
			button_info = new ToolbarButton(Info, "Info", 0, 0);
			return button_info;
			break;
		default:
			break;
	}
}