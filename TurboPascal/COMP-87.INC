{This Module is to provide the required functions for complex arithmetic}

{ Maintenance Log
  ---------------
Date             Action                         By
----             ------                         --
28/10/87       Initial Coding                   D.Kelsey
20/12/87       update power to allow small      D.Kelsey
               numbers
29/02/88       change real to accuracy          D.Kelsey


               Last Update: 29/02/88    13:40

}


const
  pi=3.1415926535;
type
  accuracy = extended;
  complex = array[0..1] of accuracy;


{The following procedures are available.They have to be procedures because
 Pascal won't allow functions of type complex.


conjugate  - returns the conjugate of a complex number
cadd       - adds 2 complex numbers
csub       - subtracts 2 complex numbers
cmult      - multiplies 2 complex numbers
cdiv       - divides 2 complex numbers
cexp       - calculates the e(z)
cln        - calculates the ln(z)
cpower     - calculates z1^z2
cipower    - raises a complex number to a positive integer power
csin       - calculates sin(z)
ccos       - calculates cos(z)
ctan       - calculates tan(z)
cabs       - calculates the modulus of a complex number
crmult     - multiplies a complex number by a real number
crpower    - raises a complex number to a real power
crdiv      - divides a complex number by a real number
csqrt      - calculates the square root of a complex number

to add a real value to a complex value, just add it to the [0] entry
to add an imaginary value to a complex value just add it to the [1] entry

the same applies for subtraction.

to raise a real number to a complex power, or divide a real number by a complex
number, first set up the real number as a complex number with a zero imaginary
part, and call cpower and cdiv respectively for the required functions.

}

{complex conjugate}

procedure conjugate(z1:complex;
                    var com_res:complex);
begin
  com_res[0]:=z1[0];
  com_res[1]:=-z1[1];
end;

{----------------------------------------------------------------------------}

{ this routine gives the modulus of a complex function}

function cabs(z1:complex):accuracy;

begin
  cabs:=sqrt(sqr(z1[0])+sqr(z1[1]));
end;

{----------------------------------------------------------------------------}

{complex multiplication}

procedure cmult(z1,z2:complex;
                var com_res:complex);
begin
  com_res[0]:=z1[0]*z2[0]-z1[1]*z2[1];
  com_res[1]:=z1[0]*z2[1]+z1[1]*z2[0];
end;

{----------------------------------------------------------------------------}

{complex Addition}

procedure cadd(z1,z2:complex;
               var com_res:complex);
begin
  com_res[0]:=z1[0]+z2[0];
  com_res[1]:=z1[1]+z2[1];
end;

{----------------------------------------------------------------------------}

{complex Subtraction}

procedure csub(z1,z2:complex;
               var com_res:complex);
begin
  com_res[0]:=z1[0]-z2[0];
  com_res[1]:=z1[1]-z2[1];
end;

{----------------------------------------------------------------------------}

{complex division}

procedure cdiv(z1,z2:complex;    {z1/z2}
               var com_res:complex);
begin
  conjugate(z2,com_res);
  cmult(z1,com_res,com_res);
  com_res[0]:=com_res[0]/(sqr(z2[0])+sqr(z2[1]));
  com_res[1]:=com_res[1]/(sqr(z2[0])+sqr(z2[1]));
end;

{----------------------------------------------------------------------------}

{complex exp function}

procedure cexp(z1:complex;
               var com_res:complex);
begin
  com_res[0]:=exp(z1[0])*cos(z1[1]);
  com_res[1]:=exp(z1[0])*sin(z1[1]);
end;

{----------------------------------------------------------------------------}

{complex Sin function}

procedure csin(z1:complex;
               var com_res:complex);

begin
  com_res[0]:=(exp(z1[1])+1/exp(z1[1]))*sin(z1[0])/2;
  com_res[1]:=(exp(z1[1])-1/exp(z1[1]))*cos(z1[0])/2;
end;

{----------------------------------------------------------------------------}

{complex Cos function}

procedure ccos(z1:complex;
               var com_res:complex);
var
  ctemp,cires:complex;

begin
  ctemp[0]:=-z1[1];
  ctemp[1]:=z1[0];
  cexp(ctemp,cires);
  ctemp[0]:=z1[1];
  ctemp[1]:=-z1[0];
  cexp(ctemp,com_res);
  com_res[0]:=(com_res[0]+cires[0])/2;
  com_res[1]:=(com_res[1]+cires[1])/2;
end;

{----------------------------------------------------------------------------}

{complex Tan Function}

procedure ctan(z1:complex;
               var com_res:complex);
var
  ctemp:complex;

begin
  csin(z1,ctemp);
  ccos(z1,com_res);
  cdiv(ctemp,com_res,com_res);
end;

{----------------------------------------------------------------------------}

{complex LN function}

procedure cln(z1:complex;
              var com_res:complex);

const
  ltol = 1e-33;


begin
  com_res[0]:=ln(cabs(z1));
  if (abs(z1[0]) < ltol) then com_res[1]:=pi/2
  else  com_res[1]:=arctan(z1[1]/z1[0]);

{must now decide correct angle, ie what the angle actually is.}

  if com_res[1] < 0 then
  begin  {is it 2nd or 4th quadrant}
    if z1[0] < 0 then com_res[1]:=com_res[1]+pi;     {2nd quadrant}
  end
  else
  begin  {is it 1st or 3rd quadrant}
    if z1[0] < 0 then com_res[1]:=com_res[1]-pi; {3rd quadrant}
  end
end;

{----------------------------------------------------------------------------}

{z1^z2 where z2 can be anything, real or complex}

procedure cpower(z1,z2:complex;
                 var com_res:complex);

const
  ptol = 1e-20;

begin
  if cabs(z1) < ptol then begin
    com_res[0]:=0;
    com_res[1]:=0;
  end
  else begin
    cln(z1,com_res);
    cmult(z2,com_res,com_res);
    cexp(com_res,com_res);
  end;
end;

{----------------------------------------------------------------------------}

{ this procedure calculates a complex number multiplied by a real number}

procedure crmult(z1:complex;
                 rx:accuracy;
                 var com_res:complex);

begin
  com_res[0]:=z1[0]*rx;
  com_res[1]:=z1[1]*rx;
end;

{----------------------------------------------------------------------------}

{ This procedure calculates a complex number raised to a real power }

procedure crpower(z1:complex;
                  rx:accuracy;
                  var com_res:complex);

var
  comp:complex;

begin
  comp[0]:=rx;
  comp[1]:=0;
  cpower(z1,comp,com_res);
end;

{----------------------------------------------------------------------------}

{ This procedure will be used for polynomial evaluation, and calculates a
  complex number raised to an integer power, via recursion
}

procedure cipower(z1:complex;
                  power:integer;
                  var com_res:complex);

begin
  if power <= 1 then com_res:=z1
  else begin
    cipower(z1,power-1,com_res);
    cmult(com_res,z1,com_res);
  end;
end;  { procedure }

{----------------------------------------------------------------------------}

{ this procedure calculates a complex number divided by a real number }

procedure crdiv(z1:complex;
                rx:accuracy;
                var com_res:complex);

begin
  com_res[0]:=z1[0]/rx;
  com_res[1]:=z1[1]/rx;
end;

{----------------------------------------------------------------------------}

{ this procedure calculates the square root of a complex number}

procedure csqrt(z1:complex;
                var com_res:complex);

begin
  crpower(z1,0.5,com_res);
end;

{----------------------------------------------------------------------------}

{end of complex definitions}
