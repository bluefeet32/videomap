#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

//Compile & run with:
// g++ -o readImage readImage.cpp `pkg-config --cflags --libs opencv` && ./readImage ../../maps/quickroutes/190630Kirkebygden-WOCT.jpg

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {

    if( argc != 2 ) {
        cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }

    Mat image;
    image = imread( argv[1], CV_LOAD_IMAGE_COLOR ); // Read the file

    if ( !image.data ) {
        cout << " Could not open or find the image" << endl;
        return -1;
    }

    cout << "image size: " << image.rows << " " << image.cols << endl;
    image = image( Range( 500, image.rows ), Range( 0, image.cols ) );

    for ( int x = 320*3; x < 330*3; x++ ) {
        for ( int y = 1130-500+70; y < 1150-500+70; y++ ) {
            image.at<uchar>(y,x) = 128;
        }
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for dislay.
    imshow( "Display window", image );  // Show our image insde it

    waitKey( 0 );

/*    Mat ROI = image( Range( 1100, 1159 ), Range( 300, 500 ) );

    imshow( "Display window", ROI );  // Show our image insde it
*/
    waitKey( 0 );
    return 0;
}
