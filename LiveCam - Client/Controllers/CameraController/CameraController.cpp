#include "CameraController.h"

CameraController::CameraController() : m_isOpen(false) {
}

CameraController::~CameraController() {
    if (this->m_isOpen) {
        this->m_capture.release();
        cv::destroyAllWindows();
    }
}

void CameraController::OpenCamera(int cameraId) {
    this->m_capture = cv::VideoCapture(cameraId);
    if (!this->m_capture.isOpened()) {
        throw std::runtime_error("Failed to open the video camera");
    }
    this->m_isOpen = true;
}

void CameraController::StartCapture() {
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

void CameraController::CloseCameraCapture() {
    if (!this->m_isOpen) {
        throw std::runtime_error("Failed to close camera because it is not opened");
    }

    this->m_isOpen = false;
    this->m_capture.release();
    cv::destroyAllWindows();
}

bool CameraController::IsOpen() const
{
    return this->m_isOpen;
}
