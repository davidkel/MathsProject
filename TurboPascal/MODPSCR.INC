{ this Procedure will deal with the polynomial screen entry }


{ Maintenance Log
  ---------------
Date              Action                           By
----              ------                           --
20/12/87         Initial Coding                    D.Kelsey
15/02/88         general housekeeping              D.Kelsey
16/02/88         Fix bug in F10 keypress           D.Kelsey


               Last Update: 15/02/88   10:00
}



type
  ComplexStr = array[0..csize,1..2] of coeffst;

procedure Polynomial_Entry(var coefficients:complexstr;
                           var coeffs:carray;
                           var degree:integer;
                           var polyprec:integer;
                           var initial_z:complex);

const
  PolyBack = black;
  Polyfore = yellow;    { Basic screen colours }
  AccBack = black;
  AccFore = yellow;
  Start_line = 7;       { Start line of first coefficient }
  End_Line = 23;
  PmessX = 10;          { message position }
  PmessY = 1;
  MessInsb = red;       { colour of insert in message }
  DegX = 1;
  DegY = 3;
  PosZ = 1;
  PosReal = 6;          { position of real part and imag parts }
  PosImag = 23;
  DecX = 40;
  DecY = 15;
  InitX = 48;           { initial value position }
  InitY = 18;
  DecLim = 8;


var
  key:char;
  ClearCoeff:boolean;
  loop1,error,BlockStart:integer;

{ display a coefficient, together with its power }

Procedure Display_Coeff(ScrData:SingleNum;
                        CoeffNum:integer);

begin
  textbackground(PolyBack);
  textcolor(PolyFore);
  GotoXY(PosZ,WhereY);   { position on current line }
  write('Z^',CoeffNum);
  GotoXY(PosReal,WhereY);
  Display_Num(ScrData);
end;

{----------------------------------------------------------------------------}

{ the next procedure sets up the input screen }

  procedure Coeff_Screen(data:complexstr);
  var
    loop1,loop2:integer;
    OneNum:SingleNum;

  begin
    GotoXY(1,Start_Line-2);
    textbackground(PolyBack);
    textcolor(PolyFore);
    writeln('               Coefficients');
    writeln('         Real       ',BAR,'   Imaginary');
    loop1:=degree;
    while (loop1 >= 0) and ((degree-loop1) < (End_Line-Start_Line+1)) do begin
      GotoXY(1,degree-loop1+Start_Line);
      OneNum[1]:=Data[degree-loop1,1];    { cannot sent data, type mismatch occurs }
      OneNum[2]:=Data[degree-loop1,2];
      Display_Coeff(OneNum,loop1);   { display a coefficient }
      loop1:=loop1-1;
    end;
    GotoXY(PmessX-9,PmessY);
    textbackground(PolyBack);
    textColor(PolyFore);
    Write('MESSAGE :');
  end;

{----------------------------------------------------------------------------}

{ the Next procedure clears the coefficients }

  Procedure Clear_Coeffs;

  var
    count:integer;
  begin
    for count:=0 to Deglimit do begin
      coefficients[count,1]:='';
      coefficients[count,2]:='';
    end;
  end;

{----------------------------------------------------------------------------}

{ this procedure Controls the editing of the coefficients }

  Procedure Edit_Coefficients(var Coefficients:complexstr;
                              var clearRequest:boolean);


  var
    sel:byte;
    entered:boolean;
    CurCoeff:integer;

{ this next procedure does a scroll down process to add a coeff at the top }

Procedure Scroll_down(nextnum:integer);

var
  Num:SingleNum;

begin
  SetCursor(false);
  GotoXY(1,Start_line);
  TextBackGround(PolyBack);
  InsLine;    { insert a line causing scroll down }
  GotoXY(1,Start_Line);  { reposition cursor }
  Num[1]:=Coefficients[Degree-nextnum,1];
  Num[2]:=Coefficients[Degree-nextnum,2];
  Display_Coeff(num,nextnum);
  SetCursor(true);
end;

{----------------------------------------------------------------------------}

{ the next procedure does the same except upwards }

Procedure Scroll_up(nextnum:integer);

var
  num:singlenum;

begin
  SetCursor(false);
  GotoXY(1,Start_line);
  TextBackGround(PolyBack);
  DelLine;
  GotoXY(1,End_Line);
  Num[1]:=Coefficients[Degree-nextnum,1];
  Num[2]:=Coefficients[Degree-nextnum,2];
  Display_Coeff(num,nextnum);
  SetCursor(true);
end;

{----------------------------------------------------------------------------}

{ main section of Edit coefficients }

  begin
    setcursor(true);
    TextColor(EditCtext);
    TextBackGround(EditBackG);  { set the edit colours }
    clearRequest:=false;
    entered:=false;   { initialise flags }
    sel:=1;
    curCoeff:=degree;
    GotoXY(posreal,start_line);

{ process keypresses }

    repeat
      BlockStart:=posreal+(sel-1)*ImagOff;  { get start pos of block }
      high_block(coefficients[Degree-CurCoeff,sel],BlockStart);
      key:=keyboard;
      if not escape then begin
        case key of
          F1: entered:=true;
          F10: ClearRequest:=true;
          CU: begin
                 if CurCoeff < degree then begin
                   dehigh_block(coefficients[Degree-CurCoeff,sel],BlockStart);
                   if WhereY > Start_Line then GotoXY(BlockStart,WhereY-1)
                   else begin
                     Scroll_Down(CurCoeff+1);
                     GotoXY(BlockStart,Start_Line);
                   end;
                   CurCoeff:=CurCoeff+1;
                 end;
               end;
       CR,CD: begin
                 if CurCoeff > 0 then begin
                   dehigh_block(coefficients[Degree-CurCoeff,sel],BlockStart);
                   if key = CR then Begin
                     Sel:=1;
                     BlockStart:=PosReal;
                   end;
                   if WhereY < End_Line then GotoXY(BlockStart,whereY+1)
                   else begin
                     Scroll_up(CurCoeff-1);
                     GotoXY(BlockStart,End_Line);
                   end;
                   CurCoeff:=CurCoeff-1;
                 end;
               end;
          F9: begin   { clears an entry }
                Dehigh_block(coefficients[Degree-CurCoeff,sel],BlockStart);
                Coefficients[Degree-CurCoeff,sel]:='';
                GotoXY(blockStart,WhereY);
              end;

          else EditLine(coefficients[Degree-CurCoeff,1],coefficients[Degree-CurCoeff,2]
          ,key,posreal,sel);

        end;  { case }
      end;     { if }
    until escape or entered or clearRequest;

{ finished editing for some reason so dehighlight the current entry }

    dehigh_block(coefficients[Degree-CurCoeff,sel],BlockStart);
  end;    { procedure }

{----------------------------------------------------------------------------}

{ main section of polynomial entry }


begin
  window(1,3,80,25);         { set the window }
  TextBackGround(PolyBack);
  clrscr;                    { clear the screen }
  ClearCoeff:=false;
  setcursor(false);          { turn cursor off }

{ Get the degree of the polynomial }

  GotoXY(DegX,DegY);
  Textcolor(PolyFore);
  write('Enter Degree  ');
  degree:=1;
  Select_Number(degree,Deglimit);  { get degree }
  if not escape then begin
  repeat
    repeat
      if ClearCoeff then clear_coeffs;
      Coeff_Screen(coefficients);     { display screen }
      Edit_Coefficients(coefficients,ClearCoeff);  { edit screen }
      if not escape and not clearCoeff then begin
        loop1:=0;

{ process the entered coefficients }

        repeat
          shrink(coefficients[loop1,1]);
          shrink(coefficients[loop1,2]);
          if length(coefficients[loop1,1]) <> 0 then val(coefficients[loop1,1],coeffs[loop1,0],error)
          else begin
            coeffs[loop1,0]:=0;
            error:=0;
          end;
          if error = 0 then begin
            if length(coefficients[loop1,2]) <> 0 then val(coefficients[loop1,2],coeffs[loop1,1],error)
            else begin
              coeffs[loop1,1]:=0;
              error:=0;
            end;
          end;
          loop1:=loop1+1;
        until (error<>0) or (loop1 > degree);
        if error <> 0 then begin

{ an error has been found, display message and the errored coefficient }

          setcursor(false);
          TextMess(PmessX,PmessY,'error in coefficient ');
          GotoXY(PmessX+21,PmessY);
          textbackground(messinsb);
          write('z^',degree-loop1+1);
          TextMess(PmessX+25,PmessY,'  Please Re-Edit');
        end;    { if }
      end;      { if }
    until escape or ((error = 0) and not ClearCoeff);
    if not escape then begin
      TextMess(PmessX,PmessY,'Select precision'); { display message }
      GotoXY(DecX,DecY);
      textbackground(accback);
      textcolor(accfore);             { position text and select colours }
      write('Decimal place accuracy ');
      polyprec:=7;
      select_number(polyprec,DecLim);   { do precision selection }
      if not escape then Init_Num(initial_Z,InitX,InitY,PmessX,PmessY);
      if not escape then begin
        setcursor(false);
        TextMess(PmessX,PmessY,'All data validiated, press Space to solve');
        repeat
          key:=keyboard;
        until escape or (key = SPC);
      end;  { if }
    end;
    until escape or (key = SPC);
  end;
end;      { procedure }

{----------------------------------------------------------------------------}