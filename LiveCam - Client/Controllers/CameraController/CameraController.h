#pragma once
#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <opencv2/opencv.hpp>

class CameraController {
public:
	CameraController();
	~CameraController();

	void OpenCamera(int cameraId);

	void StartCapture();
	void CloseCameraCapture();

	bool IsOpen() const;
private:
	bool m_isOpen;
	cv::VideoCapture m_capture;
};

#endif // !CAMERA_CONTROLLER_H
