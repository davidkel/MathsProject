Attribute VB_Name = "complexr"
Type complex
  real As Double
  imaginary As Double
End Type

Const pi = 3.1415926535
Const ltol = 1E-33
Const ptol = 1E-20
Public Function ccos(inval As complex) As complex
  Dim temp, temp2, temp3  As complex
  temp.real = -inval.imaginary
  temp.imaginary = inval.real
  temp2 = cexp(temp)
  temp.real = inval.imaginary
  temp.imaginary = -inval.real
  temp3 = cexp(temp)
  ccos.real = (temp3.real + temp2.real) / 2
  ccos.imaginary = (temp3.imaginary + temp2.imaginary) / 2
End Function


Public Function cln(inval As complex) As complex
  cln.real = ln(cabs(inval))
  If Abs(inval.real) < ltol Then cln.imaginary = pi / 2
  Else: cln.imaginary = arctan(inval.imaginary / inval.real)
  
  'must now decide correct angle, ie what the angle actually is.
  
  If cln.imaginary < 0 Then begin
    If inval.real < 0 Then cln.imaginary = cln.imaginary + pi   ' 2nd quadrant
  End If
  Else: begin 'is it 1st or 3rd quadrant
    If inval.real < 0 Then cln.imaginary = cln.imaginary - pi '3rd quadrant
  End If
End Function


Public Function crmult(inval As complex, inval2 As Double) As complex
  crmult.real = inval.real * inval2
  crmult.imaginary = inval.imaginary * inval2
End Function
Public Function crdiv(inval As complex, inval2 As Double) As complex
  crdiv.real = inval.real / inval2
  crdiv.imaginary = inval.imaginary / inval2
End Function

Public Function crpower(inval As complex, inval2 As Double) As complex
  Dim temp As complex
  temp.real = inval2
  temp.imaginary = 0
  crpower = cpower(inval, temp)
End Function
Public Function cipower(inval As complex, inval2 As Integer) As complex
Dim temp As complex
If inval2 <= 1 Then cipower = inval
Else: begin
  temp = cipower(inval, inval2 - 1)
  cipower = cmult(temp, inval)
End Function

Public Function csin(inval As complex) As complex
  csin.real = (Exp(inval.imaginary) + 1 / Exp(inval.imaginary)) * Sin(inval.real) / 2
  csin.imaginary = (Exp(inval.imaginary) - 1 / Exp(inval.imaginary)) * Cos(inval.real) / 2
End Function
Public Function csqrt(inval As complex) As complex
  csqrt = cipower(inval, 0.5)
End Function

Public Function ctan(inval As complex) As complex
  ctan = cdiv(csin(inval), ccos(inval))
End Function

Public Function cpower(inval, inval2 As complex) As complex
  If cabs(inval) < ptol Then begin
    cpower.real = 0
    cpower.imaginary = 0
  End
  Else: begin
    cpower = cexp(cmult(inval2, cln(inval)))
  End If
End Function


Public Function cmult(inval1 As complex, inval2 As complex) As complex
  cmult.real = inval1.real * inval2.real - inval1.imaginary * inval2.imaginary
  cmult.imaginary = inval1.real * inval2.imaginary - inval1.imaginary * inval2.real
End Function


Public Function cexp(inval As complex) As complex
  cexp.real = Exp(inval.real) * Cos(inval.imaginary)
  cexp.imaginary = Exp(inval.real) * Sin(inval.imaginary)
End Function


Public Function cabs(inval As complex) As Double
  cabs = sqrt(Sqr(inval.real) + Sqr(inval.imaginary))
End Function

Public Function cadd(inval1 As complex, inval2 As complex) As complex
 cadd.real = inval1.real + inval2.real
 cadd.imaginary = inval1.imaginary + inval2.imaginary
End Function

Public Function csub(inval1 As complex, inval2 As complex) As complex
 csub.real = inval1.real - inval2.real
 csub.imaginary = inval1.imaginary - inval2.imaginary
End Function


Public Function cdiv(inval1 As complex, inval2 As complex) As complex
  Dim temp1, temp2 As complex
  temp1 = conjugate(inval2)
  temp2 = cmult(inval1, temp1)
  cdiv.real = temp2.real / (Sqr(inval2.real) + Sqr(inval2.imaginary))
  cdiv.imaginary = temp2.imaginary / (Sqr(inval2.real) + Sqr(inval2.imaginary))
End Function


