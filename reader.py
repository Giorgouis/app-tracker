import struct
import pprint
import string
import subprocess
import sys
from time import time,sleep

NAME_LEN = 128
times = -1
# while (times<0):
#      times = int(input("Enter how many times session should update: "))
# print(f"Running logger.exe with {times} session updates")
# t1 = time()
# subprocess.run(["logger.exe", "-c", str(times)], capture_output=False)
# t2 = time()
# print(f'Took {t2-t1}')


def clean_name(raw_name):
    # Keep only printable ASCII characters
    printable = set(string.printable) - set('\r\n\t\x0b\x0c')  # ignore control chars
    cleaned = ''.join(c for c in raw_name if c in printable)
    return cleaned.strip()  # remove leading/trailing spaces


def read_sessions(filename):
    sessions = []
    with open(filename, "rb") as f:
        while True:
            count_bytes = f.read(4)
            if not count_bytes:
                break  # EOF
            timestamp_count = struct.unpack("<I", count_bytes)[0]

            # Read timestamps
            timestamps = [struct.unpack("<Q", f.read(8))[0] for _ in range(timestamp_count)]

            # Read process_list_lengths
            process_list_lengths = [struct.unpack("<I", f.read(4))[0] for _ in range(timestamp_count)]

            process_lists = []
            for length in process_list_lengths:
                processes = []
                for _ in range(length):
                    pid = struct.unpack("<I", f.read(4))[0]
                    is_program = struct.unpack("<B", f.read(1))[0]
                    name_len = struct.unpack("<H", f.read(2))[0]
                    name = f.read(name_len).decode("utf-8", errors="ignore").rstrip("\x00")
                    name = clean_name(name)
                    if name and is_program:
                        processes.append({
                            "pid": pid,
                            "is_program": bool(is_program),
                            "name": name
                        })
                process_lists.append(processes)

            sessions.append({
                "timestamps": timestamps,
                "process_list_lengths": process_list_lengths,
                "process_list": process_lists
            })

    return sessions[0]

def get_process_names_list(session):
    res = []
    for i in range(len(session['timestamps'])):
        curr = set()
        for process in session['process_list'][i]:
            curr.add(process['name'])
        res.append(curr)
    return res


def get_process_times(session):
    timestamps = session['timestamps']
    process_times = dict()
    l = len(timestamps)

    s = get_process_names_list(session)

    for i in range(0, l):
        if i<l-1:
            for p in s[i]:
                if p in s[i+1]:
                    if p in process_times:
                        process_times[p]+= max(timestamps[i+1]-timestamps[i],1)
                    else:
                        process_times[p] = max(timestamps[i+1]-timestamps[i],1)
    return process_times


if __name__ == '__main__':
    print('Best -c 10 -t 1000')
    print('Best -c 10 -t 1000')
    print('Best -c 10 -t 1000')

    t1 = time()
    subprocess.run(["logger.exe", "-c", '5', '-t', '1000'], capture_output=False)
    t2 = time()
    session = read_sessions('sessions.bin')
    timestamps = session['timestamps']
    pt = get_process_times(session)
    print(time()-t1)
    l = len(timestamps)
    # print(l)
    t3 = time()
    pprint.pprint(pt)
    print(f'logger.exe run for {t2-t1}\npython read data for {t3-t2}')
    # for i in range(len(timestamps)-1):
    #     print(timestamps[i+1]-timestamps[i])
