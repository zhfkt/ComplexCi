SCRIPT=`realpath $0`
SCRIPTPATH=`dirname $SCRIPT`
APP_BASE=`dirname $SCRIPTPATH`

if (( $# >= 5 ))
then
	prevOrNew=$1
	methodCentrality=$2
	csvFilesFolder=$3
	pythonOut=$4
	networkZip=$5
else
    echo './algoPython.sh [prev 0/new 1] [0] [../data/networks/] [../data/networks/pythonResults/] [../data/networks.zip] '
    exit;
fi

serID="Algopython_"$(date "+%y_%m_%d_%H_%M_%S")"_prevOrNew_"${prevOrNew}"_method_${methodCentrality}"
resultFolder=`realpath $pythonOut/$serID/`
networkZip=`realpath $networkZip`


echo "serID: " ${SCRIPTPATH}/${serID}.log

exec > ${SCRIPTPATH}/${serID}.log

date

mkdir -p  $resultFolder

for i in `ls ${csvFilesFolder}/*.csv`
do
	echo $i
	date
   
	if (( $prevOrNew == 0 ))
	then
		python3 -u ${SCRIPTPATH}/ComplexCiPython.py $i $resultFolder -1 10000000 500 $methodCentrality
	else	
		python3 ${SCRIPTPATH}/nbcen.py $methodCentrality $i $resultFolder
	fi
	
   date
done

#wait


$APP_BASE/bin/mergeResult.sh ${resultFolder}

cd $APP_BASE/Master_algorithm/

groovy -cp "target/algorithm-1.0-SNAPSHOT.jar:`readlink -f ~`/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar" src/main/java/org/dc/algorithm/NetMaster.groovy  ${resultFolder}/${serID}.csv  $networkZip

date
