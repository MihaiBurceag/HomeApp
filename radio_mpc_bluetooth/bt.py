import bluetooth
import time
from subprocess            import *  

def main():   
	#target = 'C8:8B:47:05:AD:29'
	target = 'D4:E6:B7:6C:F2:94'
	present = False
	while (True):
		results = bluetooth.lookup_name(target, timeout=5)
		services = bluetooth.find_service(address=target)

		prev_state = present
		if (results!=None):
			present = True
		else:
			present = False
		if (present==True and prev_state==False): 
			print ("Device Connected")
			output = run_cmd("mpc clear" )
			output = run_cmd("mpc add http://live.tananana.ro:8010/stream-48.aac " )
			output = run_cmd("mpc play" )
		elif (present==False and prev_state==True): 
			print ("Device Disonnected")
			output = run_cmd("mpc stop" )
			
			
			#endfor
		#endif
		time.sleep(5)

def run_cmd(cmd):  
   p = Popen(cmd, shell=True, stdout=PIPE, stderr=STDOUT)  
   output = p.communicate()[0]  
   return output  
   
if __name__ == '__main__':  
  main()     