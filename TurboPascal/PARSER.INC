{ this file contains the parser used to edit check an expression sent
  to it, and also convert it into a form of postfix in order to make
  evaluation much simpler.
  the format is as follows for a posfix system.

  [2][4][3]+*[45][22]+c*s[-7.3,.66]$/

The process for this is sin(2*(4+3)*cos(45+22))/-[-7.3,.66]

the processing required makes use of stack definitions.  }


{ Maintenance log    ( for Parser, Evaluate and convert )
  ---------------

Date                        Action                     by
----                        ------                     --
28/10/87                 initial coding                D.Kelsey
15/02/88              allow operators as signs         D.Kelsey



                Last Update: 15/02/88  9:50

}

{ program start }


type
  astring = string[80];


function parser(fn:astring;
                var newfn,errors:astring):boolean;
var
  pos_old,stackptr:integer;
  operator,cur_char,Pre_Char:char;
  stack:array[0..80] of char;
  decused,comused:boolean;

{ the next function decides on the priority of an operator }

function priority(func:char):integer;
begin
  priority:=0;
  case func of
    '$'     : priority:=5;
    's','c','l','e','t' : priority:=4;
    '^'     : priority:=3;
    '*','/' : priority:=2;
    '+','-' : priority:=1;
  end;  {case}
end;   { function }

{----------------------------------------------------------------------------}

{ the next functions define the the stack processing required.
  the stack is a stack of characters, with a stack pointer, global over
  all functions.

  the following definitions are :
    topstack   : character on the top of the stack
    pull       : removes a character from the stack
    push       : places a character on the stack
    stackempty : returns a boolean value dependent on whether the stack has anything in

}


function topstack:char;
begin
  if stackptr <> 0  then topstack:=stack[stackptr];
end;

{----------------------------------------------------------------------------}

function pull:char;
begin
  if stackptr <> 0 then begin
    pull:=topstack;
    stackptr:=stackptr-1;
  end; { if }
end;   { function }

{----------------------------------------------------------------------------}

procedure push(data:char);
begin
  stackptr:=stackptr+1;
  stack[stackptr]:=data;
end;

{----------------------------------------------------------------------------}

function stackempty:boolean;
begin
  stackempty:=false;
  if stackptr = 0 then stackempty:=true;
end;

{----------------------------------------------------------------------------}

{ this procedure sets a flag at the error pointed to by pos_old, in the error
  string.errors is a global variable defined in the parameter list.
}


procedure seterror(errtype:char);
begin
  parser:=false;
  errors[pos_old]:=errtype;
end;

{----------------------------------------------------------------------------}

{ this next procedure edit-checks each number within a block of characters, indicating a number,
  complex or normal

}


procedure process_num;
begin
  cur_char:=fn[pos_old];
  if (cur_char in ['0'..'9','.','+','-',',']) and (cur_char <> ' ') then begin

    { if next character is +/- and is not a sign character, then error.
      operators are not allowed within a complex definition.CODE = 'c' }

    if (cur_char in ['+','-']) and ((pre_char <> '[') and (pre_char <> ',')) then seterror('7')  { sign in wrong place }
    else begin
      if (cur_char = '.') and (decused) then seterror('3')    { decimal point used twice }
      else begin
        if (cur_char = ',') and (comused) then seterror('a')
        else begin
          newfn:=newfn+cur_char;
          if cur_char = '.' then decused:=true;
          if cur_char = ',' then begin
            comused:=true;
            decused:=false;
          end;
        end;
      end;
    end;
    pre_char:=fn[pos_old];
  end
  else if cur_char <> ' ' then seterror('9'); { character is either unknown, or a space}

end;  { procedure }

{----------------------------------------------------------------------------}

{ here is the start of the actual mainline }

begin
  parser:=true;
  pre_char:='(';          { initialise previous character }
  errors:='';
  for pos_old:=1 to length(fn)+1 do errors:=errors+' ';
  newfn:='';
  stackptr:=0;            { reset the stack }
  pos_old:=1;             { point to first character }
  while pos_old <= length(fn) do
  begin
    cur_char:=fn[pos_old];     { get character }

    { deal with the current character }

    case cur_char of
      '0'..'9','.' : begin     { A number is being defined }

                     if (pre_char <> 'z') and (pre_char <> ')') then begin
                       pre_char:='[';     { set up required parameters }
                       newfn:=newfn+'[';
                       decused:=false;
                       comused:=false;
                       while (fn[pos_old] in ['0'..'9','.']) and (pos_old<=length(fn)) do
                       begin
                         process_num;      { evaluate the number }
                         pos_old:=pos_old+1;
                       end;   {while}
                       pos_old:=pos_old-1;  { point to last number so to get correct next char}
                     end
                     else seterror('2');   {error, missing operator}
                     pre_char:='n';
                     newfn:=newfn+']';
                   end;

    'z','Z': begin  { the variable z has been found }

                  if (pre_char = 'o') or (pre_char = '(') then begin
                     newfn:=newfn+cur_char;
                  end     { first part of if}
                  else seterror('2');      {4z , so missing operator error}
                  pre_char:='z';
             end;


    '+','-','/','*','^' : begin  { a mathematical operator has been found }

                            if ((cur_char = '-') or (cur_char = '+')) and
                            ((pre_char = 'o') or (pre_char = '(') or (pre_char = ',')) then
                            begin  { is operator used to denote sign ? }
                              if cur_char = '-' then cur_char:='$'
                              else cur_char:='%';
                              pre_char:='n';
                            end;
                            if (pre_char <> ')') and (pre_char <> 'z') and (pre_char <> 'n') then seterror('1');
                            if cur_char <> '%' then begin
                              if stackempty then push(cur_char)
                              else begin

                                { process the operator using the stack }

                                while (priority(topstack) >= priority(cur_char)) and (not stackempty) do
                                begin
                                  operator:=pull;
                                  newfn:=newfn+operator;
                                end;   { while }
                                push(cur_char);
                              end;     { if }
                            end;       { if }
                            pre_char:='o';
                          end;

    '(','s','c','t','l','e','S','C','T','L','E'  : begin   { bracket of function }


                                 if (pre_char <> '(') and (pre_char <> 'o') then seterror('2');
                                 if (cur_char <> '(') and (fn[pos_old+1] <> '(') then seterror('8');
                                 pre_char:='(';
                                 push(cur_char);
                               end;

    ')'                 : begin   { A close bracket }

                            if (pre_char = 'n') or (pre_char = ')') or (pre_char = 'z') then begin
                              while (topstack <> '(') and (not stackempty) do begin
                                operator:=pull;
                                newfn:=newfn+operator;
                              end;   {while}
                              operator:=pull;
                              if operator <> '(' then seterror('4'); {loop terminated, not by an '('}
                            end  {1st part of if}
                            else seterror('1');  {missing operand or bracket}
                            pre_char:=')';
                          end;


    '['                 : begin  { start of a complex number definition }

                            if ( pre_char <> 'z' ) and ( pre_char <> ')' ) then begin
                              pre_char:='[';
                              newfn:=newfn+'[';
                              decused:=false;
                              comused:=false;
                              pos_old:=pos_old+1;
                              while (fn[pos_old] <> ']') and (pos_old <= length(fn)) do begin
                                process_num;
                                pos_old:=pos_old+1;
                              end;  {while}
                              newfn:=newfn+']';
                              if fn[pos_old] <> ']' then seterror('5');
                              pre_char:='n';
                            end
                            else seterror('2');    { and end of if}
                          end;  { begin }


     ' '                : ;    {  ignore spaces  }
     else               seterror('9');  { unknown character }

    end;   { case }
    pos_old:=pos_old+1;
  end;  {while}

  { pull the rest of the operators off the stack and thus complete the postfix expression }

  while not stackempty do
  begin
    operator:=pull;
    if (operator <> '(' ) then  newfn:=newfn+operator
    else seterror('6');    {error has occured, too many '(' and end if }
  end;  {while}
end;  {function}


{ end of parser }
{----------------------------------------------------------------------------}


{ this next section is the evaluator of a function which has been parsed
  and converted for use.}

procedure evaluate (f:astring;
                    z:complex;
                    var resf:complex);


var
  cstack:array [0..40] of complex;       { stack definitions for complex storage }
  cstkptr:integer;
  fvalue,svalue,fresult:complex;
  block:astring;
  loop:integer;


{ the next procedures are used to process a complex stack.Because of the nature
  of these routines, it is easier and cleaner to define them separately instead
  of trying to have one command for both complex and characters (in the parser)

}


procedure cpush(comforst:complex);
begin
  cstkptr:=cstkptr+1;
  cstack[cstkptr]:=comforst;
end;

{----------------------------------------------------------------------------}

procedure cpull(var comoffst:complex);
begin
  comoffst:=cstack[cstkptr];
  cstkptr:=cstkptr-1;
end;

{----------------------------------------------------------------------------}

{ the following routine converts either a normal number or a complex number into
 useable format
}


procedure number(anum:astring;
                 var actual:complex);

var
  real,imaginary:astring;
  realpart:boolean;
  loop1,errorval:integer;

begin
  real:='';      { clear the string entries }
  imaginary:='';
  realpart:=true;   { real part always before imaginary }
  for loop1:=1 to length(anum) do
  begin
    if anum[loop1] <> ',' then
    begin
      if realpart then real:=real+anum[loop1]
      else imaginary:=imaginary+anum[loop1];
    end
    else  realpart:=false;
  end; {for}
  if length(real) <> 0 then val (real,actual[0],errorval)
  else actual[0]:=0;
  if length(imaginary) <> 0 then val (imaginary,actual[1],errorval)
  else actual[1]:=0;
end; {procedure}

{----------------------------------------------------------------------------}

{ this is the main-line loop }

begin
  cstkptr:=1;
  loop:=1;
  while loop <= length(f) do
  begin
    case f[loop] of

    '['              : begin   { a number, Real or complex to be processed }
                         block:='';
                         loop:=loop+1;   { point past '['}
                         while f[loop] <> ']' do begin
                           block:=block+f[loop];
                           loop:=loop+1;
                         end;   {While}
                         number(block,fvalue);
                         cpush(fvalue);
                       end;


    '$'              : begin    {top number, sign to be reversed }
                         cpull(fvalue);
                         fvalue[0]:=-fvalue[0];
                         fvalue[1]:=-fvalue[1];
                         cpush(fvalue);
                       end;

    'z','Z'          : cpush(z);     { saves the variable }

    's','S'          : begin         { perform the Sin function }

                         cpull(fvalue);
                         csin(fvalue,fresult);
                         cpush(fresult);
                       end;

    'c','C'          : begin         { perform the Cos function }
                         cpull(fvalue);
                         ccos(fvalue,fresult);
                         cpush(fresult);
                       end;

    't','T'          : begin         { perform the Tan Function }

                         cpull(fvalue);
                         ctan(fvalue,fresult);
                         cpush(fresult);
                       end;

    'l','L'          : begin         { perform complex Ln function }
                         cpull(fvalue);
                         cln(fvalue,fresult);
                         cpush(fresult);
                       end;

    'e','E'          : begin         { perform Complex EXP function }
                         cpull(fvalue);
                         cexp(fvalue,fresult);
                         cpush(fresult);
                       end;

    '+'              : begin
                         cpull(fvalue);
                         cpull(svalue);
                         cadd(svalue,fvalue,fresult);
                         cpush(fresult);
                       end;

    '-'              : begin
                         cpull(fvalue);
                         cpull(svalue);
                         csub(svalue,fvalue,fresult);
                         cpush(fresult);
                       end;

    '*'              : begin
                         cpull(fvalue);
                         cpull(svalue);
                         cmult(svalue,fvalue,fresult);
                         cpush(fresult);
                       end;

    '/'              : begin
                         cpull(fvalue);
                         cpull(svalue);
                         cdiv(svalue,fvalue,fresult);
                         cpush(fresult);
                       end;

    '^'              : begin
                         cpull(fvalue);
                         cpull(svalue);
                         cpower(svalue,fvalue,fresult);
                         cpush(fresult);
                       end;
    end;  {case}
  loop:=loop+1;
  end; {while}
  cpull(resf);        { retreive the result and return }
end; {procedure}

{----------------------------------------------------------------------------}

{ this procedure converts a string to displayable informatiom }

Procedure Convert(var Data:astring);

var
  temp:astring;
  loop:integer;

begin
  temp:='F(z)=';
  for loop:=1 to length(data) do begin
    case Data[loop] of
      's','S' : temp:=temp+'Sin';
      't','T' : temp:=temp+'Tan';
      'e','E' : temp:=temp+'Exp';
      'c','C' : temp:=temp+'Cos';
      'l','L' : temp:=temp+'Ln';
      else temp:=temp+Data[loop];
    end;   { case }
  end;     { for }
  Data:=temp;
end;       { procedure }

{----------------------------------------------------------------------------}

{ end of the parser and evaluator include file }
