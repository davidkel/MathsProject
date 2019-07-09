VERSION 4.00
Begin VB.Form DisplaySoln 
   Caption         =   "Solutions"
   ClientHeight    =   6492
   ClientLeft      =   1488
   ClientTop       =   1608
   ClientWidth     =   6864
   Height          =   6816
   Left            =   1440
   LinkTopic       =   "Form2"
   ScaleHeight     =   6492
   ScaleWidth      =   6864
   Top             =   1332
   Width           =   6960
   Begin VB.TextBox Text2 
      Height          =   312
      Left            =   120
      TabIndex        =   8
      Top             =   5400
      Width           =   1572
   End
   Begin VB.TextBox Text3 
      Height          =   312
      Left            =   2040
      TabIndex        =   7
      Top             =   5400
      Width           =   1572
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Solve-again"
      Enabled         =   0   'False
      Height          =   312
      Left            =   4020
      TabIndex        =   6
      Top             =   5400
      Width           =   2412
   End
   Begin VB.CommandButton solution_stop 
      Caption         =   "Stop"
      Height          =   372
      Left            =   120
      TabIndex        =   5
      Top             =   6000
      Width           =   1332
   End
   Begin VB.CommandButton Solution_close 
      Caption         =   "Close"
      Height          =   372
      Left            =   4800
      TabIndex        =   4
      Top             =   6000
      Width           =   1332
   End
   Begin VB.CommandButton Solution_print 
      Caption         =   "Print Solns"
      Height          =   372
      Left            =   3240
      TabIndex        =   3
      Top             =   6000
      Width           =   1332
   End
   Begin VB.CommandButton solution_argand 
      Caption         =   "Argand"
      Height          =   372
      Left            =   1680
      TabIndex        =   2
      Top             =   6000
      Width           =   1332
   End
   Begin VB.ListBox solutionlist 
      Height          =   4656
      Left            =   0
      TabIndex        =   1
      Top             =   120
      Width           =   2412
   End
   Begin VB.PictureBox Picture1 
      BackColor       =   &H00000000&
      ForeColor       =   &H00FFFFFF&
      Height          =   4692
      Left            =   2520
      ScaleHeight     =   4644
      ScaleWidth      =   4284
      TabIndex        =   0
      Top             =   120
      Width           =   4332
   End
   Begin VB.Label Label2 
      Caption         =   "Initial Real"
      Height          =   252
      Left            =   120
      TabIndex        =   10
      Top             =   5040
      Width           =   1572
   End
   Begin VB.Label Label3 
      Caption         =   "Initial Imaginary"
      Height          =   252
      Left            =   2040
      TabIndex        =   9
      Top             =   5040
      Width           =   1572
   End
End
Attribute VB_Name = "DisplaySoln"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Private Sub Command4_Click()

End Sub


Dim CX, CY, F, F1, F2, i    ' Declare variables
    ScaleMode = 3   ' Set ScaleMode to pixels.
    CX = Picture1.ScaleWidth / 2 ' Get horizontal center.
    CY = Picture1.ScaleHeight / 2    ' Get vertical center.
    DrawWidth = 8   ' Set DrawWidth.
    For i = 50 To 0 Step -2
        F = i / 50  ' Perform interim
        F1 = 1 - F: F2 = 1 + F  ' calculations.
        Picture1.ForeColor = QBColor(i Mod 15)   ' Set foreground color.
        Picture1.Line (CX * F1, CY * F1)-(CX * F2, CY * F2), , BF
    Next i
    DoEvents    ' Yield for other processing.
    If CY > CX Then ' Set DrawWidth.
        DrawWidth = ScaleWidth / 25

Else
        DrawWidth = ScaleHeight / 25
    End If
    For i = 0 To 50 Step 2  ' Set up loop.
        F = i / 50  ' Perform interim
        F1 = 1 - F: F2 = 1 + F  ' calculations.
        Picture1.Line (CX * F1, CY)-(CX, CY * F1)    ' Draw upper-left.
        Picture1.Line -(CX * F2, CY) ' Draw upper-right.
        Picture1.Line -(CX, CY * F2) ' Draw lower-right.
        Picture1.Line -(CX * F1, CY) ' Draw lower-left.
        Picture1.ForeColor = QBColor(i Mod 15)   ' Change color each time.

Next i
    DoEvents    ' Yield for other processing.

End Sub

Private Sub Form_Unload(Cancel As Integer)
Form1.Command1.Enabled = True
Form1.Mnu.Enabled = True
End Sub


Private Sub Picture1_DblClick()
Dim sh, sw As Integer
sh = Picture1.ScaleHeight
sw = Picture1.ScaleWidth
Picture1.Cls
Picture1.Line (0, 0)-(sw, sh)
Picture1.Line -(sw / 2, sh / 3)
End Sub


Private Sub Solution_close_Click()
DisplaySoln.Hide
Form1.Command1.Enabled = True
Form1.Mnu.Enabled = True
End Sub


Private Sub solution_stop_Click()
solution_go = False
Form1.Command1.Enabled = True
End Sub


