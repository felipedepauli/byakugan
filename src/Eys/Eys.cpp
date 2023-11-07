/**
* @file Eys.hpp
* @brief The Eys Server Header File.
*
* This file is part of the Evil Eyes project:
* - Website: http://www.aincrivelfabrica.com.br
* - GitHub: https://github.com/felipedepauli/evil-eyes
*
* License:
* Copyright (C) 2023 Felipe Camargo de Pauli
* All Rights Reserved.
*
* This source code is provided for viewing purposes only. No part of this source code
* may be used, modified, distributed, or sublicensed for any purpose without the
* express written consent of the copyright holder.
*/


#include "Eys.hpp"


// Constructor: Display a message indicating that the "eyes" are opening.
Eys::Eys(bool debug=false) : debug_(debug), frameInterval_(std::chrono::milliseconds(1000/30)) {
    std::cout << "[Eys::Info] Creating eyes." << std:: endl;
    nextFrameTime_ = std::chrono::steady_clock::now() + frameInterval_;
}

// Destructor: Display a message indicating that the "eyes" are closing.
Eys::~Eys() {
    std::cout << "[Eys::Info] Closing eyes." << std:: endl;
}

// Open the camera using the GStreamer pipeline.
int Eys::openEyes() {

    if (false) {
        std::cout << "Debugging mode activated" << std::endl;
        cap.open("./debug.mp4");
    } else {
        std::cout << "[Eys::Info] Opening eyes!" << std::endl;
        cap.open(0);
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_BUFFERSIZE, 1);

    // Ajusta a taxa de quadros da câmera
    cap.set(cv::CAP_PROP_FPS, 5);

    // Ajusta o brilho da câmera
    // cap.set(cv::CAP_PROP_BRIGHTNESS, 0.5); // Parece que está dando problema (tela preta)


    // Check if the camera was successfully opened.
    // If not, print an error message and return -1, which will terminate the program immediately.
    if (!cap.isOpened()) {
        std::cerr << "[Eys::Error] Error opening eyes..." << std::endl;
        return -1;
    } else {
        std::cout << "[Eys::Info] Eyes opened" << std::endl;
    }
    return 0;
}


bool Eys::isOpened() {
    return cap.isOpened();
}

cv::Mat Eys::spark() {
    cv::Mat frame;
    while (true) {
        auto now = std::chrono::steady_clock::now();
        if (now >= nextFrameTime_) {
            // Capture the frame when it's time
            cap >> frame;
            if (!debug_) {
                cv::flip(frame, frame, -1);
            }
            // Calculate the time for the next frame
            nextFrameTime_ += frameInterval_;
            break;  // Break the loop as the frame has been captured
        } else {
            // Discard the frame as it's not the right time yet
            cap.grab();
        }
    }
    return frame;
}

