#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class ToolbarFactory : public ToolbarItemFactory
{
public:
	ToolbarFactory();
	~ToolbarFactory();

	enum ItemIDs {
		Preset = 1,
		Module = 2,
		Info = 3
	};

	void getAllToolbarItemIds(Array< int > &ids) override;
	void getDefaultItemSet(Array< int > &ids) override;
	ToolbarItemComponent* createItem(int itemId) override;

	ToolbarButton* button_preset;
	ToolbarButton* button_module;
	ToolbarButton* button_info;
};

