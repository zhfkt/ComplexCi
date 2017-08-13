serIDRegression="paperData"_$(date "+%y_%m_%d_%H_%M_%S")
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo $regressionResult

for i in 0 1 2 5 7 8 9 10
do
	echo Now $i
	for j in 0 1 2
	do
		./executeAll.sh $j 1 500 $i 0.001
		./calGroovyBenchmark.sh $regressionResult
	done
done

for i in 9 10
do
	echo Now $i
	for j in 0 1 2
	do
		./executeAll.sh $j 500 500 $i 0.01
		./calGroovyBenchmark.sh $regressionResult
	done
done

for i in 9 10
do
	echo Now $i
	for j in 0 1 2
	do
		./executeAll.sh $j 1 500 $i 0.01
		./calGroovyBenchmark.sh $regressionResult
	done
done

for i in 9 10
do
	echo Now $i
	for j in 0 1 2
	do
		./executeAll.sh $j 1 500 $i 0.0
		./calGroovyBenchmark.sh $regressionResult
	done
done

for i in 11 12 13 14
do
	echo Now $i
	for j in 0 1 2
	do
		./executeAll.sh $j 1 500 $i 0.001
		./calGroovyBenchmark.sh $regressionResult
	done
done

for i in 11 12 13 14
do
	echo Now $i
	for j in 0 1 2
	do
		./executeAll.sh $j 1 500 $i 0.0
		./calGroovyBenchmark.sh $regressionResult
	done
done


./bestResult.sh
./calGroovyBenchmark.sh $regressionResult

./quickResult.sh
./calGroovyBenchmark.sh $regressionResult