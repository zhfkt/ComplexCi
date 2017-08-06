
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

mkdir -p $pythonOut

for i in `ls $csvFiles`
do
   echo $i
   date
		#disable output buffer
        python3 -u ComplexCiPython.py $i $pythonOut $ballRadius $batch 500 $methodCentrality 
   date
done

wait

resultFolder=$pythonOut/$serID/

mkdir -p  $resultFolder
mv ${pythonOut}*.csv_out  $resultFolder
cd $resultFolder

../../../../bin/mergeResult.sh

date
