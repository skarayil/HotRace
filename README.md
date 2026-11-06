# 🔥 hotrace - Yüksek Performanslı Anahtar/Değer Arama Motoru

<div align="center">

![42 School](https://img.shields.io/badge/School-42-black?style=for-the-badge&logo=42)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Rush](https://img.shields.io/badge/Project-hotrace-orange?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active-success?style=for-the-badge)

**Standart girdiden okunan milyonlarca anahtar/değer çiftini hash tablosuna yükleyip, sorguları mümkün olan en hızlı şekilde yanıtlayan bir C programı**

*hotrace, tüm girdiyi tek seferde belleğe okur, satırları kopyalamadan yerinde (in-place) ayrıştırır ve özel bir 64-bit hash fonksiyonu ile zincirleme (chaining) hash tablosu üzerinde O(1) ortalama sürede arama yapar.*

[Kurulum](#-kurulum) • [Kullanım](#-kullanım) • [Nasıl Çalışır](#️-nasıl-çalışır) • [Hash Fonksiyonu](#-hash-fonksiyonu-detaylı)

</div>

---

## 🎯 Projenin Amacı

<img align="right" alt="Hotrace Animation" width="300" src="https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExZnBreWp5N2NleG90dTN3Z3JrcHNvMnE3dmUyZTgybXd6aTUwcGx2ayZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/11ZSwQNWba4YF2/giphy.gif">

Bu proje, bir **rush** projesi olarak, çok büyük girdilerle çalışan ve **hız** odaklı bir arama programı yazmayı amaçlamaktadır. Program iki bölümden oluşan bir girdi alır: önce `anahtar` / `değer` çiftleri, ardından boş bir satır ve sonrasında sorgulanacak anahtarlar. Her sorgu için değer ya da `Not found` mesajı yazdırılır.

Naif bir yaklaşım (her sorguda tüm listeyi taramak) `O(n × m)` karmaşıklığa sahiptir ve milyonlarca satırda kullanılamaz hale gelir. hotrace bu sorunu **hash tablosu** ile çözer.

### 🎓 **Öğrenme Hedefleri:**

#### ⚡ **Performans Odaklı Tasarım**
- Sistem çağrısı (`read`) sayısını minimumda tutmak için büyük chunk'larla okuma
- Satırları kopyalamak yerine girdi buffer'ını yerinde bölmek (zero-copy parsing)
- `malloc` sayısını azaltmak ve cache dostu veri erişimi

#### 🔑 **Hash Tabloları**
- Özel 64-bit hash fonksiyonu tasarımı (blok karıştırma + finalizer)
- Zincirleme (separate chaining) ile çakışma yönetimi
- İkinin kuvveti boyutlu tablo ve `&` maskesi ile hızlı indeksleme
- Yük faktörüne göre dinamik büyüyen tablo (rehash)

#### 🏗️ **Modüler Tasarım ve Bellek Yönetimi**
- Kendi `libft` fonksiyonlarımızla (`ft_strlen`, `ft_strcmp`, `ft_putstr_fd`...) çalışmak
- Dinamik büyüyen okuma buffer'ı (`t_buffer`)
- Sızıntısız bellek yönetimi, 42 Norm uyumlu kod

---

## 📁 Proje Yapısı

```
hotrace/
│
├── 🔨 Makefile                  # Build automation - derleme kuralları
├── 📄 hotrace.h                 # Struct tanımları ve fonksiyon prototipleri
├── 📄 example.htr               # Örnek girdi dosyası
│
├── 🚀 hotrace.c                 # main: okuma → yükleme → sorgulama → temizlik
├── 📥 buffer.c                  # stdin'i dinamik buffer'a okuma (ft_read_all)
├── 🧩 parser.c                  # Satır ayrıştırma, çift yükleme ve sorgu işleme
├── #️⃣  hash.c                    # Hash fonksiyonu, tablo oluşturma/arama/serbest bırakma
├── ➕ insert.c                  # Tabloya ekleme (güncelleme + resize tetikleme)
├── 📈 resize.c                  # Tabloyu iki katına çıkarıp yeniden dağıtma
├── 🧰 libft.c                   # ft_strlen, ft_strcmp, ft_putstr_fd, ft_putchar_fd
│
└── 📂 obj/                      # Object dosyaları (make sonrası oluşur)
```

---

## 📥 Girdi Formatı

Girdi iki bölümden oluşur ve bölümler **boş bir satır** ile ayrılır:

```
<anahtar_1>
<değer_1>
<anahtar_2>
<değer_2>
...
                      ← boş satır (bölüm ayırıcı)
<sorgu_1>
<sorgu_2>
...
```

| Bölüm | Format | Açıklama |
|---|---|---|
| **Veri bölümü** | Her çift için 2 satır: önce anahtar, sonra değer | Boş bir satıra rastlanana kadar okunur |
| **Ayırıcı** | Boş satır | Boş bir *anahtar* satırı, veri bölümünün bittiğini gösterir |
| **Sorgu bölümü** | Her satır bir sorgu | Boş sorgu satırları atlanır |

### 📤 Çıktı Kuralları

| Durum | Çıktı |
|---|---|
| Anahtar tabloda bulundu | `değer` + satır sonu |
| Anahtar tabloda yok | `sorgu: Not found.` + satır sonu |

> 🔁 **Tekrarlayan anahtarlar:** Aynı anahtar birden fazla kez verilirse, **son** değer geçerli olur (eski değerin üzerine yazılır).

---

## ⚙️ Nasıl Çalışır

hotrace, girdiyi şu akışla işler:

1. **Okuma** — `ft_read_all`, `stdin`'in tamamını 64 KB'lık chunk'larla okuyup tek bir dinamik buffer'da toplar. Buffer 4096 byte ile başlar, dolduğunda kapasitesi **ikiye katlanır** (amortize `O(1)` ekleme).
2. **Tablo Oluşturma** — `ft_table_create(262144)` ile `2^18` kovalı (bucket) boş bir hash tablosu oluşturulur.
3. **Ayrıştırma ve Yükleme** — `ft_parse_pairs`, girdiyi satır satır gezer. Her `'\n'` karakteri `'\0'` ile **yerinde değiştirilir**; böylece anahtar ve değerler girdi buffer'ının içine işaret eden pointer'lar olur, hiçbir string kopyalanmaz. Her çift `ft_table_insert` ile tabloya eklenir.
4. **Sorgulama** — Boş satırdan sonra kalan kısım `ft_process_queries` ile işlenir. Her sorgu için `ft_table_search` çağrılır; sonuç ya da `Not found` mesajı yazdırılır.
5. **Temizlik** — Tablo (`ft_table_free`) ve girdi buffer'ı serbest bırakılır.

### 🔄 Akış Şeması

```
   stdin (fd 0)
       │
 ft_read_all ──────────────► tek bir büyük buffer (input)
       │
 ft_table_create(262144)
       │
 ft_parse_pairs
       │   ┌──────────────────────────────────────┐
       │   │  key = ft_next_line()                │
       ├──►│  key boş mu?  ──── evet ──► DUR      │
       │   │  value = ft_next_line()              │
       │   │  ft_table_insert(key, value)         │
       │   └──────────────────────────────────────┘
       │            (cursor: sorgu bölümünün başı)
       ▼
 ft_process_queries
       │   ┌──────────────────────────────────────┐
       │   │  query = ft_next_line()              │
       ├──►│  ft_table_search(query)              │
       │   │   ├─ bulundu  → değer\n              │
       │   │   └─ yok      → query: Not found.\n  │
       │   └──────────────────────────────────────┘
       ▼
 ft_table_free + free(input)
```

---

## ✂️ Zero-Copy Ayrıştırma

`ft_next_line` (`parser.c`), satırı kopyalamadan döndürür:

```c
static char *ft_next_line(char **cursor)
{
    ...
    line = p;
    while (*p && *p != '\n')
        p++;
    if (*p == '\n')
    {
        *p = '\0';      // satır sonunu string sonuna çevir
        p++;
    }
    *cursor = p;
    return (line);      // girdi buffer'ının içine işaret eden pointer
}
```

Bu yaklaşımın sonuçları:

| Özellik | Açıklama |
|---|---|
| **Sıfır ek bellek** | Anahtar ve değerler için ayrı `malloc` yapılmaz; yalnızca tablo girdileri (`t_entry`) ayrılır |
| **Hızlı** | Her bayt yalnızca bir kez taranır |
| **Ömür bağımlılığı** | Tablo, girdi buffer'ına işaret ettiği için `input`, tablo serbest bırakıldıktan **sonra** `free` edilir (`hotrace.c`'deki sıra bilinçlidir) |

---

## #️⃣ Hash Fonksiyonu (Detaylı)

`ft_hash_str` (`hash.c`), özel olarak tasarlanmış 64-bit bir hash fonksiyonudur. Anahtarı üç aşamada işler:

### 1️⃣ Başlangıç Değeri

```c
h = 0x9E3779B97F4A7C15UL + len;   // altın oran sabiti + anahtar uzunluğu
```

Uzunluğun başlangıç değerine katılması, farklı uzunluktaki anahtarların (ör. `"a"` ve `"a\0"` benzeri durumlar, ya da ortak önekler) ayrışmasına yardımcı olur.

### 2️⃣ 8 Baytlık Blok İşleme

Anahtar 8 bayt veya daha uzunsa, her 8 baytlık blok tek seferde okunur (`__builtin_memcpy` ile, hizalama sorunu olmadan):

```c
static unsigned long ft_load_mix(const char *p)
{
    __builtin_memcpy(&v, p, 8);
    v *= 0x9E3779B97F4A7C15UL;
    return ((v << 31) | (v >> 33));      // 31 bit rotasyon
}

h ^= ft_load_mix(key + i);
h = (h << 27) | (h >> 37);               // 27 bit rotasyon
h = h * 0xBF58476D1CE4E5B9UL + 0x94D049BB133111EBUL;
```

Bayt bayt işlemek yerine 8 bayt birden işlemek, uzun anahtarlarda döngü sayısını **8 kat** azaltır.

### 3️⃣ Kuyruk (Tail) ve Finalizer

8'e tam bölünmeyen kalan baytlar FNV tarzı işlenir, ardından bitleri iyice dağıtan bir **finalizer** uygulanır:

```c
while (i < len)
{
    h ^= (unsigned char)key[i];
    h *= 0x100000001B3UL;                // 64-bit FNV asal çarpanı
    i++;
}
h ^= h >> 33;
h *= 0xFF51AFD7ED558CCDUL;
h ^= h >> 29;
```

Finalizer, düşük bitlerin de iyi karışmasını sağlar. Bu **kritik**tir; çünkü tablo indeksi hash'in düşük bitlerinden (`& (size - 1)`) üretilir.

---

## 🗃️ Hash Tablosu Tasarımı

```c
typedef struct s_entry
{
    char            *key;       // girdi buffer'ına işaret eder
    char            *value;     // girdi buffer'ına işaret eder
    struct s_entry  *next;      // aynı kovadaki sonraki girdi
}   t_entry;

typedef struct s_table
{
    t_entry **buckets;          // kova dizisi
    size_t   size;              // kova sayısı (her zaman 2'nin kuvveti)
    size_t   count;             // toplam girdi sayısı
}   t_table;
```

| Tasarım Kararı | Açıklama |
|---|---|
| **Zincirleme (chaining)** | Çakışan anahtarlar aynı kovada bağlı liste olarak tutulur; yeni girdi listenin **başına** eklenir (`O(1)`) |
| **2'nin kuvveti boyut** | `hash % size` yerine `hash & (size - 1)` kullanılır; bölme işlemi yerine tek bir AND komutu |
| **Büyük başlangıç boyutu** | `262144` (`2^18`) kova ile başlanır; küçük/orta girdilerde hiç resize gerekmez |
| **Yük faktörü 0.75** | `count > size * 3 / 4` olduğunda tablo büyütülür |
| **Güncelleme** | Anahtar zaten varsa yeni girdi açılmaz, mevcut girdinin `value` alanı güncellenir |

### 📈 Dinamik Büyüme (`resize.c`)

```
count > size * 3/4  ?
        │
       evet
        │
ft_alloc_buckets(size * 2)      ← yeni kova dizisi (NULL ile doldurulmuş)
        │
ft_move_entries                 ← her girdi yeni indekse YENİDEN BAĞLANIR
        │                          (yeni t_entry ayrılmaz, kopyalama yok)
free(eski buckets) → size *= 2
```

Girdiler yeniden `malloc` edilmez; mevcut `t_entry` node'larının `next` pointer'ı yeni kovaya göre yeniden bağlanır. Böylece rehash yalnızca pointer işlemlerinden oluşur. `ft_alloc_buckets` başarısız olursa tablo eski haliyle çalışmaya devam eder (program çökmez, yalnızca performans düşer).

---

## 🚀 Kurulum

### 📋 **Ön Gereksinimler**

![GCC](https://img.shields.io/badge/GCC-Required-red?style=flat-square&logo=gnu&logoColor=white)
![Make](https://img.shields.io/badge/Make-Required-blue?style=flat-square&logo=gnu&logoColor=white)
![Unix](https://img.shields.io/badge/Unix-Compatible-green?style=flat-square&logo=linux&logoColor=white)

- **C Compiler**: `cc` (`gcc` veya `clang`) — 64-bit bir sistem önerilir (hash fonksiyonu `unsigned long`'un 64-bit olduğunu varsayar)
- **Make**: Build automation tool
- **Unix-like System**: Linux, macOS, WSL (`read`/`write` sistem çağrıları için `unistd.h` gerekir)

### 🔨 **Derleme**

```bash
make            # hotrace çalıştırılabilir dosyasını üretir

make clean      # obj/ dizinini siler
make fclean     # obj/ dizinini ve hotrace binary'sini siler
make re         # fclean + all
```

Derleme bayrakları: `-Wall -Wextra -Werror -O2`

---

## 💻 Kullanım

### ▶️ **Çalıştırma**

```bash
# Dosyadan girdi
./hotrace < example.htr

# Pipe ile girdi
cat example.htr | ./hotrace

# Çıktıyı dosyaya yönlendirme
./hotrace < example.htr > output.txt
```

### 📝 **Örnek**

`example.htr` dosyası:

```
apple
red
banana
yellow
grep
green

apple
banana
cherry
```

Çalıştırma:

```bash
$ ./hotrace < example.htr
red
yellow
cherry: Not found.
```

Burada ilk üç çift (`apple→red`, `banana→yellow`, `grep→green`) tabloya yüklenir, boş satır veri bölümünü kapatır; ardından `apple` ve `banana` bulunur, `cherry` ise tabloda olmadığı için `Not found` döner.

### 🧪 **Büyük Girdi ile Performans Testi**

Büyük bir girdi üretip süre ölçmek için basit bir örnek:

```bash
# 1 milyon çift + 1 milyon sorgu üret
awk 'BEGIN {
    for (i = 0; i < 1000000; i++) { print "key" i; print "value" i }
    print ""
    for (i = 0; i < 1000000; i++) print "key" i
}' > big.htr

time ./hotrace < big.htr > /dev/null
```

---

## 🛠️ Teknik Detaylar

### 🔍 Neden bu mimari?

| Darboğaz | Çözüm |
|---|---|
| Çok sayıda küçük `read` çağrısı | `stdin` tek bir döngüde 64 KB'lık chunk'larla okunur |
| Her satır için `malloc` + kopya | Girdi yerinde bölünür, anahtar/değerler girdi buffer'ını gösterir |
| Yavaş modulo işlemi | Tablo boyutu 2'nin kuvveti, indeks `& (size - 1)` ile hesaplanır |
| Kötü hash dağılımı → uzun zincirler | Blok karıştırma + finalizer ile bitler iyi dağıtılır |
| Tablo dolunca zincirlerin uzaması | Yük faktörü 0.75'i geçince tablo ikiye katlanır |

### 🧵 Bellek ve Kaynak Yönetimi

- `ft_read_all`, buffer'ı 4096 byte ile başlatır ve gerektikçe iki katına çıkarır; hata durumunda buffer'ı serbest bırakıp `NULL` döner.
- `ft_table_create`, tablo yapısını ve kova dizisini ayırır; kova dizisi başarısız olursa tablo yapısını da serbest bırakır.
- Her `t_entry` ayrı bir `malloc` ile oluşturulur; `ft_table_free` tüm zincirleri gezip hepsini serbest bırakır.
- Anahtar ve değer string'leri girdi buffer'ına ait olduğundan ayrıca `free` edilmez; `main` sonunda `free(input)` ile tek seferde serbest bırakılır.
- Giriş okunamazsa veya tablo oluşturulamazsa program `1` ile çıkar.

### 🧮 Karmaşıklık

| İşlem | Ortalama | En Kötü |
|---|---|---|
| Girdiyi okuma | `O(N)` | `O(N)` |
| Ekleme (`ft_table_insert`) | `O(1)` amortize | `O(n)` (tüm anahtarlar aynı kovaya düşerse) |
| Arama (`ft_table_search`) | `O(1)` | `O(n)` |
| Resize | `O(n)` (nadiren, amortize `O(1)`) | `O(n)` |

---

## 📚 Notlar

### ⚠️ **Dikkat Edilmesi Gerekenler**

- Girdi **tamamen** belleğe okunur; çok büyük girdilerde bellek kullanımı girdi boyutuyla orantılıdır (buffer büyütme sırasında geçici olarak yaklaşık 2 kat).
- Veri bölümünde **boş anahtar** kullanılamaz; boş bir anahtar satırı, veri bölümünün sonu olarak yorumlanır.
- Boş **değer** satırları geçerlidir (anahtar → boş string).
- Sorgu bölümündeki boş satırlar sessizce atlanır.
- Girdideki `'\n'` karakterleri ayrıştırma sırasında `'\0'`'a çevrildiği için girdi buffer'ı ayrıştırmadan sonra orijinal halinde değildir.
- Anahtarlar `'\0'` karakteri içeremez (C string'i olarak işlenirler).
- `ft_table_insert` içinde `malloc` başarısız olursa ilgili çift sessizce atlanır.

### 💡 **Geliştirme Fikirleri**

1. **Çıktı buffer'ı:** Şu anda her sonuç ayrı `write` çağrılarıyla yazılıyor (`ft_putstr_fd` + `ft_putchar_fd`). Çıktıyı büyük bir buffer'da toplayıp tek seferde yazmak, çok sayıda sorguda sistem çağrısı maliyetini ciddi şekilde azaltır.
2. **Entry havuzu (arena):** `t_entry` node'larını tek tek `malloc` etmek yerine büyük bir blok ayırıp içinden dilimlemek, `malloc` maliyetini ve bellek parçalanmasını azaltır.
3. **Buffer büyütme:** `ft_buffer_grow` şu an `malloc` + bayt bayt kopyalama kullanıyor; `realloc` veya `memcpy` ile bu kısım hızlandırılabilir.
4. **Hash önbelleği:** Hash değerini `t_entry` içinde saklamak, resize sırasında anahtarların yeniden hash'lenmesini ve arama sırasında gereksiz `strcmp` çağrılarını önler.

---

<div align="center">

**📝 "Hash it once, find it forever."**

---

### 👨‍💻 Created by Sude Naz Karayıldırım

[![42 Profile](https://img.shields.io/badge/42%20Profile-skarayil-black?style=flat-square&logo=42&logoColor=white)](https://profile.intra.42.fr/users/skarayil)
[![GitHub](https://img.shields.io/badge/GitHub-skarayil-181717?style=flat-square&logo=github&logoColor=white)](https://github.com/skarayil)

**⭐ Eğer bu proje işinize yaradıysa, repo'ya star vermeyi unutmayın!**

</div>
