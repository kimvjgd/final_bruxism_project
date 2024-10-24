/*

평소에는 EMG 측정을 한다.           
if (CURRENT_MODE)
CURRENT_MODE    =   true 
MUX_SEL         =   true
EMG_VALUE -> Measured periodically

////////////////////////////////////
그러다가
EMG_VALUE > EMG_THR_VALUE 가 되면
////////////////////////////////////

Electrical Stimulation Mode 전환
if (!CURRENT_MODE)
CURRENT_MODE    =   false 
MUX_SEL         =   false
그떄는 EMG_VALUE = 0 을 맞추고 Measure을 멈춘다.
(특정 파형을 원하는 주기로 다 쏴주고)

////////////////////////////////////

다시 EMG Mode로 전환한다.
CURRENT_MODE    =   true
MUX_SEL         =   true
EMG_VALUE -> Measured periodically


이게 계속 반복된다.

*/