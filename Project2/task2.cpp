//
//  task2.cpp
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/13/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "task2.h"

int getHistogramMatchingFeatureVector(cv::Mat &src, cv::Mat &hist, int bins)
{
    const int divisor = 256 / bins;
    int i, j, k;
    int histSize[] = {bins, bins, bins}; // dimensions of the matrix
    // allocate memory for the histogram
    hist = cv::Mat::zeros(3, histSize, CV_32F); // # of Dimensions, dimension array, type of data
    // calculate the histogram
    for (i = 0; i < src.rows; i++)
    {
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        for (j = 0; j < src.cols; j++)
        {   
            int r = rptr[j][2] / divisor; // r index
            int g = rptr[j][1] / divisor; // g index
            int b = rptr[j][0] / divisor; // b index
            hist.at<float>(b, g, r) += 1; // increment the value at the index
        }
    }
    // calculate sum
    double sum = 0;
    for (i = 0; i < histSize[0]; i++)
    {
        for (j = 0; j < histSize[1]; j++)
        {
            for (k = 0; k < histSize[2]; k++)
            {
                sum += hist.at<float>(i, j, k);
            }
        }
    }
    // normalize the histogram
    for(i = 0; i < histSize[0]; i++) {
        for(j = 0; j < histSize[1]; j++) {
            for(k = 0; k < histSize[2]; k++) {
                hist.at<float>(i, j, k) = hist.at<float>(i, j, k) / sum;
            }
        }
    }
    
    return 0;
}

// make csv file
int makeCsvTask2(const std::vector<cv::Mat> trainingData, int bins)
{
    FILE *fp;

    fp = std::fopen("trainingDataTask2.csv", "w");
    if (fp == NULL)
    {
        printf("Cannot open file\n");
        return -1;
    }
    // adding the feature vector data of an image in "b g r" to csv file
    for (int i = 0; i < trainingData.size(); i++)
    {
        cv::Mat image = trainingData[i];
        int histSize[] = {bins, bins, bins};
        cv::Mat hist = cv::Mat::zeros(3,histSize , CV_32F);
        getHistogramMatchingFeatureVector(image, hist,bins);
        // write to file in the format of index and feature vector(hist is in Mat format)
        // first calculate the feature vector as a string
        std::string featureVector = "";
        for (int i = 0; i < histSize[0]; i++)
        {
            for (int j = 0; j < histSize[1]; j++)
            {
                for (int k = 0; k < histSize[2]; k++)
                {
                    featureVector += std::to_string(hist.at<float>(i, j, k));
                    featureVector += " ";
                }
            }
        }

        // write to file
        fprintf(fp, "%d,%s\n", i, featureVector.c_str());
    }

    // closing the csv file
    fclose(fp);
    return 0;
}


// compare the feature vector of the target hist with the training data from the csv file
std::vector<std::pair<int, float>> compareFeaturesTask2(cv::Mat hist, int bins){
    std::vector<std::pair<int, float>> distanceVector;
    distanceVector.clear();
    // read the csv file for training data index,feature vector
    char filename[256];
    std::strcpy(filename, "trainingDataTask2.csv");
    std::fstream csvFile;
    csvFile.open(filename, std::ios::in);
    if (!csvFile.is_open())
    {
        printf("Cannot open file\n");
        return distanceVector;
    }
    std::string line, index, featureVector;
    while (getline(csvFile, line))
    {
        std::stringstream ss(line);
        getline(ss, index, ',');
        getline(ss, featureVector, ',');
        
        float intersection = 0;
        std::string number = "";
        std::vector<float> histNumbers;
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
        if (histNumbers.size() != bins*bins*bins)
        {
            printf("histNumbers size is %lu", histNumbers.size());
        }

        // calculate the intersection
        for (int i = 0; i < histNumbers.size(); i++)
        {
            intersection += std::min(histNumbers[i], hist.at<float>(i));
        }
        intersection = 1 - intersection;
        distanceVector.push_back(std::make_pair(std::stoi(index), intersection));
    }
    csvFile.close();
    return distanceVector;
}
