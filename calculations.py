# import math

# nmbOfRecords = [1500, 10000, 50000, 200000,1000000]
# pageSize = 50
# bufferSize = 500
# tapeNumber = 3

# for i in nmbOfRecords:
#     print ("liczba rekordów: ")
#     print (i)
#     print ("liczba stron:")
#     print (i/pageSize)
#     print ("liczba buforów:")
#     print (i/bufferSize)
#     print ("liczba faz:")
#     print (math.log(i/bufferSize,tapeNumber))
#     print ("liczba dostepow do dysku: ")
#     print (2 * i/pageSize * (1+math.log(i/bufferSize,tapeNumber)))
#     print ("\n\n\n")

# ...existing code...
# ...existing code...
# filepath: /home/pik/projects/Struktury baz danych/Zadanie 1/calculations.py
# ...existing code...
import math

nmbOfRecords = [1500, 10000, 50000, 200000,1000000]
records_per_page = 50     # records per page
buffer_records = 500     # records that fit in buffer (was bufferSize)
# tapeNumber = 3            # optional cap on merge degree
tapeNumber = 10

# convert buffer capacity in records -> pages available in memory
buffer_pages = max(1, math.ceil(buffer_records / records_per_page))

for rec in nmbOfRecords:
    pages = math.ceil(rec / records_per_page)
    initial_runs = math.ceil(pages / buffer_pages)

    # typical fan-in = buffer_pages - 1 (one page for output); optionally cap by tapeNumber
    fan_in = max(2, min(buffer_pages - 1, tapeNumber))

    if initial_runs <= 1:
        passes = 0
    else:
        passes = math.ceil(math.log(initial_runs, fan_in))

    # disk accesses counted in pages: each pass reads and writes entire relation
    disk_accesses = 2 * pages * (1 + passes)

    print("liczba rekordów:", rec)
    print("liczba stron:", pages)
    print("liczba początkowych przebiegów (runs):", initial_runs)
    print("fan-in (merge degree):", fan_in)
    print("liczba faz (passes):", passes)
    print("liczba dostepow do dysku:", disk_accesses)
    print()
# ...existing code...
