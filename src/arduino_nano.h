// #include <TimerOne.h>
// #include <SPI.h>
// #include <avr/wdt.h>
// #include <GyverTimer.h>

// #define PIN_CLOCK 2
// #define PIN_DATA 3
// #define TIMEOUT 3000
// volatile int interruptState = 0;

// uint8_t Pocket[40];
// uint8_t p_int[40];
// uint8_t fill[40];
// uint8_t index = 0;

// uint8_t Pocket_Digi_Star[7];
// bool ready_data;

// struct sTab
// {
//     uint16_t Dec;
//     char chr;
// };

// sTab Tabl[] = {{0, ' '}, {96, '-'}, {360, '+'}, {648, 'Y'}, {660, 'X'}, {1020, '*'}, {3216, 'V'}, {3268, 'K'}, {4096, '_'}, {7168, 'L'}, {24576, '1'}, {25696, '4'}, {27668, 'W'}, {27744, 'H'}, {28164, 'N'}, {28288, 'M'}, {30720, 'J'}, {31744, 'U'}, {33032, 'T'}, {35936, 'F'}, {37008, 'Z'}, {37128, 'I'}, {39936, 'C'}, {40032, 'E'}, {46176, 'S'}, {48160, 'G'}, {48224, '6'}, {52320, 'P'}, {52324, 'R'}, {55392, '2'}, {57344, '7'}, {60512, 'A'}, {61536, '3'}, {61704, 'D'}, {61800, 'B'}, {62560, '9'}, {64512, 'O'}, {64516, 'Q'}, {64608, '8'}};

// uint16_t T_nach = 100;
// uint16_t T_byte = 14;

// void interrupt_();

// char decTosrc(uint16_t in_)
// {
//     uint8_t l = 0;
//     uint8_t r = 38;
//     while (l <= r)
//     {
//         uint8_t i = (l + r) / 2;
//         if (in_ == Tabl[i].Dec)
//             return Tabl[i].chr;
//         else if (Tabl[i].Dec < in_)
//             l = i + 1;
//         else
//             r = i - 1;
//     }
//     return 0;
// }

// void setup()
// {

//     pinMode(PIN_CLOCK, INPUT);
//     pinMode(PIN_DATA, INPUT);
//     attachInterrupt(0, interrupt_, RISING);
//     Timer1.initialize(500000);
//     Timer1.start();
//     Serial.begin(9600);

//     Serial.println("Start!!!");

//     wdt_enable(WDTO_8S);
// }

// void loop()
// {

//     static uint32_t at_millis = 0;
//     ready_data = false;

//     if (index == 18)
//     {

//         for (int i = 0; i < 19; i++)
//         {
//             if (i >= 6 && i <= 17)
//             {
//                 uint16_t l = Pocket[i];
//                 l <<= 8;
//                 i++;
//                 l |= Pocket[i];
//                 l &= 65532;
//                 Pocket_Digi_Star[(i - 6) / 2] = decTosrc(l);
//                 if (i == 17)
//                 {
//                     Pocket_Digi_Star[6] = Pocket[5] == 224;
//                     ready_data = true;
//                 }
//             }
//         }
//         at_millis = millis() + TIMEOUT;
//     }
//     else
//     {
//         if (at_millis <= millis())
//         {
//             Serial.println("Error Data");
//             at_millis = millis() + TIMEOUT;
//         }
//     }

//     if (ready_data)
//     {

//         Serial.write(Pocket_Digi_Star, 6);
//         Serial.println();
//     }

//     wdt_reset();
// }

// void interrupt_()
// {

//     uint16_t t = TCNT1;
//     Timer1.restart();
//     if (t < T_nach)
//     {
//         if (t >= T_byte)
//         {
//             index++;
//         }
//         bool pd = digitalRead(PIN_DATA);

//         p_int[index] = p_int[index] << 1;
//         p_int[index] = p_int[index] | pd;
//     }
//     else
//     {
//         memcpy(Pocket, p_int, 19);
//         memcpy(p_int, fill, 19);
//         index = 255;
//     }
// }
