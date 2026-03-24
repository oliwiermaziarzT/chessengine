# ♟️ Chess (C++ / Raylib)

> **Projekt w toku**

Gra szachowa napisana w C++ z wykorzystaniem biblioteki [Raylib](https://www.raylib.com/). Aktualnie zaimplementowane są podstawowe mechaniki: ruch figur, walidacja ruchów, roszada, bicie en passant oraz wykrywanie szacha.

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
- [ ] **Cofanie ruchów** - cofanie do dowolnego momentu partii

### Silnik szachowy
- [ ] Implementacja algorytmu Minimax
---

## Wymagania

- C++20 lub nowszy
- [Raylib](https://www.raylib.com/) (zainstalowany i zlinkowany)
- Tekstury figur w katalogu `assets/` (konwencja nazw: `w_pawn.png`, `b_king.png` itp.)

## Kompilacja

```bash
g++ -std=c++20 main.cpp board.cpp -o chess -lraylib
```

## Licencja

Projekt jest udostępniony na licencji MIT. Szczegóły w pliku `LICENSE`.
