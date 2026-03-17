# ♟️ Chess (C++ / Raylib)

> **Projekt w toku — nie jest jeszcze gotowy do pełnego użytku.**

Gra szachowa napisana w C++ z wykorzystaniem biblioteki [Raylib](https://www.raylib.com/). Aktualnie zaimplementowane są podstawowe mechaniki: ruch figur, walidacja ruchów, roszada, bicie en passant oraz wykrywanie szacha.

---

## Aktualny stan projektu
    
### Zaimplementowane

- Plansza 8×8 z teksturami figur
- Pełna walidacja ruchów dla wszystkich figur (pionek, wieża, skoczek, goniec, hetman, król)
- Wykrywanie szacha
- Roszada
- Bicie en passant
- System tur
---

## Do zaimplementowania

### Logika gry
- [ ] Wykrywanie **mata** — brak legalnych ruchów przy szachu
- [ ] Wykrywanie **pata** — brak legalnych ruchów bez szacha
- [ ] **Promocja pionka** — wybór figury po dojściu do ostatniej linii
- [ ] Wykrywanie remisu z innych przyczyn (np. niewystarczający materiał, trzykrotne powtórzenie pozycji, reguła 50 ruchów)

### Notacja i zapis partii
- [ ] **System FEN** — wczytywanie i eksportowanie pozycji w formacie FEN
- [ ] **Notacja algebraiczna** — generowanie i wyświetlanie historii ruchów w panelu bocznym
- [ ] **GUI notacji** — przewijana lista ruchów z numerami tur w bocznym menu

### Komfort gry
- [ ] **Cofanie ruchów** (`Ctrl+Z` / przycisk w menu)
- [ ] Historia ruchów umożliwiająca cofnięcie do dowolnego punktu partii

### Silnik szachowy
- [ ] Implementacja **Chess Engine** (algorytm Minimax z alfa-beta pruning)
- [ ] Tryb gry przeciwko komputerowi
---

## Wymagania

- C++20 lub nowszy
- [Raylib](https://www.raylib.com/) (zainstalowany i zlinkowany)
- Tekstury figur w katalogu `assets/` (konwencja nazw: `w_pawn.png`, `b_king.png` itp.)

## Kompilacja

```bash
g++ -std=c++20 main.cpp board.cpp -o chess -lraylib
```

---

## Struktura projektu

```
.
├── main.cpp          # Pętla główna, obsługa wejścia
├── Board.h           # Deklaracje klasy Board i Piece
├── board.cpp         # Logika planszy, walidacja ruchów
├── InputHandler.h    # Obsługa myszy
├── SideMenu.h        # Boczne menu (notacja, UI)
└── assets/           # Tekstury figur (PNG)
```

---

## Licencja

Projekt jest udostępniony na licencji MIT. Szczegóły w pliku `LICENSE`.