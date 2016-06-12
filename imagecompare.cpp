#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\features2d.hpp"
#include <vector>
#include <iostream>
using namespace cv;
using namespace std;

int FeatureDection(Mat base, Mat test)
{
	vector<KeyPoint> kp1, kp2;
	FAST(base, kp1, 120);
	FAST(test, kp2, 120);
	/*
	if (kp1.size() != kp2.size())
	{
		cout << "Number of FAST features different! Base=" << kp1.size() << " Test=" << kp2.size() << endl;
		//return 0;
	}
	for (vector<KeyPoint>::iterator base_it = kp1.begin(), test_it = kp2.begin(); base_it != kp1.end(); ++base_it, ++test_it)
	{
		if (base_it->pt.x + 3.0 < test_it->pt.x || test_it->pt.x < base_it->pt.x - 3.0)
		{
			cout << "x index fail" << endl;
			//return 0;
		}
		else if (base_it->pt.y + 3.0 < test_it->pt.y || test_it->pt.y < base_it->pt.y - 3.0)
		{
			cout << "y index fail" << endl;
			//return 0;
		}
		else if (base_it->angle != test_it->angle)
		{
			cout << "agnle fail" << endl;
			//return 0;
		}
		else if (base_it->class_id != test_it->class_id)
		{
			cout << "class_id fail" << endl;
			//return 0;
		}
	}
	*/
	Ptr<ORB> orb = ORB::create(5000);
	Mat desc1, desc2;
	orb->detectAndCompute(base, Mat(), kp1, desc1, true);
	orb->detectAndCompute(test, Mat(), kp2, desc2, true);

	//orb.detect(base, kp1);
	BFMatcher featureMatcher(NORM_HAMMING);
	std::vector< DMatch > matches;
	featureMatcher.match(desc2, desc1, matches);

	//If this doesnt work on big images, divide the images into small parts and try
	/*CV_PROP_RW float abc; //!< diameter of the meaningful keypoint neighborhood
	CV_PROP_RW float abc2 = 0.95;
	abc = float(matches.size()) / kp1.size();
	cout<< matches.size()<<" "<< kp1.size();*/

	if ((float(matches.size()) / kp1.size()) < 0.86)
	{
		return 0;
	}




	/*for (vector<DMatch>::iterator it = matches.begin(); it != matches.end(); ++it)
	{
		if (it->distance > 0)
		{
			cout << "Matcher distance more than zero." << endl;
			//return 0;
		}
	}*/

	return 1;

}

int HistogramMatching(Mat base, Mat test)
{
	vector<Mat> bgr_planesBase;
	split(base, bgr_planesBase);
	vector<Mat> bgr_planesTest;
	split(test, bgr_planesTest);
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_histBase, g_histBase, r_histBase;
	Mat b_histTest, g_histTest, r_histTest;
	calcHist(&bgr_planesBase[0], 1, 0, Mat(), b_histBase, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planesBase[1], 1, 0, Mat(), g_histBase, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planesBase[2], 1, 0, Mat(), r_histBase, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planesTest[0], 1, 0, Mat(), b_histTest, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planesTest[1], 1, 0, Mat(), g_histTest, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planesTest[2], 1, 0, Mat(), r_histTest, 1, &histSize, &histRange, uniform, accumulate);

	float hist_diff = 0.0;
	for (int i = 1; i < histSize; i++)
	{		
		    //hist_diff = hist_diff
			//	+ ((b_histBase.at<float>(i,0) - b_histTest.at<float>(40,0))/float(b_histBase.at<float>(i,0)))
			//	+ ((g_histBase.at<float>(i,0) - g_histTest.at<float>(i,0))/float(g_histBase.at<float>(i,0)))
			//	+ ((r_histBase.at<float>(i,0) - r_histTest.at<float>(i,0))/float(r_histBase.at<float>(i,0)));

			float a = ((b_histBase.at<float>(i, 0) - b_histTest.at<float>(40, 0)) / float(b_histBase.at<float>(i, 0)));
			//g_histBase.at<float>(i) - g_histTest.at<float>(i);
			//r_histBase.at<float>(i) -  r_histTest.at<float>(i);
	}

	cout << hist_diff;
	/*for (int i = 0; i < 256; ++i)
	{
		if (b_histBase.at<float>(i, 0) != b_histTest.at<float>(i, 0)\
			|| g_histBase.at<float>(i, 0) != g_histTest.at<float>(i, 0)\
			|| r_histBase.at<float>(i, 0) != r_histTest.at<float>(i, 0))
		{
			cout << "Colour bins mismatch";
			return 0;
		}
	}*/


	return 1;
	//waitKey(0);
}

Mat resize_img(Mat inputImg)
{
	Size size(100, 100);//the dst image size,e.g.100x100
	Mat dst;//dst image
	Mat src;//src image
	src = inputImg;
	resize(src, dst, size);//resize image
	Mat outputImg = dst;
	return outputImg;
}

int imageCompare(Mat baseImage, Mat testImage)
{
	//Mat baseImage = imread("Z:\\users\\vipaggar\\images\\compare\\perfect\\base\\pd001010.pdf.sft.tif");
	//Mat testImage = imread("Z:\\users\\vipaggar\\images\\compare\\perfect\\test\\pd001010.pdf.sft.tif");
	//Mat baseImage =imread("Z:\\users\\vipaggar\\images\\compare\\base\\pd102190_pdf.sft.tif");
	//Mat testImage = imread("Z:\\users\\vipaggar\\images\\compare\\test\\pd102190.pdf.sft.tif");
	//Mat baseImage = imread("Z:\\users\\vipaggar\\images\\compare\\base\\ex50m001.2ps.sft.tif");
	//Mat testImage = imread("Z:\\users\\vipaggar\\images\\compare\\test\\ex50m001.2ps.sft.tif");


	//Mat baseImage = imread(argv[1]);
	//Mat testImage = imread(argv[2]);


	if (baseImage.empty())
	{
        
		cout << "Can not read base image" << endl;
		return 0;
	}
	if (testImage.empty())
	{
		cout << "Can not read test image" << endl;
		return 0;
	}
	if (baseImage.rows != testImage.rows || baseImage.cols != testImage.cols)
	{
		cout << "Images have different sizes." << endl;
		return 0;
	}

	//baseImage=resize_img(baseImage);
	//testImage=resize_img(testImage);

	if (!FeatureDection(baseImage, testImage))
	{
		cout << "Feature Detection : Fail" << endl;
		return 0;
	}
	else
	{
		cout << "Feature Detection : Success" << endl;
		return 1;
	}

	/*
	if (!HistogramMatching(baseImage, testImage))
	{
	cout << "Feature Detection : Fail" << endl;
	//return 0;
	}
	else
	{
	cout << "Feature Detection : Success" << endl;
	}
	*/
	/*Mat baseImageDCT = Mat(baseImage.size(), CV_8UC1);
	Mat testImageDCT = Mat(baseImage.size(), CV_8UC1);
	Mat output = Mat(baseImage.size(), baseImage.type());
	Mat output2 = Mat(baseImage.size(), baseImage.type());
	Mat bimage;
	baseImage.convertTo(bimage, CV_32F, 1.0 / 255);
	Mat timage;
	testImage.convertTo(timage, CV_32F, 1.0 / 255);
	dct(bimage, baseImageDCT);
	dct(timage, testImageDCT);
	absdiff(baseImageDCT, testImageDCT, output);
	imshow("b dct", baseImageDCT);
	imshow("t dct", testImageDCT);
	dct(output, output2 , DCT_INVERSE);
	imshow("image", output2);*/
	//imshow("blah", baseImage);
	//cvWaitKey(0);
	//cout << "Success.";
}

vector<Mat> slice_Image(Mat bigImage)
{
	vector<Mat> smallImages;
	int x_dividents = 16;
	int y_dividents = x_dividents * (float(bigImage.rows) / bigImage.cols);
	int x_increment = bigImage.cols / x_dividents;
	int y_increment = bigImage.rows / y_dividents;
	for (int y = 0; y < bigImage.rows; y += y_increment)
	{
		for (int x = 0; x < bigImage.cols; x += x_increment)
		{
			int x_end = x_increment, y_end = y_increment;
			if (x + x_increment > bigImage.cols)
			{
				x_end = bigImage.cols-1;
			}
			if (y + y_increment > bigImage.rows)
			{
				y_end = bigImage.rows-1;
			}
			Rect rect = Rect(x, y, x_end, y_end);
			smallImages.push_back(Mat(bigImage, rect));
		}
	}
	return smallImages;
}


int main(int argc, char* argv[])
{
	//app5
	//approach2();
	
	if (argc != 3)
	{
		cout << "Input two images";
		return 0;
	}
	Mat baseImage = imread(argv[1]);
	Mat testImage = imread(argv[2]);
	cout << "Base: " << argv[1] << endl;
	cout << "Test: " << argv[2] << endl;

	if (baseImage.rows != testImage.rows || baseImage.cols != testImage.cols)
	{
		cout << "Images have different sizes." << endl;
		return 0;
	}

  	vector<Mat> Base_slices = slice_Image(baseImage);
	vector<Mat> Test_slices = slice_Image(testImage);

	int i = 1; int block_match = 0; 
	for (vector<Mat>::iterator base_it = Base_slices.begin(), test_it = Test_slices.begin(); base_it != Base_slices.end(); ++base_it, ++test_it,i++)
	{
		cout << "Comparing Image Slice : " << i << endl;
		int compare_result = imageCompare(*base_it,*test_it);

		if (compare_result == 0)
		{
			stringstream baseimage_file;
			baseimage_file << "C:\\Users\\pukaur\\Documents\\DATA_MINING_PQ\\imageCompare\\difference_output\\Base_Image_" << i << ".jpg";
			//imshow("Base", *base_it);
			imwrite(baseimage_file.str(), *base_it);

			stringstream testimage_file;
			testimage_file << "C:\\Users\\pukaur\\Documents\\DATA_MINING_PQ\\imageCompare\\difference_output\\Test_Image_" << i << ".jpg";
			//imshow("Test", *test_it);
			imwrite(testimage_file.str(), *test_it);
		}

		if (compare_result == 1) block_match++;
		
	}

	cout << "Total Blocks" << i << endl;
	cout << "Blocke Matched" << block_match << endl;
	cout << "Blocke Not Matched Matched" << i - block_match << endl;

	return 0;
}