#the steps of pre-process the netflix dataset.

# Introduction #

the steps of pre-process the netflix dataset


# Details #

the steps of pre-process the netflix dataset is as follows:
  * make the userId sequential
> (1) scan all the ratings to get a map between original userId and new sequential userId ,and scan all the rating files and transfer the rating to new sequential userId . (./dataset/netflix/getUserSeqId.php)->get new rating files in ./dataset/netflix/transfer\_set/<br />
> > cd ./dataset/netflix/<br />
> > mkdir transfer\_set<br />
> > php getUserSeqId.php  //(this is a script of php, you should have php installed, maybe I will rewrite this programme in c++ soon)
> > _important notice_
> > > because there are 17770 files to deal, the php programme takes a lot of memorys, you'd better change the memory limit of php to more than 1G(it's usaully 16M in default). The steps to change the memory limit: a.  Find the php.ini file: "php -i | grep 'php\.ini'",  b. Change the memory limit: find the memory limit setting section and change it.<br />


> (2) transfer the probe set and training set. (./dataset/netflix/transferProbeUserId.cpp)<br />
> > ./dataset/netflix/probe.txt->./dataset/netflix/probe\_t.txt
  * merge the rating files to one file (./dataset/netflix/mergeData.cpp)
> > ./dataset/netflix/transfer\_set/(files) ->./dataset/netflix/data.txt
  * get the real rate of the probe set (./dataset/netflix/getProbeReal.cpp)
> > ./dataset/netflix/probe\_t.txt->./dataset/netflix/probe\_real.txt

  * get the training dataset without probe ratings(./dataset/netflix/getDataWithoutProbe.cpp)
> > ./dataset/netflix/transfer\_set/(files) ->./dataset/netflix/data\_without\_prob.txt