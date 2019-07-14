#include "readXml.hpp"

#include <tinyxml2.h>
#include <iostream>

using namespace std;

int main( int argc, char** argv ) {

    if( argc != 2 ) {
        cout << " Usage: xml_file XmlFileToRead" << endl;
        return -1;
    }

    vector< vector< int > > coords;
    coords = readCoordsFromXML( argv[1] );
    return 0;

}
