if (( $# >= 3 ))
then
    ballRadius=$1
    batch=$2
	methodCentrality=$3
else
    echo './pythonControl.sh 2 500 0'
    exit;
fi

serID=$(date "+%y_%m_%d_%H_%M_%S")_"$ballRadius"_"$batch"_500_"$methodCentrality"

echo "serID: " $serID

exec > $serID.log

date

csvFiles=../data/networks/*.csv
pythonOut=../data/networks/pythonResults/

for i in `ls $csvFiles`
do
   echo $i
   date
        python3 ComplexCiPython.py $i $pythonOut $ballRadius $batch 500 $methodCentrality
   date
done

wait

resultFolder=$pythonOut/$serID/ 

mkdir  $resultFolder
mv ${csvFiles}_out  $resultFolder
cd $resultFolder

/scratch/fengkzhu/develop/owndev/ComplexCi/bin/mergeResult.sh 

date
