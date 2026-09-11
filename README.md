Merhaba! HotRace rush projesi için 1 günün kaldığını söylüyorsun. Panik yok, aşağıda PDF’i adım adım analiz edip ne istendiğini, nelere dikkat etmen gerektiğini ve dosya/klasör yapısını anlatıyorum. Önce çalışan bir çözüm yap, sonra optimizasyonla uğraş.

---

## 1. Proje Özeti
**HotRace**, bir arama motoru gibi çalışan bir C programı.  
- Klavyeden (stdin) önce **key-value** çiftlerini okur.
- Boş satır gelene kadar bu çiftleri hafızada saklar.
- Boş satırdan sonra gelen her satırı **aranacak key** olarak kabul eder.
- Eğer key bulunursa **value**’yu, bulunamazsa `key: Not found.` yazar.

**Örnek girdi (`example.htr`):**
```
keyword-1
value-1
keyword-2
value-2

keyword-1
keyword-2
```
**Çıktı:**
```
value-1
value-2
```

**Bulunamazsa:**
```
keyword-3: Not found.
```

---

## 2. Kesin Kurallar (PDF’ten)
- Program adı: **hotrace**
- Parametre almaz.
- C diliyle yazılacak.
- **Norm**’a uyulacak.
- Tüm dosyalar **repo’nun kök dizininde** olacak (alt klasör yok).
- Derleme: `cc` ile.
- Flag’ler: `-Wall -Wextra -Werror`
- **Makefile** olacak. Kurallar: `NAME, all, clean, fclean, re`. Relink olmayacak.
- Sadece şu fonksiyonlar kullanılabilir:  
  `read, write, malloc, free, strerror, __asm__`  
  **libft yasak!** `printf, strlen, strcmp, realloc, exit` vs. kullanma.
- Program çökmemeli (segfault, double free vb.). Aksi halde not 0.
- Not: Hız/optimizasyon önemli.

---

## 3. Dosya ve Klasör Yapısı
Repo kökünde şu dosyalar olsun:
```
/
├── Makefile
├── hotrace.c
├── hotrace.h
├── hash.c
├── hash.h
├── utils.c
└── utils.h
```
İstersen `reader.c` / `reader.h` ekleyebilirsin ama kök dizinde olmalı.  
**Test dosyalarını (example.htr vb.) repoya koyma!** PDF: “test programları oluşturabilirsin ama teslim etmemelisin.”

---

## 4. Adım Adım Yapılacaklar

### Adım 1: Makefile
Örnek:
```makefile
NAME = hotrace
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = hotrace.c hash.c utils.c
OBJS = $(SRCS:.c=.o)
HEADERS = hotrace.h hash.h utils.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
```

### Adım 2: Yardımcı Fonksiyonlar (`utils.c`)
Sadece izin verilen fonksiyonlar olduğu için kendin yazmalısın:
- `my_strlen`
- `my_strcmp` (veya `my_str_equal`)
- `write_str` (write ile string yaz)
- `write_char`

### Adım 3: Girdiyi Oku (`read_all`)
Tüm stdin’i tek bir dinamik buffer’a oku.  
- `read(0, buf, BUFFER_SIZE)` ile parça parça oku.
- Buffer dolunca 2 katı boyutta yeni malloc, eskisini kopyala, free et.
- `realloc` yasak, kendin yap.
- EOF’a kadar oku. Buffer’ı `\0` ile bitir.

### Adım 4: Hash Tablosu (`hash.c`)
Hızlı arama için **chaining** (linked list) kullan.
```c
typedef struct s_entry {
    char *key;
    char *value;
    struct s_entry *next;
} t_entry;
```
- Tablo boyutu: `TABLE_SIZE` (2’nin kuvveti, örn. 1<<16). Gerekirse resize et (count > size * 0.75).
- Hash: FNV-1a gibi hızlı bir algoritma.
- `insert(key, value)`: key varsa value güncelle, yoksa başa ekle.
- `search(key)`: bulursa value döndür, yoksa NULL.

### Adım 5: Main Akışı (`hotrace.c`)
1. `read_all` ile tüm input’u al.
2. Pointer `pos` ile satırları parse et:
   - `key = &buf[pos]`; `\n`’e kadar ilerle, `\n`’i `\0` yap, `pos`’u ilerlet.
   - Eğer `key[0] == '\0'` ise **boş satır** → ayırıcı, döngüden çık.
   - `value = &buf[pos]`; aynı şekilde satır sonu bul, `\0` yap, `pos` ilerlet.
   - `insert(key, value)`.
3. Ayırıcıdan sonra kalan satırlar query:
   - Her satır için `search(key)`.
   - Bulunursa `write_str(1, value); write_char(1, '\n');`
   - Bulunamazsa `write_str(1, key); write_str(1, ": Not found.\n");`
4. Program sonunda malloc’ları free et (şart değil ama iyi).

---

## 5. Dikkat Edilecek Kritik Noktalar
- **Boş satır tespiti:** Satır sonundaki `\n`’i `\0` yapınca `key[0] == '\0'` ise ayırıcıdır.
- **Çıktı formatı:** `key: Not found.\n` (boşluk ve nokta tam olarak böyle).
- **Norm:** `for` yasak, `while` kullan. Fonksiyonlar max 25 satır. 80 karakter sınırı.
- **Hata kontrolü:** `malloc`, `read`, `write` dönüşlerini kontrol et. Hata olursa `return 1;` (exit yasak).
- **Hız:** Tüm input’u tek seferde oku, hash tablosu kullan, gereksiz kopyalama yapma.
- **Makefile relink:** `$(NAME): $(OBJS)` şeklinde yaz, `.PHONY` ekle. Relink olmaz.
- **Test:** Kendi `example.htr` dosyanı oluştur, `./hotrace < example.htr` ile dene. Test dosyasını repoya ekleme.

---

## 6. Örnek Test
```bash
$> cat -e example.htr
keyword-1$
value-1$
keyword-2$
value-2$
$
keyword-1$
keyword-2$
$> ./hotrace < example.htr
value-1
value-2
$> ./hotrace
keyword-1
value-1
keyword-2
value-2

keyword-3
keyword-3: Not found.
keyword-1
value-1
```

---

## 7. Son Kontrol Listesi
- [ ] Tüm dosyalar repo kökünde.
- [ ] Makefile doğru, `cc -Wall -Wextra -Werror` ile derliyor.
- [ ] Sadece izin verilen fonksiyonlar kullanıldı.
- [ ] Norm’a uygun (while, 25 satır, vs.).
- [ ] Boş satır ayırıcısı doğru çalışıyor.
- [ ] Bulunamayan key için çıktı tam formatlı.
- [ ] Program çökmüyor, segfault yok.
- [ ] Test dosyaları repoda yok.

Başarılar! 1 günün var, önce çalışan versiyonu bitir, sonra hız optimizasyonu yap. Takıldığın yerde sorabilirsin.
