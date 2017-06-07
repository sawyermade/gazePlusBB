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

	//sets up vid capture and gets vid info
	VideoCapture vidin(argv[1]);
	int totalFrames = vidin.get(CV_CAP_PROP_FRAME_COUNT) + 1;
	int width = vidin.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = vidin.get(CV_CAP_PROP_FRAME_HEIGHT);
	//cout << "\ntotalFrames = " << totalFrames << "\nwidth = " << width << "\nheight = " << height << endl;

	//sets up reading gaze+ txt file
	ifstream fp(argv[2], fstream::in);
	int crapint1, crapint2, fnum = -1;
	string crapstr, fixsac;
	double tempx, tempy;
	int x, y;

	//passes all the bullshit in the start of the gaze+ txt files
	string strtemp;
	while(fp.peek() == '#')
		getline(fp,strtemp);
	getline(fp,strtemp);
	//cout << "\nstrtemp = " << strtemp << endl;

	//sets up output video shit
	string fnameout = argv[1];
	fnameout += "-BB.avi";
	Size framesize(width,height);
	//VideoWriter vidout(fnameout, vidin.get(CV_CAP_PROP_FOURCC), vidin.get(CV_CAP_PROP_FPS), framesize);
	int codec = CV_FOURCC('M', 'P', '4', 'V');
	VideoWriter vidout(fnameout, codec, vidin.get(CV_CAP_PROP_FPS), framesize);

	//draws bounding box
	Mat current;
	Rect box;
	Scalar green(0,255,0), red(0,0,255);
	int recsize = atoi(argv[3])/2;
	int ax, ay, bx, by, cx, cy, dx, dy;
	for(int i = 0; i < totalFrames; ++i) {
		
		vidin >> current;

		while(i+1 != fnum) {
			
			fp >> crapint1 >> crapstr >> crapint2 >> tempx >> tempy >> fnum >> fixsac;
			
			if(fnum > i+1)
				break;
		}
		if(fnum > i+1) {
			vidout << current;
			continue;
		}

		x = ceil(tempx);
		y = ceil(tempy);
		//cout << "\nx = " << x << "\ny = " << y << "\nfnum = " << fnum << "\nfixsac = " << fixsac << endl;
		ax = x - recsize; ay = y + recsize;
		bx = x + recsize; by = y + recsize;
		cx = x - recsize; cy = y - recsize;
		dx = x + recsize; dy = y - recsize;


		if(x > width || x < 0 || y > height || y < 0) {
			vidout << current;
			continue;
		}

		vector<Point> pts;
		pts.push_back(Point(ax,ay));
		pts.push_back(Point(bx,by));
		pts.push_back(Point(cx,cy));
		pts.push_back(Point(dx,dy));
		box = boundingRect(pts);

		if(fixsac == "Fixation")
			rectangle(current, box.tl(), box.br(), green, 2);
		else
			rectangle(current, box.tl(), box.br(), red, 2);	
		// imshow("TEST", current);
		// waitKey(0);

		//write to new video.
		vidout << current;

		if(i+1 != fnum) cout << "\ni != fnum. i = " << i << " fnum = " << fnum << endl;
	}


	return 0;
}