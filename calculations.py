
import math

def oblicz_sortowanie_zewnetrzne_rekordy(N_rek: int, B_rek: int, M_rek: int):
    """
    Oblicza teoretyczną liczbę faz scalania, odczytów, zapisów i dostępów do dysku
    dla algorytmu sortowania zewnętrznego przez scalanie, bazując na liczbie rekordów.

    Parametry:
    N_rek (int): Całkowita liczba rekordów do posortowania.
    B_rek (int): Liczba rekordów mieszcząca się w pojedynczym bloku/stronie dysku.
    M_rek (int): Liczba rekordów mieszcząca się w całym dostępnym buforze pamięci RAM.

    Zwraca:
    dict: Słownik z wynikami obliczeń lub słownik z kluczem "Błąd" w przypadku nieprawidłowych parametrów.
    """

    # Walidacja wejścia
    if N_rek <= 0:
        return {"Błąd": "N musi być większe niż 0."}
    if B_rek <= 0:
        return {"Błąd": "B_rek (rekordów na blok) musi być > 0."}
    if M_rek <= 0:
        return {"Błąd": "M_rek (rekordów w pamięci) musi być > 0."}

    print("--- Dane Wejściowe ---")
    print(f"Całkowita liczba rekordów (N_rek): {N_rek:,}")
    print(f"Rekordów na blok dysku (B_rek): {B_rek:,}")
    print(f"Rekordów w buforze pamięci (M_rek): {M_rek:,}")

    # 1. PRZYGOTOWANIE WSTĘPNE
    # Liczba bloków potrzebna na dane (zaokrąglamy w górę)
    LiczbaBloków_N = math.ceil(N_rek / B_rek)

    # Liczba bloków mieszcząca się w pamięci (liczba buforów)
    LiczbaBloków_M = math.floor(M_rek / B_rek)

    print(f"\nCałkowita liczba bloków do sortowania (LiczbaBloków_N): {LiczbaBloków_N:,}")
    print(f"Liczba buforów (bloków) mieszcząca się w pamięci (LiczbaBloków_M): {LiczbaBloków_M}")

    # Sprawdzenie minimalnej pamięci - potrzeba co najmniej 2 bloków (jeden na input, jeden na output)
    if LiczbaBloków_M < 2:
        return {
            "Błąd": "Pamięć/bufor M jest zbyt mała. Wymagane są minimum 2 bloki."
        }

    # 2. FAZA 1: TWORZENIE WSTĘPNIE POSORTOWANYCH BLOKÓW (RUNS)
    DługośćRuna_Bloki = LiczbaBloków_M
    LiczbaRunów_R = math.ceil(LiczbaBloków_N / DługośćRuna_Bloki)

    print(f"Długość wstępnie posortowanego runu (w blokach): {DługośćRuna_Bloki}")
    print(f"Liczba początkowych runów (R): {LiczbaRunów_R:,}")

    # 3. LICZBA FAZ SCALANIA
    kierunki_scalania_k = LiczbaBloków_M - 1  # jeden blok zarezerwowany na wyjście

    if LiczbaRunów_R <= 1:
        LiczbaFazScalania_P = 0
    else:
        if kierunki_scalania_k <= 1:
            return {"Błąd": "Kierunkowość scalania k jest zbyt mała (k <= 1). Potrzebne więcej buforów."}
        # P = ceil(log_k(R))
        LiczbaFazScalania_P = math.ceil(math.log(LiczbaRunów_R, kierunki_scalania_k))

    print(f"Kierunkowość scalania (k): {kierunki_scalania_k}")
    print(f"Teoretyczna liczba faz scalania (P): {LiczbaFazScalania_P}")

    # 4. LICZBA DOSTĘPÓW DO DYSKU (W BLOKACH)
    # Początkowe tworzenie runów: odczyt całych danych i zapis runów => 1 read + 1 write = 2 * LiczbaBloków_N
    # Każda faza scalania: odczyt wszystkich bloków i zapis wyników => 2 * LiczbaBloków_N na fazę
    # Zatem całkowite odczyty = (1 + P) * LiczbaBloków_N, zapisy = (1 + P) * LiczbaBloków_N
    CałkowitaLiczbaOdczytów = (1 + LiczbaFazScalania_P) * LiczbaBloków_N
    CałkowitaLiczbaZapisów = CałkowitaLiczbaOdczytów
    CałkowitaLiczbaDostępówDoDysku = 2 * CałkowitaLiczbaOdczytów  # R + W
    LiczbaPełnychSkanów = 2 * (1 + LiczbaFazScalania_P)

    return {
        "Liczba Runów (R)": LiczbaRunów_R,
        "Kierunkowość Scalania (k)": kierunki_scalania_k,
        "Liczba Faz Scalania (P)": LiczbaFazScalania_P,
        "--- Dostęp do Dysku (w blokach) ---": None,
        "Całkowita Liczba Odczytów Bloków": CałkowitaLiczbaOdczytów,
        "Całkowita Liczba Zapisów Bloków": CałkowitaLiczbaZapisów,
        "Całkowita Liczba Dostępów (R+W)": CałkowitaLiczbaDostępówDoDysku,
        "Liczba Pełnych Skanów Danych": LiczbaPełnychSkanów
    }

if __name__ == "__main__":
    # --- PRZYKŁAD UŻYCIA (możesz zmienić wartości) ---
    # Przykład:
    # Rozmiar rekordu: np. 100 bajtów (ale tu podajemy liczbę rekordów per blok i w pamięci)
    # Rozmiar bloku dysku -> B_rek (rekordów na blok)
    # Pamięć (rekordów) -> M_rek (ile rekordów zmieści pamięć)
    N_REKORDY = 10_000
    B_REKORDY_NA_BLOK = 50      # np. 50 rekordów na blok
    M_REKORDY_W_BUFORZE = 500  # np. pamięć mieszcząca 5000 rekordów

    wyniki = oblicz_sortowanie_zewnetrzne_rekordy(N_REKORDY, B_REKORDY_NA_BLOK, M_REKORDY_W_BUFORZE)

    print("\n" + "="*40)
    print("     WYNIKI OBLICZEŃ (Oparte na Rekordach)")
    print("="*40)

    if "Błąd" in wyniki:
        print(f"🚨 Błąd: {wyniki['Błąd']}")
    else:
        for klucz, wartosc in wyniki.items():
            if klucz.startswith("---"):
                print(klucz)
            elif isinstance(wartosc, int):
                print(f"{klucz}: {wartosc:,}")
            elif wartosc is None:
                print(klucz)
            else:
                print(f"{klucz}: {wartosc}")

    print("="*40)
# ...existing code...