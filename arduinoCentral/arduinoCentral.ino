// 仕様：ROW側がカソード、COLOMN側がアノードの8x8マトリックスLED
const int RPIN[8]   = { 6, 7, 8, 9, 10, 11, 12, 13 };
const int CATHODEPIN[8] = {  2,  3,  4,  5,  14,  15,  16,  17 };

const int sx = 8;
const int sy = 8;
const float density = 0.5; 
int Rworld[8][8][2];
int Gworld[8][8][2];
int Bworld[8][8][2];

const int b_size = 1;
int counter = 0;

void setup()
{
  // アノードを、LOWで初期化
  for( int i = 0; i < 8; i++ )
  {
    pinMode( RPIN[i], OUTPUT );
    digitalWrite( RPIN[i], LOW );
  }

  for( int i = 0; i < 8; i++ )
  {
    pinMode( CATHODEPIN[i], OUTPUT );
    digitalWrite( CATHODEPIN[i], HIGH );
  }

  //Make Stage
  randomSeed(analogRead(5));
  for (int i = 0; i < sx * sy * density; i++) { 
    Rworld[(int)random(sx)][(int)random(sy)][1] = 1; 
    Gworld[(int)random(sx)][(int)random(sy)][1] = 1; 
    Bworld[(int)random(sx)][(int)random(sy)][1] = 1; 
  }
  
  Serial.begin(9600);
}

void loop()
{


  
    //DRAW STAGE
    //DRAW RWORLD
    
    for (int y = 0; y < sy; y++) {
     if (CATHODEPIN[y] < 6) { PORTD &= ~_BV(CATHODEPIN[y]);}
     else { PORTC &= ~_BV(CATHODEPIN[y]-14); }

      for (int x = 0; x < sx; x++) {       
          if ((Rworld[x][y][1] == 1) || (Rworld[x][y][1] == 0 && Rworld[x][y][0] == 1)) {
            Rworld[x][y][0] = 1;

            //OUTPUT
            if (RPIN[x] < 8) {
              PORTD |= _BV(RPIN[x]);
              } else {
                  PORTB |= _BV(RPIN[x]-8);
                }            
          }
             
          if (Rworld[x][y][1] == -1) {Rworld[x][y][0] = 0;}
          Rworld[x][y][1] = 0;
          
        }
        
        delayMicroseconds(100);
 
        //RESET
        PORTD &= ~(_BV(6) | _BV(7));
        PORTB &= ~(_BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5));
        if (CATHODEPIN[y] < 6) { PORTD |= _BV(CATHODEPIN[y]);}
        else { PORTC |= _BV(CATHODEPIN[y]-14); }
        }



    ///
    // DRAW BWORLD
    ///
    for (int y = 0; y < sy; y++) {
     if (CATHODEPIN[y] < 6) { PORTD &= ~_BV(CATHODEPIN[y]);}
     else { PORTC &= ~_BV(CATHODEPIN[y]-14); }
          
     Serial.print('B');
          
      for (int x = 0; x < sx; x++) {
          if ((Bworld[x][y][1] == 1) || (Bworld[x][y][1] == 0 && Bworld[x][y][0] == 1)) {
            Bworld[x][y][0] = 1;
              
            Serial.print(1);
            } else {
            Serial.print(0);            
              }
              
          if (Bworld[x][y][1] == -1) {Bworld[x][y][0] = 0;}
          Bworld[x][y][1] = 0; 
        }
        
        while (!endCheck());

      //RESET
      if (CATHODEPIN[y] < 6) { PORTD |= _BV(CATHODEPIN[y]);}
      else { PORTC |= _BV(CATHODEPIN[y]-14); }
      }


    ///
    // DRAW GWORLD
    ///
    for (int y = 0; y < sy; y++) {
     if (CATHODEPIN[y] < 6) { PORTD &= ~_BV(CATHODEPIN[y]);}
     else { PORTC &= ~_BV(CATHODEPIN[y]-14); }
          
     Serial.print('G');
          
      for (int x = 0; x < sx; x++) {
          if ((Gworld[x][y][1] == 1) || (Gworld[x][y][1] == 0 && Gworld[x][y][0] == 1)) {
            Gworld[x][y][0] = 1;
              
            Serial.print(1);
            } else {
            Serial.print(0);            
              }
              
          if (Gworld[x][y][1] == -1) {Gworld[x][y][0] = 0;}
          Gworld[x][y][1] = 0; 
        }
        
        while (!endCheck());

      //RESET
      if (CATHODEPIN[y] < 6) { PORTD |= _BV(CATHODEPIN[y]);}
      else { PORTC |= _BV(CATHODEPIN[y]-14); }
      }

  //Change STAGE
  counter++;
  int count;
  if((counter%10) == 0) {
   for (int x = 0; x < sx; x++) { 
    for (int y = 0; y < sy; y++) { 
      count = Rneighbors(x, y); 
      //Serial.println(count);
      if (count == 3 && Rworld[x][y][0] == 0) { Rworld[x][y][1] = 1; } 
      if ((count < 2 || count > 3) && Rworld[x][y][0] == 1) { Rworld[x][y][1] = -1; }

      count = Gneighbors(x, y); 
      //Serial.println(count);
      if (count == 3 && Gworld[x][y][0] == 0) { Gworld[x][y][1] = 1; } 
      if ((count < 2 || count > 3) && Gworld[x][y][0] == 1) { Gworld[x][y][1] = -1; }

      count = Bneighbors(x, y); 
      //Serial.println(count);
      if (count == 3 && Bworld[x][y][0] == 0) { Bworld[x][y][1] = 1; } 
      if ((count < 2 || count > 3) && Bworld[x][y][0] == 1) { Bworld[x][y][1] = -1; }
    }
   }
  }
}

//足りない
inline int Rneighbors(int x, int y) 
{ 
  return Rworld[(x + 1) % sx][y][0] + 
         Rworld[x][(y + b_size) % sy][0] + 
         Rworld[(x + sx - b_size) % sx][y][0] + 
         Rworld[x][(y + sx - b_size) % sx][0] + 
         Rworld[(x + b_size) % sx][(y + b_size) % sy][0] + 
         Rworld[(x + sx - b_size) % sx][(y + b_size) % sy][0] + 
         Rworld[(x + sx - b_size) % sx][(y + sy - b_size) % sy][0] + 
         Rworld[(x + b_size) % sx][(y + sy - b_size) % sy][0]; 
}

inline int Gneighbors(int x, int y) 
{ 
  return Gworld[(x + 1) % sx][y][0] + 
         Gworld[x][(y + b_size) % sy][0] + 
         Gworld[(x + sx - b_size) % sx][y][0] + 
         Gworld[x][(y + sx - b_size) % sx][0] + 
         Gworld[(x + b_size) % sx][(y + b_size) % sy][0] + 
         Gworld[(x + sx - b_size) % sx][(y + b_size) % sy][0] + 
         Gworld[(x + sx - b_size) % sx][(y + sy - b_size) % sy][0] + 
         Gworld[(x + b_size) % sx][(y + sy - b_size) % sy][0]; 
}

inline int Bneighbors(int x, int y) 
{ 
  return Bworld[(x + 1) % sx][y][0] + 
         Bworld[x][(y + b_size) % sy][0] + 
         Bworld[(x + sx - b_size) % sx][y][0] + 
         Bworld[x][(y + sx - b_size) % sx][0] + 
         Bworld[(x + b_size) % sx][(y + b_size) % sy][0] + 
         Bworld[(x + sx - b_size) % sx][(y + b_size) % sy][0] + 
         Bworld[(x + sx - b_size) % sx][(y + sy - b_size) % sy][0] + 
         Bworld[(x + b_size) % sx][(y + sy - b_size) % sy][0]; 
}

inline boolean endCheck() {
  if(Serial.available()){
      char var = Serial.read();
      if (var == 'E') {return true;}
    }

    return false;
}
