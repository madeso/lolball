#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include <memory>

#include "an8_reader.hpp"
#include "tinyxml.h"

std::string asString(const float f) {
	ostringstream str;
	str << f;
	return str.str();
}

void convert(const std::string& pInFileName, const std::string& pOutFileName) {
	ChunkList* root = ChunkReader::loadFile(pInFileName);
	
	for(ChunksResult res = root->get("scene"); res != root->end(); ++res) {
		if( res->first != "scene" ) break;
		const ChildList& scene = res->second->getChildList();
		const std::string title = asStringChild(scene.childAt(0))->value();
		cout << "Converting " << title << ": " << endl;

		TiXmlDocument doc;
		doc.LinkEndChild( new TiXmlDeclaration( "1.0", "", "" ) );
		TiXmlElement* level = new TiXmlElement("level");

		for(unsigned long childIndex=0; childIndex < scene.count(); ++childIndex) {
			const Child* baseChild = scene.childAt(childIndex);
			if( baseChild->getType() == CCT_CHILD ) {
				const Chunk* chunk = asChunkChild( baseChild )->getChild();
				// is chunk an "entity"
				if( chunk->getName().getName() == "objectelement" ) {
					const ChildList& entity = chunk->getChildList();
					// name
					const std::string name = asStringChild(entity.childAt(0))->value();
					// type
					const std::string type = asStringChild(entity.childAt(1))->value();
					
					TiXmlElement* object = new TiXmlElement("entity");
					object->SetAttribute("name", name);
					object->SetAttribute("type", type);

					TiXmlElement* position = new TiXmlElement("position");
					TiXmlElement* rotation = new TiXmlElement("rotation");

					{
						float x=0, y=0, z=0;
						if( entity.has("loc") ) {
							const CompoundChild* location = asCompoundChild(entity.at("loc")->getChildList().childAt(0));
							x = getRealValue(location->at(0));
							y = getRealValue(location->at(1));
							z = getRealValue(location->at(2));
						}
						position->SetAttribute("x", asString(x));
						position->SetAttribute("y", asString(y));
						position->SetAttribute("z", asString(z));
					}

					{
						float x=0, y=0, z=0, w=1;
						if( entity.has("orientation") ) {
							const CompoundChild* location = asCompoundChild(entity.at("orientation")->getChildList().childAt(0));
							x = getRealValue(location->at(0));
							y = getRealValue(location->at(1));
							z = getRealValue(location->at(2));
							w = getRealValue(location->at(3));
						}
						rotation->SetAttribute("x", asString(x));
						rotation->SetAttribute("y", asString(y));
						rotation->SetAttribute("z", asString(z));
						rotation->SetAttribute("w", asString(w));
					}

					object->LinkEndChild(position);
					object->LinkEndChild(rotation);
					level->LinkEndChild(object);
				}
			}
		}
		doc.LinkEndChild( level );
		const string saveFile = pOutFileName + title + ".lvl";
		cout << "Saving to " << saveFile << endl;
		doc.SaveFile( saveFile );
	}
}

void main(int argc, char *argv[]) {
	cout << "an8 to lvl file converter by sirGustav" << endl << endl;
#define ARGUMENT(num) ((argc>num)?(argv[num]):("[null]"))
	const std::string inFileName = ARGUMENT(1);
	const std::string outFileName = ARGUMENT(2);

	if( argc != 3 ) {
		cerr << "Error: Wrong number of arguments given!" << endl;
		std::string appName = argv[0];
		cout << "Call as " << appName << " [in file] [out dir]" << endl
			<< "both arguments must be given and [in file] must point to a valid file, out dir must point to a valid directory" << endl
			<< "All I got was " << inFileName << " and " << outFileName << endl;
	}
	else {
		try {
			convert(inFileName, outFileName);
			cout << "Conversion done!" << endl;
		}
		catch( const std::string& pMessage) {
			cerr << "Error: " << pMessage << endl;
		}
	}
#ifdef _DEBUG
	cin.get();
#endif
}