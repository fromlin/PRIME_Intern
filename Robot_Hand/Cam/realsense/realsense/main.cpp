#include <librealsense2/rs.hpp>
#include <opencv2/dnn.hpp>
#include "cv-helpers.hpp"

#define org			Scalar(76, 81, 164)		//색상 지정
#define ENABLE			1
#define DISABLE			0

#define WIDTH		960
#define HEIGHT		540
#define D_WIDTH		848
#define	D_HEIGHT	480
#define FPS			60

Mat img_mask,
	img_d_mask,
	spectrum = Mat(848, 480, CV_8UC1, 1);
Mat img;



void img_Mouse_Event(int event, int x, int y, int flags, void* userdata)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		uint r, g, b;

		r = img.at<Vec3b>(y, x)[2];
		g = img.at<Vec3b>(y, x)[1];
		b = img.at<Vec3b>(y, x)[0];

		Scalar bgr(b, g, r);
		Scalar hsv;

		Mat mou_color(250, 250, CV_8UC3, Scalar(b, g, r));
		Mat hsv_color;
		cvtColor(mou_color, hsv_color, COLOR_BGR2HSV);

		int h = hsv_color.at<Vec3b>(0, 0)[0];
		int s = hsv_color.at<Vec3b>(0, 0)[1];
		int v = hsv_color.at<Vec3b>(0, 0)[2];
		((Transf_Color_Area*)userdata)->Set_color_bgr(h, s, v);

		cv::imshow("color test", mou_color);
		cv::imshow("hsv test color", hsv_color);

	}
	else if (event == cv::EVENT_RBUTTONDOWN) {
		cout << "            (x, y)        (" << x << ", " << y << ")" << endl;
	}
}



int main(int argc, char * argv[]) try
{
	namedWindow("Trackbar", WINDOW_NORMAL);
	namedWindow("color_test_bgr", WINDOW_AUTOSIZE);
	resizeWindow("Trackbar", 350, 200);
	moveWindow("Trackbar", 1250, 150);
	moveWindow("color_test_bgr", 950, 50);

	Transf_Color_Area tcArea(org);
	TCA_list TCA_list;
	TCA_list._pa = &tcArea;
	setMouseCallback("rs_img", img_Mouse_Event, &TCA_list);

	createTrackbar("hue :", "Trackbar", &Variable::track1, 90, On_track::track1, &TCA_list);
	createTrackbar("sat :", "Trackbar", &Variable::track2, 255, On_track::track2, &TCA_list);
	createTrackbar("val :", "Trackbar", &Variable::track3, 255, On_track::track3, &TCA_list);
	createTrackbar("수축 :", "Trackbar", &Variable::track4, 20, On_track::track4, nullptr);
	createTrackbar("팽창 :", "Trackbar", &Variable::track5, 20, On_track::track5, nullptr);


    // Declare depth colorizer for pretty visualization of depth data
    //colorizer color_map;
    
	config cfg, d_cfg;
	cfg.enable_stream(RS2_STREAM_COLOR, WIDTH, HEIGHT, RS2_FORMAT_BGR8, FPS);
	d_cfg.enable_stream(RS2_STREAM_DEPTH, D_WIDTH, D_HEIGHT, RS2_FORMAT_Z16, FPS);
   
	// Start streaming with own configuration
	pipeline pipe;// , d_pipe;
	pipe.start(cfg);
	//d_pipe.start(d_cfg);

	using namespace cv;
    const auto cam = "Camera";
	const auto binari = "Binarization";
	//const auto d_cam = "Depth";
    namedWindow(cam, WINDOW_AUTOSIZE);
	namedWindow(binari, WINDOW_AUTOSIZE);
	//namedWindow(d_cam, WINDOW_AUTOSIZE);
	moveWindow(cam, 0, 450);
	moveWindow(binari, 950, 450);


    while (waitKey(1) < 0 && getWindowProperty(cam, WND_PROP_FULLSCREEN) >= 0) //getWindowProperty(d_cam, WND_PROP_FULLSCREEN) >= 0 && 
    {
        frameset data = pipe.wait_for_frames();
		//frameset d_data = pipe.wait_for_frames();
        //frame depth = data.get_depth_frame().apply_filter(color_map);
		frame color = data.get_color_frame();
		//frame depth = d_data.get_depth_frame();


        //const int d_w = depth.as<video_frame>().get_width();
        //const int d_h = depth.as<video_frame>().get_height();
		const int w = color.as < video_frame>().get_width();
		const int h = color.as<video_frame>().get_height();

		
        //Mat d_image(Size(w, h), CV_8UC3, (void*)depth.get_data(), Mat::AUTO_STEP);
		Mat image(Size(w, h), CV_8UC3, (void*)color.get_data(), Mat::AUTO_STEP);
		//img_d_mask = TCA_list._pa->binarization(d_image);
		img_mask = TCA_list._pa->binarization(image);
		
		
		//imshow(binari, img_d_mask);
		//imshow(d_cam, d_image);
		imshow(binari, img_mask);
		imshow(cam, image);
    }

    return EXIT_SUCCESS;
}







// print Error
catch (const rs2::error & e)
{
    cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << endl;
    return EXIT_FAILURE;
}

catch (const exception& e)
{
    cerr << e.what() << endl;
    return EXIT_FAILURE;
}
