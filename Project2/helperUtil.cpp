//
//  helperUtil.cpp
//  Project2
//
//  Created by Dhruv Kamalesh Kumar on 2/13/23.
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <dirent.h>
#include "helperUtil.h"

std::vector<std::string> getImagePathForTraining(char dirname[256])
{
  std::vector<std::string> fileNames;
  char buffer[256];
  DIR *dirp;
  struct dirent *dp;
  int i;

  // open the directory
  dirp = opendir(dirname);
  if (dirp == NULL)
  {
    printf("Cannot open directory %s\n", dirname);
    fileNames.clear();
    return fileNames;
  }

  i = 0;
  // loop over all the files in the image file listing
  while ((dp = readdir(dirp)) != NULL)
  {

    // check if the file is an image
    if (strstr(dp->d_name, ".jpg") ||
        strstr(dp->d_name, ".png") ||
        strstr(dp->d_name, ".ppm") ||
        strstr(dp->d_name, ".tif"))
    {
      // printf("processing image file: %s\n", dp->d_name);
      //  build the overall filename
      strcpy(buffer, dirname);
      strcat(buffer, "/");
      strcat(buffer, dp->d_name);

      fileNames.push_back(buffer);
    }
  }
  return fileNames;
}

std::vector<cv::Mat> readImages(std::vector<std::string> fileNames)
{
  std::vector<cv::Mat> images;
  for (int i = 0; i < fileNames.size(); i++)
  {
    cv::Mat image = cv::imread(fileNames[i]);
    if (image.data == NULL)
    {
      printf("Cannot read image %s\n", fileNames[i].c_str());
      images.clear();
      return images;
    }
    images.push_back(image);
  }
  return images;
}

bool sortbysec(const std::pair<int, float> &a,
               const std::pair<int, float> &b)
{
  return (a.second < b.second);
}
