if (( $# >= 4 ))
then
    ballRadius=$1
    batch=$2
    outputBatch=$3
	method=$4
else
    echo './executeAll.sh 2 500 500 0'
    exit;
fi

serID=$(date "+%y_%m_%d_%H_%M_%S")_"$ballRadius"_"$batch"_"$outputBatch"_"$method"

echo "serID: " $serID

exec > $serID.log

date

csvFiles=../data/networks/*.csv

for i in `ls $csvFiles`
do
   echo $i
   date
    ./ComplexCi $i $ballRadius $batch $outputBatch $method  &
   date
done

wait

resultFolder=../data/networks/results/$serID/ 

mkdir  $resultFolder
mv ${csvFiles}_out  $resultFolder
cd $resultFolder

/scratch/fengkzhu/develop/owndev/ComplexCi/bin/mergeResult.sh 

date
