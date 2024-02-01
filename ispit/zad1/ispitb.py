from collections import defaultdict, Counter

# Podaci o veličinama fajlova (u GB)
file_sizes = [21, 4, 3, 9, 1, 12, 11, 17, 12, 9, 5, 2, 5, 45, 4, 25, 14, 20, 11, 5, 8, 22, 4, 31, 7, 6, 9, 5, 9, 12, 3, 1]

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

# Podaci o zahtevima korisnika
requests = [
    (1, 1, 9, 850), (2, 2, 15, 358), (3, 5, 5, 767), (4, 7, 12, 693),
    (5, 4, 19, 799), (6, 3, 8, 247), (7, 7, 1, 552), (8, 1, 6, 211),
    (9, 6, 3, 537), (10, 4, 8, 589), (11, 1, 7, 400), (12, 5, 17, 314),
    (13, 1, 23, 409), (14, 6, 32, 70), (15, 2, 9, 709), (16, 7, 11, 301)
]

# Kreiranje liste koja povezuje korisnike sa podacima koje zahtevaju
user_file_access = defaultdict(set)
for _, user, file_index, _ in requests:
    user_file_access[user].add(file_index)

# Inicijalizacija raspodele podataka
file_distribution = defaultdict(list)

# Analiza za svaki podatak
for file_index in range(1, len(file_sizes) + 1):
    server_accessibility = defaultdict(int)

    for user, files in user_file_access.items():
        if file_index in files:
            for server, _ in user_connections[user]:
                if server != 0:  # Ignorišemo centralni server
                    server_accessibility[server] += 1

    if server_accessibility:
        best_server = max(server_accessibility, key=server_accessibility.get)
        file_distribution[best_server].append(file_index)

# Ponovno izračunavanje ukupnog vremena nakon optimizovane raspodele
optimized_total_time = 0

for req in requests:
    _, user, file_index, num_accesses = req
    accessible_servers = [s for s, _ in user_connections[user] if s != 0]
    server_with_file = next((server for server in accessible_servers if file_index in file_distribution[server]), 0)
    access_time = next((time for server, time in user_connections[user] if server == server_with_file), user_connections[user][0][1])
    optimized_total_time += num_accesses * access_time

# Ispis raspodele podataka po lokalnim serverima i ukupnog vremena
for server, files in file_distribution.items():
    print(f"Lokalni Server {server}: Podaci {files}")

print("Optimizovano ukupno vreme:", optimized_total_time, "ms")
