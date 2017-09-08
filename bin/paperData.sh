serIDRegression="paperData"_$(date "+%y_%m_%d_%H_%M_%S")
mkdir -p regressionResult
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo $regressionResult

#######
#part0 my/opensource/newreinsert ci method
#######

for j in 0 1 2
do
	for i in 0 2
	do	
		./executeAll.sh $j 1 500 $i -1
		./calGroovyBenchmark.sh $regressionResult
	done
done

for j in 0 1 2
do	
	for i in 1 9 10
	do
		./executeAll.sh $j 1 500 $i 0.01
		./calGroovyBenchmark.sh $regressionResult
	done
done

#######
#part1 core method
#######

for i in 5 7
do
	./executeAll.sh 2 1 500 $i 0.01
	./calGroovyBenchmark.sh $regressionResult
done

for i in 15 16 17 18
do
	./executeAll.sh 2 1 500 $i 0.01
	./calGroovyBenchmark.sh $regressionResult
done

#######
#part2 reinsert start
#######

for j in 0.0 0.0001 0.001 0.01 0.1 
do
	./bestResult.sh 2 1 $j 2
	./calGroovyBenchmark.sh $regressionResult
done


#######
#part3 batch strategy 
#######

for j in 100 200 300 400 500 1000
do
	./bestResult.sh 2 $j 0.001 2
	./calGroovyBenchmark.sh $regressionResult
done

#######
#part4 best strategy 
#######

./bestResult.sh
./calGroovyBenchmark.sh $regressionResult


#######
#part5 best strategy without ballRadius / degenerate to HDA
#######

./bestResult.sh 2 200 0.001 0
./calGroovyBenchmark.sh $regressionResult

#######
#part6 concurrent method
#######

./bestResult.sh 2 1 0.01 2
./calGroovyBenchmark.sh $regressionResult

./bestResult.sh 1 1 0.01 2
./calGroovyBenchmark.sh $regressionResult

./bestResult.sh 0 1 0.01 2
./calGroovyBenchmark.sh $regressionResult

#find that only real1/real3 will boost for batch 1

./bestResult.sh 2 200 0.001 2
./calGroovyBenchmark.sh $regressionResult
#best here

./bestResult.sh 1 200 0.001 2
./calGroovyBenchmark.sh $regressionResult

#find that no dataset will boost for batch 200


#######
#part7 quick strategy 
#######

./quickResult.sh
./calGroovyBenchmark.sh $regressionResult



#######
#part8 python result hit
#######






