'''
    'stime' class definition with 'show_time' function

    get and print timestamps

    Arguments of show_time function:

    - 1 to start counting
    - 0 to show and get timestamp
    - 2 to just get the timestamp
'''
import time

class stime(object):
    'show_time class definition'

    def __init__(self,title,lname):
        'initialize with title and loop name'
        self.prev_sec = 0
        self.title = title
        self.lname = lname
        self.sec1 = time.time()
#        print("init time=",self.sec1)
#        self.start_sec = self.sec1
        self.start_sec = self.get_time()
        diff_start_sec = 0
        diff_sec = 0
#        print ("%-35s %03.6f %3.6f -------------" %("---- Start "+self.title,diff_start_sec,diff_sec))


    @classmethod
    def get_time(self):
#        return time.time()
        return time.perf_counter()

    def stime1(cls,title):
        return cls(title,"Γραμμή")

    def test(self,sleep_time=1):
#        print ("sleeping for ",sleep_time)
        time.sleep(sleep_time)

    def show_time(self,description, init):
#        self.sec1 = time.time()
        self.sec1 = self.get_time()
        
        if init == 1:
            self.start_sec=self.sec1
            self.prev_sec = 0
            diff_start_sec = 0
            diff_sec = 0
        else:
            diff_start_sec = self.sec1 - self.start_sec
            diff_sec = self.sec1 - self.prev_sec

        if init == 0:
            print ("%-35s %03.6f %3.6f" %(self.lname+": "+description,diff_start_sec,diff_sec))
        else:
            if init == 1:
                print ("%-35s -----------------" %(self.lname+": "+description))
            
        self.prev_sec = self.sec1

        return diff_start_sec
