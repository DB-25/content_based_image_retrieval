//
//  task4.cpp
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/15/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "task4.h"
#include "task2.h"
#include "task3.h"

// get Feature Vector for each image of Task 4
// we will create feature vectors for each image by calculating 2 histograms:
// 1 - histogram of whole image (as done in Task 2)
// 2 - histogram of texture of whole image (we will find sobel magnitude and then histogram of it)
// after which we will combine the two histograms into one feature vector
// and return it in the form of a vector of floats. (which will be the feature vector for the image)
std::vector<float> getFeatureVectorTask4(cv::Mat &src, int bins)
{
    // get histogram of whole image
    cv::Mat hist;
    int histSize[] = {bins, bins, bins};
    hist = cv::Mat::zeros(3, histSize, CV_32F);
    getHistogramMatchingFeatureVector(src, hist, bins);
    // get histogram of texture of whole image
    cv::Mat sobelX, sobelY, sobelMag;
    cv::Sobel(src, sobelX, CV_32F, 1, 0);
    cv::Sobel(src, sobelY, CV_32F, 0, 1);
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
int makeCsvTask4(const std::vector<cv::Mat> trainingData, int bins)
{
    FILE *fp;
    fp = std::fopen("trainingDataTask4.csv", "w");
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
        featureVector = getFeatureVectorTask4(image, bins);
        if(featureVector.size() == 0){
            printf("Something went wrong during getting the feature vector for task 4\n");
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
std::vector<std::pair<int, float>> compareFeaturesTask4(cv::Mat target, int bins){
    std::vector<std::pair<int, float>> distanceVector;
    distanceVector.clear();
    // get the feature vector of the target image
    std::vector<float> targetFeatureVector;
    targetFeatureVector.clear();
    targetFeatureVector = getFeatureVectorTask4(target, bins);
    if(targetFeatureVector.size() == 0){
        printf("Something went wrong during getting the feature vector of target for task 4\n");
        return distanceVector;
    }
    // read the csv file for training data index,feature vector
    char filename[256];
    std::strcpy(filename, "trainingDataTask4.csv");
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
        // check if both vectors have the same size
        if (trainingFeatureVector.size() != targetFeatureVector.size())
        {
            printf("trainingFeatureVector size is %lu\n", trainingFeatureVector.size());
            printf("targetFeatureVector size is %lu\n", targetFeatureVector.size());
            return distanceVector;
        }
        // calculate the intersection of the two histograms
        for(int i = 0; i < trainingFeatureVector.size(); i++){
            intersection += std::min(trainingFeatureVector[i], targetFeatureVector[i]);
        }
        // add the index and distance to the distance vector
        intersection = 2 - intersection; // we subtratct 2 because we have 2 histogram values concatenated to each other.
        distanceVector.push_back(std::make_pair(std::stoi(index), intersection));
    }
    csvFile.close();

    return distanceVector;
}
