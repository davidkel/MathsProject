program solve;

{ This is the Main Line For the whole Package and controls the compiling and
  the bring together if the modules }

{ this whole suite of programs were created on 12/2/88 by D.Kelsey

  No futher modifications

  Copyright 1988     D.kelsey

Maintenance log
---------------
Date                    action                         by
----                    ------                         --
20/12/87           initial coding                    D.kelsey
15/02/88           changed Argand call               D.kelsey

   }


{  program start }

{$v-}

uses
  dos,crt,graph,printer;

const
  NonLinOPt = 1;
  PolyOption = 2;
  ExitOption = 3;


{$i cursor.inc }    { used by include files }
{$i complex.inc}    { handles complex algebra }
{$i hardcopy.inc}   { provides Hardcopy }
{$i parser.inc}     { includes the evaluator fopr functions }
{$i keymess.inc}    { global key definitions, messages and text control }


{$i initscr.inc}    { this is the initial screen display file }
{$i option.inc}     { process option include file }
{$i modnonsc.inc}   { nonlinear screen entry process control}
{$i modpscr.inc}    { polynomial screen process control}
{$i argand.inc}     { displays roots on argand diagram }
{$i nonlin.inc}     { method for solving non-linear eqns }
{$i modpoly.inc}    { method for solving polynomials }


var
  option,precision,degree,NumRoots,counter:integer;
  selection:select;
  postfix,StartEqn:astring;
  initial_z:complex;
  coefficients,roots:carray;
  StartCoeff:complexstr;

{ mainline
  All procedures called will be in the above include files

}

begin
  StartEqn:='';               { clear the initial start equation and coefficients }
  for degree:=0 to deglimit do begin
    StartCoeff[degree,1]:='';
    StartCoeff[degree,2]:='';
  end;
  window(1,1,80,25);
  clrscr;
  repeat
    escape:=false;          { clear the escape flag }
    Initial_Screen;
    selection[NonLinOpt]:='GENERAL';
    selection[PolyOption]:='POLYNOMIAL';
    selection[ExitOption]:='END';
    Option_Select(selection,3,option);   { do option selection }
    if option <> ExitOption then begin

    { process the selected option }

      case option of
        NonLinOpt: begin
                     Nonlinear_Entry(StartEqn,postfix,precision,initial_z);
                     if not escape then Solve_Nonlinear(StartEqn,PostFix,roots,NumRoots,Precision,Initial_Z);
                   end;

        PolyOption: begin
                      Polynomial_Entry(StartCoeff,coefficients,degree,precision,initial_z);
                      if not escape then Solve_Polynomial(coefficients,degree,roots,NumRoots,precision,initial_z);
                    end;

      end;  { case }

      { if roots do exit and no escape key, display an argand diagram }

      if (NumRoots <> 0) and (not escape) then begin
        ArgDiag(roots,NumRoots,Precision,coefficients,degree,StartEqn,option);
      end;
    end;
  until option = ExitOption;
  ClrScr;
  SetCursor(true);
end.
{----------------------------------------------------------------------------}