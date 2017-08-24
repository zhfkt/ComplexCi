if (( $# >= 3 ))
then
	networkPath=$1
    ballRadius=$2
	isPrintMinPointCausingMinComponent=$3
else
    echo './traditionalCollectiveInfluence.sh <networkPath> <ballRadius> <isPrintMinPointCausingMinComponent>'
	echo '<networkPath> is the file path'
	echo '<ballRadius> is the Radius parameter defined in the Collective Influence Algorithm'
	echo '<isPrintMinPointCausingMinComponent> whether output limited point leading to 0.01 of giant component ratio or all points'
	echo './traditionalCollectiveInfluence.sh /home/network/model1.csv 0 1'
	
    exit;
fi


../../bin/ComplexCi $networkPath $ballRadius 1 500 1 0.01 $isPrintMinPointCausingMinComponent
