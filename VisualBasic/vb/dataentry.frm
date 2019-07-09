VERSION 4.00
Begin VB.Form dataentry 
   Caption         =   "data entry"
   ClientHeight    =   1248
   ClientLeft      =   3048
   ClientTop       =   3744
   ClientWidth     =   3756
   Height          =   1572
   Left            =   3000
   LinkTopic       =   "Form2"
   ScaleHeight     =   1248
   ScaleWidth      =   3756
   Top             =   3468
   Width           =   3852
   Begin VB.CommandButton Command2 
      Caption         =   "Cancel"
      Height          =   372
      Left            =   2160
      TabIndex        =   2
      Top             =   720
      Width           =   1452
   End
   Begin VB.CommandButton Command1 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   372
      Left            =   120
      TabIndex        =   1
      Top             =   720
      Width           =   1452
   End
   Begin VB.TextBox Text1 
      Height          =   372
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   3492
   End
End
Attribute VB_Name = "dataentry"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Private Sub Command1_Click()
  Dim getinfo As Double
  getinfo = Val(dataentry.Text1.Text)
  Form1.Grid1.Text = getinfo
  dataentry.Hide
End Sub


Private Sub Command2_Click()
  dataentry.Hide
End Sub


