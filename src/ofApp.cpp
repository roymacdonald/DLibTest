#include "ofApp.h"

ofRectangle toOf(const dlib::rectangle& r){
    return ofRectangle(r.left(), r.top(), r.width(), r.height());
}
ofPoint toOf(const dlib::point& p){
    return ofPoint(p.x(), p.y(), p.z() );
}
bool toDLib(const ofPixels& inPix, array2d<rgb_pixel>& outPix){
    
    int width = inPix.getWidth();
    int height = inPix.getHeight();
    outPix.set_size( height, width );
    int chans = inPix.getNumChannels();
    const unsigned char* data = inPix.getData();
    for ( unsigned n = 0; n < height;n++ )
    {
        const unsigned char* v =  &data[n * width *  chans];
        for ( unsigned m = 0; m < width;m++ )
        {
            if ( chans==1 )
            {
                unsigned char p = v[m];
                assign_pixel( outPix[n][m], p );
            }
            else{
                rgb_pixel p;
                p.red = v[m*3];
                p.green = v[m*3+1];
                p.blue = v[m*3+2];
                assign_pixel( outPix[n][m], p );
            }
        }
    }
//    if(inPix.getNumChannels() == 3){
//        int h = inPix.getHeight();
//        int w = inPix.getWidth();
//        outPix.clear();
//        outPix.set_size(h,w);
//        for (int i = 0; i < h; i++) {
//            for (int j = 0; j < w; j++) {
//                
//                outPix[i][j].red = inPix.getColor(j, i).r; //inPix[i*w + j];
//                outPix[i][j].green = inPix.getColor(j, i).g; //inPix[i*w + j + 1];
//                outPix[i][j].blue = inPix.getColor(j, i).b; //inPix[i*w + j + 2];
//            }
//        }
//        return true;
//    }else{
//        return  false;
//    }
    return true;
}
//--------------------------------------------------------------
void ofApp::setup(){
     uint64_t st =ofGetElapsedTimeMillis();
    detector = get_frontal_face_detector();
    cout << "get_frontal_face_detector: " << ofGetElapsedTimeMillis() - st << " ms." << endl;

    ofFile f(ofToDataPath("shape_predictor_68_face_landmarks.dat"));
    if (f.exists()) {
        deserialize(f.getAbsolutePath()) >> sp;
    }else{
        cout << "SHAPE PREDICTOR DAT FILE MISSING!!!" << endl;
    }

    
#ifdef USE_VIDEO
#ifdef USE_VIDEO_GRABBER
    video.setDeviceID(0);
    video.setup(720, 480);
#else
    ofFileDialogResult r = ofSystemLoadDialog();
    if(r.bSuccess){
        video.load(r.getPath());
    }
#endif
#endif
    bDetect = true;
    
}

//--------------------------------------------------------------
void ofApp::findFaces(const ofPixels& pixels){
    
     int64_t st =ofGetElapsedTimeMillis();
    int64_t st2 = st;
        array2d<rgb_pixel> img;
    
    if(toDLib(pixels , img)){
        cout << "OfPixels to DlibPixels: " << ofGetElapsedTimeMillis() - st << " ms." << endl;
        
        // Make the image larger so we can detect small faces.
st =ofGetElapsedTimeMillis();
        //pyramid_up(img);
        
        cout << "PyramidUp: " << ofGetElapsedTimeMillis() - st << " ms." << endl;
        // Now tell the face detector to give us a list of bounding boxes
        // around all the faces in the image.
        if(bDetect){
            dets.clear();
            st =ofGetElapsedTimeMillis();
           
            dets = detector(img);
        cout << "face detector: " << ofGetElapsedTimeMillis() - st << " ms." << endl;
            
            cout << "Number of faces detected: " << dets.size() << endl;
            
            // Now we will go ask the shape_predictor to tell us the pose of
            // each face we detected.
            shapes.clear();
            st =ofGetElapsedTimeMillis();
            for (unsigned long j = 0; j < dets.size(); ++j){
                full_object_detection shape = sp(img, dets[j]);
                //        cout << "number of parts: "<< shape.num_parts() << endl;
                //        cout << "pixel position of first part:  " << shape.part(0) << endl;
                //        cout << "pixel position of second part: " << shape.part(1) << endl;
                //        // You get the idea, you can get all the face part locations if
                // you want them.  Here we just store them in shapes so we can
                // put them on the screen.
                shapes.push_back(shape);
            }
            cout << "full_object_detection: " << ofGetElapsedTimeMillis() - st << " ms." << endl;
//            win.clear_overlay();
//            win.set_image(img);
//            win.add_overlay(render_face_detections(shapes));
//            
//            // We can also extract copies of each face that are cropped, rotated upright,
//            // and scaled to a standard size as shown here:
//            dlib::array<array2d<rgb_pixel> > face_chips;
//            extract_image_chips(img, get_face_chip_details(shapes), face_chips);
//            win_faces.set_image(tile_images(face_chips));
        }
    }//*/
    
     cout << "findFaces: " << ofGetElapsedTimeMillis() - st2 << " ms." << endl;
    cout << "-------------------------------" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
#ifdef USE_VIDEO
    video.update();
    if(video.isFrameNew()){
        findFaces(video.getPixels());
    }
#endif
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
#ifdef USE_VIDEO
    video.draw(0, 0);
#else
    if(ofImg.isAllocated()){
    ofImg.draw(0,0);
    }
#endif
    ofPushStyle();
    ofSetColor(ofColor::red);
    ofNoFill();
    for (auto& r:dets) {
        ofDrawRectangle(toOf(r));
    }
    ofFill();
    ofSetColor(ofColor::red);
    for (auto & s:shapes) {
        for (int i = 0; i < s.num_parts(); i++) {
            ofDrawCircle(toOf(s.part(i)),3);
        }
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
#ifndef USE_VIDEO
    if (key == ' ') {
        ofFileDialogResult r = ofSystemLoadDialog();
        if(r.bSuccess){
            if(ofImg.load(r.getPath())){
                findFaces(ofImg.getPixels());
            }
        }
    }
#endif
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
