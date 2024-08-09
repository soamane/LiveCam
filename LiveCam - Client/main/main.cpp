#include "../Controllers/Physical/VideoController/VideoController.h"
#include "../Controllers/Physical/AudioController/AudioController.h"

#include <windows.h>

int main() {
    try {
        AudioController audio;

        audio.SetupInputDevice();
        audio.StartRecording(5); 
        Pa_Sleep(5000);  
        audio.Release();

        audio.SetupOutputDevice();
        audio.PlayRecording();  
    }
    catch (const std::runtime_error& exception) {
        MessageBoxA(GetConsoleWindow(), exception.what(), "Exception", MB_ICONERROR);
    }
    return 0;
}