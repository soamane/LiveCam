#include "VideoController.h"

VideoController::VideoController() : m_isOpen(false) {
}

VideoController::~VideoController() {
    if (this->m_isOpen) {
        this->m_capture.release();
        cv::destroyAllWindows();
    }
}

void VideoController::OpenCamera(int cameraId) {
    this->m_capture = cv::VideoCapture(cameraId);
    if (!this->m_capture.isOpened()) {
        throw std::runtime_error("Failed to open the video camera");
    }
    this->m_isOpen = true;
}

void VideoController::StartCapture() {
    if (!this->m_isOpen) {
        throw std::runtime_error("Video camera object is not opened");
    }

    const std::string windowName = "Camera";
    cv::namedWindow(windowName);

    while (true) {
        cv::Mat frame;

        if (!this->m_capture.read(frame)) {
            this->m_isOpen = false;
            throw std::runtime_error("Failed to read frame from camera");
        }

        cv::imshow(windowName, frame);

        if (cv::waitKey(30) >= 0) {
            break;
        }
    }

    cv::destroyWindow(windowName);
}

void VideoController::StopCapture() {
    if (!this->m_isOpen) {
        throw std::runtime_error("Failed to close camera because it is not opened");
    }

    this->m_isOpen = false;
    this->m_capture.release();
    cv::destroyAllWindows();
}

bool VideoController::IsOpen() const
{
    return this->m_isOpen;
}
