//
//  task5.cpp
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/15/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "task5.h"
#include "task2.h"
#include "task3.h"
#include <fstream>


// generate a feature vector which will combine 2 histograms:
// 1. 3D histogram of whole image
// 2. texture histogram of bottom half of the image.
// we will return the vector<float> also used as feature vector of the image
std::vector<float> getFeatureVectorTask5(cv::Mat &image, int bins){
    // get histogram of bottom half of the image
    cv::Mat hist;
    int histSize[] = {bins, bins, bins};
    hist = cv::Mat::zeros(3, histSize, CV_32F);
    getHistogramMatchingFeatureVector(image, hist, bins);
    // get histogram of texture of bottom half of the image
    cv::Mat bottomHalf;
    bottomHalf = image(cv::Rect(0, image.rows / 2, image.cols, image.rows / 2));
    cv::Mat sobelX, sobelY, sobelMag;
    cv::Sobel(bottomHalf, sobelX, CV_32F, 1, 0);
    cv::Sobel(bottomHalf, sobelY, CV_32F, 0, 1);
    cv::magnitude(sobelX, sobelY, sobelMag);
    cv::Mat hist2;
    hist2 = cv::Mat::zeros(3, histSize, CV_32F);
    getHistogramMatchingFeatureVector(sobelMag, hist2, bins);
    // concatenate the two histograms into one feature vector
    std::vector<float> featureVector;
    // we will use the function getFeatureVectorIn1DFloat from task3.cpp
    featureVector = getFeatureVectorIn1DFloat(hist, hist2, bins);
    return featureVector;
}

// make csv file
int makeCsvTask5(const std::vector<cv::Mat> trainingData, int bins)
{
    FILE *fp;
    fp = std::fopen("trainingDataTask5.csv", "w");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }
    // get the feature vectors for each image and store in csv file
    // store (i, feature vector) in csv file
    // where i is the index
    // and feature vector is the feature vector for the image
     for (int i = 0; i < trainingData.size(); i++)
    {
        cv::Mat image;
        image = trainingData[i];
        std::vector<float> featureVector;
        featureVector.clear();
        featureVector = getFeatureVectorTask5(image, bins);
        if(featureVector.size() == 0){
            printf("Something went wrong during getting the feature vector for task 5\n");
            return -1;
        }
        std::string str;
        str = "";
        for (int j = 0; j < featureVector.size(); j++)
        {
            str += std::to_string(featureVector[j]);
            str += " ";
        }
        // write the histFeatureVector to the csv file
        fprintf(fp, "%d,%s\n", i, str.c_str());
    }
     // closing the csv file
    fclose(fp);
    return 0;
}

// compare feature vector of target image with training data from csv file
std::vector<std::pair<int, float>> compareFeaturesTask5(cv::Mat target, int bins, float weight1, float weight2){
    std::vector<std::pair<int, float>> distanceVector;
    distanceVector.clear();
    // get the feature vector of the target image
    std::vector<float> targetFeatureVector;
    targetFeatureVector.clear();
    targetFeatureVector = getFeatureVectorTask5(target, bins);
    if(targetFeatureVector.size() == 0){
        printf("Something went wrong during getting the feature vector of target for task 5\n");
        return distanceVector;
    }
    // read the csv file for training data index,feature vector
    char filename[256];
    std::strcpy(filename, "trainingDataTask5.csv");
    std::fstream csvFile;
    csvFile.open(filename, std::ios::in);
    if (!csvFile.is_open())
    {
        printf("Cannot open file\n");
        return distanceVector;
    }
    std::string line, index, featureVector;
    while(getline(csvFile,line)){
       std::stringstream ss(line);
        getline(ss, index, ',');
        getline(ss, featureVector, ',');
        
        float intersection = 0;
        std::string number = "";
        std::vector<float> trainingFeatureVector;
        trainingFeatureVector.clear();
        // convert the string to a vector of floats
        for (int i = 0; i < featureVector.length(); i++)
        {
            if (featureVector[i] == ' ')
            {
                trainingFeatureVector.push_back(std::stof(number));
                number = "";
            }
            else
            {
                number += featureVector[i];
            }
        }
        
        intersection = getDistanceTask5( trainingFeatureVector, targetFeatureVector, weight1, weight2);
        distanceVector.push_back(std::make_pair(std::stoi(index), intersection));
    }
    csvFile.close();

    return distanceVector;
}


float getDistanceTask5(std::vector<float> v1, std::vector<float> v2, float weight1, float weight2){
    float distance = 0;
    // check if both vectors have the same size
    if (v1.size() != v2.size())
    {
        printf("v1 size is %lu\n", v1.size());
        printf("v2 size is %lu\n", v2.size());
        return distance;
    }
    // calculate the intersection of the two histograms
    // we will add custom weight to the intersection of the two histograms
    // we will add the intersection of the two histograms
    // we will give the first histogram a weight of weight1 and the second histogram a weight of weight2
    float distanceA = 0;
    float distanceB = 0;
    // calculate the intersection of the two histograms
    // first we will do for odd indices
    for (int i = 0; i < v1.size(); i += 2)
    {
        distanceA += std::min(v1[i], v2[i]);
    }
    // then we will do for even indices
    for (int i = 1; i < v1.size(); i += 2)
    {
        distanceB += std::min(v1[i], v2[i]);
    }    
    distance = (distanceA * weight1 + distanceB * weight2);
    
    // add the index and distance to the distance vector
    distance = 1 - distance;
    return distance;
}
