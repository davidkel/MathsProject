{ this module contains the logic to solve a non-linear equation }


{ Maintenance log
  ---------------
Date          Action                       By
----          ------                       --
20/12/87     Initial Coding                D.Kelsey
15/02/88     Approach logic included       D.Kelsey



            Last update: 15/02/88    11:25

}




procedure solve_nonlinear(expression,postfix:astring;
                          var Root:carray;
                          var RootCnt:integer;
                          prec:integer;
                          init_z:complex);

const
  TxtCol = white;
  BackCol = Black;
  Bar = #179;
  TxtMessBack = black;
  TxtMessFore = White;
  InitReal = 48;
  Ypos = 18;
  RootBack = black;
  RootFore = green;
  EqnFore = White;
  EqnBack = blue;
  ApproxBack = black;
  ApproxFore = green;
  lines = 25;     { number of lines that can be used }
  MaxRoot = 20;   { maximun number of roots allowed }

var
  MaxX,MaxY,CharWidth,New_Line,StrLen,RealRootX,ImagRootX,RootY:integer;
  ApproxRealX,ApproxImagX,ApproxY,MessX,MessY,Div_line,count,Start:integer;
  ApproxRoots,Approach:carray;
  place:integer;
  wrapover,GraphOn,exit:boolean;
  e1,e2:accuracy;

  Procedure Disp_Root(cval:complex;
                         n:integer);


  var
    text:astring;

  begin
    TextBackground(RootBack);
    TextColor(RootFore);    { Set colours }
    Str(cval[0]:strlen:prec,text);
    if length(text) > strlen then text[0]:=chr(strlen);
    GotoXY(RealRootX,RootY+n*New_Line);
    write(text);
    Str(cval[1]:strlen:prec,text);
    if length(text) > strlen then text[0]:=chr(strlen);
    GotoXY(ImagRootX,RootY+n*New_Line);
    write(text);
  end;   { procedure }

{----------------------------------------------------------------------------}

  procedure Display_Screen(root:carray;
                           num:integer);

  var
    loop:integer;


  begin
    MaxX:=80;
    MaxY:=25;
    CharWidth:=1;    { set the screen dimensions }
    New_Line:=MaxY div lines;
    Strlen:=((MaxX div 4) div charwidth)-2;
    RealRootX:=1;
    ImagRootX:=RealRootX+(strlen+2)*charwidth;   { set the root positions }
    Div_line:=RealRootX+strlen+round(charwidth/2);
    RootY:=New_Line*5;                           { root start line }
    ApproxRealX:=ImagRootX+(strlen+1)*charwidth; { approx positions }
    ApproxImagX:=ApproxRealX+(strlen+1)*charwidth;
    ApproxY:=New_line*(lines div 2);             { approx start line }
    MessX:=Charwidth*9;                          { message positions }
    MessY:=MaxY;
    Window(1,1,80,25);    { reset the window }
    setcursor(false);
    ClrScr;
    GotoXY(1,1);
    TextColor(EqnFore);        { set the equation colours }
    TextBackGround(EqnBack);
    write(expression);         { display the equation being solved }
    TextColor(TxtCol);         { restore the colours }
    TextBackground(BackCol);

    { display the text on the screen }

    GotoXY(RealRootX+((Strlen*charwidth) div 2)-2*charwidth,RootY-1*New_line);
    write('Real');
    GotoXY(ImagRootX+((Strlen*charwidth) div 2)-2*charwidth,RootY-1*New_line);
    write('Imag');
    GotoXY((RealRootX+strlen*charwidth)-2*charwidth,RootY-2*New_line);
    writeln('ROOTS');
    GotoXY(ApproxRealX+((Strlen*charwidth) div 2)-2*charwidth,ApproxY-1*New_line);
    write('Real');
    GotoXY(ApproxImagX+((Strlen*charwidth) div 2)-2*charwidth,ApproxY-1*New_line);
    write('Imag');
    GotoXY(ApproxRealX+(strlen*charwidth)-3*charwidth,ApproxY-2*New_line);
    writeln('Approx');

    { display the currently found roots }

    for loop:=0 to MaxRoot-1 do begin
      if num > loop then Disp_Root(Root[loop+1],loop);  { display a root if found }
      TextBackground(BackCol);   { set colours and display separator }
      TextColor(TxtCol);
      GotoXY(Div_line,RootY+loop*New_line);
      write(bar);
   end;
   GotoXY(MessX-8,MessY);
   TextBackGround(txtMessBack);
   TextColor(TxtMessFore);
   write('MESSAGE:');
  end;   { procedure }

{----------------------------------------------------------------------------}

procedure ApprDiag(ar:carray;
                   nu:integer);


const
   lines = 20;
   TxtBack = black;
   TxtFore = blue;
   ScrBack = black;
   LineFore = blue;
   CircFore = lightblue;
   Xcol = green;
   RangeCol = white;
   CircSize = 6;

var
  graphdriver,graphmode,errorcode:integer;
  MaxX,MaxY,CharWidth,New_Line,GrMessX,GrMessY,PlotX,PlotY,OldPlotX,OldPlotY:integer;
  Xline,Yline,looproot:integer;
  ScaleX,ScaleY,SizeX,SizeY:real;
  strroot:astring;
  MaxRoot,MinRoot:complex;
  KeyPress:char;

procedure initscr;

begin
  graphdriver:=EGA;    { select EGA graphics }
  GraphMode:=EGA64hi;
  initgraph(graphdriver,graphmode,'');  { initialise graphics driver }
  errorcode:=graphresult;
  if errorcode = grok then begin

    { sey up required graphic variables }

    MaxX:=getMaxX;
    MaxY:=getMaxY;
    Charwidth:=8;
    New_line:=MaxY div lines;
    GrMessX:=9*charwidth;
    GrMessY:=MaxY-New_line;
    SetBkColor(TxtBack);
    SetColor(txtFore);
    OuttextXY(GrMessX-9*charwidth,GrMessY,'MESSAGE:');
    OutTextXY(MaxX div 4,1,'Root Approach Diagram');
  end;  { if }
end; { procedure }

{----------------------------------------------------------------------------}


begin
  initscr;
  if errorcode = grok then begin

    { locate the maximum real and imaginary parts as well as the minimum }

    MaxRoot[0]:=ar[1,0];
    MaxRoot[1]:=ar[1,1];
    MinRoot[0]:=ar[1,0];
    MinRoot[1]:=ar[1,1];
    for looproot:=1 to nu do begin
      if MaxRoot[0] < ar[looproot,0] then MaxRoot[0]:=ar[looproot,0];
      if MaxRoot[1] < ar[looproot,1] then MaxRoot[1]:=ar[looproot,1];
      if MinRoot[0] > ar[looproot,0] then MinRoot[0]:=ar[looproot,0];
      if MinRoot[1] > ar[looproot,1] then MinRoot[1]:=ar[looproot,1];
    end;

    { calculate the resolution }

    SizeX:=MaxRoot[0]-MinRoot[0];
    SizeY:=MaxRoot[1]-MinRoot[1];
    if SizeX < 1e-10 then SizeX:=1e-5;
    if SizeY < 1e-10 then SizeY:=1e-5;
    SizeX:=SizeX+SizeX/10;
    SizeY:=SizeY+SizeY/10;

    { locate the scale conversion for a point }

    scaleX:=(MaxX-6*charwidth)/SizeX;
    ScaleY:=(MaxY-New_line*6)/SizeY;
    SetBkColor(scrBack);
    SetColor(LineFore);
    if MinRoot[0]*MaxRoot[0] < 0 then Xline:=trunc(abs(MinRoot[0])*ScaleX)
    else Xline:=0;
    if MinRoot[1]*MaxRoot[1] < 0 then YLine:=MaxY-2*new_Line-trunc(abs(MinRoot[1])*ScaleY)
    else Yline:=MaxY-2*new_line;
    line(Xline,New_Line*2,Xline,MaxY-2*New_line);  { display axes }
    line(0,Yline,MaxX,Yline);

    { plot the points }

    for looproot:=1 to nu do begin
      dectrunc(ar[looproot,0],prec);
      dectrunc(ar[looproot,1],prec);
      PlotX:=6*charwidth+trunc((ar[looproot,0]-MinRoot[0])*ScaleX);
      PlotY:=MaxY-3*new_Line-trunc((ar[looproot,1]-MinRoot[1])*ScaleY);
      SetColor(Xcol);
      Line(PlotX-2,PlotY-2,PlotX+2,PlotY+2);
      Line(PlotX-2,PlotY+2,PlotX+2,PlotY-2);
      SetColor(LineFore);

      { if possible, join up the points }

      if looproot<> 1 then line(OldPlotX,OldPlotY,PlotX,PlotY);
      OldPlotX:=PlotX;
      OldPlotY:=PlotY;
    end;
    SetColor(circfore);
    circle(PlotX,PlotY,CircSize);  { circle last point }

    { display the ranges }

    SetColor(RangeCol);
    Str(MinRoot[1]:4:1,strroot);
    OutTextXY(XLine+charwidth,MaxY-2*New_Line-8,strroot);
    Str(MaxRoot[1]:4:1,strroot);
    OutTextXY(XLine+charwidth,2*New_Line,strroot);
    Str(MinRoot[0]:4:1,strroot);
    OuttextXY(0,YLine-New_Line,strroot);
    Str(MaxRoot[0]:4:1,StrRoot);
    OuttextXY(MaxX-6*charwidth,YLine-New_line,strroot);
    GraphMess(GrMessX,GrMessY,'Press Space to continue');
  end
  else TextMess(MessX,MessY,'Cannot display screen, Press space to continue');
  repeat
    keypress:=KeyBoard;
  until escape or (keypress = SPC);
  closegraph;
end; { procedure }


{----------------------------------------------------------------------------}

{ this next procedure displays an approximation }

  Procedure Disp_Approx(cval:complex);

  var
    text:astring;

  begin
    TextBackground(ApproxBack);
    TextColor(ApproxFore);
    Str(cval[0]:strlen:prec,text);
    if length(text) > strlen then text[0]:=chr(strlen);
    GotoXY(ApproxRealX,ApproxY);
    write(text);
    Str(cval[1]:strlen:prec,text);
    if length(text) > strlen then text[0]:=chr(strlen);
    GotoXY(ApproxImagX,ApproxY);
    write(text);
  end;   { procedure }

{----------------------------------------------------------------------------}

{ this procedure stores a number on a special structure }

  procedure store(val:complex);

  begin
    Place:=place+1;   { move the pointer along 1 }
    if Place = csize+1 then begin   { if at the end of carray }
      place:=1;                     { point to beginning, and set flag }
      Wrapover:=true;
    end;
    ApproxRoots[place]:=val;        { save the value of structure }
  end;

{----------------------------------------------------------------------------}

procedure muller(Eqn:astring;
                 var z3:complex;
                 var found:boolean);

var
  z1,z2   :complex;
  f1,f2,f3:complex;   { f(z1),f(z2),f(z3) }
  h,h2,g  :complex;   {intermediate values}
  lambda  :complex;
  oldtemp,temp,temp2,den1,den2,den,tempg,tempg2,temp3:complex;
  Key:char;


begin
  found:=false;    { initialise found flag }
  z2:=z3;
  z1:=z3;
  z1[0]:=z2[0]-0.05;
  z2[0]:=z3[0]+0.05; { get 3 approximations to the root }
  evaluate(Eqn,z3,f3);
  evaluate(Eqn,z2,f2);
  evaluate(Eqn,z1,f1);

  csub(z3,z2,h);
  crmult(z3,2,oldtemp);
  csub(z2,z1,h2);
  cdiv(h,h2,lambda);
  repeat
    if keypressed then begin
      key:=readkey;
      if key = ESC then escape:=true;
    end;
    csub(z3,z2,temp);
    if cabs(temp) >= cabs(oldtemp) then begin
      crdiv(h,2,h);
      crdiv(lambda,2,lambda);
    end;
    oldtemp:=temp;
    crmult(lambda,2,g);
    g[0]:=g[0]+1;     {lambda*2+1}
    csub(f3,f2,tempg);
    cmult(g,tempg,g);
    csub(f2,f1,tempg);
    cipower(lambda,2,tempg2);
    cmult(tempg,tempg2,tempg);
    csub(g,tempg,g);          { (1+2*lambda)*(f3-f2)-lambda^2*(f2-f1) }
    temp[0]:=lambda[0]+1;
    temp[1]:=lambda[1];
    cmult(f3,temp,temp);
    crmult(temp,-2,temp);   { -2*f3*(1+lambda) }
    csub(f2,f1,temp2);
    cmult(lambda,temp2,temp2);
    csub(f2,temp2,temp2);
    csub(f3,temp2,temp2);
    cmult(lambda,temp2,temp2);
    temp3[0]:=lambda[0]+1;
    temp3[1]:=lambda[1];
    cmult(temp2,temp3,temp2);
    cmult(temp2,f3,temp2);
    crmult(temp2,4,temp2);
    cipower(g,2,temp3);
    csub(temp3,temp2,temp2);
    crpower(temp2,0.5,temp2);
    cadd(g,temp2,den1);
    csub(g,temp2,den2);
    if cabs(den1) > cabs(den2) then den:=den1
    else den:=den2;
    cdiv(temp,den,lambda);
    z1:=z2;
    z2:=z3;
    cmult(h,lambda,h);
    cadd(z3,h,z3);
    Disp_Approx(z3);
    store(z3);                        { display approx and save }
    f1:=f2;
    f2:=f3;
    evaluate(Eqn,z3,f3);
    csub(z3,z2,temp);
    if (cabs(temp) < 0.5*e1) and (cabs(f3) < 0.5*e2) then found:=true;
  until escape or found;
end;

{----------------------------------------------------------------------------}



var
  key : char;
  found,stop,rep:boolean;
  loop:integer;


{ main line of solve_nonlinear }

begin
  e1:=exp(ln(10)*-prec);     {set the precision }
  e2:=exp(ln(10)*-(prec));
  RootCnt:=0;                { set initial root count }
  Convert(expression);
  exit:=false;
  Display_screen(root,RootCnt);
  repeat
    TextMess(MessX,MessY,'Press ESC to stop root location');
    WrapOver:=false;   { initialise special structure }
    Place:=0;
    stop:=false;       { initialise flags }
    Muller(postfix,Init_z,found);  { root returned in init_z }
    if found then begin
      rep:=false;
      loop:=1;

{ root found, compare with others to see if it is similar }

      while (loop <= RootCnt) and not rep do begin
        if (abs(Root[loop,0]-Init_z[0]) <= e1) and (abs(Root[loop,1]-Init_Z[1]) <= e1) then rep:=true;
        loop:=loop+1;
      end;

{ if different, then save and display }

      if not rep then begin
        Disp_Root(Init_z,RootCnt);
        RootCnt:=RootCnt+1;
        Root[RootCnt]:=Init_z;
      end;
      TextMess(messX,MessY,'Display Approach to Root (Y/N)');
      repeat
        key:=keyboard;
      until (key = 'y') or (key = 'n') or (key = 'Y') or (key = 'N') or escape;

{ prepare to display appoach, by unravelling the special structure }

      if (key = 'y') or (key = 'Y') then begin
        if wrapover=true then begin
          Start:=1;
          for count:=place+1 to csize do begin
            Approach[Start]:=ApproxRoots[count];
            Start:=Start+1;
          end;
          for count:=1 to place do begin
            Approach[start]:=ApproxRoots[count];
            Start:=start+1;
          end;
          place:=csize;   { set no. of approxes in place }
        end
        else approach:=ApproxRoots;
        ApprDiag(Approach,place);        { display approach }
        Display_screen(root,RootCnt);    { redisplay iteration screen }
      end;
    end;

{ get next initial number if no stop requested and still can fit roots on the screen }

    if not escape and (RootCnt <> MaxRoot) then begin
      Init_Num(init_z,InitReal,Ypos,MessX,MessY);
      setcursor(false);
    end;
    if escape or (RootCnt = MaxRoot) then begin
      if escape then begin
        escape:=false;
        stop:=true;
        TextMess(messX,MessY,'System Stopped, Press Space to See Roots.');
      end
      else TextMess(messX,MessY,'Max no. of roots found, Press Space to See Roots.');
      repeat
        key:=keyboard;
      until (key = SPC) or escape;
    end;
  until escape or (RootCnt = MaxRoot) or stop;
end;   { procedure }

{----------------------------------------------------------------------------}