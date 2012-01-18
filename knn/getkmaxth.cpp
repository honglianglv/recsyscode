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
 * this programme get k-largetst similiar of each item and store it to the file
 *
 * steps:
 * (0)calculate the similarity matrix and store it to the file(getSim.cpp)
 * (1)get the similarity matrix from the file.
 * (2)for every item i，use the minimal heap to get k-max similarity value
 * (3)store the k-max similarity value to file
 *    
 *  
 */
#include "../commonHeader.h"

#define TRAINING_SET "../dataset/movielens/u1.base"
#define RATE_SP " "  //rate Separator
#define USER_NUM 943 //10K:943 1M:6040
#define ITEM_NUM 1682 //10K:1682 1M:3900
#define SIM_FILE "avg.sim"

/*
  #define TRAINING_SET "../dataset/netflix/data_without_prob.txt"
  #define RATE_SP ","  //rate Separator
  #define USER_NUM 480189 //10K:943 1M:6040
  #define ITEM_NUM 17770 //10K:1682 1M:3900
*/
namespace knn{
    vector<float> mi(ITEM_NUM+1,0.0);         //store the mean rate of every item(用来存储每个item的平均打分)
    float w[ITEM_NUM+1][ITEM_NUM+1] = {0};    //item-item similarity matrix(item-item相似矩阵)
    map<int,short> rateMatrix[ITEM_NUM+1];    //use a map to store the sparse rate matrix(使用一个map数组存储稀疏的打分矩阵)
    float mean = 0;                           //mean of all ratings(全局的平均值)
    
    //function declaration    
    float getKmax(vector<float>& array, int K);
    bool cmp(double a, double b);
    
    void getKMaxSim(int K, const char* source="movielens")
    {
        cout << "begin initialization: " << endl;
        char rateStr[256];
        char* pch;    
        vector<float> simArray;
        int itemNum = 0;
        string dst = string(SIM_FILE) + "_kmax";
        std::ifstream from(SIM_FILE);
        ofstream to(dst.c_str());
        if (!from.is_open()) {
            cout << "can't open  operation failed!\n";
            exit(1);
        }
        int itemI = 0, itemJ = 0;
        float sim = 0.0;
        while(from.getline(rateStr,256)){
            string strTemp(rateStr);
            int pos = strTemp.find(":");
            if(-1 != pos) {
                itemJ = itemI; //store the last item
                itemI = atoi(strTemp.substr(0,pos).c_str());
                if(0 == itemJ) {//the first line
                    continue;
                }
                if(0 == itemI ) {
                    cout<<strTemp<<"#####################"<<endl;
                    exit(1);
                }
                float kmaxValue = getKmax(simArray,K); //get the k-max of the last item
                to << itemJ << '\t' << kmaxValue <<endl;
                simArray.clear(); //清空vector,为获取下一个item的k-max similarity做准备    
                ++itemNum;     
                if(itemNum %3000 ==0) {
                    cout<<"read item "<<itemNum<<" sucessfully!"<<endl;
                }
                continue;
            }
            if(strTemp.length() < 3)continue;
            int i = 0;
            pch = strtok (rateStr,RATE_SP);
            while (pch != NULL) {
                if(0 == i) itemJ = atoi(pch);
                else if(1 == i) sim = atof(pch);
                else if(i > 1) break;
                ++i;
                pch = strtok (NULL,RATE_SP);
            }
            if(0 == itemI || 0 == itemJ) {
                cout<<strTemp<<"#####################"<<endl;
                exit(1);
            }        
            simArray.push_back(sim);
        }
        from.close();
        cout<<"end load similarity!"<<endl;
        cout<<"successfully exit!"<<endl;
    }
    
    //下面的这个函数用来利用最小堆找出第k大的相似度
    //get the k-max largest similarity in the array  using the minimum heap
    float getKmax(vector<float>& array, int K)
    {
        int arraySize = array.size();
        if(arraySize < K)return 0.0; //if size < K, then the k-max value is 0
        vector<float> heapTmp;
        for(int i=0; i < array.size(); ++i) {
            heapTmp.push_back(array[i]);
            if(i == K-1) {
                make_heap(heapTmp.begin(),heapTmp.end(),cmp);
            }
            else if(i >=K) {
                push_heap(heapTmp.begin(),heapTmp.end(),cmp);
                pop_heap(heapTmp.begin(),heapTmp.end(),cmp);
                heapTmp.pop_back();
            }
            //cout << i<<'\t'<<heapTmp.size()<<endl;
        }
        return heapTmp.front();
    }
    
    bool cmp(double a, double b)
    {
        return a > b;
    }
};

int main(int argc, char ** argv)
{
    time_t start,end;
    struct tm * timeStartInfo;
    struct tm * timeEndInfo;
    double duration; 
    start = time(NULL);
    timeStartInfo = localtime(&start);
    string timeStartStr = asctime(timeStartInfo);
    knn::getKMaxSim(100);
    end = time(NULL);
    duration = (end-start);
    timeEndInfo = localtime(&end);
    cout << "Start at "<<timeStartStr<<", and end at "<< asctime(timeEndInfo);
    cout << "duration:"<<duration <<" s!" <<endl;
    return 0;
}
