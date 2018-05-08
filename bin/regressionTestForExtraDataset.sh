serIDRegression=regressionTestForDailyUse_$(date "+%y_%m_%d_%H_%M_%S")
mkdir regressionResult
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo $regressionResult


csvFiles=../../data/randomGraph/*.csv


for j in traditionalCollectiveInfluence  newReinsertCollectiveInfluence
do
	for ballRadius in 0 1 2
	do

		serID=regressionTestForDailyUse_"$j"_"$ballRadius"_$(date "+%y_%m_%d_%H_%M_%S")
		echo "serID: " $serID
		exec > $serID.log

		cd ../dailyUse/linux/

		for i in `ls $csvFiles`
		do
			if [ $j == "traditionalCollectiveInfluence" ]
			then
				./$j.sh $i $ballRadius 0 &
			else
				./$j.sh $i $ballRadius 1 0 &
			fi
		done

		wait

		resultFolder=../../data/randomGraphPythonResults/$serID/

		mkdir -p  $resultFolder
		mv ../../data/randomGraph/*.csv_out  $resultFolder
		cd $resultFolder

		../../../bin/mergeResult.sh

		exec > /dev/tty

		cd ../../../bin/

		./calGroovyBenchmarkExtraData.sh $regressionResult

		echo $regressionResult

	done

done



