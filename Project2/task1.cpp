//
//  task1.cpp
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/12/23.
//

#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <dirent.h>
#include "task1.h"
#include <opencv2/opencv.hpp>

const char csvFileName[100] = "trainingDataTask1.csv";

// method to get the feature vector for a given image, using the 9x9 grid
// at the center of the image
std::string getBaselineFeatureVector(cv::Mat &src)
{
    int rows = src.rows;
    int cols = src.cols;
    int x = cols / 2 - 4;
    int y = rows / 2 - 4;
    int width = 9;
    int height = 9;
    cv::Mat grid = src(cv::Rect(x, y, width, height));
    std::string featureVector = "";
    for (int i = 0; i < grid.rows; i++)
    {
        for (int j = 0; j < grid.cols; j++)
        {
            cv::Vec3b pixel = grid.at<cv::Vec3b>(i, j);
            int b = pixel[0];
            int g = pixel[1];
            int r = pixel[2];
            featureVector += std::to_string(b) + " " + std::to_string(g) + " " + std::to_string(r) + " ";
        }
    }
    return featureVector;
}

// method to get the distance between two feature vectors
// distance is the sum-of-squared-difference
float distanceBetweenBaselineFeatureVectors(std::string a, std::string b)
{
    float distance = 0;
    // extract the numbers from the feature vectors
    std::vector<int> aNumbers;
    std::vector<int> bNumbers;
    std::string number = "";
    // extract the numbers from a
    for (int i = 0; i < a.length(); i++)
    {
        if (a[i] == ' ')
        {
            aNumbers.push_back(std::stoi(number));
            number = "";
        }
        else
        {
            number += a[i];
        }
    }
    if (aNumbers.size() != 243)
    {
        printf("aNumbers size is %lu", aNumbers.size());
    }
    number = "";
    // extract the numbers from b
    for (int i = 0; i < b.length(); i++)
    {
        if (b[i] == ' ')
        {
            bNumbers.push_back(std::stoi(number));
            number = "";
        }
        else
        {
            number += b[i];
        }
    }
    if (bNumbers.size() != 243)
    {
        printf("bNumbers size is %lu", bNumbers.size());
    }

    // calculate the distance
    for (int i = 0; i < aNumbers.size(); i++)
    {
        distance += ((aNumbers[i] - bNumbers[i]) * (aNumbers[i] - bNumbers[i]));
    }
    return distance;
}

int makeCsvTask1(const std::vector<cv::Mat> trainingData)
{
    FILE *fp;

    fp = std::fopen("trainingDataTask1.csv", "w");
    if (fp == NULL)
    {
        printf("Cannot open file\n");
        return -1;
    }
    // adding the feature vector data of an image in "b g r" to csv file
    for (int i = 0; i < trainingData.size(); i++)
    {
        cv::Mat image = trainingData[i];
        std::string featureValue;
        featureValue = "";
        featureValue = getBaselineFeatureVector(image);
        fprintf(fp, "%d,%s\n", i, featureValue.c_str());
    }

    // closing the csv file
    fclose(fp);
    return 0;
}

std::vector<std::pair<int, float>> compareFeaturesTask1(cv::Mat targetImage)
{
    std::vector<std::pair<int, float>> distanceVector;
    std::string targetFeatureValue;
    targetFeatureValue = "";
    targetFeatureValue = getBaselineFeatureVector(targetImage);

    // read the csv file for training data index,feature vector
    char filename[256];
    std::strcpy(filename, "trainingDataTask1.csv");
    std::fstream csvFile;
    csvFile.open(filename);
    std::vector<std::string> row;
    std::string line, word;
    while (std::getline(csvFile, line))
    {
        row.clear();
        std::stringstream s(line);
        while (std::getline(s, word, ','))
        {
            row.push_back(word);
        }
        int index = std::stoi(row[0]);
        std::string trainingFeatureValue = row[1];
        float distance = distanceBetweenBaselineFeatureVectors(targetFeatureValue, trainingFeatureValue);
        distanceVector.push_back(std::make_pair(index, distance));
    }
    // close the file
    csvFile.close();
    return distanceVector;
}
