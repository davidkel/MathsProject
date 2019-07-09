VERSION 4.00
Begin VB.Form Form1 
   Caption         =   "Complex Solution Solver"
   ClientHeight    =   3372
   ClientLeft      =   1032
   ClientTop       =   2820
   ClientWidth     =   7776
   Height          =   3924
   Left            =   984
   LinkTopic       =   "Form1"
   ScaleHeight     =   3372
   ScaleWidth      =   7776
   Top             =   2316
   Width           =   7872
   Begin VB.ComboBox Combo1 
      Height          =   288
      Left            =   180
      Style           =   2  'Dropdown List
      TabIndex        =   7
      Top             =   720
      Visible         =   0   'False
      Width           =   672
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Solve"
      Height          =   492
      Left            =   4020
      TabIndex        =   6
      Top             =   1380
      Width           =   3492
   End
   Begin VB.TextBox Text3 
      Height          =   288
      Left            =   5940
      TabIndex        =   3
      Top             =   900
      Width           =   1572
   End
   Begin VB.TextBox Text2 
      Height          =   288
      Left            =   4020
      TabIndex        =   2
      Top             =   900
      Width           =   1572
   End
   Begin VB.TextBox Text1 
      Height          =   288
      Left            =   480
      TabIndex        =   0
      Top             =   120
      Width           =   7092
   End
   Begin VB.Label Degree 
      BorderStyle     =   1  'Fixed Single
      Caption         =   "Degree"
      Height          =   252
      Left            =   180
      TabIndex        =   9
      Top             =   480
      Visible         =   0   'False
      Width           =   672
   End
   Begin MSGrid.Grid Grid1 
      Height          =   2772
      Left            =   960
      TabIndex        =   8
      Top             =   480
      Visible         =   0   'False
      Width           =   2892
      _Version        =   65536
      _ExtentX        =   5101
      _ExtentY        =   4890
      _StockProps     =   77
      BackColor       =   16777215
      Rows            =   41
      Cols            =   3
   End
   Begin VB.Label Label3 
      Caption         =   "Initial Imaginary"
      Height          =   252
      Left            =   5940
      TabIndex        =   5
      Top             =   540
      Width           =   1572
   End
   Begin VB.Label Label2 
      Caption         =   "Initial Real"
      Height          =   252
      Left            =   4020
      TabIndex        =   4
      Top             =   540
      Width           =   1572
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "f(z)="
      Height          =   252
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   372
   End
   Begin VB.Menu Mnu 
      Caption         =   "&Option"
      Begin VB.Menu MnuFunction 
         Caption         =   "&Function"
         Checked         =   -1  'True
      End
      Begin VB.Menu MnuPolynomial 
         Caption         =   "&Polynomial"
      End
      Begin VB.Menu bar1 
         Caption         =   "-"
      End
      Begin VB.Menu MnuExit 
         Caption         =   "&Exit"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Dim functype As Integer


Private Sub Combo1_Click()
  Dim temp As Integer
  temp = Val(Combo1.ListIndex)
  Grid1.Rows = temp + 3  '1 to note that index x = number x+ 1
                         '1 to cover the fixed part of the grid
                         '1 to cover to 0th coefficient
  loadcoef (temp + 1)
End Sub


Private Sub Command1_Click()

Dim coeffs(40) As complex
Dim coeff_count, grid_count, i, j As Integer

If functype = 1 Then
' This is a function solution
' Parse the function so it is error checked and can be evaluated
Else:
  ' This is a polunomial solution
  ' 8 grid rows = 1-7 entries to check = 7 coefficients
  coeff_count = Grid1.Rows - 1
  For i = 1 To coeff_count
    For j = 1 To 2
      Grid1.Row = i
      Grid1.Col = j
      If j = 1 Then coeffs(coeff_count - i).real = Val(Grid1.Text)
      If j = 2 Then coeffs(coeff_count - i).imaginary = Val(Grid1.Text)
    Next j
  Next i
End If

' Validate starting values
' solve the problem
solution_go = True
Command1.Enabled = False
Mnu.Enabled = False
DisplaySoln.Show
DisplaySoln.solutionlist.Clear
DisplaySoln.Picture1.Cls

If functype = 1 Then
  DisplaySoln.Label2.Visible = True
  DisplaySoln.Label3.Visible = True
  DisplaySoln.Text2.Visible = True
  DisplaySoln.Text3.Visible = True
  DisplaySoln.Command1.Visible = True
  DisplaySoln.Command1.Enabled = False
Else
  DisplaySoln.Label2.Visible = False
  DisplaySoln.Label3.Visible = False
  DisplaySoln.Text2.Visible = False
  DisplaySoln.Text3.Visible = False
  DisplaySoln.Command1.Visible = False
End If

'Call the correct solver routine
'- Solver routine must be able to evaluate a function or polynomial based on a value
'- function version requires a postfix parser, must work for text such as sin(
'- parser should also accept real and imaginary numbers
'- parser will need to point out errors by replacing the text string
'- Calculate the next value to try
'- display each a polyline in the picture box or an approach display
'- Add a root to the list box
'- enable argand, print solns, close at end or when stop is pressed
'- invoke do_events during the solution execution

'Other mathematical functions to include.
'arctan, inv, hyperbolic
End Sub

Private Sub Form_Click()
'  Dim test As complex
 ' Dim test2 As complex
  'test.real = 0.32
 ' test2 = csin(test)
  'Print test2.real
End Sub

Private Sub Form_Load()
Dim i As Integer
For i = 1 To 40
  Combo1.AddItem i
Next
Combo1.ListIndex = 5
Grid1.Rows = 8
Grid1.Row = 0
Grid1.Col = 1
Grid1.Text = "Real"
Grid1.Col = 2
Grid1.Text = "Imaginary"
Grid1.ColWidth(1) = 1000
Grid1.ColWidth(2) = 1000
'loadcoef (6)
functype = 1
End Sub


Private Sub Form_Unload(Cancel As Integer)
Unload Argand
Unload DisplaySoln
Unload dataentry
End Sub

Private Sub Grid1_KeyPress(KeyAscii As Integer)
  If KeyAscii > 47 And KeyAscii < 58 Then
    dataentry.Text1.Text = Chr(KeyAscii)
  Else: dataentry.Text1.Text = ""
  End If
  dataentry.Text1.SelStart = 1
  dataentry.Show 1 ' modal show
End Sub


Private Sub MnuExit_Click()
End
End Sub


Private Sub MnuFunction_Click()
functype = 1
MnuPolynomial.Checked = False
MnuFunction.Checked = True
Label1.Visible = True
Text1.Visible = True
Degree.Visible = False
Combo1.Visible = False
Grid1.Visible = False
End Sub

Private Sub MnuPolynomial_Click()
functype = 2
MnuPolynomial.Checked = True
MnuFunction.Checked = False
Label1.Visible = False
Text1.Visible = False
Degree.Visible = True
Combo1.Visible = True
Grid1.Visible = True
Grid1.SetFocus
Grid1.Col = 1
Grid1.Row = 1
End Sub



Public Sub loadcoef(count As Integer)
  Dim i As Integer
  Dim temp As String
  Grid1.Col = 0
  For i = 0 To count
    Grid1.Row = count - i + 1
    temp = "Z^" + Str(i)
    Grid1.Text = temp
  Next
End Sub
