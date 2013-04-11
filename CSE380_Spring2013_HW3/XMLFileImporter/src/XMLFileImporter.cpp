#include "stdafx.h"
#include "src\tinystr.h"
#include "src\tinyxml.h"
#include "src\XMLFileImporter.h"

const char* XMLFileImporter::newCharArrayFromString(string text)
{
	char *charArray = new char[text.size() + 1];
	int i = 0;
	for (; i < text.size(); i++)
		charArray[i] = text[i];
	charArray[i] = '\0';
	return charArray;
}

const char* XMLFileImporter::newCharArrayFromWstring(wstring wideText)
{
	char *charArray = new char[wideText.size() + 1];
	int i = 0;
	for ( ; i < wideText.size(); i++)
		charArray[i] = wideText[i];
	charArray[i] = '\0';
	return charArray;
}

int XMLFileImporter::extractIntAtt(const TiXmlElement *pElem, string att)
{
	stringstream ss;
	int num;
	const char* text;
	text = pElem->Attribute(att.c_str());
	ss << text;
	ss >> num;
	ss.str("");
	return num;
}

const char* XMLFileImporter::extractCharAtt(const TiXmlElement *pElem, string att)
{
	stringstream ss;
	const char* text;
	text = pElem->Attribute(att.c_str());
	return text;
}

bool XMLFileImporter::extractBoolAtt(const TiXmlElement *pElem, string att)
{
	const char* text;
	text = pElem->Attribute(att.c_str());
	const char trueText[] = "true";
	if (strcmp(text, trueText) == 0)
		return true;
	else
		return false;
}