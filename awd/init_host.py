import requests
import threading

li = lambda x : print('\x1b[01;38;5;214m' + x + '\x1b[0m')
ll = lambda x : print('\x1b[01;38;5;1m' + x + '\x1b[0m')

pwn_port = 80

def check_ip(i):
    try:
        url = f'http://1.94.1.{i}/' #*
        response = requests.get(url, timeout=0.5)
        if response.status_code == 200:
            li('[+] ' + url)
            with open('hosts', 'a+') as f:
                f.write(f'1.94.1.{i}:{pwn_port}\n') #*
        else:
            raise Exception("Not 200 OK")
    except Exception as e:
        ll('[-] ' + url)
        with open('h', 'a+') as f:
            f.write(f'1.94.1.{i}:{pwn_port}\n') #*

NUM_THREADS = 256

threads = []
for i in range(1, 256):
    thread = threading.Thread(target=check_ip, args=(i,))
    threads.append(thread)
    thread.start()

    if len(threads) >= NUM_THREADS:
        for t in threads:
            t.join()
        threads = []

for t in threads:
    t.join()