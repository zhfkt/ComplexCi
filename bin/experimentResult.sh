serIDRegression="experimentResult"_$(date "+%y_%m_%d_%H_%M_%S")
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo $regressionResult

date

for i in 7 8
do
	echo Now $i
	
	./executeAll.sh 2 1 500 $i 0.001
	./calGroovyBenchmark.sh $regressionResult

done

date
