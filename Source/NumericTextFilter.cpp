#include "NumericTextFilter.h"

NumericTextFilter::NumericTextFilter()
{
}

NumericTextFilter::~NumericTextFilter()
{
}

String NumericTextFilter::filterNewText(TextEditor &, const String &newInput) {
	std::string input = newInput.toStdString();
	char filter[] = "abcdefghijklmnopqrstuvwxyz_%&+*#-,;><";
	for (int i = 0; i < strlen(filter); i++) {
		input.erase(std::remove(input.begin(), input.end(), filter[i]), input.end());
	}
	return juce::String(input);
}
