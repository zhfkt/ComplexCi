serIDRegression="paperData"_$(date "+%y_%m_%d_%H_%M_%S")
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`

for i in 5 7 8 1 13
do
	echo Now $i
	for j in 0 1 2
	do
		
		ratio=0.001
		
		if((i==13))
		then
			ratio=0.01
		fi
	
		./executeAll.sh $j 1 500 $i $ratio
		./calGroovyBenchmark.sh $regressionResult
	done
done

./bestResult.sh
./calGroovyBenchmark.sh $regressionResult

./quickResult.sh
./calGroovyBenchmark.sh $regressionResult