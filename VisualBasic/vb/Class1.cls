VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "Complextest"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Private Real_part As Double
Private Imaginary_part As Double
Public Property Get real() As Double
  real = Real_part
End Property

Public Property Let real(vNewValue As Double)
Real_part = vNewValue
End Property

Public Property Get imaginary() As Double
imaginary = Imaginary_part
End Property

Public Property Let imaginary(vNewValue As Double)
Imaginary_part = vNewValue
End Property

Public Function csin() As Double
  csin = Sin(Real_part)
End Function
