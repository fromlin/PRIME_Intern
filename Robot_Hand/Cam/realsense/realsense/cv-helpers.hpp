#pragma once

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include <exception>

//<수정 금지>//
#define HUE_8B_MAX            180
#define SAT_OR_VAL_8B_MAX     255

#define HUE_8B_MIN            0
#define SAT_OR_VAL_8B_MIN     0


//<수정 가능>//
#define  HUE_GRAVITY          10
#define  SATURATION_GRAVITY   80
#define  VALUE_GRAVITY        90

using namespace rs2;
using namespace std;
using namespace cv;



class Variable {
public:
	static int track1;
	static int track2;
	static int track3;
	static int track4;
	static int track5;
};

int Variable::track1 = HUE_GRAVITY;
int Variable::track2 = SATURATION_GRAVITY;
int Variable::track3 = VALUE_GRAVITY;
int Variable::track4 = 3; // 수축
int Variable::track5 = 3; // 팽창



class Transf_Color_Area {
	int High1[3], Low1[3], High2[3], Low2[3], key = 0;
	int vaule[3];
	int * _track4 = &Variable::track4,
		*_track5 = &Variable::track5;
	Scalar hsv_Scalar[4];
	unsigned int Transfer_Error[3] = { HUE_GRAVITY, SATURATION_GRAVITY, VALUE_GRAVITY };

	void set_color_Area(int Dimen) {							//Dimen : 0(h),1(s),2(v)    3(모두)
		if ((Dimen > 3) | (Dimen < 0)) {
			cout << "not correct Dimen !!" << endl;
			return;
		}
		if (Dimen == 3) {
			this->set_color_Area(0);
			this->set_color_Area(1);
			this->set_color_Area(2);

			return;
		}

		int high = vaule[Dimen] + Transfer_Error[Dimen];
		int low = vaule[Dimen] - Transfer_Error[Dimen];

		//Hue 설정
		if (Dimen == 0) {
			cout << setfill('0');

			if (low < HUE_8B_MIN) {
				this->key = 1;

				High1[0] = HUE_8B_MAX;
				Low1[0] = low + HUE_8B_MAX;
				High2[0] = high;
				Low2[0] = HUE_8B_MIN;
			}
			else if (high > HUE_8B_MAX) {
				this->key = 2;

				High1[0] = HUE_8B_MAX;
				Low1[0] = low;
				High2[0] = high - HUE_8B_MAX;
				Low2[0] = HUE_8B_MIN;
			}
			else {
				this->key = 3;

				High1[0] = high;
				Low1[0] = low;
				High2[0] = this->High1[0];
				Low2[0] = this->High1[0];
			}
			///범위가 1개 인경우를 else로 지정 범위가 1개임을 표시
			///위 또는 아래로 영역이 넘어간 경우(180또는 0) 범위2개를 이용해서 표기
		}
		//saturation, value 설정
		else {
			if (low < SAT_OR_VAL_8B_MIN) {

				High1[Dimen] = high;
				Low1[Dimen] = SAT_OR_VAL_8B_MIN;

				High2[Dimen] = High1[Dimen];
				Low2[Dimen] = Low1[Dimen];
			}
			else if (high > SAT_OR_VAL_8B_MAX) {

				High1[Dimen] = SAT_OR_VAL_8B_MAX;
				Low1[Dimen] = low;

				High2[Dimen] = High1[Dimen];
				Low2[Dimen] = Low1[Dimen];
			}
			else {

				High1[Dimen] = high;
				Low1[Dimen] = low;

				High2[Dimen] = this->High1[Dimen];
				Low2[Dimen] = this->Low1[Dimen];

			}
		}
	}//set_color_Area End

	void set_vaule_bgr(int b, int g, int r) {
		Scalar sca_hsv = bgr2hsv(b, g, r);

		this->vaule[0] = sca_hsv.val[0];
		this->vaule[1] = sca_hsv.val[1];
		this->vaule[2] = sca_hsv.val[2];
	}

	void set_hsv_Scalar() {
		hsv_Scalar[0] = Scalar(High1[0], High1[1], High1[2]);
		hsv_Scalar[1] = Scalar(Low1[0], Low1[1], Low1[2]);
		hsv_Scalar[2] = Scalar(High2[0], High2[1], High2[2]);
		hsv_Scalar[3] = Scalar(Low2[0], Low2[1], Low2[2]);
	}

public:
	Transf_Color_Area(const int& vaule1, const int& vaule2, const int& vaule3) {	//생성자 > 기준 값 set 후, 색 영역지정
		this->Set_color_bgr(vaule1, vaule2, vaule3);
	}
	Transf_Color_Area(Scalar s) {
		this->Set_color_bgr(s.val[0], s.val[1], s.val[2]);
	}
	void Set_color_bgr(const int& vaule1, const int& vaule2, const int& vaule3) {						//외부 호출용
		printf("                         (b,g,r) : (%.3d, %.3d, %.3d)    \n", vaule1, vaule2, vaule3);
		this->set_vaule_bgr(vaule1, vaule2, vaule3);
		this->set_color_Area(3);
		this->print();
		this->set_hsv_Scalar();

		this->rgb_color_JPG();
	}

	void Set_Gravity(int vaule, int Dimen) {										//Dimen : 0(h),1(s),2(v)    3(모두)
		if ((Dimen > -1) || (Dimen < 3))
			Transfer_Error[Dimen] = vaule;
		else
			std::cout << endl << "error! piz recording!" << endl;
		this->set_color_Area(3);
		this->print();
		this->set_hsv_Scalar();

		this->rgb_color_JPG();
	}


	void print() {
		std::cout
			<< "\t<HSV Area>" << "       (h,s,v) : (" << setw(3) << vaule[0] << ", " << setw(3) << vaule[1] << ", " << setw(3) << vaule[2] << ")" << endl;
		std::cout
			<< "                  hue : " << setw(3) << this->Low1[0] << " ~ 180(0) ~ " << setw(3) << this->High2[0] << endl
			<< "                  sat : " << setw(3) << this->Low1[1] << "  ~  " << setw(3) << this->High1[1] << endl
			<< "                  val : " << setw(3) << this->Low1[2] << "  ~  " << setw(3) << this->High1[2] << endl
			<< "*************************************************" << endl << endl << endl << endl << endl;
	}

	void rgb_color_JPG() const {
		namedWindow("Color", WINDOW_NORMAL);
		moveWindow("Color", 650, 50);
		Mat bgr_color_PNG = imread("color.jpg"),
			img_mask,
			gray_PNG;

		gray_PNG = this->binarization(bgr_color_PNG);

		cvtColor(gray_PNG + 70, img_mask, COLOR_GRAY2BGR);
		img_mask = ~(~img_mask + ~bgr_color_PNG);

		resize(img_mask, bgr_color_PNG, Size(302, 302), 0, 0, INTER_LINEAR);
		cv::imshow("Color", bgr_color_PNG);
	}

	Scalar bgr2hsv(int b, int g, int r) const {
		Mat _bgr_color = Mat(1, 1, CV_8UC3, Scalar(b, g, r)),	//CV_8UC3 >> 8비트 1채널(180도 255 255)
			_hsv_color;

		cvtColor(_bgr_color, _hsv_color, COLOR_BGR2HSV);

		int h = _hsv_color.at<Vec3b>(0, 0)[0];
		int s = _hsv_color.at<Vec3b>(0, 0)[1];
		int v = _hsv_color.at<Vec3b>(0, 0)[2];

		return Scalar(h, s, v);
	}

	Mat binarization(const Mat& _img) const {

		Mat _img_mask1,
			_img_mask2,
			_img_hsv;

		cvtColor(_img, _img_hsv, COLOR_BGR2HSV);

		inRange(_img_hsv,
			this->Low_Scalar(),
			this->High_Scalar(),
			_img_mask1);		//지정한 hsv 범위를 이용하여 영상을 이진화

		if ((this->key == 1) | (this->key == 2)) {
			inRange(_img_hsv,
				this->Low_Scalar(true),
				this->High_Scalar(true),
				_img_mask2);
			_img_mask1 |= _img_mask2;
		}

		//morphological closing 모폴로지 열림연산
		dilate(_img_mask1, _img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(*_track5, *_track5)));
		erode(_img_mask1, _img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(*_track4, *_track4)));

		return _img_mask1;
	}

	bool get_key() {//
		return key;
	}
	Scalar High_Scalar(int key = 0) const {
		return hsv_Scalar[0 + key * 2];
	}
	Scalar Low_Scalar(int key = 0) const {
		return hsv_Scalar[1 + key * 2];
	}
};



// Convert rs2::frame to cv::Mat
static Mat frame_to_mat(const frame& f)
{
	auto vf = f.as<video_frame>();
	const int w = vf.get_width();
	const int h = vf.get_height();

	if (f.get_profile().format() == RS2_FORMAT_BGR8)
	{
		return Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
	}
	else if (f.get_profile().format() == RS2_FORMAT_RGB8)
	{
		auto r_rgb = Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
		Mat r_bgr;
		cvtColor(r_rgb, r_bgr, COLOR_RGB2BGR);
		return r_bgr;
	}
	else if (f.get_profile().format() == RS2_FORMAT_Z16)
	{
		return Mat(Size(w, h), CV_16UC1, (void*)f.get_data(), Mat::AUTO_STEP);
	}
	else if (f.get_profile().format() == RS2_FORMAT_Y8)
	{
		return Mat(Size(w, h), CV_8UC1, (void*)f.get_data(), Mat::AUTO_STEP);
	}
	else if (f.get_profile().format() == RS2_FORMAT_DISPARITY32)
	{
		return Mat(Size(w, h), CV_32FC1, (void*)f.get_data(), Mat::AUTO_STEP);
	}

	throw runtime_error("Frame format is not supported yet!");
}

// Converts depth frame to a matrix of doubles with distances in meters
static Mat depth_frame_to_meters(const depth_frame & f)
{
	Mat dm = frame_to_mat(f);
	dm.convertTo(dm, CV_64F);
	dm = dm * f.get_units();
	return dm;
}

struct TCA_list {
	Transf_Color_Area* _pa = nullptr;
};

class On_track {
public:
	static void track1(int track_vaule, void* userdata) {
		(((TCA_list*)userdata)->_pa)->Set_Gravity(track_vaule, 0);
	}
	static void track2(int track_vaule, void* userdata) {
		(((TCA_list*)userdata)->_pa)->Set_Gravity(track_vaule, 1);
	}
	static void track3(int track_vaule, void* userdata) {
		(((TCA_list*)userdata)->_pa)->Set_Gravity(track_vaule, 2);
	}
	static void track4(int track_vaule, void* userdata) {	}
	static void track5(int track_vaule, void* userdata) {	}
};

//
//void labels(const Mat& cam_imag, const Mat& img_mask, Mat& spectrum)
//{
//	Mat img_labels,
//		stats,
//		cemtrpods;
//	int numoflables,
//		idx;
//}