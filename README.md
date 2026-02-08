# Visualisasi Taman Kota Ramah Lingkungan Menggunakan OpenGL

Project UAS Grafika Komputer ini memvisualisasikan sebuah **taman kota ramah lingkungan** menggunakan **OpenGL** dengan library GLUT, GLEW, dan FreeImage. Taman dirancang dengan elemen hijau dominan (pohon, semak, rumput), fasilitas publik (bangku, meja piknik, karpet), air mancur, lampu taman, dan pencahayaan malam untuk mendukung tema keberlanjutan dan ekologi kota.

Proyek ini memenuhi semua ketentuan UAS:
- Elemen 2D vektor minimal 2 objek + transformasi affin 2D
- Kurva polar dan kurva parametrik 3D
- Objek 3D berbasis polygon minimal 3
- Transformasi 3D (translasi, rotasi, skala)
- Kamera virtual dengan gluLookAt
- Proyeksi perspective dan orthographic (3D & 2D)
- Interaksi keyboard/mouse yang intuitif

## Fitur Utama

- **Scene Taman Lengkap**:
  - Lantai segi 8 multi-zona (rumput hijau, pedestrian abu-abu, border putih)
  - Pohon rindang bertingkat
  - Air mancur dengan aliran air animasi (kurva parametrik 3D)
  - Bangku taman dan meja piknik + karpet piknik
  - Lampu taman dengan toggle nyala/mati
  - Semak-semak dan dekorasi tambahan (tong sampah, dll.)

- **Mode Tampilan**:
  - Perspective 3D (default)
  - Orthographic 3D (tanpa distorsi perspektif)
  - Orthographic 2D (top-down view)

- **Interaksi Pengguna**:
  - WASD + QE : Gerak kamera (Free Camera)
  - Mouse drag : Rotasi pandangan (Free Camera)
  - `M` : Switch mode kamera (Free ↔ Orbit ↔ Top-down)
  - `V` : Switch mode view (3D ↔ 2D top-down)
  - `O` : Toggle proyeksi (Perspective ↔ Orthographic 3D)
  - `L` : Toggle lampu taman (nyala/mati)
  - `ESC` : Keluar program

- **Animasi**:
  - Rotasi lambat pada aliran air mancur
  - Rotasi otomatis pada mode Orbit

- **Pencahayaan**:
  - Lighting realistis dengan glLight (ambient, diffuse, specular)
  - Attenuation untuk efek jarak
  - Toggle nyala/mati lampu via tombol `L`

## Teknologi yang Digunakan

- **Bahasa**: C++
- **Library**:
  - OpenGL (via GLEW)
  - GLUT / FreeGLUT (windowing & input)
  - GLU (untuk gluLookAt, gluPerspective, gluOrtho2D)
  - FreeImage (opsional untuk texture di masa depan)
- **Compiler yang direkomendasikan**: g++ (Linux/Mac) atau MinGW (Windows)
