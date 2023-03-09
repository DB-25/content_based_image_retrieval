//
//  helperUtil.h
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/13/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

std::vector<std::string> getImagePathForTraining(char dirname[256]);

std::vector<cv::Mat> readImages(std::vector<std::string> fileNames);

bool sortbysec(const std::pair<int, float> &a,
               const std::pair<int, float> &b);
