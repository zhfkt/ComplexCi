call ..\..\dailyUse\windows\traditionalCollectiveInfluence.cmd ..\..\data\networks\model1.csv 0 0
call ..\..\dailyUse\windows\newReinsertCollectiveInfluence.cmd ..\..\data\networks\model2.csv 0 1 0
call ..\..\dailyUse\windows\cppCollectiveInfluence.cmd ..\..\data\networks\model3.csv 0 1 0

call mergeResult.cmd

echo model1 0.212114535
echo model2 a little worse than 0.174871586
echo model3 0.348837118

pause