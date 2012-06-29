Imports System
Imports System.IO

Public Class Form1
    Inherits System.Windows.Forms.Form


#Region " Windows Form Designer generated code "

    Public TextForClipboard$
    'Public StartX%, KoniecX%
    Public x%
    Public y%
    Public xStep%
    Public yStep%
    Public StartX%
    Public KoniecX%
    Public tmp$
    Dim f As StreamWriter

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub
    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    '<System.Diagnostics.DebuggerStepThrough()> 
    Private Sub InitializeComponent()
        '
        'Form1
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.BackColor = System.Drawing.Color.White
        Me.ClientSize = New System.Drawing.Size(504, 300)
        Me.Cursor = System.Windows.Forms.Cursors.SizeAll
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None
        Me.Name = "Form1"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Kreator modeli"
        'Me.TopMost = True
        'Me.WindowState = System.Windows.Forms.FormWindowState.Maximized

    End Sub
#End Region
    Protected Overrides Sub OnLoad( _
       ByVal e As EventArgs _
    )
        Beep()
        NarysujHaki()
        'My.Computer.Clipboard.SetText(TextForClipboard)
        Beep()
        Me.Close()
        End
    End Sub

    Private Sub DodajRaster(ByRef klasa$)
        If StartX% > -1 Then
            tmp$ = _
            "t1.Position.x = " & CStr(StartX%) & ";"
            tmp$ = tmp$ & _
            "t1.Position.y = " & CStr(y%) & ";"
            tmp$ = tmp$ & _
            "t2.Position.x = " & CStr(KoniecX%) & ";"
            tmp$ = tmp$ & _
            "t2.Position.y = " & CStr(y%) & ";"
            tmp$ = tmp$ & _
            "t3.Position.x = " & CStr(KoniecX%) & ";"
            tmp$ = tmp$ & _
            "t3.Position.y = " & CStr(y% + 1) & ";"
            tmp$ = tmp$ & _
            "t4.Position.x = " & CStr(StartX%) & ";"
            tmp$ = tmp$ & _
            "t4.Position.y = " & CStr(y% + 1) & ";" & Chr(13)
            tmp$ = tmp$ & _
            klasa & ".AddTriangle( t1, t2, t3, true );" & Chr(13)
            tmp$ = tmp$ & _
            klasa & ".AddTriangle( t3, t4, t1, true );" & Chr(13)

            f.WriteLine(tmp$)
            f.WriteLine(klasa$ & ".AddTriangle( t1, t2, t3, true );")
            f.WriteLine(klasa$ & ".AddTriangle( t3, t4, t1, true );")

            StartX% = -1
            KoniecX% = -1
        End If
    End Sub
    Private Sub NarysujJeden(ByRef obrazek As Bitmap, ByRef klasa$)

        f = New StreamWriter("DumpModel.txt")
        xStep% = 1
        yStep% = 1
        StartX% = -1
        KoniecX% = -1

        For y% = 0 To obrazek.Height - 1
            For x% = 0 To obrazek.Width - 1
                If obrazek.GetPixel(x%, y%).B = 0 Then
                    If StartX% = -1 Then
                        StartX% = x%
                    End If
                    KoniecX% = x%
                Else
                    DodajRaster(klasa$)
                End If
            Next
            DodajRaster(klasa$)
        Next

        f.Close()

    End Sub

    Private Sub NarysujHaki()
        Dim m_hak0 As New Bitmap("..\middle_hook.bmp")
        NarysujJeden(m_hak0, "mh")
    End Sub

End Class
