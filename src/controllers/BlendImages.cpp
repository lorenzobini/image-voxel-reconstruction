#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using std::cout;
using std::endl;

int blendImages(void)
{
    // blend frames of background.avi together to create background.png

    String path, camera, fullPath;

    //path = "C:\\Users\\neele\\OneDrive\\Documenten\\Education\\2019-2020 Blok 3\\Computer Vision\\ComputerVision\\VoxelReconstruction\\VoxelReconstruction\\data\\cam";
    path = "C:\\Users\\Lorenzo\\Desktop\\University\\Computer Vision\\Code\\VoxelReconstruction\\VoxelReconstruction\\data\\cam";
    camera = "4";
    fullPath = path + camera;

    Mat src[13];
    Mat result[12];

    //loading video
    VideoCapture video(fullPath + "\\background.avi");
    if (!video.isOpened()) {
        cout << "Error loading video" << endl; return EXIT_FAILURE;
    }

    //retrieving frames
    for (int i = 0;i < 13;i++) {
        video >> src[i];
        if (src[i].empty()) { cout << "Error loading frame " << i << endl; return EXIT_FAILURE; }
    }

    int split;
    double weight;
    // add weighted sum of images, per pair
    for (int i = 0;i < 12;i++) {
        split = i + 2; //number of elements in sequence
        weight = 1.0 / split; //at first run 1/2, at second run 1/3 etc.
        
        addWeighted(src[i], 1.0-weight, src[i+1], weight, 0.0, result[i]);
    }

    // create background image
    imwrite(fullPath + "\\background.png", result[11]);

    return 0;
}