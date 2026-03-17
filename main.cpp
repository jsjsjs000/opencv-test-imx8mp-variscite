#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>

int main()
{
	cv::VideoCapture cap(
			"v4l2src device=/dev/video2 ! "
			"video/x-raw,width=1280,height=720,framerate=30/1 ! "
			"videoconvert ! video/x-raw,format=YUY2 ! appsink",  // imxvideoconvert_g2d
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

	std::cout << frame.cols << "x" << frame.rows << " " << 
			frame.type() << " " << frame.channels() << "\n";

	std::string pipeline =
			"appsrc is-live=true do-timestamp=true format=time ! "
			"video/x-raw,width=" + std::to_string(frame.cols) + // format=BGR
					",height=" + std::to_string(frame.rows) + ",framerate=30/1 ! " +
			"videoconvert ! " +
			"waylandsink sync=false window-width=" +
					std::to_string(frame.cols) + " window-height=" + std::to_string(frame.rows);
			// "fpsdisplaysink sync=false video-sink=\"waylandsink sync=false window-width=" +
			// 		std::to_string(frame.cols) + " window-height=" + std::to_string(frame.rows) + "\"";

	cv::VideoWriter out(pipeline, cv::CAP_GSTREAMER, 0, 30, frame.size(), true);

	if (!out.isOpened())
	{
		std::cout << "gstreamer error\n";
		return -1;
	}

	while (true)
	{
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		struct tm *tm_info = localtime(&ts.tv_sec);
		int us = ts.tv_nsec / 1000;

		std::cout << "frame " << us / 1000 << " " << frame.type() << " " << frame.channels() << "\n";

		cap >> frame;
		if (frame.empty())
			break;

		// konwersja do grayscale
		// cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		// // wykrywanie krawędzi
		// cv::Canny(gray, edges, 80, 150);

		// // wykrywanie twarzy
		// std::vector<cv::Rect> faces;
		// face_cascade.detectMultiScale(gray, faces);

		// for (const auto &f : faces)
		// 	cv::rectangle(frame, f, cv::Scalar(0,255,0), 2);

		out.write(frame);

// cv::Mat bgr;
// if(frame.channels() == 1)
//     cv::cvtColor(frame, bgr, cv::COLOR_YUV2BGR_YUY2);
// else
//     bgr = frame;
// out.write(bgr);

		if (cv::waitKey(1) == 27) // ESC
			break;
	}

	return 0;
}
