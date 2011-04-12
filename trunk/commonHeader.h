/* Copyright (C) 2011 Lv Hongliang. All Rights Reserved.
 * please maitain the copyright information completely when you redistribute the code.
 * 
 * If there are some bugs, please contact me via email honglianglv@gmail.com or submit the bugs 
 * in the google code project http://code.google.com/p/recsyscode/issues/list
 * 
 * my blog: http://lifecrunch.biz
 * my twitter: http://twitter.com/honglianglv
 * my google profile:https://profiles.google.com/honglianglv/about?hl=en
 *
 * It is free software; you can redistribute it and/or modify it under 
 * the license GPLV3.
 *
 * this file contains some  General Function include lib 
 */
#ifndef COMMONHEADER_H_
#define COMMONHEADER_H_

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <math.h>
#include <map>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <dirent.h>
#include <malloc.h>
#include <errno.h>
#include <time.h>

using namespace std;

struct rateNode
{
    short item;
    short rate;
};

//use different struct between test set and training set to save memory space
//when the training set is very large( eg netflix dataset), the saving space is significant
struct testSetNode
{
    int user;
    short item;
    short rate;
};
float predictRate(int user,int item,int dim);
#endif // COMMONHEADER_H_
