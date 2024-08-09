#include "../Controllers/Physical/VideoController/VideoController.h"

#include <windows.h>

int main() {
    try {
        VideoController video;

        // Trying to open any camera
        video.OpenCamera(cv::CAP_ANY);
        std::cout << "Camera status: " << std::boolalpha << video.IsOpen() << std::endl;

        // Starts the capture of camera
        video.StartCapture();

        // Close the camera capture after usage
        video.StopCapture();
    }
    catch (const std::runtime_error& exception) {
        MessageBoxA(GetConsoleWindow(), exception.what(), "Exception", MB_ICONERROR);
    }
    return 0;
}