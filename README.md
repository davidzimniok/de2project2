# Project 2

Precise generation of several PWM channels. Application of two (or more) Servo motors SG90.

## Instructions

The goal of the project is cooperation in pairs, further study of the topic, design of own solutions, implementation, creation of project documentation and presentation of results. The division of roles and tasks within the team is up to their members.

* Students work on a project in the labs during the 12th and 13th weeks of the semester.

* Through the BUT e-learning, students submit a link to the GitHub repository, which contains the complete project in PlatfomIO and a descriptive README file. The submission deadline is the day before the next laboratory, i.e. in 2 weeks.

* The source code for the AVR must be written in C and/or Assembly and must be implementable on Arduino Uno board using toolchains from the semester, ie PlatformIO and not in the Arduino-style. No other development tools are allowed.

## Recommended GitHub repository structure

```c
   project2        // PlatfomIO project
   ├── include         // Included files
   │   └── timer.h
   ├── lib             // Libraries
   │   └── gpio
   │      └── gpio.c
   │      └── gpio.h
   │   └── lcd
   │      └── lcd.c
   │      └── lcd.h
   │      └── lcd_definitions.h
   ├── src             // Source file(s)
   │   └── main.c
   ├── platformio.ini  // Project Configuration File
   └── README.md       // Report of this project
   ```


### Team members

* Vojtěch Vídenský (zodopovědný za zapojení, github repozitář, video, testování aplikace)
* David Zimniok (zodpovědný za zdrojový kód)

## Hardware description

V projektu je použit mikrokontrolér Arduino Uno, který je založen na AVR čipu ATMEGA328P. Jako vnější periferie jsou použity LCD displey Digilent pro zobrazování informací, joystick pro ovládání zařízení a dva servo motory SG90. Servo motory představují pohony  pohyb robotické ruky ve dvou dimenzích.

### Schéma zapojení


### Digilent PmodCLP LCD module
LCD diplej je výstupní zařízení, které umožňuje zobrazení znaků ASCII kódu uložených v interní paměti driveru HD44780, nebo znaků vložených do interní paměti uživatelem (8 pozic pro uživatelské znaky). Obsahuje 8 pinů pro vstupní data, 4 piny pro nastavování registrů a piny napájení. V naší aplikaci se používá pouze 4 datové piny, avšak informace vysílané na LCD jsou pořád 8-mi bitové, pouze vysílané ve 2 fázích. Podrobnou dokumentaci k tomuto LCD dipleji naleznete [zde](https://digilent.com/reference/_media/pmod:pmod:pmodCLP_rm.pdf).

V našem projektu se na displeji zobrazuje úhel otočení jednotlivých motorů robotické ruky ve dvou osách a směr otáčení, případně nulová poloha.

### Joystick
Joystick je dvou osé vstupní zařízení, které nám umožńuje pohyb ve čtyřech směrech. Jeho konstrukce obsahuje dva potenciometry, každý pro jednu osu. Napětí na potenciometru přivádíme na analogový vstup mikrokontroléru a převádíme na digitální hodnotu. Přesný popis funkčnosti naleznete [zde](https://create.arduino.cc/projecthub/MisterBotBreak/how-to-use-a-joystick-with-serial-monitor-1f04f0). Obsahuje 5 pinů. Dva pro napájení (+5 V, GND), jeden pro talčítko (SW) a dva pro analogové hodnoty napětí z potenciometru (VRX, VRY).

Joystick používáme pro ovládání simulátoru 2D robotické ruky.

### Servo motory SG90
Servo motory jsou speciální tip DC motorů, které obsahují řídící elektroniku. Ta z otáčejícího motoru udělá ovládací mechanismus, který drží svoji polohu v nastavené pozici. Navíc jsou servo motory schopny vyvinout poměrně velkou sílu při malé hmotnosti a rozměrech. Použité servo motory obsahují tři vodiče. Dva jsou napájecí červený (+5 V) a hnědý (GND). Poslední má obvykle oranžovou barvu a slouží pro komunikaci se servo motorem. Úhel natočení servo motoru probíhá pomocí PWM signálu, kdy úhel natočení určuje délka pulzu v logické 1. 

![image](https://user-images.githubusercontent.com/99399676/206911080-c242640b-169d-4054-bc55-c7c20d389ee4.png)
![image](https://user-images.githubusercontent.com/99399676/206911622-6686023e-4188-4ff6-b1fe-3b91884e14c5.png)


Převzato z: https://navody.dratek.cz/arduino-projekty/servo-motor.html

V projekty tyto motory simulují pohyb robotické ruky ve dvou dimenzích. Jeden servo motor zajišťuje pohyb v ose x a druhý v ose y.

## Software description

### Knihovna timer.h
Knihovna je použitá pouze pro nastavení časovače 1, který obsuhuje joystick a operace spojené s ním. Natáčí servomotorky a vypisuje data na LCD.

Knihovna timer.h je dostupná [zde](/include/timer.h). Popis registrů je dostupný v oficiálním manuálu [ATmega328P_Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf) na straně 108-113 pro timer 1.

### Knihovna gpio.h
Z knihovny gpio.h je využitá pouze jedna funkce, která by se dala nahradit pouhým zápisem 1čky do registru. Jedná se o funkci ```GPIO_output()```, která přiřazuje do DDRx registru 1čku na pozici pinu který používáme např.: ```DDRD |= (1UL<<2)``` je nastavení pinu 2 na portu D (PD2). Díky použití této funkce můžeme jednoduše změnit tyto piny, když bychom změnili zařízení například na Arduino Mega.

### Library lcd.h
Knihovna použita od autora Peter Fleury. Stránky projektu jsou dostupné na: [http://www.peterfleury.epizy.com/avr-lcd44780.html](http://www.peterfleury.epizy.com/avr-lcd44780.html). Ke knihovně je přidán soubor [lcd_definitions.h](/lib/lcd/lcd_definitions.h), který pouze definuje podobným způsobem jako pro GPIO porty, na kterých pinech jsou zapojené datové a řídící signály pro LCD display.

### Zdrojový kód main.c
Soubor se skládá z funkce main, která má za úkol nastavit všechny registry a vektory přerušení a následně obsahuje pouze vektor přerušení pro časovač 1, obsluhující pohyby joysticku.

#### Nastavení timerů pro použití PWM módu

Pro použití rychlého PWM je nutno nastavit správně registry. Prvním důležitým registrem je TCCRxB, který nastavuje předděličku pro výsledné PWM. Zde je důležité dodržet to, že pro natoční servomotorku do pozice odpovídající 0 stupňům je nutné přivést na servo signál, který nehledě na délku nulového impulsu musí být jedničkový stav dlouhý 1ms a pro natočení do úhlu 180 stupňů musí být impuls dlouhý 2ms. Proto je nutné nastavit dělení tak, aby perioda PWM signálu byla minimálně 2ms. Pokud nastavíme prescaler na 256 tak při frekvencí jádra 16MHz získáme frekvenci kolem 4ms (viz rovince).

![výpočet frekvence PWM signálu](/images/eq1.png)

Dále teoreticky pokud budeme signál délku periody vydělíme 255 (maximální hodnota, která určuje střídu) a vynásobíme požadovanou střídou v rozsahu 0 až 255, získáme požadovaný signál. Bohužel tato úvaha se nám nepotvrdila, avšak empiricky jsme nastavili střídy na: minimálně 35 a maximálne 135.

Registr TCCRxA nastavuje: bit 7 nastaví, že dojde k vynulování registru OC2A, když dojde k vyrovnání vnitčních čítačů, bit 0 a 1 nastavují právě fast PWM mód. Vše je opět dohledatelné v datesheetu v sekci popisující timer-y. ([ATmega328P_Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf))

Nastavení timeru 0
```c
    //set timer0 to fast PWM mode
    TCCR0A |= ((1UL<<7) | (1UL<<0) | (1UL<<1));
    //prescale clock for timer0 with N=256 for ensure maximal pulse width 2ms
    TCCR0B |= (1UL<<2);
```

Nastavení timeru 1
```c
    //set timer2 to fast PWM mode
    TCCR2A |= ((1UL<<7) | (1UL<<0) | (1UL<<1));
    //prescale clock for timer2 with N=256 for ensure maximal pulse width 2ms
    TCCR2B |= ((1UL<<2) | (1UL<<1));
```

#### Nastavení ADC převodníku pro čtení dat z joysticku

Nastavení převodníku je provedeno dle laboratorního cvičení ([dostupné zde](https://github.com/tomas-fryza/digital-electronics-2/tree/master/labs/05-adc)). 

```c
    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX |= (1UL<<6);
    // Select input channel ADC0 (voltage divider pin)
    ADMUX &= ~((1UL<<0)|(1UL<<1)|(1UL<<2)|(1UL<<3));
    // Enable ADC module
    ADCSRA |= (1UL<<7);
    // Enable conversion complete interrupt
    ADCSRA |= (1UL<<3);
    // Set clock prescaler to 128
    ADCSRA |= (1UL<<0)|(1UL<<1)|(1UL<<2);
```

#### Funkce main - vývojový diagram

![Vývojový diagram pro funkci main](/images/main_diag.png)

#### Vektor přerušení pro TIMER 1 vývojový diagram

![Vývojový diagram pro vektor přerušení časovače 1](/images/ovf_diag.png)

## Video

Insert a link to a short video with your practical implementation example (1-3 minutes, e.g. on YouTube).

## References

1. Arduino UNO pinout: [https://docs.arduino.cc/retired/boards/arduino-uno-rev3-with-long-pins](https://docs.arduino.cc/retired/boards/arduino-uno-rev3-with-long-pins)
2. Práce s PWM a se servo motorky: [https://www.teachmemicro.com/arduino-servo-motor-tutorial/](https://www.teachmemicro.com/arduino-servo-motor-tutorial/)
3. Datasheet ATmega 328P: [https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
