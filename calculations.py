import math

# --- STAŁE (TWOJA SPECYFIKACJA PROJEKTU) ---
MERGE_DEGREE = 2  # Stopień scalania (m) = 3 taśmy - 1 = 2
BLOCK_SIZE_RECORDS = 50 # Rozmiar bloku w rekordach

# --- DANE DO ANALIZY ---
EXPERIMENT_N_VALUES = [100, 500, 1000, 5000, 10000]

# --- FUNKCJE ---

def calculate_theoretical_F(N: int) -> int:
    """Oblicza F_teoretyczne (liczba faz), zakładając L=N (najgorszy przypadek)."""
    if N <= 1:
        return 0
    # Wzór: ceil(log_m(N)), gdzie m=2
    return math.ceil(math.log(N, MERGE_DEGREE))

def calculate_theoretical_O(N: int, F: int) -> int:
    """Oblicza O_teoretyczne (operacje dyskowe)."""
    if F == 0:
        return 0
    
    # Liczba Stron (S): ceil(N / 4)
    S = math.ceil(N / BLOCK_SIZE_RECORDS)
    
    # Wzór: O ≈ 2 * S * F
    O = 2 * S * F
    return int(O)

# --- WYKONANIE I WYNIKI ---

print("--- TEORETYCZNA ANALIZA POLIFAZOWA (k=3, Blok=4) ---")
print("| N | F_teor | S (Strony) | O_teor (Dostępów) |")
print("|---|--------|------------|-------------------|")

for N in EXPERIMENT_N_VALUES:
    F_teor = calculate_theoretical_F(N)
    S = math.ceil(N / BLOCK_SIZE_RECORDS)
    O_teor = calculate_theoretical_O(N, F_teor)
    
    # Wyświetlenie wyników w formie tabeli
    print(f"| {N:<3} | {F_teor:<6} | {S:<10} | {O_teor:<17} |")

print("\n*F_teor (Fazy) i O_teor (Dostępy) są obliczone jako teoretyczne górne granice (zakładając, że L=N).")
