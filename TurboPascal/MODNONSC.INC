
{ this procedure provides the logic for the entry of a general non linear
  equation, precision and initial value }


{ Maintenamce Log
  ---------------
Date            Action                                By
----            ------                                --
20/12/87      Initial Coding                          D.Kelsey


               Last Update: 20/12/87   15:20
}



Procedure Nonlinear_Entry(var eqn:astring;
                          var postfix:astring;
                          var precision:integer;
                          var initial_z:complex);

Const
   EqnBack = lightgray;   { set the colours for the background and foreground }
   EqnFore = blue;
   accback = black;      { Colour of Text for selection of accuracy part }
   accfore = yellow;
   NonLinBack = black;
   NonLinFore = yellow;
   Start_Col = 6;   { the starting coloumn of the eqn entry }
   Start_Line = 3;  { the starting Row of the eqn entry }
   line_length = 70; { length of an equation allowed }
   MessageX = 10;    { position of the message text }
   MessageY = 20;
   errorcol1 = red;        { colours of error highlighting }
   errorcol2 = green;
   errorcol3 = magenta;
   DecX = 40;              { precision entry location }
   DecY = 15;
   InitX = 48;             { initial value entry location }
   InitY = 18;
   DecLim = 7;            { decimal place accuracy limit }

var
  key:char;
  loop:integer;
  passed:boolean;
  errorpos:astring;

{ this Procedure displays the nonlinear screen }

procedure Nonlinear_Screen;

var
  over:integer;

  begin
    window(1,3,80,25);            { Set Screen Size, and background, clear }
    TextBackGround(NonLinBack);
    clrscr;
    writeln;
    writeln;
    write('F(z)=');
    TextBackground(Eqnback);
    TextColor(EqnFore);
    write(eqn);
    for over:=1 to line_length-length(eqn) do write(' ');
    textbackground(NonLinBack);
    textcolor(NonLinFore);
    writeln;
    writeln;
    writeln('ERROR CODES');
    writeln('-----------');
    textbackground(errorcol1);
    write(' ');
    textbackground(NonLinBack);
    writeln('  SYNTAX ERROR');
    writeln;
    textbackground(errorcol2);
    write(' ');
    textbackground(NonLinBack);
    writeln('  BRACKETS MISMATCH ');
    writeln;
    textbackground(errorcol3);
    write(' ');
    textbackground(NonLinBack);
    writeln('  UNKNOWN CHARACTER');
    GotoXY(MessageX-9,MessageY);
    write('MESSAGE :');
    TextMess(MessageX,MessageY,'Enter equation and press return');
  end;

{----------------------------------------------------------------------------}

{ the next procedure edits the equation that is being entered }

  procedure Edit_Equation(var expression:astring);

  var
    loopctr:integer;

  begin
    setcursor(true);   { turn the cursor on }
    GotoXY(Start_Col,Start_line);    { position it at the start of eqn }
    TextColor(EqnFore);            { select the colours }
    Textbackground(EqnBack);

{ process The Keypresses }

    repeat
      key:=keyboard;
      if not escape then begin
        case key of
          CB: if WhereX > Start_Col then write(bs);
          CF: if WhereX < Length(expression)+Start_Col then GotoXY(WhereX+1,WhereY);

          DEL: if length(expression) >= 1 then Remove_Char(0,Start_col,expression);

          F10: begin   { clear entry key }
                 expression:='';
                 textbackground(EqnBack);
                 textcolor(EqnFore);
                 GotoXY(start_col,start_line);
                 for loopctr:=1 to line_length do write(' ');
                 GotoXY(Start_Col,Start_Line);
               end;

          BS: if (length(expression) >= 1) and (WhereX > Start_Col) then Remove_Char(-1,Start_col,expression);
          CR: ;  { ignore Carriage return for the moment }

          else if length(expression) <= (line_length-1) then Insert_Char(key,start_Col,expression);

        end;   { Case }
      end;     { if }
    until escape or (key = CR);
  end;

{----------------------------------------------------------------------------}

{ the next procedure processes the entry for precision and initial value }

Procedure PrecAndInit;

begin
  TextMess(messagex,messagey,'Select precision'); { display message }
  GotoXY(DecX,DecY);
  textbackground(accback);
  textcolor(accfore);             { position text and select colours }
  write('Decimal place accuracy ');
  precision:=5;
  select_number(precision,DecLim);   { do precision selection }
  if not escape then begin
    Init_Num(initial_Z,InitX,InitY,messagex,messagey);
  end;
end;  { procedure }

{----------------------------------------------------------------------------}

{ Main section for Nonlinear routine }

begin
  Nonlinear_Screen;
  repeat
    repeat
      Edit_Equation(eqn);
      if (length(eqn) <> 0) and not escape then begin
        passed:=parser(eqn,postfix,errorpos);  { check equation }
        if not passed then begin               { is it ok ? }
          GotoXY(Start_Col,Start_Line);
          for loop:=1 to length(errorpos) do begin

            { highlight the errored places }

            case errorpos[loop] of
              '1','2','3','5','7','a' : textbackground(errorcol1);
              '4','6','8' : textbackground(errorcol2);
              '9'         : textbackground(errorcol3);
              else          textbackground(EditBackg);
            end;
            if loop <= length(eqn) then write(eqn[loop])
            else write(' ');
          end;       { for }
          TextMess(MessageX,MessageY,'Error(s) in above equation., Please Re-type.');
        end;
      end
      else escape:=true;   { in case length = 0, set escape to true }
    until escape or passed;
    if not escape then begin
      PrecAndInit;  { all ok so far, now get the precision and initial value }
      if not escape then begin    { if escape not pressed }
        setcursor(false);
        TextMess(messageX,messageY,'All data validiated, Space to solve');
        repeat
          key:=keyboard;
        until (escape) or (key = SPC);
      end;  { if }
    end;
  until (key = SPC) or escape;
end;   { procedure }

{----------------------------------------------------------------------------}
