#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, char** argv ) {

    if( argc != 2 ) {
        cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }
    VideoCapture cap(argv[1]);

    if ( !cap.isOpened() ) {
        cout << "Error opening video stream or file." << endl;
        return -1;
    }

    while(1) {

        Mat frame;
        // Capture frame by frame
        cap >> frame;

        // If the frame is empty, break immediately
        if ( frame.empty() ) break;

        //Displae the frame
        imshow( "Frame", frame );

        //Press ESC to exit
        char c = (char)waitKey(1);
        if ( c == 27 ) break;
    }

    // When everything is done, release the video capture
    cap.release();

    //Close all the frames;
    destroyAllWindows();

    return 0;

}
