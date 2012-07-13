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
 * this file contains some  General Function use in most model
 */
#ifndef COMMON_CPP_
#define COMMON_CPP_

template <class myType> 
int partition(myType array[], int start,int end);

float dot(double* p, double* qLocal,int dim)
{
    double result = 0.0;
    for (int i=1; i<dim+1; ++i){
        result += p[i]*qLocal[i];
    }
    return result;
}
 
/*
 *    this function is the same as function "explode()" in php
 */

void explode(const char * probe,  string dataStr ,vector<string> &result)
{
    result.clear();  //initial the vector
    int pos1 = 0;
    int pos2 = 0;
    int probeLength = strlen(probe);
    while((pos2=dataStr.find(probe,pos1)) != string::npos){
        result.push_back(dataStr.substr(pos1,pos2-pos1));
        pos1=pos2 + probeLength;
    }
    result.push_back(dataStr.substr(pos1));
}

/**
 * calculate the mean of all the ratings
 */
float setMeanRating(int userNum, vector< vector<rateNode> > rateMatrixLocal)
{
    //calculate the mean
    double sum = 0;
    int num = 0;
    cout<<"beging mean:"<<endl;
    for(int i = 1; i < userNum+1; ++i){
        for(int j=0; j < rateMatrixLocal[i].size(); ++j) {
            sum += rateMatrixLocal[i][j].rate;
            ++num;
        }
    }
    cout<<"end mean!mean:"<<(sum/num)<<endl;
    return sum/num;
}

double get_rand(int dim)
{
    return 0.1 * (rand()/(double)RAND_MAX) / sqrt(dim);
}

/**
 * set the vector to random values
 */
void setRand(double  p[], int dim, float base)
{
    for(int i=1;i<dim+1;++i){
        double temp = base+get_rand(dim);
        p[i] = temp;
    }
}

/**
 *  load the data in the fileName to array a
 */
void loadArray(float a[],char* fileName,int length)
{
    char rateStr[256];    
    std::ifstream in(fileName,ifstream::in);
    int i = 1;
    int id,pos1;
    double value;
    while(in.getline(rateStr,256) && i < length) {
        string strTemp(rateStr);
        if(strTemp.length() < 3)continue;
        pos1 = strTemp.find("");
        id = atoi(strTemp.substr(0,pos1).c_str());
        value = atof(strTemp.substr(pos1+1).c_str());
        a[id] = value;
        ++i;
    }
    in.close();
}

//compute the rmse of test set
double RMSEProbe(vector<testSetNode>& probeSet,int dim)
{
    int probeSize = probeSet.size();
    float pRate,err;
    long double rmse = 0;
    
    for(int i = 0; i < probeSize; ++i) {
        pRate = predictRate(probeSet[i].user,probeSet[i].item,dim);      //predict rate
        //cout<<pRate<<"    "<<probeSet[i].rate<<"    "<<probeSet[i].user<<"    "<<probeSet[i].item<<endl;
        err = pRate-probeSet[i].rate;
        rmse += err*err;
    }
    rmse = sqrt( rmse / probeSize);
    cout<<"RMSE:"<<rmse<<" probeNum:"<<probeSize<<endl;
    return rmse;
}

//quick sort 
template <class myType>
void qsort(myType array[], int start, int end)
{
    if(start >= end)return;
    //cout<<"begin partition:"<<endl;
    int q = partition(array,start,end);
    //cout<< q<<endl;
    qsort(array,start,q-1);
    qsort(array,q+1,end);
}

//partition function for quick sort
template <class myType>
int partition(myType array[], int start,int end)
{
    if(start >= end) return start;
    if(start < 0) start = 0;
    int original  = start;
    float tmp = array[start];
    ++start;
    while(1) {
        while (start<=end && array[start] >= tmp) ++start;
        while (start<=end && array[end] < tmp) --end;
        if(start < end) {
            swap(array[start],array[end]);
        }
        else break;
    }
    swap(array[start-1],array[original]);
    return start-1;
}
#endif // COMMON_CPP_
