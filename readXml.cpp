#include "readXml.hpp"

#include <tinyxml2.h>
#include <iostream>

// Compile & run with:
// g++ -o readXml readXml.cpp -ltinyxml2

using namespace tinyxml2;
using namespace std;

vector< vector< int > > readCoordsFromXML( const char * fileName ) {
    XMLDocument doc;
    doc.LoadFile( fileName );

//    std::vector<int> seconds;
//    int prevSeconds = -1;
    std::vector<std::vector<int> > coords;
    vector<int> row;

    XMLElement *segment = doc.FirstChildElement()->FirstChildElement();

    for ( XMLElement *samp = segment->FirstChildElement(); samp != NULL; samp = samp->NextSiblingElement() ) {
        row.clear();
        row.push_back( atoi( samp->Attribute("imageX") ) );
        row.push_back( atoi( samp->Attribute("imageY") ) );
/*        seconds.push_back( atoi ( samp->Attribute("elapsedTimeFromStart") ) );
        if ( seconds.back() != prevSeconds + 1 ) {
            std::cout << prevSeconds << " " << seconds.back() << std::endl;
        }*/
        coords.push_back(row);
//        prevSeconds = seconds.back();
    }
    return coords;
}
