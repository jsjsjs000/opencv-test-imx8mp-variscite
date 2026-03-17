#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>

// #define FACE_DETECT

int main()
{
	cv::VideoCapture cap(
			"v4l2src device=/dev/video2 ! "
			"video/x-raw,format=YUY2,width=1280,height=720,framerate=50/1 ! "
			"appsink",
			// "videoconvert ! video/x-raw,format=YUY2 ! appsink",  // imxvideoconvert_g2d
			cv::CAP_GSTREAMER);
	if (!cap.isOpened())
	{
		std::cout << "kamera error\n";
		return -1;
	}

	// cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
	// cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
	// cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('N','V','1','2')); // warning

	cv::CascadeClassifier face_cascade;
	if (!face_cascade.load("haarcascade_frontalface_default.xml"))
	{
		std::cerr << "Nie można załadować klasyfikatora twarzy\n";
		return -1;
	}

	cv::Mat frame, gray, edges;
	cap >> frame;

	printf("%dx%d, type=%d, channels=%d\n", frame.cols, frame.rows,
			frame.type(), frame.channels());

	std::string pipeline =
			"appsrc is-live=true do-timestamp=true format=time ! "
			"video/x-raw,format=BGR,width=" + std::to_string(frame.cols) +
					",height=" + std::to_string(frame.rows) + " ! " +
			"videoconvert ! " +
			// "waylandsink sync=false window-width=" +
			// 		std::to_string(frame.cols) + " window-height=" + std::to_string(frame.rows);
			"fpsdisplaysink sync=false video-sink=\"waylandsink sync=false window-width=" +
					std::to_string(frame.cols) + " window-height=" + std::to_string(frame.rows) + "\"";

	cv::VideoWriter out(pipeline, cv::CAP_GSTREAMER, 0, 30, frame.size(), true);

	if (!out.isOpened())
	{
		std::cout << "gstreamer error\n";
		return -1;
	}

	struct timespec last_ts;
	while (true)
	{
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		struct tm *tm_info = localtime(&ts.tv_sec);
		int us = ts.tv_nsec / 1000;
		int diff_us = (ts.tv_nsec - last_ts.tv_nsec) / 1000;
		if (diff_us < 0)
			diff_us += 1000000;
		memcpy(&last_ts, &ts, sizeof(ts));

		printf("%2ld.%03d s (+%2d ms)\n", ts.tv_sec % 60, us / 1000, diff_us / 1000);

		cap >> frame;
		if (frame.empty())
			break;

#ifdef FACE_DETECT
		// konwersja do grayscale
		// cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		cv::cvtColor(frame, gray, cv::COLOR_YUV2GRAY_YUY2);

		// wykrywanie krawędzi
		cv::Canny(gray, edges, 80, 150);

		// wykrywanie twarzy
		std::vector<cv::Rect> faces;
		face_cascade.detectMultiScale(gray, faces);

		for (const auto &f : faces)
			cv::rectangle(frame, f, cv::Scalar(0,255,0), 2);
#endif

		cv::Mat bgr;
		cv::cvtColor(frame, bgr, cv::COLOR_YUV2BGR_YUY2);
		out.write(bgr);

		if (cv::waitKey(1) == 27) // ESC
			break;
	}

	return 0;
}
