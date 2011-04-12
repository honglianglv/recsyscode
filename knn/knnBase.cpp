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
 * knnBase.cpp contains some common functions of svd model.
 */
#ifndef KNN_KNNBASE_CPP_
#define KNN_KNNBASE_CPP_
namespace knn{
    //use some global variables，s(similarity matrix) 
    double mi[ITEM_NUM+1] = {0.0};             //store the mean rating of each item
    int    biNum[ITEM_NUM+1] = {0};            //the rating num of every item(打过item i分的用户总数)
    float s[ITEM_NUM+1][ITEM_NUM+1] = {0};     //item-item similarity matrix(item-item相似矩阵)
    float kmax[ITEM_NUM+1] = {0};              //k-max array ,the k-max similarity of each item
    //map<int,int> rateMatrix[ITEM_NUM+1];     //use a map array to store the sparse matrix of training-set rating 
    float mean = 0.0;
    
    vector < vector<rateNode> > rateMatrix(USER_NUM+1);   //store training set
    vector<testSetNode> probeRow;                            //store test set
    
    //load the similarity matrix from file
    void loadSimMatrix(float s[ITEM_NUM+1][ITEM_NUM+1], const char* simFile)
    {
        char rateStr[256];
        char* pch;    
        int itemNum = 0;
        std::ifstream from(simFile);
        if (!from.is_open()) {
            cout << "can't open  operation failed!\n";
            exit(1);
        }
        char* separator = "    ";
        int itemI = 0, itemJ = 0;
        float sim = 0.0;
        while(from.getline(rateStr,256)){
            string strTemp(rateStr);
            int pos = strTemp.find(":");
            if(-1 != pos) {
                itemI = atoi(strTemp.substr(0,pos).c_str());
                
                if(0 == itemI ) {
                    cout<<strTemp<<"#####################"<<endl;
                    exit(1);
                }
                ++itemNum;     
                if(itemNum %3000 ==0) {
                    cout<<"read item "<<itemNum<<" sucessfully!"<<endl;
                }
                continue;
            }
            if(strTemp.length() < 3)continue;
            int i = 0;
            pch = strtok (rateStr,separator);
            while (pch != NULL) {
                if(0 == i) itemJ = atoi(pch);
                else if(1 == i) sim = atof(pch);
                else if(i > 1) break;
                ++i;
                pch = strtok (NULL,separator);
            }
            if(0 == itemI || 0 == itemJ) {
                cout<<strTemp<<"#####################"<<endl;
                exit(1);
            }        
            s[itemI][itemJ] = sim;
        }
        from.close();
        cout<<"end load training rate!"<<endl;
    }
    
    //load k-max similarity value from file
    void loadKMax(float kmaxLocal[ITEM_NUM+1], const char* fileName, const char* separator)
    {
        char rateStr[256];
        char* pch;   
        int fileNum = 0;
        std::ifstream from (fileName);
        if (!from.is_open()) {
            cout << "can't open input file!\n";
            exit(1);
        }
            
        int itemId = 0;
        float simTmp = 0.0;
        string strTemp = "";
        
        while(from.getline(rateStr,256)){
            string strTemp(rateStr);
            if(strTemp.length() < 3)continue;
            int i = 0;
            pch = strtok (rateStr,separator);
            while (pch != NULL) {
                if(0 == i) itemId = atoi(pch);
                else if(1 == i) simTmp = atof(pch);
                else if(i > 1) break;
                ++i;
                pch = strtok (NULL,separator);
            }
            if(0 == itemId) {
                cout<<strTemp<<"#####################"<<endl;
                exit(1);
            }
            kmaxLocal[itemId] = simTmp;
        }
        from.close();
        cout<<"read k-max file sucessfully!"<<endl;
        return;
    }
    
    void getItemMean()
    {
        //求mi，即每个item的平均打分
        using namespace knn;
        int i,j;
        for(i = 1; i < USER_NUM+1; ++i){
            int vSize = rateMatrix[i].size();
            for(j=0; j < vSize; ++j) {
                mi[rateMatrix[i][j].item] += rateMatrix[i][j].rate;
                biNum[rateMatrix[i][j].item] += 1;
            }            
        }
        
        for(i = 1; i < ITEM_NUM+1; ++i) {
            if(biNum[i] >=1)mi[i] = mi[i]/biNum[i];
            else mi[i] = mean;
        }
    }
    
    void model(const char* simFile, const char* kmaxFile)
    {
        cout << "begin initialization: " << endl;
        loadSimMatrix(s, simFile);               //load sim matrix
        loadKMax(kmax, kmaxFile,"    ");                //load k-max
        
        loadRating(TRAINING_SET,rateMatrix,RATE_SP);  //load training set
        loadProbe(PROBE_SET,probeRow,RATE_SP);   //load test set
        
        mean = setMeanRating(USER_NUM,rateMatrix); //calculate the mean
        getItemMean();         //calculate the mean of each item
        int i,u,j,k;
        cout <<"begin testRMSEProbe(): " << endl;
        RMSEProbe(probeRow,K_NUM);
        cout <<"end testRMSEProbe(): " << endl;
        return;
    }
};

/**
 * predict the rate
 */
float predictRate(int user, int item, int dim)
{
    using namespace knn;
    
    int RuNum = rateMatrix[user].size(); //the num of items rated by user(用户user打过分的item数目)
    double sumNumerator = 0.0;
    double sumDenominator = 0.0;
    
    for(int p=0; p< RuNum; ++p) {
        int j = rateMatrix[user][p].item;
        int rate = rateMatrix[user][p].rate;
        if( s[item][j] < kmax[item]) continue; //the K of the KNN is here(KNN 的K就体现在这里啦)
        else {
            sumDenominator += fabs(s[item][j]);
            sumNumerator =+ s[item][j] * ( rate - mi[j]);
        }
    }
    float ret =0.0;
    if(sumDenominator > 0.02) ret =  mi[item] + (sumNumerator/sumDenominator);
    else ret =  mi[item];
    if(ret < 1.0) ret = 1;
    if(ret > 5.0) ret = 5;
    return ret;
}
#endif // KNN_KNNBASE_CPP_ 
