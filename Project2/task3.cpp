//
//  task3.cpp
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/14/23.
//

#include <stdio.h>
#include "task3.h"
#include "task2.h"
#include <opencv2/opencv.hpp>
#include <fstream>


std::vector<float> getFeatureVectorIn1DFloat(cv::Mat &hist1,cv::Mat &hist2, int bins)
{
    std::vector<float> featureVector;
    int i, j, k;
    for(i = 0; i < bins; i++) {
        for(j = 0; j < bins; j++) {
            for(k = 0; k < bins; k++) {
                featureVector.push_back(hist1.at<float>(i, j, k));
                featureVector.push_back(hist2.at<float>(i, j, k));
            }
        }
    }
    return featureVector;
}

// We will create feature vectors for each image by calculating 2 histograms:
// 1 - histogram of top half image
// 2 - histogram of bottom half image
// after which we will combine the two histograms into one feature vector
// and return it to hist.

std::vector<float> getMultiHistogramMatchingFeatureVector(cv::Mat &src, int bins)
{
    // get top half of the image and bottom half
    cv::Mat top, bottom;
    top = src(cv::Rect(0, 0, src.cols, src.rows/2));
    bottom = src(cv::Rect(0, src.rows/2, src.cols, src.rows/2));
    // get the histograms for the top and bottom halves
    cv::Mat topHist, bottomHist;
    int histSize[] = {bins, bins, bins};
    topHist = cv::Mat::zeros(3, histSize, CV_32F);
    bottomHist = cv::Mat::zeros(3, histSize, CV_32F);
    getHistogramMatchingFeatureVector(top, topHist, bins);
    getHistogramMatchingFeatureVector(bottom, bottomHist, bins);
    // concatenate the two histograms into one feature vector
    std::vector<float> featureVector = getFeatureVectorIn1DFloat(topHist, bottomHist, bins);
    return featureVector;
}

// make csv file
int makeCsvTask3(const std::vector<cv::Mat> trainingData, int bins)
{
    FILE *fp;
    fp = std::fopen("trainingDataTask3.csv", "w");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }
    // get the feature vectors for each image and store in csv file
    // store (i, feature vector) in csv file
    int i ;
    for (i = 0; i < trainingData.size(); i++)
    {
        //cv::Mat hist = cv::Mat::zeros(bins,bins, CV_32FC2);
        cv::Mat image = trainingData[i];
        std::vector<float> histFeatureVector = getMultiHistogramMatchingFeatureVector(image, bins);
        if(histFeatureVector.size() == 0){
            printf("Something went wrong during getting the feature vector for task 3\n");
            return -1;
        }
        std::string featureVector;
        featureVector = "";
        for (int j = 0; j < histFeatureVector.size(); j++)
        {
            featureVector += std::to_string(histFeatureVector[j]);
            featureVector += " ";
        }
        // write the histFeatureVector to the csv file
        fprintf(fp, "%d,%s\n", i, featureVector.c_str());
    }
    // closing the csv file
    fclose(fp);
    return 0;
}

// we compare the feature vector of the target image with the feature vectors of the training images
// and return distance vector. we will load the training data from the csv file.
// we calculate the intersection of the two histograms and return the distance vector
// the distance vector is a vector of pairs (index, distance)
std::vector<std::pair<int, float>> compareFeaturesTask3(cv::Mat target, int bins){
     std::vector<std::pair<int, float>> distanceVector;
    distanceVector.clear();
    // read the csv file for training data index,feature vector
    char filename[256];
    std::strcpy(filename, "trainingDataTask3.csv");
    std::fstream csvFile;
    csvFile.open(filename, std::ios::in);
    if (!csvFile.is_open())
    {
        printf("Cannot open file\n");
        return distanceVector;
    }
    std::string line, index, featureVector;
    // make another std::vector of floats for the target histogram
    std::vector<float> histNumbersTarget;
    histNumbersTarget.clear();
    histNumbersTarget = getMultiHistogramMatchingFeatureVector(target, bins);
    while(getline(csvFile,line)){
       std::stringstream ss(line);
        getline(ss, index, ',');
        getline(ss, featureVector, ',');
        
        float intersection = 0;
        std::string number = "";
        std::vector<float> histNumbers;
        histNumbers.clear();
        // convert the string to a vector of floats
        for (int i = 0; i < featureVector.length(); i++)
        {
            if (featureVector[i] == ' ')
            {
                histNumbers.push_back(std::stof(number));
                number = "";
            }
            else
            {
                number += featureVector[i];
            }
        }
        // check if both vectors have the same size
        if (histNumbers.size() != histNumbersTarget.size())
        {
            printf("histNumbers size is %lu\n", histNumbers.size());
            printf("histNumbersTarget size is %lu\n", histNumbersTarget.size());
            return distanceVector;
        }
        // calculate the intersection of the two histograms
        for(int i = 0; i < histNumbers.size(); i++){
            intersection += std::min(histNumbers[i], histNumbersTarget[i]);
        }
        // add the index and distance to the distance vector
        intersection = 2 - intersection; // we subtratct 2 because we have 2 histogram values concatenated to each other.
        distanceVector.push_back(std::make_pair(std::stoi(index), intersection));
    }
    csvFile.close();
    return distanceVector;
}
