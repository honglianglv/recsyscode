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
 * 本程序的目的是实现koren在SIGKDD'08论文中的SVD方法, 使用的数据是netflix的数据
 */

#include "../commonHeader.h"
#include "../common.cpp"
#define USER_NUM 480189  //10K:943 1M:6040
#define ITEM_NUM 17770 //10K:1682 1M:3900
#define K_NUM  30
#define TRAINING_SET "../dataset/netflix/data_without_prob.txt"
#define PROBE_SET "../dataset/netflix/probe_real.txt"
#define RATE_SP ","  //rate Separator



namespace svd{
    //使用一些全局变量，存储需要估计的参数, bu, bi, p, q
    double bu[USER_NUM+1] = {0};
    double bi[ITEM_NUM+1] = {0};       //baseline预测器中的用户偏置和item偏置
    
    int buNum[USER_NUM+1] = {0};    //用户u打分的item总数，
    int biNum[ITEM_NUM+1] = {0};   //打过item i分的用户总数
    
    double p[USER_NUM+1][K_NUM+1] = {0};   //用于存储用户的属性描述p
    double q[ITEM_NUM+1][K_NUM+1] = {0};   //用于item的属性描述q
    
    vector < vector<rateNode> > rateMatrix(USER_NUM+1);   //store training set
    vector<testSetNode> probeRow;                            //store test set
    float mean = 0;                         //全局的平均值
    
    //函数声明
    double RMSEProbe(vector<testSetNode>& probeSet);
    float predictRate(int user,int item);
    void loadRating(char * fileNam, vector< vector<rateNode> >& rateMatrixLocal, const char* probe);
    void loadProbe(char * fileName,vector<testSetNode>& probeSet, const char* separator);
    
    void model(int dim, float  alpha1, float alpha2, float beta1, float beta2)
    {
        cout << "begin initialization: " << endl;
        
        loadRating(TRAINING_SET,rateMatrix,RATE_SP);  //load training set
        loadProbe(PROBE_SET,probeRow,RATE_SP);   //load test set
        
        mean = setMeanRating(USER_NUM,rateMatrix); //求平均值，求bu和bi的值
        
        
        int i,u,j,k;
        
        srand((unsigned)time(0));
        //@TODO 不知道是否能针对初始化的过程做一些优化
        //对p,q进行初始化，初始化的方法是随机函数，不知道这种方法是否好，是否会影响结果？？？？？？？
        for(int i = 1; i < ITEM_NUM+1; ++i){
            setRand(q[i],K_NUM,0);    //初始化q[i]
        }
        
        for(int i = 1; i < USER_NUM+1; ++i){
            setRand(p[i],K_NUM,0);    //初始化p[i]
        }
       
        cout <<"initialization end!"<<endl<< "begin iteration: " << endl;
        
        float pui = 0.0 ; // 预测的u对i的打分
        double preRmse = 1000000000000.0; //用于记录上一个rmse，作为终止条件的一种，如果rmse上升了，则停止
        double nowRmse = 0.0;
        
        cout <<"begin testRMSEProbe(): " << endl;
        RMSEProbe(probeRow);
        
        //main loop
        for(int step = 0; step < 60; ++step){  //只迭代60次
            long double rmse = 0.0;
            int n = 0;
            for( u = 1; u < USER_NUM+1; ++u) {   //循环处理每一个用户 
                
                int RuNum = rateMatrix[u].size(); //用户u打过分的item数目
                float sqrtRuNum = 0.0;
                if(RuNum>1) sqrtRuNum = (1.0/sqrt(RuNum));
                   
                //迭代处理
                for(i=0; i < RuNum; ++i) {// 循环处理u打分过的每一个item
                    int itemI = rateMatrix[u][i].item;
                    short rui = rateMatrix[u][i].rate; //实际的打分
                    double bui = mean + bu[u] + bi[itemI];
                    pui = predictRate(u,itemI);
                    
                    float eui = rui - pui;
                    
                    if( isnan(eui) ) {// fabs(eui) >= 4.2 || 
                        cout<<u<<'\t'<<i<<'\t'<<pui<<'\t'<<rui<<"    "<<bu[u]<<"    "<<bi[itemI]<<"    "<<mean<<endl;
                        //printArray(q[itemI],p[u],K_NUM+1);
                        exit(1);
                    }
                    rmse += eui * eui; ++n;
                    if(n % 10000000 == 0)cout<<"step:"<<step<<"    n:"<<n<<" dealed!"<<endl;
                    
                    bu[u] += alpha1 * (eui - beta1 * bu[u]);
                    bi[itemI] += alpha1 * (eui - beta1 * bi[itemI]);
                    
                    for( k=1; k< K_NUM+1; ++k) {
                           //double tempPu = p[u][k];
                           p[u][k] += alpha2 * (eui*q[itemI][k] - beta2*p[u][k]);
                           q[itemI][k] += alpha2 * (eui*p[u][k] - beta2*q[itemI][k]);
                       }
                } 
            }
            nowRmse =  sqrt( rmse / n);
            
            if( nowRmse >= preRmse && step >= 3) break; //如果rmse已经开始上升了，则跳出循环
            else
                preRmse = nowRmse;
            cout << step << "\t" << nowRmse <<'\t'<< preRmse<<"     n:"<<n<<endl;
            RMSEProbe(probeRow);;  // check test set rmse
            
            alpha1 *= 0.9;    //逐步减小学习速率
            alpha2 *= 0.9;
        }
        RMSEProbe(probeRow);  // 检查训练集情况
        return;
    }
    
   /**
	 * load filePath中的数据到data这个vector中和 rateMatrix中
	 * 
	 */
	void loadRating(char * fileName, vector< vector<rateNode> >& rateMatrixLocal, const char* separator)
	{
	    char rateStr[256];
	    char* pch;    
	    vector<string> rateDetail;
	    int fileNum = 0;
	    std::ifstream from (fileName);
	    if (!from.is_open()) {
	    	cout << "can't open  operation failed!\n";
	    	exit(1);
	  	}
	    
	    int itemId = 0, userId = 0, rate = 0;
	    while(from.getline(rateStr,256)){
	    	string strTemp(rateStr);
			int pos = strTemp.find(":");
		    if(-1 != pos) {
		    	itemId = atoi(strTemp.substr(0,pos).c_str());
		    	if(0 == itemId ) {
		    		cout<<strTemp<<"#####################"<<pos<<"####"<<strTemp.substr(0,pos).c_str()<<endl;
		    		exit(1);
		    	}		
		    	 ++fileNum;	 
		    	if(fileNum %3000 ==0) {
		    		cout<<"read file "<<fileNum<<" sucessfully!"<<endl;
		    	}
		    	continue;
		    }
	    	if(strTemp.length() < 3)continue;
	    	int i = 0;
	    	pch = strtok (rateStr,separator);
		    while (pch != NULL) {
		        if(0 == i) userId = atoi(pch);
		        else if(1 == i) rate = atoi(pch);
		        else if(i > 1) break;
		        ++i;
		        pch = strtok (NULL,separator);
		  	}
	    	if(0 == itemId || 0 == userId ||  0 == rate || itemId > ITEM_NUM) {
	    		cout<<strTemp<<"#####################"<<endl;
	    		exit(1);
	    	}		
	    	//initialization rateMatrix
	    	try {
	    		rateNode tmpNode;
	    		tmpNode.item = itemId;
	    		tmpNode.rate = (short)rate;
	    		rateMatrixLocal[userId].push_back(tmpNode);
	    	}
	    	catch (bad_alloc& ba) {
	    		cerr << "bad_alloc caught: " << ba.what() << endl;
	    		cout << "Can't allocate the momery!" << endl; exit(1);
	    	}
	    }
	    from.close();
	   	cout<<"read file sucessfully!"<<endl;
	    return;
	}
    
    float predictRate(int user,int item)
    {
        int RuNum = svd::rateMatrix[user].size(); //用户u打过分的item数目
        double ret; 
        if(RuNum > 1) {
            ret = mean + bu[user] + bi[item] +  dot(p[user],q[item],K_NUM);//这里先不对k进行变化，先取k=无穷大
        }
        else ret  = mean+bu[user] + bi[item];
        if(ret < 1.0) ret = 1;
        if(ret > 5.0) ret = 5;
        return ret;
    }
    
    //load test set
    void loadProbe(char * fileName,vector<testSetNode>& probeSet, const char* separator)
    {
        ifstream in(fileName);
        if (!in.is_open()) {
            cout << "can't open test set file!\n";
            exit(1);
        }
        char rateStr[256];
        char* pch ; // store a token of a string

        string strTemp;
        int rateValue,itemId,userId,probeNum=0;
        
        while(in.getline(rateStr,256)){
            strTemp = rateStr;
            if(strTemp.length() < 4) continue;
            int i = 0;
            pch = strtok (rateStr,separator);
		    while (pch != NULL) {
		        if(0 == i) itemId = atoi(pch);
		        else if(1 == i) userId = atoi(pch);
		        else if(2 == i) rateValue = atoi(pch);
		        else if(i > 2) break;
		        ++i;
		        pch = strtok (NULL,separator);
		  	}
		  	try {
	    		testSetNode tmpNode;
	    		tmpNode.item = itemId;
	    		tmpNode.rate = (short)rateValue;
	    		tmpNode.user = userId;
	    		probeSet.push_back(tmpNode);
	    		 ++probeNum;
	    	}
	    	catch (bad_alloc& ba) {
	    		cerr << "bad_alloc caught: " << ba.what() << endl;
	    		cout << "Can't allocate the momery!" << endl; exit(1);
	    	}
        }
        cout << "Load " << probeNum << " test ratings successfully!"<<endl;
        in.close(); 
    }
    
    //检查测试集情况
    double RMSEProbe(vector<testSetNode>& probeSet)
    {
        int probeSize = probeSet.size();
        float pRate,err;
        long double rmse = 0;
        
        for(int i = 0; i < probeSize; ++i) {
            pRate = predictRate(probeSet[i].user,probeSet[i].item);      //predict rate
            err = pRate-probeSet[i].rate;
            rmse += err*err;
        }
        rmse = sqrt( rmse / probeSize);
        cout<<"RMSE:"<<rmse<<" probeNum:"<<probeSize<<endl;
        return rmse;
    }
};

int main(int argc, char ** argv)
{
    time_t start,end;
    struct tm* startInfo;
    struct tm* endInfo;
    double duration;
    start = time(NULL);
    startInfo = localtime(&start);
    string startStr = asctime(startInfo);
    float alpha1 = 0.008;    //according suggestion of xlvector
    float alpha2 = 0.008;    //according suggestion of xlvector
    float beta1  = 0.01;     //according suggestion of xlvector
    float beta2  = 0.01;     //according suggestion of xlvector
    
    //for(int i=0; i < 10; i++)
    {
    //    beta = i*0.001 + 0.002;
    //    cout << beta << endl;
        svd::model(K_NUM,alpha1,alpha2,beta1,beta2);
        
    }
    end = time(NULL);
    duration = end-start;
    endInfo =   localtime(&end);
    cout << "start at" << startStr << ". And end at "<< asctime(endInfo) <<endl;
    cout << "duration:"<<duration <<" s!" <<endl;
    return 0;
}
