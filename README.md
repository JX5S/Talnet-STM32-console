# Talnet-STM32-console
## Objednávání - plošné spoje
1. Otevřít https://jlcpcb.com/
2. "Add gerber file" a vybrat "GerberA.zip"
3. Změnit "Surface finish" na "LeadFree HASL", pokud nechceme olovo
4. Ostatní nastavení by se měla nastavit podle zip souboru
5. "Save to cart" pro přidání do košíku

6. Opakovat s "GerberB.zip" a objednat

## Objednávání - komponenty z LCSC
1. Otevřít https://lcsc.com/
2. "BOM Tool"
3. Nahrát "LCSC_komponenty.csv"
4. Nad sloupcem množství vybrat v nabídce "Quantity". V souboru jsou sloupce s množstvími pro 1 desku, 5 desek, 10 desek
5. Nad sloupcem "LCSC" vybrat v nabídce "LCSC Part Number"
6. "Next" pro pokračování
7. Zkontrolovat, že je vše na skladě, a kliknout "Add to cart" pro přidání do košíku

8. Pokud něco už není na skladě, je potřeba vybrat náhradní součástky. Většinu součástek by šlo nahradit jednodušše. Dne 30.4.2023 bylo na skladě vše, ale zásoba mikrokontroléru už se krátila - měl by jít nahradit rychlejším, ale dražším STM32F446RET6, bez úpravy desky.
9. Objednat. Pokud první objednáme z JLCPCB, získáme slevu na poštovné

## Objednávání - díly z Aliexpressu
Nabíjecí obvod: https://www.aliexpress.com/item/1005001580997981.html

Displej: https://www.aliexpress.com/item/32843115817.html

BMI160: https://www.aliexpress.com/item/32718555571.html

USB-C: https://www.aliexpress.com/item/1005004598158601.html

## Nastavení programovacího prostředí
1. Stáhneme, nainstalujeme a otevřeme Visual Studio Code
2. V sekci "Extensions" nainstalujeme rozšíření PlatformIO
3. Počkáme na dokončení instalace PlatformIO. Trvá to několik minut
4. File -> Open Folder -> otevřeme nějaký příklad. Měla by být vybrána složka obsahující soubor platformio.ini
5. V sekci "Platformio" otevřeme "Platformio home->Libraries" přidáme do otevřeného projektu knihovnu TFT_eSPI
6. V knihovně (cesta je Příklad/Lib/TFT_eSPI/) zaměníme soubor user_setup.h se souborem z tohoto repositáře. Tento soubor nastaví, jaký konkrétní displej máme. Je možné, že bude nutné mírně upravit pro jiné displeje, i když budou vypadat stejně
7. V sekci "Platformio -> Project tasks" spustíme "Upload" pro nahrání programu
