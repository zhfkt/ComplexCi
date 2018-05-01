SCRIPT=`realpath $0`
SCRIPTPATH=`dirname $SCRIPT`

if (( $# >= 3 ))
then
	methodCentrality=$1
	csvFilesFolder=$2
	pythonOut=$3
else
    echo './pythonControl.sh [0] [../data/networks/] [../data/networks/pythonResults/] '
    exit;
fi

serID="nbcen_"$(date "+%y_%m_%d_%H_%M_%S")_"$methodCentrality"



echo "serID: " ${SCRIPTPATH}/${serID}.log

exec > ${SCRIPTPATH}/${serID}.log

date

#csvFiles=../data/networks/*.csv
#pythonOut=../data/networks/pythonResults/

mkdir -p $pythonOut

for i in `ls ${csvFilesFolder}/*.csv`
do
   echo $i
   date
		#disable output buffer
        python3 nbcen.py $methodCentrality $i $pythonOut
   date
done

wait

resultFolder=$pythonOut/$serID/

mkdir -p  $resultFolder
mv ${pythonOut}/*.csv_out  $resultFolder

${SCRIPTPATH}/../bin/mergeResult.sh ${resultFolder}

date
