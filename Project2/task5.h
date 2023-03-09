//
//  task5.h
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/15/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

std::vector<float> getFeatureVectorTask5(cv::Mat &src, int bins);

int makeCsvTask5(const std::vector<cv::Mat> trainingData, int bins);

std::vector<std::pair<int, float>> compareFeaturesTask5(cv::Mat target, int bins, float weight1, float weight2);

float getDistanceTask5(std::vector<float> v1, std::vector<float> v2, float weight1, float weight2);
