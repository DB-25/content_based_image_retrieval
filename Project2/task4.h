//
//  task4.h
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/15/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

std::vector<float> getFeatureVectorTask4(cv::Mat &src, int bins);

int makeCsvTask4(const std::vector<cv::Mat> trainingData, int bins);

std::vector<std::pair<int, float>> compareFeaturesTask4(cv::Mat target, int bins);
