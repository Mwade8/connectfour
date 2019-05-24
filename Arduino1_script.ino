/*Arduino 1 Code

  THIS CODE SETS UP THE 8x8 LED GRID THAT WE WILL BE CONTROLLING

*/

int player_state = 0;
int incoming_serial_data = 2;
int data = 3;
int data_output_enable = 4;
int latch = 5;
int clock_ = 6;
int reset = 7;
bool pressed = true;
bool left_pressed = true;
bool right_pressed= true;

int button = 10;
int left_button = 9;
int right_button = 8;

byte selector = 1;
byte x_ = 2;
byte y_ = 2;
byte row_color = 4;
byte cursor_color = 5;
byte player_one_value;
byte player_two_value;
byte background_color = 0;
byte setup_state = 0;
byte height[6] = {1, 1, 1, 1, 1, 1};
byte available_colors[6] = {1, 2, 3, 4, 5, 6};

byte board[8][8] = {{0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0}};

void displayColorOptions()
{
  for(byte i = 1; i < 7; i++)
  {
    board[rotate_x(1, i)][rotate_y(1, i)] = available_colors[i-1];
  }
}

void displaySelectorRow(byte sColor, byte rColor)
{
  byte lineIndex;
  byte rowIndex;
  for(rowIndex = 1; rowIndex < 7; rowIndex++)
  {
    byte rot_x = rotate_x(0, rowIndex);
    byte rot_y = rotate_y(0, rowIndex);
    board[rot_x][rot_y] = rColor;
  }
  board[rotate_x(0, selector)][rotate_y(0, selector)] = sColor;
}

void displayBackground(byte color)
{
  byte lineIndex;
  byte rowIndex;
  for(lineIndex = 1; lineIndex < 8; lineIndex++)
  {
    for(rowIndex = 1; rowIndex < 7; rowIndex++)
    {
      byte rot_x = rotate_x(lineIndex, rowIndex);
      byte rot_y = rotate_y(lineIndex, rowIndex);
      board[rot_x][rot_y] = color;
    }
  }
}

void setupGame()
{
  if(setup_state == 0)
  {
    if(digitalRead(button) == HIGH && !pressed)
    {
      pressed = true;
      setup_state = 1;
      player_one_value = available_colors[selector-1];
      available_colors[selector-1] = 0;
    }
  }
  if(setup_state == 1)
  {
    if(digitalRead(button) == HIGH && !pressed)
    {
      pressed = true;
      if(available_colors[selector-1] != 0)
      {
        setup_state = 2;
        player_two_value = available_colors[selector-1];
        available_colors[selector-1] = 0;
      }
    }
  }
  if(setup_state == 2)
  {
    if(digitalRead(button) == HIGH && !pressed)
    {
      pressed = true;
      if(available_colors[selector-1] != 0)
      {
        setup_state = 3;
        background_color = available_colors[selector-1];
        available_colors[selector-1] = 0;
      }
    }
  }
  if(setup_state == 3)
  {
    if(digitalRead(button) == HIGH && !pressed)
    {
      pressed = true;
      if(available_colors[selector-1] != 0)
      {
        setup_state = 4;
        cursor_color = available_colors[selector-1];
        available_colors[selector-1] = 0;
      }
    }
  }
  if(setup_state == 5)
  {
    if(digitalRead(button) == HIGH && !pressed)
    {
      pressed = true;
      if(available_colors[selector-1] != 0)
      {
        setup_state = 6;
        row_color = available_colors[selector-1];
        available_colors[selector-1] = 0;
      }
    }
  }

  displayBackground(background_color);
  displayColorOptions();
  displaySelectorRow(cursor_color, row_color);
  
  if(digitalRead(left_button) == HIGH && !left_pressed)
  {
    move_color_selector(-1, cursor_color, row_color);
    left_pressed = true;
  }
  if(digitalRead(left_button) == LOW && left_pressed)
  {
    left_pressed = false;
  }
  if(digitalRead(button) == LOW && pressed)
  {
    pressed = false;
  }
}

void sendBit(uint8_t data_bit)
{
  digitalWrite(data, data_bit);
  digitalWrite(clock_, HIGH);
  digitalWrite(clock_, LOW);
  digitalWrite(data, LOW);
}

void sendDataFromArray()
{
  byte lineIndex;
  byte rowIndex;

  for(lineIndex = 0; lineIndex < 8; lineIndex++)
  {
    //Set Anode Line
    for(rowIndex = 0; rowIndex < 8; rowIndex++)
    {
      if (lineIndex == rowIndex)
      {
        sendBit(HIGH);
      }
      else
      {
        sendBit(LOW);
      }
    }

    //Set Green Row
    for(rowIndex = 0; rowIndex < 8; rowIndex++)
    {
      if ((board[lineIndex][rowIndex] & 1) == 0)
      {
        sendBit(HIGH);
      }
      else
      {
        sendBit(LOW);
      }
    }

    //Set Red Row
    for(rowIndex = 0; rowIndex < 8; rowIndex++)
    {
      if ((board[lineIndex][rowIndex] & 2) == 0)
      {
        sendBit(HIGH);
      }
      else
      {
        sendBit(LOW);
      }
    }

    //Set Blue Row
    for(rowIndex = 0; rowIndex < 8; rowIndex++)
    {
      if ((board[lineIndex][rowIndex] & 4) == 0)
      {
        sendBit(HIGH);
      }
      else
      {
        sendBit(LOW);
      }
    }

    //Send data to output
    digitalWrite(latch, HIGH);
    digitalWrite(latch, LOW);
  }
}

void clear_()
{
  byte lineIndex;
  byte rowIndex;
  for(lineIndex = 0; lineIndex < 8; lineIndex++)
  {
    for(rowIndex = 0; rowIndex < 8; rowIndex++)
    {
      board[lineIndex][rowIndex] = 0;
    }
  }
}

void solidColor(int color)
{
  byte lineIndex;
  byte rowIndex;

  for(lineIndex = 0; lineIndex < 8; lineIndex++)
  {
    for(rowIndex = 0; rowIndex < 8; rowIndex++)
    {
      board[lineIndex][rowIndex] = color;
    }
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(data, OUTPUT);
  pinMode(data_output_enable, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(clock_, OUTPUT);
  pinMode(reset, OUTPUT);
  pinMode(button, INPUT);
  pinMode(left_button, INPUT);
  pinMode(right_button, INPUT);
  
  digitalWrite(data_output_enable, LOW);
  digitalWrite(reset, HIGH);
  digitalWrite(clock_, LOW);
  digitalWrite(latch, LOW);
  digitalWrite(data, LOW);

  display_control_row();
  board[rotate_x(0, selector)][rotate_y(0, selector)] = 5;
  ShowRainBow();
  delay(200);

}

void ShowRainBow()
{  
  byte lineIndex;
  byte rowIndex;

  for(lineIndex = 1; lineIndex < 8; lineIndex++)
  {
    for(rowIndex = 1; rowIndex < 7; rowIndex++)
    {
      byte rot_x = rotate_x(lineIndex, rowIndex);
      byte rot_y = rotate_y(lineIndex, rowIndex);
      board[rot_x][rot_y] = 3;
    }
  }
}

byte rotate_x(byte x, byte y)
{
  return y;
}

byte rotate_y(byte x, byte y)
{
  return 7-x;
}


void display_control_row()
{
  byte lineIndex;
  byte rowIndex;
  for(rowIndex = 1; rowIndex < 7; rowIndex++)
  {
    byte rot_x = rotate_x(0, rowIndex);
    byte rot_y = rotate_y(0, rowIndex);
    board[rot_x][rot_y] = 4;
  }
  board[rotate_x(0, selector)][rotate_y(0, selector)] = 5;
}

void reset_board()
{
  selector = 1;
  ShowRainBow();
  display_control_row();
  for(int i = 0; i < 6; i++)
  {
    height[i] = 1;
  }
  player_state = 0;
}

bool place(byte y)
{
  byte val;
  if(height[y-1]>7)
  {
    return false;
  }
  if(player_state == 0)
  {
    val = 1;
    player_state = 1;
  }
  else
  {
    val = 2;
    player_state = 0;
  }
  byte x_ = height[y-1];
  // NECESSARY FOR ARDUINO COMMUNICATIONS
  Serial.println(x_);
  Serial.println(y);
  Serial.println(val);
  //
  height[y-1]++;
  byte rot_x = rotate_x(x_, y);
  byte rot_y = rotate_y(x_, y);
  board[rot_x][rot_y] = val;
  return true;
}

void move_color_selector(byte x, byte selector_color, byte row_color)
{
  byte x_ = rotate_x(0, selector);
  byte y_ = rotate_y(0, selector);
  board[x_][y_] = row_color;
  update_selector(x);
  x_ = rotate_x(0, selector);
  y_ = rotate_y(0, selector);
  board[x_][y_] = selector_color;
}

void move_selector(byte x)
{
  byte x_ = rotate_x(0, selector);
  byte y_ = rotate_y(0, selector);
  board[x_][y_] = 4;
  update_selector(x);
  x_ = rotate_x(0, selector);
  y_ = rotate_y(0, selector);
  board[x_][y_] = 5;
}

void update_selector(byte x)
{
  selector += x;
  if(selector > 6)
  {
    selector = 1;
  }
  if(selector < 1)
  {
    selector = 6;
  }
}

void loop() {
  if(digitalRead(button) == HIGH && !pressed)
  {
    pressed = true;
    place(selector);
  }
  if(digitalRead(right_button) == HIGH && !left_pressed && !right_pressed)
  {
    right_pressed = true;
    move_selector(1);
  }
  if(digitalRead(left_button) == HIGH && !left_pressed && !right_pressed)
  {
    left_pressed = true;
    move_selector(-1);
  }
  
  // TODO: IMPLEMENT FUNCTIONALITY HERE
  if(Serial.available() > 0)
  {
    char inByte = Serial.read();
//    BlinkBoard(int color);
    reset_board();
//    SetupGame();
  }
  sendDataFromArray();

  if(pressed && digitalRead(button) == LOW)
  {
    pressed = false;
  }
  if(left_pressed && digitalRead(left_button) == LOW)
  {
    left_pressed = false;
  }
  if(right_pressed && digitalRead(right_button) == LOW)
  {
    right_pressed = false;
  }
}
