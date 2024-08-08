#include "../Controllers/CameraController/CameraController.h"

#include <windows.h>

int main() {
    try {
        CameraController cameraController;

        // Trying to open any camera
        cameraController.OpenCamera(cv::CAP_ANY);
        std::cout << "Camera status: " << std::boolalpha << cameraController.IsOpen() << std::endl;

        // Starts the capture of camera
        cameraController.StartCapture();

        // Close the camera capture after usage
        cameraController.CloseCameraCapture();
    }
    catch (const std::runtime_error& exception) {
        MessageBoxA(GetConsoleWindow(), exception.what(), "Exception", MB_ICONERROR);
    }
    return 0;
}