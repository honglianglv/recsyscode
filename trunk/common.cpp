/* This file is Copyright (C) 2011 Lv Hongliang. All Rights Reserved.
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

float dot(double* p, double* qLocal,int dim)
{
    double result = 0.0;
    for (int i=1; i<dim+1; ++i){
        result += p[i]*qLocal[i];
    }
    return result;
}
 
/*
 *	this function is the same as function "explode()" in php
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
 * 计算全局的平均值
 */
float setMeanRating(int userNum, vector< vector<rateNode> > rateMatrixLocal)
{
    //计算平均值;
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
 * setRand的值
 */
void setRand(double  p[], int dim, float base)
{
    for(int i=1;i<dim+1;++i){
        double temp = base+get_rand(dim);
        p[i] = temp;
    }
}

/**
 *  将fileName中的数据 load 入 array a中
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

#endif // COMMON_CPP_
