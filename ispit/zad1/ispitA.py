# Ponovno definisanje potrebnih podataka

# Podaci o zahtevima korisnika
requests = [
    (1, 1, 9, 850), (2, 2, 15, 358), (3, 5, 5, 767), (4, 7, 12, 693),
    (5, 4, 19, 799), (6, 3, 8, 247), (7, 7, 1, 552), (8, 1, 6, 211),
    (9, 6, 3, 537), (10, 4, 8, 589), (11, 1, 7, 400), (12, 5, 17, 314),
    (13, 1, 23, 409), (14, 6, 32, 70), (15, 2, 9, 709), (16, 7, 11, 301)
]

# Podaci o povezanosti korisnika sa serverima i vremenu pristupa (u ms)
user_connections = {
    1: [(0, 1012), (1, 170)],
    2: [(0, 467), (1, 28)],
    3: [(0, 321), (2, 70)],
    4: [(0, 226), (2, 86)],
    5: [(0, 361), (1, 26)],
    6: [(0, 522), (3, 155)],
    7: [(0, 1415), (3, 163)]
}

# Izračunavanje ukupnog vremena koristeći samo centralni server
total_time = 0

for req in requests:
    request_number, user, file_index, num_accesses = req
    # Vreme pristupa podatku za ovog korisnika preko centralnog servera
    access_time = [connection[1] for connection in user_connections[user] if connection[0] == 0][0]
    # Računamo doprinos ovog zahteva ukupnom vremenu
    total_time += num_accesses * access_time

print(total_time)
