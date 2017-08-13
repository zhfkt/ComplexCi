serIDRegression=$(date "+%y_%m_%d_%H_%M_%S")_"$ballRadius"
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo " , model1 , model2 , model3 , model4 , real1 , real2 , real3 , real4 , total " > $regressionResult

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