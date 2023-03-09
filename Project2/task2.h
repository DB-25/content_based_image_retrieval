//
//  task2.h
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/13/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

int getHistogramMatchingFeatureVector(cv::Mat &src, cv::Mat &hist, int bins);

int makeCsvTask2(const std::vector<cv::Mat> trainingData, int bins);

std::vector<std::pair<int, float>> compareFeaturesTask2(cv::Mat hist, int bins);
