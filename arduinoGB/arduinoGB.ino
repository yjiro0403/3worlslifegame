int BPIN[8]   = { 2, 3, 4, 5, 6, 7, 8, 9 };
int GPIN[8] = {  10,  11,  12,  13,  14,  15,  16,  17 };

void setup()
{
  // アノードを、LOWで初期化
  for( int i = 0; i < 8; i++ )
  {
    pinMode( BPIN[i], OUTPUT );
    digitalWrite( BPIN[i], LOW );
  }

  // カソードを、HIGHで初期化
  for( int i = 0; i < 8; i++ )
  {
    pinMode( GPIN[i], OUTPUT );
    digitalWrite( GPIN[i], LOW );
  }
  
  Serial.begin(9600);
}

void loop()
{
    // 受信バッファに３バイト（ヘッダ＋int）以上のデータが着ているか確認
  while ( !Serial.available());
    char checkBit = Serial.read();

    for (int x = 0; x < 8; x++) {  
        while ( !Serial.available() );
        char val = Serial.read();

        if (checkBit == 'G') {
         if (val == '1') {
          if(GPIN[x] < 14) { PORTB |= _BV(GPIN[x]-8);}
          else {PORTC |= _BV(GPIN[x]-14);}
          }
        } else if (checkBit == 'B') {
         if (val == '1') {
          if(BPIN[x] < 8) { PORTD |= _BV(BPIN[x]);}
          else {PORTB |= _BV(BPIN[x]-8);}
          }
        }
      }

      delayMicroseconds(0);

      if (checkBit == 'G') {
        //for (int x = 0; x < 8; x++) {digitalWrite(GPIN[x], LOW);}
        PORTB &= ~(_BV(2) | _BV(3) | _BV(4) | _BV(5));
        PORTC &= ~(_BV(0) | _BV(1) | _BV(2) | _BV(3));
      } else if (checkBit == 'B') {
        //for (int x = 0; x < 8; x++) {digitalWrite(BPIN[x], LOW);}
        PORTD &= ~(_BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7));
        PORTB &= ~(_BV(0) | _BV(1));
        }
      delayMicroseconds(5000);
      Serial.print('E'); //End this row

      

}
