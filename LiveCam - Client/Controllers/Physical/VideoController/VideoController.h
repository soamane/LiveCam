#pragma once
#ifndef VIDEO_CONTROLLER_H
#define VIDEO_CONTROLLER_H

#include <opencv2/opencv.hpp>

class VideoController {
public:
	VideoController();
	~VideoController();

	void OpenCamera(int cameraId);

	void StartCapture();
	void StopCapture();

	bool IsOpen() const;
private:
	bool m_isOpen;
	cv::VideoCapture m_capture;
};

#endif // !VIDEO_CONTROLLER_H
