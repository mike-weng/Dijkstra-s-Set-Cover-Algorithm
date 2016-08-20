'''
Download test data (if necessary)

Run local executable argv[1] 4 times.

Upload cover size and mean run time to server.

This script is easy to hack to alter your program results that are
posted to the server. Please respect your peers and do not alter
this script or test files.

Author: Andrew Turpin
Date: Sat 25 Apr 2015 19:03:34 AEST
'''
import sys
import os.path
import urllib.request
import subprocess
import timeit

BASE_URL = 'http://128.250.182.32/comp20007/'
LOCAL_PREFIX = './'
TESTS = ['c_t1', 'c_t2', 'c_t3', 'c_t4']

def get_test_file(file, filename):
    '''Download file from BASE_URL and write to filename.
    '''
    try:
        text = urllib.request.urlopen(BASE_URL + file).read()
        f = open(filename, 'w')
        f.write(text.decode('utf-8'))
        f.close()     
    except urllib.error.HTTPError:
        print('Could not get {} from {}'.format(file, BASE_URL))
        sys.exit()
    except:
        print('Could not write '+filename)
        sys.exit()

def verify(exe, filename, num_repeats=1):
    ''' Run exe < filename | ./verify filename, which should return 0 for success.
    '''
    cat = subprocess.Popen(['cat', filename], stdout=subprocess.PIPE)
    output = subprocess.Popen([exe], stdin=cat.stdout, stdout=subprocess.PIPE)
    verify = subprocess.Popen(['verify.exe', filename], stdin=output.stdout)
    
    return(verify.wait())




def do_runs(exe, filename, num_repeats=1):
    ''' Repeat exe < filename num_repeats times and return answer and average.
        Answer is a list of integers.
    '''
    time = 0
    for i in range(num_repeats):
        start = timeit.default_timer()

        cat = subprocess.Popen(['cat', filename], stdout=subprocess.PIPE)
        output = subprocess.Popen([exe], stdin=cat.stdout, stdout=subprocess.PIPE)

        a = sorted([int(i.decode('utf-8').strip('\n')) for i in output.stdout])
    
        stop = timeit.default_timer()

        time += stop - start

    return( (a, time/num_repeats) )



def send_result(testNum, time, cover_size, handle):
    ''' Send the 3 args to the server.
    '''
    d = {'testNum': testNum+1 , 'name': handle, 'cover': cover_size, 'time': time}
    data = urllib.parse.urlencode(d)
    url = BASE_URL+'add.php?'+data
    req = urllib.request.urlopen(url)



'''
Main
'''
if len(sys.argv) != 3:
    print('Usage: python3 {} exe_file_name two-char-handle'.format(sys.argv[0]))
    sys.exit()

exe = sys.argv[1]
handle = sys.argv[2]

for test_num in range(len(TESTS)):
    test_file = TESTS[test_num]

    filename = LOCAL_PREFIX + test_file   # the test file

    if not os.path.isfile(filename):
        get_test_file(test_file, filename)
    
    if verify(exe, filename) == 0:
        answer,time = do_runs(exe, filename, 3)
        send_result(test_num, time, len(answer), handle)
        print('Sent time= {} cover= {} to server'.format(time, len(answer)))
    else:
        print('Test file {} failed - answer does not cover all houses'.format(test_file))
