import threading
from time import sleep
import json
import requests

success = lambda x : print('\x1b[01;38;5;214m' + x + '\x1b[0m')
fail = lambda x : print('\x1b[01;38;5;1m' + x + '\x1b[0m')

threads = []
token = 

def submit(flag):
    try:
        submit_api = ""
        # submic flag api
        headers = { "Authorization": token }
        # team token
        data = { "flag": flag }
        # the flag to submic
        success("submit flag!")
    except Exception as e:
        faile(f"error:{e}")

def main():
    with open("./flags") as flag_txt:
        flags = flag_txt.readlines()
        for flag in flags:
            thread = threading.Thread(target=submit, args=(flag,))
            threads.append(thread)
            thread.start()

        for thread in threads:
            thread.join()

main()