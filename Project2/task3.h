//
//  task3.h
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/14/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

std::vector<float> getMultiHistogramMatchingFeatureVector(cv::Mat &src, int bins);

std::vector<float> getFeatureVectorIn1DFloat(cv::Mat &hist,cv::Mat &hist2, int bins);

int makeCsvTask3(const std::vector<cv::Mat> trainingData, int bins);

std::vector<std::pair<int, float>> compareFeaturesTask3(cv::Mat target, int bins);

