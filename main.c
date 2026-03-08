#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>

#include <iostream>

int main()
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
			std::cerr << "Nie można otworzyć kamery\n";
			return -1;
	}

	cv::CascadeClassifier face_cascade;
	if (!face_cascade.load("haarcascade_frontalface_default.xml")) {
			std::cerr << "Nie można załadować klasyfikatora twarzy\n";
			return -1;
	}

	cv::Mat frame, gray, edges;

	while (true) {

		cap >> frame;
		if (frame.empty())
				break;

		// konwersja do grayscale
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		// wykrywanie krawędzi
		cv::Canny(gray, edges, 80, 150);

		// wykrywanie twarzy
		std::vector<cv::Rect> faces;
		face_cascade.detectMultiScale(gray, faces);

		for (const auto &f : faces) {
				cv::rectangle(frame, f, cv::Scalar(0,255,0), 2);
		}

		cv::imshow("kamera", frame);
		cv::imshow("krawedzie", edges);

		if (cv::waitKey(1) == 27) // ESC
				break;
	}

	return 0;
}
