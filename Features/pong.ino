int player1y = 25;
int player2y = 25;
int p1s = 0;
int p2s = 0;
int lineLength = 15;

long ballx = 64.0;
long bally = 32.0;
long directionx = 1.0;
long directiony = 2.0;
int ballspeed = 0;
int prev = millis();

void displayPong(){
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos(53, 10);
  u8g.print(p1s);
  u8g.setPrintPos(72, 10);
  u8g.print(p2s);
  u8g.drawLine(64, 64, 64, 0); 
  u8g.drawLine(2, player1y, 2, player1y+lineLength);
  u8g.drawLine(3, player1y, 3, player1y+lineLength);
  u8g.drawLine(126, player2y, 126, player2y+lineLength);
  u8g.drawLine(125, player2y, 125, player2y+lineLength);
  u8g.drawCircle(ballx,bally,2);
}
void movePlayer1Up(){
  if(player1y > 4){
    player1y = player1y-3;
  }
}
void movePlayer1Down(){
  if(player1y < 64-lineLength){
    player1y = player1y+3;
  }
}
void movePlayer2Up(){
  if(player2y > 4){
    player2y = player2y-3;
  }
}
void movePlayer2Down(){
  if(player2y < 64-lineLength){
    player2y = player2y+3;
  }
}

void startPong(){
  p1s = 0;
  p2s = 0;
  player1y = 25;
  player2y = 25;
  ballx = random(60,70);
  bally = random(20,40);
  directiony = random(1,2);
  directionx = random(1,2);
  ballspeed = random(7,13);
  char key = keypad.getKey();
  while (key != '#')
  {
    key = keypad.getKey();
    if(key == '#'){
      break;
    }


      ballx = ballx+directionx;
      bally = bally+directiony;
      if(bally <= 1 || bally >= 63){
        directiony = 0-directiony;
      }
      if(ballx <= 3){
        if(player1y == bally || player1y == bally-1 || player1y == bally-2 || player1y == bally-3 || player1y == bally-4 || player1y == bally-5 || player1y == bally-6 || player1y == bally-7 || player1y == bally-8 || player1y == bally-9 || player1y == bally-10 || player1y == bally-11 || player1y == bally-12 || player1y == bally-13 || player1y == bally-14){
          directionx = 0-directionx;
        } else {
          p2s = p2s + 1;
          ballx = random(60,70);
          bally = random(20,40);
          directiony = random(1,2);
          directionx = random(1,2);
          ballspeed = random(7,13);
        }
      }
      if(ballx >= 125){
        if(player2y == bally || player2y == bally-1 || player2y == bally-2 || player2y == bally-3 || player2y == bally-4 || player2y == bally-5 || player2y == bally-6 || player2y == bally-7 || player2y == bally-8 || player2y == bally-9 || player2y == bally-10 || player2y == bally-11 || player2y == bally-12 || player2y == bally-13 || player2y == bally-14){
          directionx = 0-directionx;
        } else {
          p1s = p1s + 1;
          ballx = random(60,70);
          bally = random(20,40);
          directiony = random(1,2);
          directionx = random(1,2);
          ballspeed = random(7,13);
        }
      delay(10);
    }

    if(key == '1'){
      movePlayer1Up();
    }
    if(key == '4'){
      movePlayer1Down();
    }
    if(key == '3'){
      movePlayer2Up();
    }
    if(key == '6'){
      movePlayer2Down();
    }
    u8g.firstPage();  
    do 
    {
      displayPong();
    } while( u8g.nextPage() );
}}
