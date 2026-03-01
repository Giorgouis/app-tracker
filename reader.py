import struct
import pprint
import subprocess
from time import time,sleep


times = -1
while (times<0):
     times = int(input("Enter how many times session should update: "))
print(f"Running logger.exe with {times} session updates")
t1 = time()
subprocess.run(["logger.exe", "-c", str(times)], capture_output=False)
t2 = time()
print(f'Took {t2-t1}')





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
                    name = f.read(name_len).decode("utf-8", errors="ignore")
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
    return sessions

sleep(2)
print('Reading data')
t1 = time()

all_sessions = read_sessions("sessions.bin")
t2 = time()
print(f'Took {t2-t1}')

print('printing all data')
sleep(1)
t1 = time()
for session in all_sessions:
    # if session.name != '':
        pprint.pprint(session)
t2 = time()
print(f'\n\n\nTook {t2-t1}')