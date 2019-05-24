// initialize LCD pins and button pins
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
const int RED = 1, BLUE = 2;
int board[8][8] = 
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void resetBoard()
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      board[i][j] = 0;
    }
  }
}

void setup() {
  // setup the lcd and buttons. Set up the interrupt properties of the buttons.
//  lcd.begin(16, 2); <<<<<
  Serial.begin(9600);
  resetBoard();
}

void update(int x, int y, int value)
{
  board[x][y] = value;
}

int CheckHorizontal()
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 5; j++)
    {
      if(board[i][j] == 1 && board[i][j+1] == 1 && board[i][j+2] == 1 && board[i][j+3] == 1)
      {
        return 1;
      }
      if(board[i][j] == 2 && board[i][j+1] == 2 && board[i][j+2] == 2 && board[i][j+3] == 2)
      {
        return 2;
      }
    }
  }
  return 0;
}

int CheckVertical()
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 5; j++)
    {
      if(board[j][i] == 1 && board[j+1][i] == 1 && board[j+2][i] == 1 && board[j+3][i] == 1)
      {
        return 1;
      }
      if(board[j][i] == 2 && board[j+1][i] == 2 && board[j+2][i] == 2 && board[j+3][i] == 2)
      {
        return 2;
      }
    }
  }
  return 0;
}

bool CheckDiagonalRight()
{
  for(int i = 0; i < 5; i++)
  {
    for(int j = 0; j < 5; j++)
    {
      if(board[i][j] == 1 && board[i+1][j+1] == 1 && board[i+2][j+2] == 1 && board[i+3][j+3] == 1)
      {
        return 1;
      }
      if(board[i][j] == 2 && board[i+1][j+1] == 2 && board[i+2][j+2] == 2 && board[i+3][j+3] == 2)
      {
        return 2;
      }
    }
  }
  return 0;
}

bool CheckDiagonalLeft()
{
  for(int i = 0; i < 5; i++)
  {
    for(int j = 0; j < 5; j++)
    {
      if(board[i][7-j] == 1 && board[i+1][6-j] == 1 && board[i+2][5-j] == 1 && board[i+3][4-j] == 1)
      {
        return 1;
      }
      if(board[i][7-j] == 2 && board[i+1][6-j] == 2 && board[i+2][5-j] == 2 && board[i+3][4-j] == 2)
      {
        return 2;
      }
    }
  }
  return 0;
}

int Checkwin()
{
  int val = CheckHorizontal();
  if(val != 0)
  {
    return val;
  }
  val = CheckVertical();
  if(val != 0)
  {
    return val;
  }
  val = CheckDiagonalRight();
  if(val != 0)
  {
    return val;
  }
  val = CheckDiagonalLeft();
  if(val != 0)
  {
    return val;
  }
  return 0;
}

void loop() {
  if(Serial.available() >= 3)
  {
    int x_pos = Serial.parseInt();
    int y_pos = Serial.parseInt();
    int val = Serial.parseInt();
    update(x_pos, y_pos, val);
    if(Checkwin() != 0)
    {
      Serial.print(Checkwin());
      resetBoard();
    }
  }
}
