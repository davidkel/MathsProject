{ this module contains the logic to driver a graphics interface and so provide
  a hardcopy of the screen.

  The Mathematics Graphics drivers must be installed for this to work.}

{ Maintenance Log
  ---------------
Date              Action                           By
----              ------                           --
20/12/87       initial coding                      D.Kelsey



               Last Update: 20/12/87   14:30

}


{ module start }

{ these variables MUST be global }



var
  control:array [1..10] of integer;
  intin  :array [1..100] of integer;
  ptsin  :array [1..20] of integer;
  intout :array [1..45] of integer;
  ptsout :array [1..12] of integer;
  parmblock:array [1..10] of integer;

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

{----------------------------------------------------------------------------}

procedure hardcopy;

{ initialise ega system by setting up the parameter table, and opening the
  workstation.
}


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

{ section to output screen to printer }

  control[1]:=5;
  control[2]:=0;
  control[4]:=0;
  control[6]:=17;
  gsx;
end;
