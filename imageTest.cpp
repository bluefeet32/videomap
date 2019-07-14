#include "readXml.hpp"

#include <tinyxml2.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

// Compile & run with:
// g++ -o readXml readXml.cpp -ltinyxml2

//Compile & run with:
// g++ -o readImage readImage.cpp `pkg-config --cflags --libs opencv` && ./readImage ../../maps/quickroutes/190630Kirkebygden-WOCT.jpg

using namespace cv;
using namespace std;
using namespace tinyxml2;

int main( int argc, char** argv ) {

    if( argc != 3 ) {
        cout << " Usage: xml_file XmlFileToRead, display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }

    while(1) {
        cout << "Please press a key" << endl;
        char c = waitKey(0);
        if ( c == 27 ) break;
        cout << "pressed " << c << endl;
    }

    std::vector<std::vector<int> > coords;
    coords = readCoordsFromXML( argv[1] );

    Mat image;
    image = imread( argv[2], CV_LOAD_IMAGE_COLOR ); // Read the file

    if ( !image.data ) {
        cout << " Could not open or find the image" << endl;
        return -1;
    }

    cout << "image size: " << image.rows << " " << image.cols << endl;
    cout << "coords size: " << coords.size() << endl;
    image = image( Range( 500, image.rows ), Range( 0, image.cols ) );

    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for dislay.

    for ( size_t iter = 0; iter < coords.size(); iter++ ) {
        size_t xmin = ( coords[iter][0]-5 ) * 3;
        size_t xmax = ( coords[iter][0]+5 ) * 3;
        size_t ymin = ( coords[iter][1]-5 ) - 500 + 70;
        size_t ymax = ( coords[iter][1]+5 ) - 500 + 70;
        size_t windowXmin = ( coords[iter][0]-100 );
        size_t windowXmax = ( coords[iter][0]+100 );
        size_t windowYmin = ( coords[iter][1]-100 ) - 500 + 70;
        size_t windowYmax = ( coords[iter][1]+100 ) - 500 + 70;
        cout << "xrange " << xmin << " " << xmax
             << " yrange " << ymin << " " << ymax << endl;
        cout << "windowxrange " << windowXmin << " " << windowXmax
             << " windowyrange " << windowXmin << " " << windowYmax << endl;
        for ( int i = xmin; i <= xmax; i++ ) {
            for ( int j = ymin; j <= ymax; j++ ) {
                image.at<uchar>(j,i) = 128;
            }
        }
        Mat ROI = image( Range( windowYmin, windowYmax ), Range( windowXmin, windowXmax ) );

        imshow( "Display window", ROI );  // Show our image insde it
        waitKey( 10 );
    }




/*    Mat ROI = image( Range( 1100, 1159 ), Range( 300, 500 ) );

    imshow( "Display window", ROI );  // Show our image insde it
*/
    imshow( "Display window", image );  // Show our image insde it
    waitKey( 0 );
    return 0;
}
