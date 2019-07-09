VERSION 4.00
Begin VB.Form Argand 
   Caption         =   "Argand Diagram"
   ClientHeight    =   6720
   ClientLeft      =   2760
   ClientTop       =   1488
   ClientWidth     =   8352
   Height          =   7044
   Left            =   2712
   LinkTopic       =   "Form2"
   ScaleHeight     =   50
   ScaleMode       =   0  'User
   ScaleWidth      =   50
   Top             =   1212
   Width           =   8448
   Begin VB.CommandButton Command2 
      Caption         =   "Close"
      Height          =   372
      Left            =   1800
      TabIndex        =   3
      Top             =   6000
      Width           =   1332
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Print"
      Height          =   372
      Left            =   120
      TabIndex        =   2
      Top             =   6000
      Width           =   1332
   End
   Begin VB.ListBox List1 
      Height          =   5616
      Left            =   4800
      TabIndex        =   1
      Top             =   120
      Width           =   3492
   End
   Begin VB.PictureBox Picture1 
      Height          =   5652
      Left            =   120
      ScaleHeight     =   5604
      ScaleWidth      =   4404
      TabIndex        =   0
      Top             =   120
      Width           =   4452
   End
End
Attribute VB_Name = "Argand"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
