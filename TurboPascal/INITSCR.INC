{ this include file contains the procedure for the initial screen }

{ Maintenance Log
  ---------------
Date                Action                         By
----                ------                         --
28/10/87          initial coding                   D.Kelsey


             Last Update: 28/10/87    12:00

}

procedure Initial_screen;

var
  ch:char;


begin
  window(1,3,80,25);
  textbackground(black);
  clrscr;
end;    { procedure }
