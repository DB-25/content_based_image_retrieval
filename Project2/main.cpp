//
//  main.cpp
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/9/23.
//

#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <dirent.h>
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "task5.h"
#include "helperUtil.h"
#include <opencv2/opencv.hpp>

int main(int argc, const char *argv[])
{
  // check for sufficient arguments
  if (argc < 5)
  {
    printf("usage: %s <directory path>\n", argv[0]);
    exit(-1);
  }

  char dirname[256];
  // get the directory path
  strcpy(dirname, argv[1]);
  printf("Processing directory %s\n", dirname);

  // method to get list of file paths for training data
  std::vector<std::string> fileNames;
  fileNames = getImagePathForTraining(dirname);
  if (fileNames.size() == 0)
  {
    return -1;
  }

  // read the training images
  std::vector<cv::Mat> trainingData;
  trainingData = readImages(fileNames);
  if (trainingData.empty())
  {
    printf("Cannot read images\n");
    return -1;
  }

  // read sample training data for task 5
  char task5AdditionalData[256];
  strcpy(task5AdditionalData, argv[4]);
  std::vector<std::string> task5TainingFilesName;
  task5TainingFilesName = getImagePathForTraining(task5AdditionalData);
  if (task5TainingFilesName.size() == 0)
  {
    return -1;
  }
  std::vector<cv::Mat> sampleTrainingDataTask5;
  sampleTrainingDataTask5 = readImages(task5TainingFilesName);
  if (sampleTrainingDataTask5.size() == 0)
  {
    printf("Cannot read task 5 training images\n");
    return -1;
  }


  // reading all target images
  char targetImageDir[256];
  strcpy(targetImageDir, argv[2]);
  std::vector<std::string> targetFileNames;
  targetFileNames = getImagePathForTraining(targetImageDir);
  if (targetFileNames.size() == 0)
  {
    return -1;
  }
  // sort the target file names
  std::sort(targetFileNames.begin(), targetFileNames.end());
  std::vector<cv::Mat> targetData;
  targetData = readImages(targetFileNames);
  if (targetData.size() == 0)
  {
    printf("Cannot read images\n");
    return -1;
  }

  // target image
  cv::Mat targetImage;
  std::vector<std::pair<int, float>> distanceVector;
  distanceVector.clear();

  // handling various tasks
  printf("Task 1: Baseline Matching\n");
  printf("Task 2: Histogram Matching\n");
  printf("Task 3: Multi-histogram Matching\n");
  printf("Task 4: Texture and Color\n");
  printf("Task 5: Custom Method(created to classify Desert image)\n");
  // input task number
  int taskNumber;
  printf("Enter task number: ");
  scanf("%d", &taskNumber);
  // handle switch case
  switch (taskNumber)
  {
  case 1:
  {
    targetImage = targetData[0];
    // making csv file for task 1
    int result = makeCsvTask1(trainingData);
    if (result != 0)
    {
      printf("Something went wrong while making the csv file.");
      return -1;
    }
    // comparing the feature vector of first target image with training data
    distanceVector = compareFeaturesTask1(targetImage);
    if (distanceVector.size() == 0)
    {
      printf("No data in distance vector.\n");
      return -1;
    }
    break;
  }

  case 2:{
   targetImage = targetData[1];
   int bins = 8;
    // making csv file for task 2
    int result = makeCsvTask2(trainingData, bins);
    if (result != 0)
    {
      printf("Something went wrong while making the csv file.");
      return -1;
    }
      // get hist vector of target
    cv::Mat targetHist ;
    getHistogramMatchingFeatureVector(targetImage,targetHist, bins);
      // compare hist with training data
      distanceVector = compareFeaturesTask2(targetHist, bins);
      if (distanceVector.size() == 0)
      {
        printf("No data in distance vector.\n");
        return -1;
      }
    break;
  }

  case 3:{
    targetImage = targetData[2];
    int bins = 8;
    // making csv file for task 3
    int result = makeCsvTask3(trainingData, bins);
    if (result != 0)
    {
      printf("Something went wrong while making the csv file.");
      return -1;
    }
     // compare target image with training data
     distanceVector = compareFeaturesTask3(targetImage, bins);
     if (distanceVector.size() == 0)
     {
       printf("No data in distance vector.\n");
       return -1;
     }
     
    break;
  }
  case 4:{
    targetImage = targetData[3];
    int bins = 8;
    // making csv file for task 4
    int result = makeCsvTask4(trainingData, bins);
    if (result != 0)
    {
      printf("Something went wrong while making the csv file.");
      return -1;
    }
    // compare target image with training data
    distanceVector = compareFeaturesTask4(targetImage, bins);
    if (distanceVector.size() == 0)
    {
      printf("No data in distance vector.\n");
      return -1;
    }
    break;
  }
  case 5:{
      targetImage = targetData[4];
      int bins = 8;
      float weight1 = 0.45;
      float weight2 = 0.55;
      // now lets compare the target image with sample data
      std::vector<float> targetFeatureVector = getFeatureVectorTask5(targetImage, bins);
      if(targetFeatureVector.size() == 0)
      {
        printf("No data in target feature vector for task 5.\n");
        return -1;
      }
      // lets try this model on some sample data
      std::vector<std::vector<float>> distanceVectorSample;
      distanceVectorSample.clear();
      for(int i = 0; i < sampleTrainingDataTask5.size(); i++)
      {
        distanceVectorSample.push_back(getFeatureVectorTask5(sampleTrainingDataTask5[i], bins));
      }
      if(distanceVectorSample.size() == 0)
      {
        printf("No data in distance vector of sample data for task 5.\n");
        return -1;
      }
      
      // now lets compare the target image with sample data
       std::vector<std::pair<int, float>> sampleDistanceVector;
       sampleDistanceVector.clear();
      for(int i = 0; i < distanceVectorSample.size(); i++)
      {
        float distance = 0;
        distance = getDistanceTask5(targetFeatureVector, distanceVectorSample[i], weight1, weight2);
        sampleDistanceVector.push_back(std::make_pair(i, distance));
      }
      // sort the vector of pair based on second value of pair
      sort(sampleDistanceVector.begin(), sampleDistanceVector.end(), sortbysec);
      // lets print the top 3 results
      printf("Top 3 results for sample data:\n");
      for(int i = 0; i < 3; i++)
      {
        printf("Image %d: %f\n", sampleDistanceVector[i].first, sampleDistanceVector[i].second);
      }
      // making csv file for task 5
      int result = makeCsvTask5(trainingData, bins);
      if (result != 0)
      {
        printf("Something went wrong while making the csv file.");
        return -1;
      }
      // compare target image with training data
      distanceVector = compareFeaturesTask5(targetImage, bins, weight1,weight2);
      if (distanceVector.size() == 0)
      {
        printf("No data in distance vector.\n");
        return -1;
      }
      break;
  }
  default:
  {
    printf("Invalid task number.\n");
    return -1;
  }
  }

  // sort the vector of pair based on second value of pair
  sort(distanceVector.begin(), distanceVector.end(), sortbysec);

    printf("%f,%f,%f\n",distanceVector[0].second,distanceVector[1].second,distanceVector[2].second);
  // display target image
  cv::imshow("target image", targetImage);

  // display the top N images from the sorted list, N is the 3rd argument
  int N = atoi(argv[3]);
  for (int i = 0; i < N; i++)
  {
    cv::Mat image = trainingData[distanceVector[i].first];
    std::string windowName;
    windowName = "image " + std::to_string(i);

    cv::imshow(windowName, image);
  }
  cv::waitKey(0);

  printf("Terminating\n");
  return (0);
}
