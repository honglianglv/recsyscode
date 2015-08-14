#the steps to quick start

# Introduction #

the steps to quick start


# Details #
**[English vesion](#English_vesion.md)**

快速使用本project的步骤:
  * 获取源代码方式是使用svn checkout，详情[点击这里](http://code.google.com/p/recsyscode/source/checkout)
  * 认识目录结构
```
    |-common.cpp  //所有程序都可能用到的一些公共函数
     -commonHeader.h //所有程序都可能用到的一些头文件，训练集，测试集节点 结构
     -mlBase.cpp     //movielens 数据集可能用到的一些公共函数，包括load 测试集数据和load 训练集数据这两个函数
     -netflixBase.cpp //netflix 数据集可能用到的一些公共函数，包括load测试集和训练集的两个函数
     -dataset-|
               -movielens-|
                          -u1.base（训练集）
                          -u1.test（测试集）
               -netflix-|
                         -data_without_prob.txt(测试集）
                         -probe_real.txt(测试集）
                         -other files(构造以上两个文件的程序）
     -svd-|
           -svdBase.cpp svd model的一些公共函数，包括svd的所有相关内容
           -svd_ml.cpp  svd model使用movielens的情况，中间有main函数，这个是最终需要执行的文件
           -svd_netflix.cpp svd model 使用netflix的情况，中间有main函数，程序入口
     -svdplusplus(svd++ model,内容从svd类推）
     -asymSvd(asymmetric svd model,内容从svd类推）
     -gNbr(global neighborhood based model,内容从svd类推)
     -baseline(baseline model, 内容从svd类推）
     -combine(asymSvd+svdplusplus,内容从svd类推)
     -knn([knnstep 详情点击这里])
     -stat-|
           -statBase.cpp(统计数据集宏观信息，目前仅仅统计item和user打分数量)
           -stat_ml.cpp(统计movielens数据集的宏观信息)
           -stat_netflix.cpp(统计netflix数据集宏观信息)

```
  * 获得测试数据
    1. movielens 数据可以直接从官方网站下[movielens\_dataset](http://www.grouplens.org/node/73)
    1. netflix 数据官方目前不提供，可以从我的博客上下载[netflix\_dataset](http://www.lifecrunch.biz/archives/207)
    1. 下载后预处理步骤在这里：[netflix数据集预处理步骤](http://code.google.com/p/recsyscode/wiki/netflixpreprocess)
  * 开始试用第一个model-svd
    1. 将代码check out到本地
    1. 下载movielens的数据集，解压缩，并且把u1.test,u1.base这两个文件放入./dataset/movielens/文件夹中
    1. 检查/svd/svd\_ml.cpp define部分参数是否正确
    1. 编译运行
> > > cd ./svd/<br />
> > > g++ svd\_ml.cpp<br />
> > > ./a.out
  * 结果比较

> > 想知道你的结果是否正确，可以参考koren SIGKDD‘08的论文，或者 Ma, C C（Guide to Singular Value Decomposition for Collaborative Filtering）的结果。本人运行的结果在这里：[svd结果](http://code.google.com/p/recsyscode/wiki/SVDResult),[knn结果](http://code.google.com/p/recsyscode/wiki/knnResults)。欢迎大家把自己运行的结果贴上来


# English vesion #
The step of quick use this project:
  * the way to get the source code is "svn checkout"，for detials [click here](http://code.google.com/p/recsyscode/source/checkout)
  * Familiar with the directory structure:
```
    |-common.cpp  //The common functions used by most of the codes
     -commonHeader.h //The common header，including the node structure of training set and test set
     -mlBase.cpp     //the common function of movielens dataset, including the function of loading testing dataset and the function of loading training set
     -netflixBase.cpp //the common function of movielens dataset, including the function of loading testing dataset and the function of loading training set
     -dataset-|
               -movielens-|
                          -u1.base（test set）
                          -u1.test（training set）
               -netflix-|
                         -data_without_prob.txt(training set）
                         -probe_real.txt(test set）
                         -other files(for generating data_without_prob.txt and probe_real.txt[netflixpreprocess click to get detail]）
     -svd-|
           -svdBase.cpp the common function for svd model
           -svd_ml.cpp  use movielens dataset to test the svd model，there is a "main" function in this file and it is the entrance of the programme.
           -svd_netflix.cpp svd model  use movielens dataset to test the svd model，there is a "main" function in this file and it is the entrance of the programme.
     -svdplusplus(svd++ model, similar as svd model）
     -asymSvd(asymmetric svd model, similar as svd model）
     -gNbr(global neighborhood based model, similar as svd model)
     -baseline(baseline model, similar as svd model）
     -combine(asymSvd+svdplusplus, similar as svd model)
     -knn([knnstep details described here])
     -stat-|
           -statBase.cpp(stat the overall info of the dataset. For now, just stat the num of the item rated and the num of the ratings of user)
           -stat_ml.cpp(stat the overall info of movielens dataset)
           -stat_netflix.cpp(stat the overall info of netflix dataset)

```
  * get the test datasets
    1. movielens dataset can get from the offical websit:[movielens\_dataset](http://www.grouplens.org/node/73)
    1. netflix dataset isn't available on netflix offical website, but can get from my blog:[netflix\_dataset](http://www.lifecrunch.biz/archives/207)
    1. The preprocess of netflix dataset is here：[netflix prepocess](http://code.google.com/p/recsyscode/wiki/netflixpreprocess)
  * start to test the first modle: svd modle
    1. check out the code, [click here](http://code.google.com/p/recsyscode/source/checkout) to get details
    1. download the movielens 100K dataset，decompress the dataset，and put the two files u1.test,u1.base into the directory "./dataset/movielens/"
    1. check the define setcion of the file "/svd/svd\_ml.cpp" to make them correct.
    1. complie and run
> > > cd ./svd/<br />
> > > g++ svd\_ml.cpp<br />
> > > ./a.out
  * compare the result:

> > To get whether your result is right or wrong, you can reference the paper of koren in SIGKDD'08，or the paper of  Ma, C C（Guide to Singular Value Decomposition for Collaborative Filtering）。My result of running the model is here: [the result of svd](http://code.google.com/p/recsyscode/wiki/SVDResult),[the result of knn](http://code.google.com/p/recsyscode/wiki/knnResults). Welcome to paste the result of yours.