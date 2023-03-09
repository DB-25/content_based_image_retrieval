//
//  task1.h
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/12/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

std::string getBaselineFeatureVector(cv::Mat &src);

float distanceBetweenBaselineFeatureVectors(std::string a, std::string b);

int makeCsvTask1(const std::vector<cv::Mat> trainingData);

std::vector<std::pair<int, float>> compareFeaturesTask1(cv::Mat targetImage);
