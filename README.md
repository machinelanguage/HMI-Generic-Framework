# ESP32 Generic HMI Framework

Arduino/PlatformIO tabanli, ekrana bagimli olmayan hafif HMI katmani. Cizim ve dokunma surucusunu frameworkten ayirir; ayni ekran modeli TFT, UART HMI veya seri hata ayiklama ciktisiyla calisabilir.

## Ozellikler

- `Renderer` soyutlamasi ile ekran surucusu degistirilebilir.
- `Screen` widget sahipligini uygulamada birakir; dinamik bellek kullanmaz.
- `Label` ve `Button` temel widgetlari, üstteki widget oncelikli hit-test ve merkezi olay yakalama.
- Sadece ekran kirliyken yeniden cizim.
- Calisan seri-cikti demo adaptoru; harici kutuphane gerektirmez.

## Kurulum

1. PlatformIO ile bu klasoru acin.
2. ESP32 kartini USB ile baglayin.
3. `pio run -t upload` ve `pio device monitor` komutlarini calistirin.

`src/main.cpp` icindeki `SerialRenderer`, hedef ekran surucusune uyarlanacak noktadir. Dokunmatik veya UART HMI olayinda `app.tap(x, y)` cagrisi yapin. Ekranlar `Screen`, kontroller ise `Widget` siniflariyla olusturulur.

## Mimari

```text
touch / UART input -> App::tap -> Screen -> Widget event -> application handler
application state   -> App::invalidate -> Screen::draw -> Renderer -> display
```

Donanim baglantilari bilerek projeye gomulmedi: ekran kutuphanesi, pinler, cozunurluk ve dokunmatik kalibrasyonu hedef karta gore `Renderer` ve giris adaptoru katmaninda belirlenmelidir.
