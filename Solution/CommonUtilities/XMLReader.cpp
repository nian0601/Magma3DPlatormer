#include "stdafx.h"


#include "XMLReader.h"
#include "CommonHelper.h"

XMLReader::XMLReader()
{
	myHasOpenedDoc = false;
	myDoc = nullptr;
}


XMLReader::~XMLReader()
{
	if (myHasOpenedDoc == true)
	{
		CU::String<80> error("XML Document was not closed properly: " + myFilePath);
		DL_ASSERT(error.c_str());
	}
	if (myDoc != nullptr)
	{
		delete myDoc;
		myDoc = nullptr;
	}
}

void XMLReader::OpenDocument(const CU::String<80>& aFilePath)
{
	delete myDoc;
	myDoc = nullptr;

	if (myDoc == nullptr)
	{
		myDoc = new tinyxml2::XMLDocument();
	}

	tinyxml2::XMLError error = myDoc->LoadFile(aFilePath.c_str());
	if (error == 0)
	{
		myHasOpenedDoc = true;
		myFilePath = aFilePath;
	}
	else
	{
#pragma region Error-Codes
		switch (error)
		{
		case tinyxml2::XML_NO_ATTRIBUTE:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_NO_ATTRIBUTE", aFilePath.c_str()));
			break;
		case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_WRONG_ATTRIBUTE_TYPE", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_FILE_NOT_FOUND", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_FILE_COULD_NOT_BE_OPENED", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_FILE_READ_ERROR:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_FILE_READ_ERROR", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_ELEMENT_MISMATCH:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_ELEMENT_MISMATCH", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_PARSING_ELEMENT:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_ELEMENT", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_ATTRIBUTE", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_IDENTIFYING_TAG:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_IDENTIFYING_TAG", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_PARSING_TEXT:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_TEXT", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_PARSING_CDATA:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_CDATA", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_PARSING_COMMENT:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_COMMENT", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_PARSING_DECLARATION:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_DECLARATION", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING_UNKNOWN", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_EMPTY_DOCUMENT", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_MISMATCHED_ELEMENT", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_PARSING:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_PARSING", aFilePath.c_str()));
			break;
		case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_CAN_NOT_CONVERT_TEXT", aFilePath.c_str()));
			break;
		case tinyxml2::XML_NO_TEXT_NODE:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_NO_TEXT_NODE", aFilePath.c_str()));
			break;
		case tinyxml2::XML_ERROR_COUNT:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: XML_ERROR_COUNT", aFilePath.c_str()));
			break;
		default:
			DL_ASSERT(CU::Concatenate<256>("Failed to Open XML File. File: %s, Error: Unknown, non of tinyxml's error codes caught it :(", aFilePath.c_str()));
			break;
		}
#pragma endregion

		DL_ASSERT("Failed to Open XML File. Check Debuglog for error");
	}
}

void XMLReader::CloseDocument()
{
	if (myHasOpenedDoc == true)
	{
		myHasOpenedDoc = false;
		myFilePath = "";
		delete myDoc;
		myDoc = nullptr;
	}
}

CU::String<80> XMLReader::GetElementName(tinyxml2::XMLElement* aElement)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [GetElementName] before Opening the document");

	return aElement->Name();
}

/*
---------------------------
Normal Find Operations
---------------------------
*/

tinyxml2::XMLElement* XMLReader::FindFirstChild(const CU::String<80>& aChildName)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");

	return myDoc->FirstChildElement(aChildName.c_str());
}

tinyxml2::XMLElement* XMLReader::FindFirstChild(tinyxml2::XMLElement* aParent) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");

	return aParent->FirstChildElement();
}

tinyxml2::XMLElement* XMLReader::FindFirstChild(tinyxml2::XMLElement* aParent, const CU::String<80>& aChildName) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");


	return aParent->FirstChildElement(aChildName.c_str());
}

tinyxml2::XMLElement* XMLReader::FindNextElement(const CU::String<80>& aElementName)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	return myDoc->NextSiblingElement(aElementName.c_str());

}

tinyxml2::XMLElement* XMLReader::FindNextElement(tinyxml2::XMLElement* aPrevElement) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	return aPrevElement->NextSiblingElement();
}

tinyxml2::XMLElement* XMLReader::FindNextElement(tinyxml2::XMLElement* aPrevElement, const CU::String<80>& aElementName) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	return aPrevElement->NextSiblingElement(aElementName.c_str());
}



/*
---------------------------
Force Find Operations
---------------------------
*/

tinyxml2::XMLElement* XMLReader::ForceFindFirstChild(const CU::String<80>& aChildName)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");

	DebugFirstChild(aChildName);

	return myDoc->FirstChildElement(aChildName.c_str());
}

tinyxml2::XMLElement* XMLReader::ForceFindFirstChild(tinyxml2::XMLElement* aParent) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");

	DebugFirstChild(aParent);

	return aParent->FirstChildElement();
}

tinyxml2::XMLElement* XMLReader::ForceFindFirstChild(tinyxml2::XMLElement* aParent, const CU::String<80>& aChildName) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindFirstChild] before Opening the document");


	DebugFirstChild(aParent, aChildName);

	return aParent->FirstChildElement(aChildName.c_str());
}

tinyxml2::XMLElement* XMLReader::ForceFindNextElement(const CU::String<80>& aElementName)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	DebugNextElement(aElementName);

	return myDoc->NextSiblingElement(aElementName.c_str());

}

tinyxml2::XMLElement* XMLReader::ForceFindNextElement(tinyxml2::XMLElement* aPrevElement) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	DebugNextElement(aPrevElement);

	return aPrevElement->NextSiblingElement();
}

tinyxml2::XMLElement* XMLReader::ForceFindNextElement(tinyxml2::XMLElement* aPrevElement, const CU::String<80>& aElementName) const
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [FindNextElement] before Opening the document");

	DebugNextElement(aPrevElement, aElementName);

	return aPrevElement->NextSiblingElement(aElementName.c_str());
}



/*
---------------------------
Normal Read Operations
---------------------------
*/

bool XMLReader::ReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, CU::String<80>& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(string)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to Read from nullptr");

	if (aElementToReadFrom == nullptr)
		return false;

	if (aElementToReadFrom->FindAttribute(aAttributeToRead.c_str()) != 0)
	{
		aTargetVariable = aElementToReadFrom->Attribute(aAttributeToRead.c_str());
		return true;
	}

	return false;
}

bool XMLReader::ReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, int& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(int)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to Read from nullptr");

	if (aElementToReadFrom == nullptr)
		return false;

	if (aElementToReadFrom->QueryIntAttribute(aAttributeToRead.c_str(), &aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	return false;
}

bool XMLReader::ReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, float& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(float)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to Read from nullptr");

	if (aElementToReadFrom == nullptr)
		return false;

	if (aElementToReadFrom->QueryFloatAttribute(aAttributeToRead.c_str(), &aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	return false;
}

bool XMLReader::ReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, double& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(double)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to Read from nullptr");

	if (aElementToReadFrom == nullptr)
		return false;

	if (aElementToReadFrom->QueryDoubleAttribute(aAttributeToRead.c_str(), &aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	return false;
}

bool XMLReader::ReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, bool& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(bool)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to Read from nullptr");

	if (aElementToReadFrom == nullptr)
		return false;

	if (aElementToReadFrom->QueryBoolAttribute(aAttributeToRead.c_str(), &aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	return false;
}




/*
---------------------------
Force Read Operations
---------------------------
*/

bool XMLReader::ForceReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, CU::String<80>& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ForceReadAttribute(string)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to ForceRead from nullptr");

	if (aElementToReadFrom != nullptr 
		&& aElementToReadFrom->FindAttribute(aAttributeToRead.c_str()) != 0)
	{
		aTargetVariable = aElementToReadFrom->Attribute(aAttributeToRead.c_str());
		return true;
	}

	DL_ASSERT(CU::Concatenate<256>("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str()));
	return false;
}

bool XMLReader::ForceReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, int& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ForceReadAttribute(int)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to ForceRead from nullptr");

	if (aElementToReadFrom != nullptr
		&& aElementToReadFrom->QueryIntAttribute(aAttributeToRead.c_str(), &aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	DL_ASSERT(CU::Concatenate<256>("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str()));
	return false;
}

bool XMLReader::ForceReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, unsigned int& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ForceReadAttribute(int)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to ForceRead from nullptr");

	if (aElementToReadFrom != nullptr
		&& aElementToReadFrom->QueryUnsignedAttribute(aAttributeToRead.c_str(), &aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	DL_ASSERT(CU::Concatenate<256>("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str()));
	return false;
}

bool XMLReader::ForceReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, float& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(float)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to ForceRead from nullptr");

	if (aElementToReadFrom != nullptr 
		&& aElementToReadFrom->QueryFloatAttribute(aAttributeToRead.c_str(), &aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	DL_ASSERT(CU::Concatenate<256>("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str()));
	return false;
}

bool XMLReader::ForceReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, double& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(double)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to ForceRead from nullptr");

	if (aElementToReadFrom != nullptr 
		&& aElementToReadFrom->QueryDoubleAttribute(aAttributeToRead.c_str(), &aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	DL_ASSERT(CU::Concatenate<256>("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str()));
	return false;
}

bool XMLReader::ForceReadAttribute(const tinyxml2::XMLElement* aElementToReadFrom, const CU::String<80>& aAttributeToRead, bool& aTargetVariable)
{
	if (myHasOpenedDoc == false)
		DL_ASSERT("[XMLReader]: Tried to [ReadAttribute(bool)] before Opening the document");

	DL_ASSERT_EXP(aElementToReadFrom != nullptr, "[XMLReader]: Tried to ForceRead from nullptr");

	if (aElementToReadFrom != nullptr 
		&& aElementToReadFrom->QueryBoolAttribute(aAttributeToRead.c_str(), &aTargetVariable) == tinyxml2::XML_NO_ERROR)
		return true;

	DL_ASSERT(CU::Concatenate<256>("Failed to read Attribute: [ %s ], from Element: [ %s ], in Document: [ %s ]", aAttributeToRead.c_str(), aElementToReadFrom->Name(), myFilePath.c_str()));
	return false;
}



/*
---------------------------
Debug Operations
---------------------------
*/
void XMLReader::DebugFirstChild(const CU::String<80>& aChildName)
{
	if (myDoc->FirstChildElement(aChildName.c_str()) == nullptr)
	{
		DL_ASSERT(CU::Concatenate<256>("Failed to [ForceFindFirstChild]. Parent: [ Document ], Child: [ %s ], File: [ %s ]", aChildName.c_str(), myFilePath.c_str()));
	}
}

void XMLReader::DebugFirstChild(tinyxml2::XMLElement* aParent) const
{
	if (aParent->FirstChildElement() == nullptr)
	{
		DL_ASSERT(CU::Concatenate<256>("Failed to [ForceFindFirstChild]. Parent: [ %s ], Child: [ First ], File:  [ %s ]", aParent->Name(), myFilePath.c_str()));
	}
}

void XMLReader::DebugFirstChild(tinyxml2::XMLElement* aParent, const CU::String<80>& aChildName) const
{
	if (aParent->FirstChildElement(aChildName.c_str()) == nullptr)
	{
		DL_ASSERT(CU::Concatenate<256>("Failed to [ForceFindFirstChild]. Parent: [ %s ], Child: [ %s ], File: [ %s ]", aParent->Name(), aChildName.c_str(), myFilePath.c_str()));
	}
}

void XMLReader::DebugNextElement(const CU::String<80>& aChildName)
{
	if (myDoc->NextSiblingElement(aChildName.c_str()) == nullptr)
	{
		DL_ASSERT(CU::Concatenate<256>("Failed to [ForceFindNextElement]. Parent: [ Document ], Child: [ %s ], File: [ %s ]", aChildName.c_str(), myFilePath.c_str()));
	}
}

void XMLReader::DebugNextElement(tinyxml2::XMLElement* aParent) const
{
	if (aParent->NextSiblingElement() == nullptr)
	{
		DL_ASSERT(CU::Concatenate<256>("Failed to [ForceFindNextElement]. Parent: [ %s ], Child: [ First ], File: [ %s ]", aParent->Name(), myFilePath.c_str()));
	}
}

void XMLReader::DebugNextElement(tinyxml2::XMLElement* aParent, const CU::String<80>& aChildName) const
{
	if (aParent->NextSiblingElement(aChildName.c_str()) == nullptr)
	{
		DL_ASSERT(CU::Concatenate<256>("Failed to [ForceFindNextElement]. Parent: [ %s ], Child: [ %s ], File: [ %s ]", aParent->Name(), aChildName.c_str(), myFilePath.c_str()));
	}
}