#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <string>
#include <glob.h>
using namespace cv;
using namespace std;

int main(int argc,char **argv){
	vector<cv::String> fn;
	cv::Mat kernel;
	cv::Mat src,bright;
	int w=10;
	int i;
	double min, max;
	int A,s=0;
	clock_t t;

	string dir="*.jpg";
	if (argv[1])
		dir=argv[1];
	glob(dir, fn, false);
	size_t count = fn.size();
	


	t=clock();	
	src = imread(argv[1],CV_16SC1);
	//cvtColor(src, bright, CV_BGR2GRAY);
	Mat temp, darkch, channels[3];
	kernel = cv::Mat::ones( w, w, CV_16U );
	split(src, channels);
	//cout<<src.type()<<endl;	
	(cv::min)(channels[0], channels[1], temp);           //find minimum between B and G channels
	(cv::min)(temp, channels[2], darkch);	
	//darkch.convertTo(darkch,CV_16UC1);
	erode(darkch, darkch, kernel);
	cv::minMaxLoc(darkch, &min, &max);
	/*
	Mat tmp=src.reshape(0,1);
	//cout<<tmp.type()<<endl;

	Mat flatdark=darkch.reshape(1,1);

	sortIdx(flatdark,flatdark,CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
	//cout<<flatdark.type()<<endl;
	//cout<<tmp.channels();
	for(i=0;i<10;i++)
		//cout<<tmp.at<Vec3b>(flatdark.at<int32_t>(i))<<endl;*/


	int(max)>220?A=220:A=int(max);
	//cout<<A<<endl;


	//A-=20;
	//cout<<"Thresh:"<<thresh<<endl;
	
	threshold(darkch,bright, 180, 255, THRESH_BINARY);
	s = cv::sum( darkch )[0]/(darkch.cols*darkch.rows);
	float thresh=0.0;
	if (s<=60)
		thresh=0.8;
	else if (s<=140 && s>=100)
		thresh=0.6;
	else if (s<=100 || s>180)
		thresh=0.6;
	else if (s>140)
		thresh=0.8;


	darkch.convertTo(darkch, CV_32FC1);
	Mat Tx=((0.95 * darkch) /A);
	threshold(Tx,Tx, thresh, 1, THRESH_TRUNC);
	Tx=1-Tx;
	Mat refinedTx;
	erode(Tx, refinedTx, kernel);
	
	refinedTx.convertTo(refinedTx,CV_32FC1);
	
	for(i=0;i<3;i++){
	channels[i].convertTo(channels[i],CV_32FC1);
	channels[i]=(channels[i]-A)/refinedTx+A;
	}
	//threshold(channels[0],channels[0], 255, 255, THRESH_TRUNC);
	//threshold(channels[1],channels[1], 255, 255, THRESH_TRUNC);
	//threshold(channels[2],channels[2], 255, 255, THRESH_TRUNC);
	Mat radiance;
	cv:Scalar tempVal;
	merge(channels,3,radiance);
	tempVal = mean(radiance);
	//cout<<tempVal.val[0]<<endl;
	radiance.convertTo(radiance,CV_8UC1);
	cout<<"Time : " << double(clock()-t)/CLOCKS_PER_SEC<<endl;
	
	cout<<thresh;
	imshow("img",radiance);
	//imwrite("img.jpg",radiance);
	waitKey(0);
	return 0;
	}
