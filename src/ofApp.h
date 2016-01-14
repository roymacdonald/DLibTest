#pragma once

#include "ofMain.h"
#ifdef SHIFT
#undef SHIFT
#endif
#include <dlib/image_processing/frontal_face_detector.h>
#include "dlib/image_processing/render_face_detections.h"
#include "dlib/image_processing.h"
#include "dlib/gui_widgets.h"
#include "dlib/image_io.h"


#define USE_VIDEO_GRABBER
#define USE_VIDEO
using namespace dlib;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        frontal_face_detector detector;
        shape_predictor sp;
        
        std::vector<rectangle> dets;
    
        std::vector<full_object_detection> shapes;
    //image_window win, win_faces;
    
    bool bDetect;
#ifdef USE_VIDEO
#ifdef USE_VIDEO_GRABBER
    ofVideoGrabber video;
#else
    ofVideoPlayer video;
#endif
#else
    ofImage ofImg;
#endif
    void findFaces(const ofPixels& pixels);
    
};



        // This example takes in a shape model file and then a list of images to
        // process.  We will take these filenames in as command line arguments.
        // Dlib comes with example images in the examples/faces folder so give
        // those as arguments to this program.
//            cout << "Call this program like this:" << endl;
//            cout << "./face_landmark_detection_ex shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
//            cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
//            cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;



// ----------------------------------------------------------------------------------------

