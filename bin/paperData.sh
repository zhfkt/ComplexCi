serIDRegression="paperData"_$(date "+%y_%m_%d_%H_%M_%S")
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo $regressionResult

#######
#part0 my/opensource ci method
#######

for i in 0 2
do
	for j in 0 1 2
	do
		./executeAll.sh $j 1 500 $i -1
		./calGroovyBenchmark.sh $regressionResult
	done
done

for j in 0 1 2
do	
	for i in 1 9
	do
		./executeAll.sh $j 1 500 $i 0.01
		./calGroovyBenchmark.sh $regressionResult
	done
done

#######
#part1 core method
#######

for i in 9 10
do
	for j in 0.01 0.001
	do
		./executeAll.sh 0 1 500 $i $j
		./calGroovyBenchmark.sh $regressionResult		
	done
done

for i in 5 7
do
	for j in 0 1 2
	do
		./executeAll.sh $j 1 500 $i 0.001
		./calGroovyBenchmark.sh $regressionResult
	done
done

for i in 15 16 17 18
do
	./executeAll.sh 2 1 500 $i 0.001
	./calGroovyBenchmark.sh $regressionResult
done


#######
#part2 concurrent for best method
#######

./bestResult.sh 2
./calGroovyBenchmark.sh $regressionResult

./bestResult.sh 1
./calGroovyBenchmark.sh $regressionResult

#find that only real1/real3 will boost

#######
#part3 best stragety 
#######

./bestResult.sh
./calGroovyBenchmark.sh $regressionResult

#######
#part4 reinsert start
#######

for j in 0.0 0.0001 0.01 0.1 
do
	./bestResult.sh 0 1 $j
	./calGroovyBenchmark.sh $regressionResult
done

#######
#part5 batch stragety 
#######

for j in 100 200 300 400 500 1000
do
	./bestResult.sh 0 $j 0.001
	./calGroovyBenchmark.sh $regressionResult
done


#######
#part6 quick stragety 
#######

./quickResult.sh
./calGroovyBenchmark.sh $regressionResult







