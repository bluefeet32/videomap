#include "readXml.hpp"

#include <tinyxml2.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <ctime>

//Compile:
// g++ -o readImage readImage.cpp `pkg-config --cflags --libs opencv` && ./readImage ../../maps/quickroutes/190630Kirkebygden-WOCT.jpg

//Execution example
// 
using namespace cv;
using namespace std;
using namespace tinyxml2;

int main( int argc, char** argv ) {

    if( argc != 5 ) {
        cout << " Usage: xml_file XmlFileToRead, display_image ImageToLoadAndDisplay, video VideoToAddImageTo, offset offsetInSecondsToStartGPS" << endl;
        return -1;
    }

    std::vector<std::vector<int> > coords;
    coords = readCoordsFromXML( argv[1] );

    // Read in the map omage
    Mat image;
    image = imread( argv[2], CV_LOAD_IMAGE_COLOR ); // Read the file

    if ( !image.data ) {
        cout << " Could not open or find the image" << endl;
        return -1;
    }

    cout << "image size: " << image.rows << " " << image.cols << endl;
    cout << "coords size: " << coords.size() << endl;
    cout << "vid " << argv[3] << endl;

    VideoCapture cap(argv[3]);
    size_t vidLength = cap.get( CV_CAP_PROP_FRAME_COUNT );
    size_t fps = cap.get( CV_CAP_PROP_FPS );
    // Default resolution of the frame is obtained.The default resolution is system dependent.
    int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    if ( !cap.isOpened() ) {
        cout << "Error opening video stream or file." << endl;
        return -1;
    }

    size_t count = 0;
    size_t frameCount = 0;
    size_t coordsIdx = -1;
    size_t offset = atoi( argv[4] ) * fps;
    Mat routeImage;
    size_t waitTime = 10;
    size_t windowSize = 100;
    size_t minWindow = 50;
    size_t maxWindow = 300;
    double scale = 1.0;

    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
    VideoWriter video("outcpp.avi",CV_FOURCC('M','J','P','G'), fps, Size(frame_width,frame_height));

    time_t imgDrawTime = 0;
    time_t imgPasteTime = 0;
    time_t vidReadTime = 0;
    time_t vidWriteTime = 0;
    time_t startTime = time(NULL);
    while(1) {
        //if ( count % 60 == 0 ) {
        if ( count > coords.size() * fps ) {
            break;
        }

        time_t imgDrawStart = time(NULL);
        if ( count % fps == 0 ) {
            coordsIdx++;
            size_t xmin = ( coords[coordsIdx][0]-5 ) * 3;
            size_t xmax = ( coords[coordsIdx][0]+5 ) * 3;
            size_t ymin = ( coords[coordsIdx][1]-5 ) + 70; // +70 to account for the QuickRoute header
            size_t ymax = ( coords[coordsIdx][1]+5 ) + 70;
            // Make a 100x100 box of the image
            size_t window = windowSize / scale;
            size_t windowXmin = max<size_t>(  coords[coordsIdx][0]-window, 0 );
            size_t windowXmax = min<size_t>( coords[coordsIdx][0]+window, image.cols ) ;
            size_t windowYmin = max<size_t>( ( coords[coordsIdx][1]-window ) + 70, 0 );
            size_t windowYmax = min<size_t>( ( coords[coordsIdx][1]+window ) + 70, image.rows );
    //        cout << "xrange " << xmin << " " << xmax
    //             << " yrange " << ymin << " " << ymax << endl;
    //        cout << "windowxrange " << windowXmin << " " << windowXmax
    //             << " windowyrange " << windowXmin << " " << windowYmax << endl;
            for ( int i = xmin; i <= xmax; i++ ) {
                for ( int j = ymin; j <= ymax; j++ ) {
                    image.at<uchar>(j,i) = 128;
                }
            }
            routeImage = image( Range( windowYmin, windowYmax ), Range( windowXmin, windowXmax ) );
            if ( scale != 1 ) {
                resize(routeImage, routeImage, cv::Size(), scale, scale);
            }
//           cout << "route channels " << routeImage.channels() << endl;
        }
        imgDrawTime += time(NULL) - imgDrawStart;

        if ( true ) {
            Mat frame;
            time_t vidReadStart = time(NULL);
            cap >> frame;
            vidReadTime += time(NULL) - vidReadStart;
            // Capture frame by frame
            while ( frameCount < offset ) {
                cap >> frame;
                frameCount++;
            }
    //        cout << "frame channels " << frame.channels() << endl;

            // If the frame is empty, break immediately
            if ( frame.empty() ) {cout << "empty frame! " << count << endl; break; }

            int x = 100;
            int y = 100;
    //        cout << "routetype " << routeImage.type() << " vid type " << frame.type() << endl;

            time_t imgPasteStart = time(NULL);
            routeImage.copyTo( frame(Rect(x,y,routeImage.cols,routeImage.rows) ) );
            imgPasteTime += time(NULL) - imgPasteStart;

            //Display the frame
            imshow( "Frame", frame );

            //Write the frame to the output
            time_t vidWriteStart = time(NULL);
            video.write( frame );
            vidWriteTime += time(NULL) - vidWriteStart;
        }
        else {
            //Use an fps sized frame buffer
            vector<Mat> frameBuffer;
            time_t vidReadStart = time(NULL);
            for ( size_t i = 0; i < fps; i++ ) {
                cap >> frameBuffer[i];
            }
            vidReadTime += time(NULL) - vidReadStart;

            for ( size_t i = 0; i < frameBuffer.size(); i++ ) {
                while ( frameCount < offset && i < frameBuffer.size() ) {
                    i++;
                    frameCount++;
                }
        //        cout << "frame channels " << frame.channels() << endl;

                // If the frame is empty, break immediately
                if ( frameBuffer[i].empty() ) {cout << "empty frame! " << count << endl; break; }

                int x = 100;
                int y = 100;
        //        cout << "routetype " << routeImage.type() << " vid type " << frame.type() << endl;

                time_t imgPasteStart = time(NULL);
                routeImage.copyTo( frameBuffer[i](Rect(x,y,routeImage.cols,routeImage.rows) ) );
                imgPasteTime += time(NULL) - imgPasteStart;

                //Display the frame
                imshow( "Frame", frameBuffer[i] );
            }

            //Write the frame to the output
            time_t vidWriteStart = time(NULL);
            for ( size_t i = 0; i < frameBuffer.size(); i++ ) {
                video.write( frameBuffer[i] );
            }
            vidWriteTime += time(NULL) - vidWriteStart;
        }

        //Press ESC to exit
        char c = (char)waitKey(waitTime);
        if ( c == 27 ) {
            break;
/*            char c = (char)waitKey(0);
            if ( c == 27 ) break;
            else cout << "c = " << c << endl;*/
        }
        else if ( c == 'S' ) {
            waitTime /= 2;
            if ( waitTime == 0 ) waitTime = 1;
            cout << "waitTime: " << waitTime << endl;
        }
        else if ( c == 'Q' ) {
            waitTime *= 2;
            cout << "waitTime: " << waitTime << endl;
        }
        else if ( c == 'R' ) {
            if ( windowSize < maxWindow ) {
                windowSize += 50;
                cout << "windowSize: " << windowSize << endl;
            }
        }
        else if ( c == 'T' ) {
            if ( windowSize > minWindow ) {
                windowSize -= 50;
                cout << "windowSize: " << windowSize << endl;
            }
        }
        else if ( c == '=' ) {
            scale += 0.1;
            cout << "scale: " << scale << endl;
        }
        else if ( c == '-' ) {
            scale -= 0.1 ;
            cout << "scale: " << scale << endl;
        }
        else if ( c == 'p' ) {
            char c = (char)waitKey(0);
            if ( c == 27 ) break;
        }

        frameCount++;
        count++;
        if ( count % 100 == 0 ) {
            time_t elapsedTime = time(NULL) - startTime;
            cout << ( count * 100 ) / vidLength << "%. fps: " << count/elapsedTime << endl;
//            cout << count " << count << " time " << elapsedTime << endl;
            cout << "vidWrite: " << vidWriteTime << " vidRead: " << vidReadTime << " imgDraw: " << imgDrawTime << " imgPaste: " << imgPasteTime << endl;
        }
    }
    cout << "frameCount " << frameCount << " count " << count << endl;

    // When everything is done, release the video capture
    cap.release();
    video.release();

    //Close all the frames;
    destroyAllWindows();

    return 0;
}
