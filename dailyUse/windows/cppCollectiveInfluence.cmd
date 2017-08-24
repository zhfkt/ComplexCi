ECHO ON

set argC=0

for %%x in (%*) do 
(
	set /A argC+=1
)

if %argC% LSS 4
(
	echo './cppCollectiveInfluence.cmd <networkPath> <ballRadius> <batchSize> <whetherOutputLimitedNode>'
	echo '<ballRadius> is the Radius parameter defined in the Collective Influence Algorithm'
	echo '<batchSize> batch size of deleted points per updating Collective Influence value'
	echo '<whetherOutputLimitedNode> whether output limited point leading to 0.01 of giant component ratio or all points'
	echo 'cppCollectiveInfluence.cmd /home/network/model1.csv 2 500 1'
	
    exit;
)

set networkPath=%1
set ballRadius=%2
set batchSize=%3
set whetherOutputLimitedNode=%4


../../x64/Release/ComplexCi $networkPath $ballRadius $batchSize 500 9 0.01 $whetherOutputLimitedNode

pause