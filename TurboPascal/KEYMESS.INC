{ this file contains all the global definitions and procedures required
  for the include files
  It contains the message display routines, the screen edit routines and
  keyboard control routines }


{ Maintenance Log
  ---------------
Date                   Action                           By
----                   ------                           --
20/12/87              Initial Coding                    D.Kelsey
15/02/88          Fix bug in edit number                D.Kelsey


                  Last Update: 15/02/88  10:20

}




var
  escape:boolean;  { global escape key pressed flag, cleared before option select }





const
  SND = #0;                  { second keypress }
  BS = #8;                   { Back space }
  ESC = #27;                 { escape key }
  CB = #75;                 { Cursor Back }
  CF = #77;                 { Cursor Forward }
  DEL = #83;                { DEL character }
  STB = #15;                { Shifted TaB }
  TAB = #9;                 { TAB }
  CR = #13;                 { Carriage Return }
  CU = #72;                 { Cursor Up }
  CD = #80;                 { Cursor Down }
  SPC = #32;
  F1  = #59;
  F2  = #60;
  F3  = #61;
  F4  = #62;
  F5  = #63;                 { function Keys }
  F6  = #64;
  F7  = #65;
  F8  = #66;
  F9  = #67;
  F10 = #68;
  BAR = #179;                { seperation character }
  sepback = black;          { background and foreground for separation char }
  SepFore = yellow;
  EditBackg = lightgray;    { background and foreground for complex no. entry }
  EditCtext = blue;
  HiEdit = red;             { highlight colour for number entry }
  selectback = black;       { colour definitions for text on initial number }
  selectfore = yellow;
  text_lim = 14;            { text limit for a number }
  ImagOff = 17;             { text_Lim+3 }
  csize = 40;                  { size of carray }
  Deglimit = csize;            { degree entry limit max of csize }

type
  coeffst = string[text_lim];    { string for one part of a complex number string }
  singlenum = array [1..2] of coeffst;        { number string }
  position = array [1..2] of integer;         { position pointer type }
  carray = array [0..csize] of complex;       { array for coeffs }


{ this truncates a decimal value to number of decimal places required
  for display on the screen }

Procedure dectrunc(var num:accuracy;
                   decplac:integer);

var
  loopst:integer;
  dec1:accuracy;

begin
  dec1:=frac(num);                  { get fraction }
  dec1:=exp(ln(10)*decplac)*dec1;   { move required accurracy up }
  dec1:=int(dec1+0.5);              { remove other bits }
  dec1:=dec1/exp(ln(10)*decplac);   { move bits down }
  num:=int(num)+dec1;
end;

{----------------------------------------------------------------------------}


{ the next function waits for a keypress then returns a value }

function keyboard:char;

var
  pressed:char;

begin
  repeat until keypressed;
  pressed:=readkey;
  if (pressed = SND) and keypressed then pressed:=readkey;
  if pressed = ESC then escape:=true;
  keyboard:=pressed;
end;

{----------------------------------------------------------------------------}


{ the first section is message control }

Procedure TextMess(PosXm,PosYm:integer;    { display a message on a Text Screen }
                  messinfo:astring);

const
  messback = black;   { message colour definitions }
  messfore = green;
  clearmess = '                                                               ';

begin
  textbackground(messback);
  textcolor(messfore);
  GotoXY(PosXm,PosYm);
  write(clearmess);
  GotoXY(PosXm,PosYm);
  write(messinfo);
end;

{----------------------------------------------------------------------------}

Procedure Graphmess(MposX,MposY:integer;  { display a message on a graphic screen }
                    Mess:astring);

 const
  Messfore = green;

var
  MaxiX,MaxiY:integer;


begin
  MaxiX:=GetMaxX;
  MaxiY:=GetMaxY;

{ clears the area from the message to the bottom of the screen }

  SetViewPort(MposX,MposY,MaxiX,MaxiY,true);
  ClearViewPort;
  setViewPort(0,0,MaxiX,MaxiY,true);
  SetColor(messFore);
  OutTextXY(MposX,MposY,Mess);
end;

{----------------------------------------------------------------------------}

{ provide Character control for screen and text string }

procedure Remove_char(position,Start_col:integer;  { remove a character }
                      var text:astring);

var
  SaveX:byte;
begin
  SaveX:=WhereX;
  delete (text,WhereX-Start_col+1+position,1);  { remove char from string }
  GotoXY(Start_col,WhereY);
  write(text,' ');                              { display new string }
  GotoXY(SaveX+position,WhereY);                { place cursor }
end;

{----------------------------------------------------------------------------}

procedure Insert_Char(ch:char;                  { insert a character }
                      start_col:integer;
                      var text:astring);

var
  SaveX:byte;

begin
  SaveX:=WhereX;
  insert(ch,text,WhereX-Start_Col+1);   { insert character into string }
  GotoXY(Start_col,WhereY);
  write(text);                          { display text }
  GotoXY(SaveX+1,WhereY);               { position cursor }
end;

{----------------------------------------------------------------------------}

{ the next procedure highlights a block for number entry }

procedure high_block(text:coeffst;
                     posx:integer);

var
  SaveX:byte;
  loop:integer;

begin
  setcursor(true);
  textbackground(hiEdit);
  SaveX:=WhereX;
  GotoXY(posx,Wherey);    { posx contains the start position }
  write(text);            { highlight the text }
  for loop:=1 to text_lim-length(text) do write(' ');
  GotoXY(SaveX,WhereY);
end;

{----------------------------------------------------------------------------}

{ This procedure removes the highlight }

procedure dehigh_block(text:coeffst;
                       posx:integer);

var
  SaveX:byte;
  loop:integer;

begin
  textbackground(EditBackg);
  SaveX:=WhereX;
  GotoXY(posx,WhereY);
  write(text);
  for loop:=1 to text_lim-length(text) do write(' ');
  GotoXY(SaveX,WhereY);
end;

{----------------------------------------------------------------------------}

{ This procedure removes unwanted spaces from a string to allow conversion to
 a number.It also puts a '0' in front of a '.' if required }

procedure shrink(var number1:coeffst);

var
  numberb1:coeffst;
  count:integer;

begin
  count:=1;
  numberb1:='';
  while (count <= length(number1)) and (number1[count] <> ' ') do begin
    numberb1:=numberb1+number1[count];
    count:=count+1;
  end;
  number1:=numberb1;
end;  { procedure }

{----------------------------------------------------------------------------}

{ the next procedure displays a complex number string on the screen }

Procedure Display_Num(complex_number:SingleNum);

var
  loop:integer;

begin

  { cursor already positioned by calling routine }

  TextBackground(EditBackG);
  TextColor(EditCtext);
  write(Complex_Number[1]);
  for loop:=1 to Text_Lim-length(Complex_Number[1]) do write(' ');
  TextBackGround(SepBack);
  TextColor(SepFore);
  Write(' ',Bar,' ');
  TextBackGround(EditBackG);
  TextColor(EditCtext);
  write(Complex_Number[2]);
  for loop:=1 to Text_Lim-length(Complex_Number[2]) do write(' ');
end;

{----------------------------------------------------------------------------}

{ This procedure Edits a line for complex number entry }

procedure EditLine(var number1,number2:coeffst;
                   key:char;
                   Xstart:integer;
                   var side:byte);

var
  number:singlenum;
  post:position;


begin
  post[1]:=Xstart;         { set up array of positions for real and imag parts }
  Post[2]:=Xstart+ImagOff;
  number[1]:=number1;      { this has to be done this way, type mismatch occurs otherwise }
  number[2]:=number2;
  case key of
    CB: if WhereX > post[side] then write(bs);
    CF: if WhereX < post[side]+length(number[side]) then GotoXY(WhereX+1,WhereY);
    DEL: if length(number[side]) <> 0 then remove_char(0,post[side],number[side]);
    STB: GotoXY(post[side],WhereY);   { goto start of block being edited }
    TAB: begin                        { move to next block }
           dehigh_block(number[side],post[side]);
            if side=1 then side:=2
               else side:=1;
               GotoXY(post[side],WhereY);
               high_block(number[side],post[side]);
             end;
    BS : if WhereX > post[side] then remove_char(-1,post[side],number[side]);
    else if length(number[side]) < Text_Lim then Insert_char(key,post[side],number[side]);
  end;
  number1:=number[1];
  number2:=number[2];          { retrieve the edited values }
end;    { procedure }

{----------------------------------------------------------------------------}

{ this procedure gets an initial number }

Procedure Init_num(var init_val:complex;
                       PosX,PosY,MessX,MessY:integer);



var
  key:char;
  Compdata:singlenum;
  sel:byte;
  error:integer;
  clear_entry:boolean;

begin
  TextMess(MessX,MessY,'Enter Initial value');  { display message }
  GotoXY(PosX+text_lim - 5,PosY-1);
  textbackground(selectback);
  textcolor(selectfore);
  write('Initial Value');
  clear_entry:=true;       { initial clearing and displaying of data }
  repeat

    repeat
      if clear_entry then begin
        GotoXY(PosX,PosY);    { clear the all characters }
        TextBackground(editbackg);
        textcolor(editctext);
        for error:=1 to text_lim do write(' ');
        TextBackground(SepBack);
        textcolor(SepFore);
        write(' ',BAR,' ');
        TextBackground(editbackg);
        textcolor(editctext);
        for error:=1 to text_lim do write(' ');
        sel:=1;
        GotoXY(PosX,PosY);
        compdata[1]:='';
        compdata[2]:='';   { no need to keep initial value or precision }
        High_block(compdata[1],PosX);
        clear_entry:=false;
      end;
      key:=keyboard;
      if not escape then begin
        if key=F10 then clear_entry:=true { F10 pressed, set clear flag }
        else if key <> CR then EditLine(compdata[1],compdata[2],key,PosX,sel);
      end;
    until escape or (key = CR);
    dehigh_block(compdata[sel],PosX+(sel-1)*ImagOff);
    if key = CR then begin

 { Return Pressed, so shrink the coefficients and covert to numbers }

      shrink(compdata[1]);
      shrink(compdata[2]);
      if length(compdata[1]) <> 0 then val(compdata[1],init_val[0],error)
      else init_val[0]:=0;
      if error = 0 then begin
        if length(compdata[2]) <> 0 then val(compdata[2],init_val[1],error)
        else init_val[1]:=0;
      end;
      if error <> 0 then begin
        TextMess(MessX,MessY,'Incorrect data, please retype.');
        TextBackground(editbackg);
        textcolor(editctext);
        GotoXY(PosX,PosY);
        Sel:=1;
        High_Block(compdata[1],PosX);
      end;
    end;
  until (error=0) or escape;
end;  { procedure }

{----------------------------------------------------------------------------}


{ This procedure provides a way of selecting a number using only up and down
  cursor keys }

Procedure Select_number(var number:integer;
                        limit:integer);

const
  selback = green;
  selfore = white;

var
  key:char;

begin
  setcursor(false);
  textbackground(selback);
  textcolor(selfore);   { cursor off and set colours }
  repeat
    if number < 10 then write (' ',number,bs,bs)   { display number }
    else write(number,bs,bs);
    key:=keyboard;      { process a key press }
    if not escape then begin
      case key of
        CU : begin
               number:=number+1;
               if number > limit then number:=1;
             end;

        CD : begin
               number:=number-1;
               if number = 0 then number:=limit;
             end;

      end;  { case }
    end;    { if }
  until escape or (key = CR);
end;

{----------------------------------------------------------------------------}

{ end of include file }