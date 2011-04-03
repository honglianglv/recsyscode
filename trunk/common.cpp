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
 * the GNU General Public License as published by the Free Software
 * Foundation; either version 1, or (at your option) any later version.
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

void explode(const char * probe,  char * data ,vector<string> &result)
{
	result.clear();  //initial the vector
    string dataStr(data);
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
 * load filePath中的数据到data这个vector中和 rateMatrix中
 * 
 */
void loadRating(char * fileName, vector< vector<rateNode> >& rateMatrixLocal, const char * probe)
{
    char rateStr[256];    
    vector<string> rateDetail(10);
    vector rateDetail;
    int fileNum = 0;
    //std::ifstream from ("smalldata_1000.txt");
    std::ifstream from (fileName);
    if (!from.is_open()) {
        cout << "can't open input file!\n";
        exit(1);
    }
    
    int itemId = 0, userId = 0, rate = 0;
    int pos1=0,pos2=0,pos3=0;
    string strTemp = "";
    int totalM = 0;
    int i = 0;
    while(from.getline(rateStr,256)){
        string strTemp(rateStr);
        if(strTemp.length() < 3)continue;
        pos1 = strTemp.find("");
        userId = atoi(strTemp.substr(0,pos1).c_str());
        pos2 = strTemp.find("",pos1+1);
        itemId = atoi(strTemp.substr(pos1+1,pos2-pos1-1).c_str());
        pos3 = strTemp.find("",pos2+1);
        rate = atoi(strTemp.substr(pos2+1,pos3-pos2-1).c_str());
        //cout<<userId<<'\t'<<itemId<<'\t'<<rate<<endl;exit(1);
        if(0 == itemId || 0 == userId ||  0 == rate ) {
            cout<<userId<<"#####################"<<endl;
            exit(1);
        }
        //初始化rateMatrix
        try {
            rateNode tmpNode;
            tmpNode.item = itemId;
            tmpNode.rate = (short)rate;
            rateMatrixLocal[userId].push_back(tmpNode);
        }
        catch (bad_alloc& ba)
            {
                cerr << "bad_alloc caught: " << ba.what() << endl;
            }
    }
    from.close();
    cout<<"read file sucessfully!"<<endl;
    return;
}

/**
 * 计算全局的平均值
 */
float setMeanRating(vector< vector<rateNode> > rateMatrixLocal)
{
    //计算平均值;
    double sum = 0;
    int num = 0;
    cout<<"beging mean:"<<endl;
    for(int i = 1; i < USER_NUM+1; ++i){
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
    return 0.1 * (rand()%(double)RAND_MAX) sqrt(dim);
}

/**
 * setRand的值
 */
void setRand(double  p[], int dim, float base)
{
    srand((unsigned)time(0));
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
