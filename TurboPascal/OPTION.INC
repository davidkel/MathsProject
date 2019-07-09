{ this include file will process the options sent to it }

{ Maintenance Log
  ---------------
Date                  Action                       By
----                  ------                       --
20/12/87             Initial Coding                D.Kelsey



                 Last Update: 07/03/88   15:00

}



type
  optionstr = string[15];
  select = array[1..10] of optionstr;



procedure Option_Select(options:select;
                        option_count:integer;
                        var chosen:integer);

const
  hi_col = red;     { Define the colours used }
  back_col = lightblue;
  txt_col = white;


var
  loop,loop1,amount:integer;
  key:char;
  offset:array[0..10] of integer;

{ This procedure will highlight an option }

  procedure highlight(chosen:integer);

  begin
    GotoXY(offset[chosen],1);
    textbackground(hi_col);   { select Colour, and highlight }
    write(options[chosen]);
    textbackground(back_col);
  end;

{----------------------------------------------------------------------------}

{ This procedure returns a highlighted option to normal }

  procedure unhighlight(chosen:integer);

  begin
    GotoXY(offset[chosen],1);
    write(options[chosen]);
  end;

{----------------------------------------------------------------------------}

{ The MAIN LINE routine }

begin
  setcursor(false);      { turn cursor off }
  TextColor(txt_col);
  textbackground(back_col);
  window(1,1,80,2);
  clrscr;                { set window and clear Screen }
  TextBackground(back_col);
  for loop:=1 to 80 do write(' ');
  chosen:=1;
  amount:=trunc(80/option_count);    { calculate postions of options }
  for loop:=1 to option_count do begin
    offset[loop]:=trunc((amount-length(options[loop]))/2);
    if offset[loop] < 0 then offset[loop]:=0;
    offset[loop]:=offset[loop]+amount*(loop-1);
    GotoXY(offset[loop],1);
    write(options[loop]);             { display an option }
  end;
  GotoXY(2,2);
  for loop:=1 to 78 do write('_');

{ Initial screen displayed, now process keypresses }

  repeat
    highlight(chosen);
    repeat until keypressed;
    key:=readkey;
    if (key = #0) and keypressed then key:=readkey;
    if (key = #77) and (chosen < option_count) then begin   { --> arrow pressed }
      unhighlight(chosen);
      chosen:=chosen+1;
    end;
    if (key = #75) and (chosen > 1) then begin   { <-- arrow pressed }
      unhighlight(chosen);
      chosen:=chosen-1;
    end;
  until key = #13;
end;  { Procedure }

{----------------------------------------------------------------------------}