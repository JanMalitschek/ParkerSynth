#ifndef NUMERIC_TEXT_FILTER
#define NUMERIC_TEXT_FILTER

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>
#include <algorithm>

class NumericTextFilter : public TextEditor::InputFilter
{
public:
	NumericTextFilter();
	~NumericTextFilter();

	String filterNewText(TextEditor &, const String &newInput) override;
private:
};

#endif