#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <fstream>
#include <cmath>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {

	if(argc < 4) {
		cout << "\nToo few arguments.\nUsage: gazebb video.avi gazefile.txt squareSize1D" << endl;
		return -1;
	}

	//clock start
	clock_t startTime = clock();

	//sets up vid capture and gets vid info
	VideoCapture vidin(argv[1]);
	int totalFrames = vidin.get(CV_CAP_PROP_FRAME_COUNT) + 1;
	int width = vidin.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = vidin.get(CV_CAP_PROP_FRAME_HEIGHT);
	//cout << "\ntotalFrames = " << totalFrames << "\nwidth = " << width << "\nheight = " << height << endl; //debug

	//sets up reading gaze+ txt file
	ifstream fp(argv[2], fstream::in);
	if(!fp.is_open()) {
		cout << "\nCould not open " << argv[2] << ". Please try again." << endl;
		return -1;
	}
	int crapint1, crapint2, fnum = -1;
	string crapstr, fixsac;
	double tempx, tempy;
	int x, y;

	//passes all the bullshit in the start of the gaze+ txt files
	string strtemp;
	while(fp.peek() == '#')
		getline(fp,strtemp);
	getline(fp,strtemp);

	//sets up output video shit
	string fnameout = argv[1], key(".");
	size_t pos = fnameout.rfind(key);
	if(pos != std::string::npos)
		fnameout.replace(pos, key.size(), "-BB.");
	Size framesize(width,height);
	int codec = CV_FOURCC('M', 'P', '4', 'V');
	VideoWriter vidout(fnameout, codec, vidin.get(CV_CAP_PROP_FPS), framesize);

	/* position test debug shit */
	// Mat tempImg;
	// Rect tempBox;
	// vector<Point> tempPts;
	// tempPts.push_back(Point(5, 305)); //a
	// tempPts.push_back(Point(305, 305)); //b
	// tempPts.push_back(Point(5, 5)); //c
	// tempPts.push_back(Point(305, 5)); //d
	// tempBox = boundingRect(tempPts);
	// vidin >> tempImg;
	// rectangle(tempImg, tempBox.tl(), tempBox.br(), Scalar(0,0,255), 2);
	// imshow("TEST", tempImg);
	// waitKey(0);

	//draws bounding box
	Mat current;
	Rect box;
	Scalar green(0,255,0), red(0,0,255);
	int recsize = atoi(argv[3])/2;
	int ax, ay, bx, by, cx, cy, dx, dy;
	for(int i = 0; i < totalFrames; ++i) {
		
		vidin >> current;

		//gets the next coordinates that match the actual video frame i with fnum(frame from gaze txt).
		while(i+1 != fnum) {
			
			fp >> crapint1 >> crapstr >> crapint2 >> tempx >> tempy >> fnum >> fixsac;
			
			if(fnum > i+1)
				break;
		}
		if(fnum > i+1) {
			vidout << current;
			continue;
		}

		//rounds up gaze x & y coordinates. then sets 4 box points.
		x = ceil(tempx);
		y = ceil(tempy);
		//cout << "\nx = " << x << "\ny = " << y << "\nfnum = " << fnum << "\nfixsac = " << fixsac << endl;
		ax = x - recsize; ay = y - recsize;
		bx = x + recsize; by = y - recsize;
		cx = x - recsize; cy = y + recsize;
		dx = x + recsize; dy = y + recsize;

		//if x or y are out of range of video, skips.
		if(x > width || x < 0 || y > height || y < 0) {
			vidout << current;
			continue;
		}

		//creates a vector of the points and makes a Rect out of it.
		vector<Point> pts;
		pts.push_back(Point(ax,ay));
		pts.push_back(Point(bx,by));
		pts.push_back(Point(cx,cy));
		pts.push_back(Point(dx,dy));
		box = boundingRect(pts);

		//if points are Fixations, makes box green. Saccade, red.
		if(fixsac == "Fixation")
			rectangle(current, box.tl(), box.br(), green, 2);
		else
			rectangle(current, box.tl(), box.br(), red, 2);	
		// imshow("TEST", current); //debug
		// waitKey(0); //debug

		//write to new video.
		vidout << current;

		//debug if shit gets fucked up. fnum should always be equal to i+1
		if(i+1 != fnum) cout << "\ni != fnum. i = " << i << " fnum = " << fnum << endl;
	}

	cout << "\a" << "Runtime: " << (double)(clock()-startTime)/(double)CLOCKS_PER_SEC << " seconds." << endl;

	return 0;
}