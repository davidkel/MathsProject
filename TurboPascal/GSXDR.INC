
type
  text = string[80];

{ these varaibles MUST be global }



var
  control:array [1..10] of integer;
  intin  :array [1..100] of integer;
  ptsin  :array [1..20] of integer;
  intout :array [1..45] of integer;
  ptsout :array [1..12] of integer;
  parmblock:array [1..10] of integer;
  curry_colour:integer;

{ this procedure sets up the parameter table address in the registers
  then uses an interrupt to process the driver
  }

procedure gsx;
begin
  inline
    ($50/$53/$51/$52/$56/$57/$1e/$06/   { save registers }
     $ba/parmblock/$b9/$73/$04/         { setup DX and CX }
     $cd/$e0/                           { int 224 }
     $07/$1f/$5f/$5e/$5a/$59/$5b/$58)
end;

{ initialise ega system by setting up the parameter table, and opening the
  workstation.
}

procedure init_driver;

var
  loop:integer;
begin
  parmblock[2]:=seg(control);
  parmblock[1]:=ofs(control);
  parmblock[4]:=seg(intin);
  parmblock[3]:=ofs(intin);
  parmblock[6]:=seg(ptsin);
  parmblock[5]:=ofs(ptsin);
  parmblock[8]:=seg(intout);
  parmblock[7]:=ofs(intout);
  parmblock[10]:=seg(ptsout);
  parmblock[9]:=ofs(ptsout);

{ try to open workstation }

  control[1]:=1;
  control[2]:=0;
  control[4]:=10;
  for loop:=1 to 10 do begin
    intin[loop]:=1;
  end;
  gsx;
end;

{ enter graphics mode procedure }

procedure graphicsmode;
begin
  control[1]:=5;
  control[2]:=0;
  control[4]:=0;
  control[6]:=2;

  gsx;
end;


{ enter text mode procedure }

procedure rettext;
begin
  control[1]:=5;
  control[2]:=0;
  control[4]:=0;
  control[6]:=3;

  gsx;
end;

{ procedure to draw a line }

procedure line(x1,y1,x2,y2:integer);
begin
  control[1]:=6;
  control[2]:=2;
  ptsin[1]:=x1;
  ptsin[2]:=y1;
  ptsin[3]:=x2;
  ptsin[4]:=y2;

  gsx;
end;

{ procedure to plot a point on the screen }

procedure plot(x,y:integer);
begin
  line(x,y,x,y);
end;

{ defines the colour to draw in.
  0 - black
  1 - red
  2 - green
  3 - blue
  4 - cyan
  5 - yellow
  6 - magenta
  7 - white

}

procedure foreground(colour:integer);
begin
  control[1]:=17;
  control[2]:=0;
  intin[1]:=colour;
  curry_colour:=colour;
  gsx;
end;

{ effectively sets a backgound colour by drawing in a rectangle in the requested colour }


procedure fillarea(x1,y1,x2,y2,colour:integer);
begin
  foreground(colour);
  control[1]:=11;
  control[2]:=2;
  control[4]:=0;
  control[6]:=1;
  ptsin[1]:=x1;
  ptsin[2]:=y1;
  ptsin[3]:=x2;
  ptsin[4]:=y2;
  gsx;
  foreground(curry_colour);
end;

{ this procedure will write text to a screen
  mode 1 = replace
  mode 2 = overstrike
  mode 4 = erase

}

procedure writetext(strg:text;
                    x,y,mode:integer);

var
  loop:integer;

begin
  control[1]:=32;
  control[2]:=0; intin[1]:=mode;
  gsx;

  control[1]:=8;
  control[2]:=1;
  control[4]:=length(strg);
  for loop:=1 to length(strg) do begin
    intin[loop]:=ord(strg[loop]);
  end;
  ptsin[1]:=x; ptsin[2]:=y;
  gsx;
end;

{ procedure to output screen to printer }

procedure hardcopy;
begin
  control[1]:=5;
  control[2]:=0;
  control[4]:=0;
  control[6]:=17;
  gsx;
end;
