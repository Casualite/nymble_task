import serial
from datetime import timedelta
from timeit import default_timer as timer

SAMPLE_FREQUENCY=10

s=serial.Serial('/dev/ttyUSB0',2400)
string="Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for predicting that the next banking crisis would be triggered by MSME lending, saying postmortem is easier than taking action when it was required. Rajan, who had as the chief economist at IMF warned of impending financial crisis of 2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, saying that they could be the sources of next banking crisis. Government should focus on sources of the next crisis, not just the last one.In particular, government should refrain from setting ambitious credit targets or waiving loans. Credit targets are sometimes achieved by abandoning appropriate due diligence, creating the environment for future NPAs, Rajan said in the note. Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined more closely for potential credit risk. Rajan, who was RBI governor for three years till September 2016, is currently.$"
print('len:',len(string))
string=string.encode('utf-8').strip()
s.write(string)
c=1
#recieve string
st=''
start=timer()
while(True):
    try:
        ReceivedString = s.read(1).decode('utf-8')
    except:
        continue
    if(ReceivedString=="$"):
        break
    st+=ReceivedString
    if(c%SAMPLE_FREQUENCY==0):
        stop=timer()
        print("for every",SAMPLE_FREQUENCY,"bytes, bps:",(SAMPLE_FREQUENCY*8*1e6)/(timedelta(seconds=stop-start).microseconds))
        start=timer()
    c+=1
print(st)
    
