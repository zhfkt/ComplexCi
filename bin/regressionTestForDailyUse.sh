serIDRegression=regressionTestForDailyUse_$(date "+%y_%m_%d_%H_%M_%S")
mkdir regressionResult
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo $regressionResult


csvFiles=../../data/networks/*.csv


for j in traditionalCollectiveInfluence  newReinsertCollectiveInfluence cppCollectiveInfluence
do	
	for ballRadius in 0 1 2
	do
	
		serID=regressionTestForDailyUse_$j_$ballRadius_$(date "+%y_%m_%d_%H_%M_%S")
		echo "serID: " $serID
		exec > $serID.log	
	
		cd ../dailyUse/linux/
	
		for i in `ls $csvFiles`
		do			
			if [ $i == "traditionalCollectiveInfluence" ]
			then
				./$j.sh $i $ballRadius 0 &
			else
				./$j.sh $i $ballRadius 1 0 &
			fi
		done
		
		wait
					
		resultFolder=../../data/networks/results/$serID/

		mkdir -p  $resultFolder
		mv ../../data/networks/*.csv_out  $resultFolder
		cd $resultFolder

		../../../../bin/mergeResult.sh

		cd ../../../../bin/

		./calGroovyBenchmark.sh $serIDRegression
		
		echo $regressionResult

	done
		
done



