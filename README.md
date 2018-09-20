# ComplexCi

This project (ComplexCi) mainly focuses on the c++ implementation of Collective Influence (CI) algorithm, which is designed to find the most important node (or influencers) in the Complex Network via optimal percolation developed by:

> Morone F, Makse H A. Influence maximization in complex networks through optimal percolation[J]. Nature, 2015, 524(7563): 65-U122. 

> Morone F, Min B, Bo L, et al. Collective Influence Algorithm to find influencers via optimal percolation in massively large social media[J]. Scientific reports, 2016, 6.  

  

## Algorithm  
  
Overall, the target of CI algorithm is to give a ranking list of nodes according to their importance and the top-ranked nodes will have more importance. We can remove the nodes from the top-ranked ones in the ranking list generated by CI algorithm and calculate the size of giant component after each removal, which will break down the network into many disconnected pieces. The ratio of giant component will reach zero with the one-by-one removal operation finally. Therefore,  the better algorithm,  the sooner the network will collapse to the zero giant component with smaller count of provided nodes.

Users can get the minimal set of influencers of the Complex Network by the C++ program in this repository . Considered that CI algorithms are able to be used in so many scientific fields , I implement this algorithm in modern C++ style code. Compared with the original c code http://www-levich.engr.ccny.cuny.edu/~hernanlab/uploads/CI_HEAP.c mentioned on the http://www-levich.engr.ccny.cuny.edu/webpage/hmakse/software-and-data/ with above paper , the ComplexCi has the following features :

* This project is using the c++ code style of modern oriented object programming with Standard Template Library (STL), which is easier for users to modify than the original c code. After reviewing the original c language code CI_HEAP.c, I believe it is difficult for users to read , maintain and implement it if they have their own idea, especially CI_HEAP.c contains lots of simple variable abbreviation, multilevel pointer, unsafe memory management and unfriendly c style code.
* ComplexCi accepts more input parameters and they can be used in more flexible  behavior of operating nodes in the provided network. 
  * Users can determine the batch size of deleting nodes in ComplexCi per updating CI values when the Complex Network collapses
  * Users can determine the certain giant component ratio when starting re-inserting algorithm which complex network collapses to
  * Users can determine whether they need re-insert operation in the CI algorithm to optimize the result
* Verified that the result of traditional c implementation and new c++ can both achieve the nearly same result in the metric of Robustness, even the new c++ implementation is more efficient and spends much less time on some datasets than the traditional c program. See in the Benchmark Section
* The traditional CI_HEAP.c is merged into this project as well. Users can switch the option to use the new c++ designer or the traditional c style code depending on their taste.

## Usage

This section describes the Usage of the ComplexCi and its corresponding scripts

###  Get Repository

####  Download Release

Binary file compile based on windows 7 x64 is provided on the https://github.com/zhfkt/ComplexCi/releases

#### Or Fetch from Github Source and Compile

Users can directly clone the repository by the git command or just download the zip archiver on the webpage

```bash
git clone https://github.com/zhfkt/ComplexCi.git
```
There are lots of C++11 features and syntax in the code so that the C++ Compiler needs to support C++11 Standard . In fact, there is only one cpp file ComplexCi/ComplexCi.cpp need to be compiled and it doesn’t rely on the other extra library

* Linux

Users can enter into the “bin/” under root project folder and execute 

```bash
g++ -pthread  ../ComplexCi/ComplexCi.cpp  -o ComplexCi -O3 -std=c++11 
```

or just execute

```bash
./make.sh
```

to generate binary bin/ComplexCi . Pls notice that the version of g++ needs to support c++11. For my own dev, the compilation is passed under g++ 5.4.0 on Ubuntu 16.04. Otherwise , you will get failure of several incorrect syntax errors.

* Windows

There has already been a windows binary file x64/Release/ComplexCi.exe compiled on the Windows7 x64 in the repository. It can just be used if users do not need to recompile or change the code. If users want to compile themselves and have the Visual Studio 2013 or higher version, they can directly open the ComplexCi.sln and compile the code in the IDE. The binary file will be generated under x64/Release/ComplexCi.exe .  I believe users can also pick up any other IDE or Compiler supporting C++11 to compile the file ComplexCi/ComplexCi.cpp . Pls note that users need to replace the x64/Release/ComplexCi.exe with their own generate file after compilation.


###  Run

####  Network Input File Format

The data of network can be written in one txt/csv files and the network is considered as undirected network. Each row contains 2 node IDs divided by one comma, which means there is a connection between these 2 nodes. The node IDs should be integer and started from 0. For example:

```
0,1
1,2
2,3
3,4
3,5
...
```

The complete Example is at data/test/karate.csv

####  Easy Start to use Script

Using Script in the project is a quick start to utilize Collective Influence (CI) algorithm. There are 3 scripts users can execute in the project. The project contains both bash scripts for linux and cmd scripts for windows. Bash for linux can be found in the dailyUse\linux and cmd for windows are in the dailyUse\windows

* traditionalCollectiveInfluence (CI_HEAP)

Users can use this script to utilize traditional c style code of http://www-levich.engr.ccny.cuny.edu/~hernanlab/uploads/CI_HEAP.c . I merge it into this repository and set the compatible interface layer to call the c code in c++ .

Script “traditionalCollectiveInfluence” accepts 3 parameters:

```bash
./traditionalCollectiveInfluence.sh <networkPath> <ballRadius> <isPrintMinPointCausingMinComponent>


# "networkPath" is the file path. The file format is described in the section Network Input File Format
# "ballRadius" is the Radius parameter defined in the Collective Influence Algorithm. When the ballRadius is zero, pls notice that Collective Influence Algorithm will degenerate into HDA (high degree adaptive) algorithm. a.k.a. CI value of each node will be equal with degree of the node.
# "isPrintMinPointCausingMinComponent" whether the output file contains limited point leading to 0.01 of giant component ratio or all points. If it is set to 0, the program will output all nodes. Otherwise the program will output partial points, which will make the giant component ratio to 0.01 in the deleting nodes process

```

e.g. 

```bash
./traditionalCollectiveInfluence.sh /home/network/model1.csv 3 1
```

It means users are using the traditionalCollectiveInfluence (CI_HEAP) strategy for the input file model1.csv with parameter ballRadius 3. The output file only contain the partial points, which will make the giant component ratio to 0.01 in the deleting nodes process


* cppCollectiveInfluence

Users can use this script to utilize new c++ implementation of Collective Influence (CI) algorithm.

Script “cppCollectiveInfluence” accepts 4 parameters:

```bash
./cppCollectiveInfluence.sh <networkPath> <ballRadius> <updateBatch> <isPrintMinPointCausingMinComponent>


# "networkPath" is the file path.  The file format is described in the section Network Input File Format
# "ballRadius" is the Radius parameter defined in the Collective Influence Algorithm. When the ballRadius is zero, pls notice that Collective Influence Algorithm will degenerate into HDA (high degree adaptive) algorithm. a.k.a. CI value of each node will be equal with degree of the node.
# "updateBatch" batch size of deleting nodes in Network per updating CI values when the Complex Network collapses
# "isPrintMinPointCausingMinComponent" whether the output file contains limited point leading to 0 of giant component ratio or all points. If it is set to 0, the program will output all nodes. Otherwise the program will output partial points, which will make the giant component ratio to 0 in the deleting nodes process.  i.e. There is no edge but still left point in the network. Pls notice the different behavior compared with traditionalCollectiveInfluence (CI_HEAP)

```

e.g. 

```bash
./cppCollectiveInfluence.sh /home/network/model1.csv 2 500 0
```

It means users are using the cppCollectiveInfluence strategy for the input file model1.csv with parameter ballRadius 2. The output file contain all nodes and 500 nodes will be deleted in a batch per updating CI values when the Complex Network collapses.

* newReinsertCollectiveInfluence (CI_DR)

This script involves the new re-insert algorithm of collective influence (CI_DR). After verified on the 8 datasets, this new algorithm can achieve better performance in the metric of Robustness than original re-insert algorithm in collective influence. See in the benchmark sections.

Script “newReinsertCollectiveInfluence” accepts 4 parameters:

```bash
./newReinsertCollectiveInfluence.sh <networkPath> <ballRadius> <updateBatch> <isPrintMinPointCausingMinComponent>

# "networkPath" is the file path.  The file format is described in the section Network Input File Format
# "ballRadius" is the Radius parameter defined in the Collective Influence Algorithm. When the ballRadius is zero, pls notice that Collective Influence Algorithm will degenerate into HDA (high degree adaptive) algorithm. a.k.a. CI value of each node will be equal with degree of the node.
# "updateBatch" batch size of deleting nodes in Network per updating CI values when the Complex Network collapses
# "isPrintMinPointCausingMinComponent" whether the output file contains limited point leading to 0 of giant component ratio or all points. If it is set to 0, the program will output all nodes. Otherwise the program will output partial points, which will make the giant component ratio to 0 in the deleting nodes process.  i.e. There is no edge but still left point in the network. Pls notice the different behavior compared with traditionalCollectiveInfluence (CI_HEAP)

```

e.g. 

```bash
./newReinsertCollectiveInfluence.sh /home/network/model1.csv 2 500 0
```

It means users are using the newReinsertCollectiveInfluence (CI_DR) strategy for the input file model1.csv with parameter ballRadius 2. The output file contain all nodes and 500 nodes will be deleted in a batch per updating CI values when the Complex Network collapses.


####  Output file

When the algorithm finishes, the output file will be generated in the same folder of the original file with suffix “_out”. Each row in the output file contains nodes, whose count is a group of "outputNumBatch" defined in running ComplexCi binary. If users runs the script, the value "outputNumBatch" will be fixed to 500, for instance：


```
model1,4,5,2,33,…,2214
model1,2432,4554,3222,1123,…,2233
...
```

From the above instance, the nodes of networks model1 will be removed following such order: 4, 5, 2, 33, …, 2214, 2432, 4554, 3222, 1123, …, 2233… . If nodes count % outputNumBatch !=0, some comma will be at the last line of the output.

##  Benchmark

In order to demonstrate the performance of 3 main algorithms mentioned in the scripts traditionalCollectiveInfluence (CI_HEAP), cppCollectiveInfluence and newReinsertCollectiveInfluence (CI_DR) , I provide 8 test datasets downloaded from DataCastle Competition at https://github.com/zhfkt/ComplexCi/releases/download/v0.1/networks.zip . Here the metric of Robustness is another measure to quantify the performance of ranking methods introduced by the paper
	
> Schneider C M, Moreira A A, Andrade J S, et al. Mitigation of malicious attacks on networks[J]. Proceedings of the National Academy of Sciences, 2011, 108(10): 3838-3841. 

The smaller value is, the better the algorithm is.



| traditionalCollectiveInfluence (CI_HEAP)  |  model1               |  model2               |  model3               |  model4               |  real1                 |  real2                |  real3                |  real4                |  total              | 
|---------------------------------|-----------------------|-----------------------|-----------------------|-----------------------|------------------------|-----------------------|-----------------------|-----------------------|---------------------| 
|    ballRadius 0                 |                       |                       |                       |                       |                        |                       |                       |                       |                     | 
| Robustness score                           |  0.2121    |  0.1770  |  0.3484    |  0.1285  |  0.0450  |  0.0902  |  0.1022  |  0.0755  |  1.1793 | 
| time                            |  188s                 |  155s                 |  304s                 |  117s                 |  705s                  |  93s                  |  504s                 |  170s                 |  705s               | 
|    ballRadius 1                 |                       |                       |                       |                       |                        |                       |                       |                       |                     | 
| Robustness score                           |  0.2080  |  0.1732  |  0.3460  |  0.1249  |  0.0409  |  0.0490  |  0.0964  |  0.0658  |  1.1045 | 
| time                            |  200s                 |  157s                 |  341s                 |  121s                 |  597s                  |  69s                  |  727s                 |  173s                 |  727s               | 
|    ballRadius 2                 |                       |                       |                       |                       |                        |                       |                       |                       |                     | 
| Robustness score                           |  0.2078  |  0.1731  |  0.3446  |  0.1188  |  0.0388   |  0.0417  |  0.0955  |  0.0488  |  1.0695 | 
| time                            |  1558s                |  539s                 |  1435s                |  306s                 |  30738s                |  57s                  |  74935s               |  930s                 |  74935s             | 


| cppCollectiveInfluence   |  model1               |  model2               |  model3               |  model4               |  real1                 |  real2                |  real3                |  real4                |  total              | 
|-----------------------------------------|-----------------------|-----------------------|----------------------|-----------------------|------------------------|-----------------------|-----------------------|-----------------------|---------------------| 
|   ballRadius 0    |                       |                       |                      |                       |                        |                       |                       |                       |                     | 
| Robustness score                        |  0.2129    |  0.1782  |  0.3488  |  0.1304  |  0.0458  |  0.0917  |  0.1029  |  0.0751   |  1.1863  | 
| time                                    |  150s                 |  121s                 |  219s                |  91s                  |  203s                  |  90s                  |  173s                 |  144s                 |  219s               | 
|    ballRadius 1   |                       |                       |                      |                       |                        |                       |                       |                       |                     | 
| Robustness score                        |  0.2078  |  0.1728  |  0.3459  |  0.1257  |  0.0407  |  0.0544  |  0.0969  |  0.0653  |  1.1099 | 
| time                                    |  233s                 |  175s                 |  321s                |  128s                 |  500s                  |  62s                  |  637s                 |  167s                 |  637s               | 
|    ballRadius 2   |                       |                       |                      |                       |                        |                       |                       |                       |                     | 
| Robustness score                        |  0.2083  |  0.1732  |  0.3446  |  0.1188  |   0.0386  |  0.0416  |  0.0954  |  0.0492  |  1.0700 | 
| time                                    |  1833s                 |  709s                 |  1674s                |  450s                 |  27263s                  |  57s                  |  43663s                 |  1012s                 |  43663s               | 




| newReinsertCollectiveInfluence (CI_DR)   |  model1               |  model2               |  model3               |  model4               |  real1                 |  real2                |  real3                |  real4                |  total              | 
|-------------------|-----------------------|-----------------------|-----------------------|-----------------------|------------------------|------------------------|-----------------------|------------------------|---------------------| 
| ballRadius 0      |                       |                       |                       |                       |                        |                        |                       |                        |                     | 
| Robustness score  |  0.2100  |  0.1744  |  0.3603  |  0.1174  |  0.0315  |  0.0069  |  0.0978  |  0.0417  |  1.0402  | 
| time              |  149s                 |  127s                 |  204s                 |  92s                  |  163s                  |  102s                  |  162s                 |  133s                  |  204s               | 
|    ballRadius 1   |                       |                       |                       |                       |                        |                        |                       |                        |                     | 
| Robustness score  |  0.2104  |  0.1743  |  0.3655  |  0.1152  |  0.0317  |  0.0046  |  0.0968  |  0.0421   |  1.0409 | 
| time              |  228s                 |  171s                 |  321s                 |  123s                 |  461s                  |  65s                   |  609s                 |  159s                  |  609s               | 
|    ballRadius 2   |                       |                       |                       |                       |                        |                        |                       |                        |                     | 
| Robustness score  |  0.2106  |  0.1739   |  0.3585   |  0.1147  |  0.0303   |  0.0039  |  0.0954  |  0.0369   |  1.0246 | 
| time              |  1841s                |  769s                 |  1679s                |  392s                 |  27375s                |  58s                   |  43405s               |  1009s                 |  43405s             | 


The time doesn't cover IO read/write from/to disk and 8 datasets are all running in parallel on the 4-core cpu machine (Intel Xeon E5-2667v4 Broadwell 3.2 GHz) with linux.

From the benchmark ,we can see that the result of traditional c implementation traditionalCollectiveInfluence (CI_HEAP) and new c++ cppCollectiveInfluence can both achieve nearly the same result in the metric of Robustness, even the new c++ implementation is more efficient and spends much less time on some datasets than the traditional c program. Data structure of disjoint-set is used in the reinsertion in the new c++ implementation ComplexCi and it can boost a lot. The traditionalCollectiveInfluence (CI_HEAP) didn’t use this data structure and I think that’s the reason why the traditional c program was slow.

We can also see that newReinsertCollectiveInfluence (CI_DR) can achieve better Robustness Value result. It can be proved that the newReinsertCollectiveInfluence (CI_DR)  performs well even the ballRadius is set to 0 in the simple HDA (high degree adaptive) algorithm without using Collective Influence algorithm.

##  DataCastle Competition

http://www.pkbigdata.com/common/cmpt/%E5%A4%A7%E5%B8%88%E8%B5%9B_%E7%AB%9E%E8%B5%9B%E4%BF%A1%E6%81%AF.html?lang=en_US

### Competition background

Disparate networks, including social networks, communication networks and biological networks, are playing an increasingly important role on natural and socio-economic systems. A core problem, therein, is to measure the significance of individual nodes. For instance, a super spreader in Hong Kong triggered transmission of SARS to a significantly greater number of other people than 100 normal infected persons;a rumor re-tweeted by a celebrity may spread much broader than that by an obscure person.

Therefore it is necessary to develop a method to identify thevirulence genes in large-scale gene regulatory networks, to find the super-spreaders in large-scale social networks, and to detect the key enterprises with serious systematic financial risk in large-scale financial networks.

Those tasks could be formalized as a generic challenge that is identifying vital nodes in networks that are important for sustaining connectivity. This challenge, aka optimal percolation, is a well-documented issue in network science. With great anticipation of making big progress on this problem, we successfully invited some experts and hope the great participants will create novel and effective solutions.

### How to get Quick/Best Result

Here I write 2 scripts both on the windows and linux platform to help users to get the quick and best results based on the ComplexCi for DataCastle Competition. 

The quickresult script is a quick way to generate the output. Compared with the best result, it doesn’t spend too much time and the result is still competitive and surprisingly in the scale of million. The quickresult script can achieve 1.0287 in the metric of Robustness. 

The bestresult script will get the score of 0.9928 in the best performance in terms of Robustness. However, it will take much longer time to finish in serveral hours.

If users want to experience the quick/best result of the raised algorithm for DataCastle Competition, they can follow the steps:

* Compile the ComplexCi code according to the above instruction on windows/linux or just use the current binary file under windows in the Release zip

* Download zip files networks.zip containing 8 datasets from https://github.com/zhfkt/ComplexCi/releases/download/v0.1/networks.zip and extract it under data/ in the project ComplexCi. That’s say, the path of 8 dataset files will be

```
data/networks/model1.csv
data/networks/model2.csv
data/networks/model3.csv
data/networks/model4.csv
data/networks/real1.csv
data/networks/real2.csv
data/networks/real3.csv
data/networks/real4.csv
```

* quickResult script: 

For the linux, users can execute 

```bash
./quickResult.sh
```

under bin/ folder to get a quick result and wait until finish. The task will be completed in about 5 minutes in serveral million-scale networks on the 4-core machine.

For the windows, users can execute

```cmd
quickResult.cmd
```
under x64/release/ folder and the cmd will pop up 8 separate consoles. Until these consoles all complete the algorithm, users are able to use 

```cmd
mergeResult.cmd 
```
to merge the 8 files into one result file.

* bestResult script: 

For the linux, users are also able to execute 

```bash
./bestResult.sh 
```

under bin/ folder to get the best result but it will take nearly 3 hours to complete the whole algorithm for all datasets. 

For the windows, users can execute 

```cmd
bestResult.cmd
```

 to get a best result, but which will take longer time than linux version because it is not implemented in parallel for 8 datasets.

* Output results will be in a folder under data/networks/results/ for both windows and linux

### Monitor Quick/Best Result on the linux environment

Due to the reason that Quick/Best Result script run in background on the linux environment, users can monitor the background job by the command in the bin folder:

```bash
tail -f "serID".log
```

"serID" will be shown on the script running screen



### Quick/Best Result Benchmark


| quickResult  |  model1               |  model2               |  model3              |  model4               |  real1                 |  real2                 |  real3                |  real4                 |  total              | 
|--------------|-----------------------|-----------------------|----------------------|-----------------------|------------------------|------------------------|-----------------------|------------------------|---------------------| 
| score        |  0.2100  |  0.1744  |  0.3488  |  0.1174  |  0.0315  |  0.0069  |  0.0978  |  0.0417  |  1.0287 | 
| time         |  150s                 |  128s                 |  215s                |  100s                 |  165s                  |  103s                  |  162s                 |  135s                  |  215s               | 


| bestResult  |  model1               |  model2               |  model3               |  model4               |  real1                |  real2                  |  real3               |  real4                 |  total              | 
|-------------|-----------------------|-----------------------|-----------------------|-----------------------|-----------------------|-------------------------|----------------------|------------------------|---------------------| 
| score       |  0.2060  |  0.1698  |  0.3438  |  0.1117  |  0.0306  |  0.0023  |  0.0939  |  0.0356  |  0.9928 | 
| time        |  6057s               |  5114s               |  10145s               |  3278s                |  9621s               |  4013s                  |  4812s              |  5008s                |  10145s             | 

The time doesn't cover IO read/write from/to disk and 8 datasets are all running in parallel on the 4-core cpu machine (Intel Xeon E5-2667v4 Broadwell 3.2 GHz) with linux.

PPT in CCCN2017 (www.cccn2017.top:2017).
https://github.com/zhfkt/ComplexCi/blob/master/Fengkuangtian%20Zhu%20-%20teamID%20zhfkt%20-%20presentation.pptx



## Folder/Files

* ComplexCi: 
  * ComplexCi/ComplexCi.cpp contains main algorithm for Collective Influence Algorithm
  * ComplexCi/CI_HEAP.c contains main algorithm for Collective Influence Algorithm fetched from http://www-levich.engr.ccny.cuny.edu/~hernanlab/uploads/CI_HEAP.c  . I slighlty modify the code in order to make it compatiable with C++ compiler.

* ComplexCiPython:
  * Python code of other algorithms to verify the dataset from DataCastle as the compartive algorithm. 	

* Master_algorithm:
  * Evaluation program in verifying 8 datasets in the DataCastle Competition which is grabbed from http://share.pkbigdata.com/ID.4407/Master_algorithm .

* Bin: 
  * Contains lots of various linux scripts to be used in the different behaivour of controling testing, compiling and running.
	quickResult.sh is the bash script to generate the quick result of DataCastle Competition for 8 datasets
bestResult.sh is the bash script to generate the best result of DataCastle Competition for 8 datasets

* dailyUse:
  * Contains 3 scripts of main algorithms for both linux bash and windows cmd

* data/test:
  * Some test input datasets.

* x64/Release:
  * x64/Release/ComplexCi.exe is the Windows binary file compiled on Windows 7 x64
  * x64/Release/quickResult.cmd is the cmd script to generate the quick result of DataCastle Competition for 8 datasets
  * x64/Release/bestResult.cmd is the cmd script to generate the best result of DataCastle Competition for 8 datasets

* ComplexCi.sln
  * Visual Studio 2013 Project file of ComplexCi.



If you have any question on this project, welcome to file the issue on the github.

## DOI

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.887989.svg)](https://doi.org/10.5281/zenodo.887989)

